#include "net.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // strtoull, strtof, strtol
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h> // inet_pton
#include <netinet/in.h>
#include <sys/socket.h>

static int make_nonblocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0)
        return -1;
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
        return -1;
    return 0;
}

bool net_init(NetSystem *ns,
              const char *bind_ip, uint16_t bind_port,
              const char *peer_ip, uint16_t peer_port,
              const char *self_name)
{
    if (!ns)
        return false;
    memset(ns, 0, sizeof(*ns));
    ns->sock_fd = -1;
    ns->last_tx_ms = 0;
    ns->bind_port = bind_port;
    strncpy(ns->self_name, self_name && *self_name ? self_name : "player", sizeof(ns->self_name) - 1);

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0)
        return false;

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(bind_port);
    if (bind_ip && *bind_ip)
    {
        if (inet_pton(AF_INET, bind_ip, &addr.sin_addr) != 1)
        {
            close(fd);
            return false;
        }
    }
    else
    {
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        close(fd);
        return false;
    }
    if (make_nonblocking(fd) < 0)
    {
        close(fd);
        return false;
    }

    ns->sock_fd = fd;

    ns->has_peer = false;
    memset(&ns->peer_addr, 0, sizeof(ns->peer_addr));
    if (peer_ip && *peer_ip && peer_port != 0)
    {
        ns->peer_addr.sin_family = AF_INET;
        ns->peer_addr.sin_port = htons(peer_port);
        if (inet_pton(AF_INET, peer_ip, &ns->peer_addr.sin_addr) == 1)
        {
            ns->has_peer = true;
        }
        else
        {
            memset(&ns->peer_addr, 0, sizeof(ns->peer_addr));
        }
    }

    return true;
}

void net_shutdown(NetSystem *ns)
{
    if (!ns)
        return;
    if (ns->sock_fd >= 0)
    {
        close(ns->sock_fd);
        ns->sock_fd = -1;
    }
}

bool net_poll(NetSystem *ns, NetPeerState *peer, uint64_t now_ms)
{
    if (!ns || ns->sock_fd < 0 || !peer)
        return false;
    bool got_any = false;

    for (;;)
    {
        char buf[256];
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        ssize_t n = recvfrom(ns->sock_fd, buf, sizeof(buf) - 1, 0, (struct sockaddr *)&from, &fromlen);
        if (n <= 0)
            break;
        buf[n] = 0;

        // Expect: name|ms|freq|progress|status\n
        char name[64] = {0};
        unsigned long long ms = 0ULL;
        float freq = -1.0f, prog = 0.0f;
        int status = 0;

        char *save = NULL;
        char *p = buf;

        // name
        char *tok = strtok_r(p, "|\n", &save);
        if (!tok)
            continue;
        strncpy(name, tok, sizeof(name) - 1);

        // ms
        tok = strtok_r(NULL, "|\n", &save);
        if (!tok)
            continue;
        ms = strtoull(tok, NULL, 10);

        // freq
        tok = strtok_r(NULL, "|\n", &save);
        if (!tok)
            continue;
        freq = strtof(tok, NULL);

        // progress
        tok = strtok_r(NULL, "|\n", &save);
        if (!tok)
            continue;
        prog = strtof(tok, NULL);

        // status
        tok = strtok_r(NULL, "|\n", &save);
        if (!tok)
            continue;
        status = (int)strtol(tok, NULL, 10);

        strncpy(peer->name, name, sizeof(peer->name) - 1);
        peer->last_freq_hz = freq;
        if (prog < 0.0f)
            prog = 0.0f;
        if (prog > 1.0f)
            prog = 1.0f;
        peer->last_progress01 = prog;
        peer->last_status = (NetStatus)status;
        peer->last_ms_remote = (uint64_t)ms;
        peer->last_heard_ms = now_ms;
        peer->seen = true;

        got_any = true;
    }

    return got_any;
}

bool net_send_state(NetSystem *ns, float freq_hz, float progress01, NetStatus status, uint64_t now_ms)
{
    if (!ns || ns->sock_fd < 0 || !ns->has_peer)
        return false;
    if (progress01 < 0.0f)
        progress01 = 0.0f;
    if (progress01 > 1.0f)
        progress01 = 1.0f;

    char line[256];
    int len = snprintf(line, sizeof(line), "%s|%llu|%.2f|%.3f|%d\n",
                       ns->self_name,
                       (unsigned long long)now_ms,
                       freq_hz > 0.f ? freq_hz : -1.f,
                       progress01,
                       (int)status);
    if (len <= 0)
        return false;

    ssize_t sent = sendto(ns->sock_fd, line, (size_t)len, 0,
                          (struct sockaddr *)&ns->peer_addr, sizeof(ns->peer_addr));
    return sent == len;
}
