#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "config.h"
#include "audio.h"
#include "dsp.h"
#include "ui.h"
#include "scene_manager.h"
#include "net.h"
#include <locale.h>

NetSystem g_net; // global for scene access
NetPeerState g_peer;

static void parse_peer(const char *s, char *ip_out, size_t ip_cap, uint16_t *port_out)
{
    // expects "IP:PORT"
    if (!s || !*s)
    {
        ip_out[0] = 0;
        *port_out = 0;
        return;
    }
    const char *colon = strrchr(s, ':');
    if (!colon)
    {
        ip_out[0] = 0;
        *port_out = 0;
        return;
    }
    size_t len = (size_t)(colon - s);
    if (len >= ip_cap)
        len = ip_cap - 1;
    memcpy(ip_out, s, len);
    ip_out[len] = 0;
    *port_out = (uint16_t)strtoul(colon + 1, NULL, 10);
}

static void usage(const char *argv0)
{
    fprintf(stderr,
            "Usage: %s [--name NAME] [--port PORT] [--peer IP:PORT]\n"
            "  --name NAME    Your display name (default: player)\n"
            "  --port PORT    UDP bind port (default: 55555)\n"
            "  --peer IP:PORT Peer address to race (optional; set on both machines)\n",
            argv0);
}

int main(int argc, char **argv)
{
    const char *name = "player";
    uint16_t bind_port = 55555;
    char peer_ip[64] = {0};
    uint16_t peer_port = 0;
    setlocale(LC_ALL, "");

    for (int i = 1; i < argc; ++i)
    {
        if (!strcmp(argv[i], "--name") && i + 1 < argc)
        {
            name = argv[++i];
        }
        else if (!strcmp(argv[i], "--port") && i + 1 < argc)
        {
            bind_port = (uint16_t)strtoul(argv[++i], NULL, 10);
        }
        else if (!strcmp(argv[i], "--peer") && i + 1 < argc)
        {
            parse_peer(argv[++i], peer_ip, sizeof(peer_ip), &peer_port);
        }
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            usage(argv[0]);
            return 0;
        }
        else
        {
            usage(argv[0]);
            return 1;
        }
    }

    // ---- Audio ----
    AudioSystem audio;
    if (!audio_init(&audio))
        return 1;
    if (!audio_start(&audio))
    {
        audio_shutdown(&audio);
        return 1;
    }

    // ---- Net ----
    memset(&g_peer, 0, sizeof(g_peer));
    if (!net_init(&g_net, NULL, bind_port,
                  (peer_ip[0] ? peer_ip : NULL), peer_port,
                  name))
    {
        fprintf(stderr, "Warning: networking disabled (could not init UDP). Running single-player.\n");
        memset(&g_net, 0, sizeof(g_net));
        g_net.sock_fd = -1;
    }

    // ---- UI ----
    ui_init();

    // ---- Scenes ----
    SceneManager sm;
    Scene *scene = create_menu_scene();
    scene_manager_init(&sm, scene);

    float window[WINDOW_SIZE];
    float freq_hz = -1.0f, peak_norm = 0.0f;

    uint64_t next_tick = cfg_now_ms();
    bool running = true;

    while (running)
    {
        int ch = getch();
        if (ch != ERR)
        {
            if (ch == 'q' || ch == 'Q')
            {
                // announce quit (best effort)
                net_send_state(&g_net, -1.0f, 0.0f, NET_STATUS_QUIT, cfg_now_ms());
                running = false;
                break;
            }
            if (sm.current && sm.current->handle_input)
                sm.current->handle_input(sm.current, ch);
        }

        uint64_t now_ms = cfg_now_ms();
        if (now_ms >= next_tick)
        {
            next_tick = now_ms + HOP_INTERVAL_MS;

            bool ok = audio_copy_latest_window(&audio, window, WINDOW_SIZE);
            if (ok)
            {
                PitchResult pr = dsp_estimate_pitch_autocorr(window, WINDOW_SIZE, (float)SAMPLE_RATE);
                freq_hz = pr.freq_hz;
                peak_norm = pr.peak_norm;
            }
            else
            {
                freq_hz = -1.0f;
                peak_norm = 0.0f;
            }

            if (sm.current && sm.current->update)
                sm.current->update(sm.current, freq_hz, peak_norm, now_ms);

            if (sm.current && sm.current->next_scene != -1)
            {
                int ns = sm.current->next_scene;
                sm.current->next_scene = -1;
                Scene *next = NULL;
                switch (ns)
                {
                case SCENE_MENU:
                    next = create_menu_scene();
                    break;
                case SCENE_HOME:
                    next = create_level_scene(SCENE_HOME);
                    break;
                case SCENE_LEVEL_BLUEBOX:
                    next = create_level_scene(SCENE_LEVEL_BLUEBOX);
                    break;
                case SCENE_LEVEL_PRACTICE_440:
                    next = create_level_scene(SCENE_LEVEL_PRACTICE_440);
                    break;
                case SCENE_LEVEL_PENTAGON:
                    next = create_level_scene(SCENE_LEVEL_PENTAGON);
                    break;
                case SCENE_VICTORY:
                    next = create_victory_scene();
                    break;
                case SCENE_DEFEAT:
                    next = create_defeat_scene(g_peer.name);
                    break;
                default:
                    next = create_menu_scene();
                    break;
                }
                scene_manager_switch(&sm, next);
            }

            if (sm.current && sm.current->render)
                sm.current->render(sm.current);
        }

        struct timespec ts = {0, 3 * 1000 * 1000};
        nanosleep(&ts, NULL);
    }

    ui_shutdown();
    scene_manager_destroy(&sm);
    net_shutdown(&g_net);
    audio_shutdown(&audio);
    return 0;
}
