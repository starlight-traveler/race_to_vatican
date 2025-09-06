#ifndef NET_H
#define NET_H

#include <stdbool.h>
#include <stdint.h>
#include <netinet/in.h> // struct sockaddr_in
#include <sys/socket.h> // socket types

typedef enum
{
    NET_STATUS_PLAYING = 0,
    NET_STATUS_WIN = 1,
    NET_STATUS_QUIT = 2
} NetStatus;

typedef struct
{
    char name[32];
    float last_freq_hz;
    float last_progress01; // 0..1
    NetStatus last_status;
    uint64_t last_ms_remote; // remote's own timestamp
    uint64_t last_heard_ms;  // local monotonic when we received
    bool seen;
} NetPeerState;

typedef struct
{
    int sock_fd; // -1 if disabled
    uint64_t last_tx_ms;
    struct sockaddr_in peer_addr; // valid if peer configured
    bool has_peer;
    char self_name[32];
    uint16_t bind_port;
} NetSystem;

// Initialize UDP socket bound to bind_ip:bind_port (bind_ip can be NULL/empty for INADDR_ANY).
// peer_ip may be NULL/empty to run single-player (no send).
bool net_init(NetSystem *ns,
              const char *bind_ip, uint16_t bind_port,
              const char *peer_ip, uint16_t peer_port,
              const char *self_name);

// Close socket.
void net_shutdown(NetSystem *ns);

// Non-blocking receive; update peer state if a valid packet arrives.
// Returns true if we consumed at least one packet.
bool net_poll(NetSystem *ns, NetPeerState *peer, uint64_t now_ms);

// Send current state to peer if configured.
bool net_send_state(NetSystem *ns, float freq_hz, float progress01, NetStatus status, uint64_t now_ms);

#endif
