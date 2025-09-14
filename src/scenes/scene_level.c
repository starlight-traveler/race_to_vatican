#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include <string.h>
#include "scene_manager.h"
#include "config.h"
#include "ui.h"
#include "dsp.h"
#include "net.h"

extern NetSystem g_net;     // defined in main.c
extern NetPeerState g_peer; // defined in main.c

typedef struct
{
    Scene base;
    int scene_id;

    Player pl;
    float last_freq;
    float last_peak;

    uint64_t hold_start_ms;
    bool in_window;

    uint64_t start_ms;

    // Race progress (0..1)
    float progress01;
    bool announced_win; // to send win once
} LevelScene;

static bool within_window(float freq)
{
    for (int i = 0; i < TONE_COUNT; i++) {
        if (fabsf(freq - tone_table[i]) <= TONE_TOLERANCE) {
            return true;
        }
    }
    return false;
}

static float compute_progress(uint64_t now_ms, uint64_t hold_start_ms, uint32_t needed_ms, bool in_window)
{
    if (!in_window || needed_ms == 0)
        return 0.0f;
    uint64_t held = now_ms - hold_start_ms;
    if ((double)held >= (double)needed_ms)
        return 1.0f;
    float p = (float)((double)held / (double)needed_ms);
    if (p < 0.f)
        p = 0.f;
    if (p > 1.f)
        p = 1.f;
    return p;
}

static bool level_init(Scene *s)
{
    LevelScene *ls = (LevelScene *)s;
    ui_getmax(&ls->pl);
    ls->last_freq = -1.0f;
    ls->last_peak = 0.0f;
    ls->hold_start_ms = 0;
    ls->in_window = false;
    ls->start_ms = cfg_now_ms();
    ls->progress01 = 0.0f;
    ls->announced_win = false;
    return true;
}

static void level_handle(Scene *s, int ch)
{
    LevelScene *ls = (LevelScene *)s;
    switch (ch)
    {
    case KEY_LEFT:
    case 'a':
    case 'A':
        ls->pl.x -= 1;
        break;
    case KEY_RIGHT:
    case 'd':
    case 'D':
        ls->pl.x += 1;
        break;
    case KEY_UP:
    case 'w':
    case 'W':
        ls->pl.y -= 1;
        break;
    case KEY_DOWN:
    case 's':
    case 'S':
        ls->pl.y += 1;
        break;
    default:
        break;
    }
    ui_clamp(&ls->pl);
}

static void level_update(Scene *s, float freq, float peak, uint64_t now_ms)
{
    LevelScene *ls = (LevelScene *)s;
    const Level *lv = &g_levels[ls->scene_id];

    ls->last_freq = freq;
    ls->last_peak = peak;

    if (freq > 0.0f)
        ui_move_player_bins(&ls->pl, freq);

    if (freq > 0.0f && within_window(freq))
    {
        if (!ls->in_window)
        {
            ls->in_window = true;
            ls->hold_start_ms = now_ms;
        }
    }
    else
    {
        ls->in_window = false;
    }

    ls->progress01 = compute_progress(now_ms, ls->hold_start_ms, lv->hold_ms, ls->in_window);

    // --- Networking: receive peer updates, then broadcast our state ---
    net_poll(&g_net, &g_peer, now_ms);
    NetStatus status = NET_STATUS_PLAYING;

    // If we completed locally, announce WIN and transition
    if (ls->progress01 >= 1.0f)
    {
        status = NET_STATUS_WIN;
        if (!ls->announced_win)
        {
            net_send_state(&g_net, freq, ls->progress01, status, now_ms);
            ls->announced_win = true;
        }
        s->next_scene = lv->next_scene_id;
        return;
    }

    // If peer already won, we lose
    if (g_peer.seen && g_peer.last_status == NET_STATUS_WIN)
    {
        s->next_scene = SCENE_DEFEAT;
        return;
    }

    // Periodic state send (<= 20 Hz implicit via HOP_INTERVAL_MS 50ms)
    net_send_state(&g_net, freq, ls->progress01, status, now_ms);

    // Optional: time limit fallback
    if (lv->time_limit_ms > 0 && (now_ms - ls->start_ms) >= lv->time_limit_ms)
    {
        s->next_scene = SCENE_MENU;
        return;
    }
}

static void level_render(Scene *s)
{
    LevelScene *ls = (LevelScene *)s;
    const Level *lv = &g_levels[ls->scene_id];

    erase();
    ui_draw_border();
    ui_header_pitch(ls->last_freq, ls->last_peak);

    // Title & story
    mvprintw(2, 2, "[Level] %s", lv->name);
    int row = 3;
    {
        const char *p = lv->blurb;
        while (*p)
        {
            const char *nl = strchr(p, '\n');
            if (!nl)
            {
                mvprintw(row++, 2, "%s", p);
                break;
            }
            char buf[256];
            size_t len = (size_t)(nl - p);
            if (len > sizeof(buf) - 1)
                len = sizeof(buf) - 1;
            memcpy(buf, p, len);
            buf[len] = 0;
            mvprintw(row++, 2, "%s", buf);
            p = nl + 1;
        }
    }

    mvprintw(row + 1, 2, "Target: %.1f Hz  ±%.1f Hz  | Hold: %u ms",
             lv->target_hz, lv->tolerance_hz, lv->hold_ms);

    // Local progress bar
    int bar_y = row + 3;
    int bar_x = 2;
    int bar_w = COLS - 4;
    if (bar_w < 10)
        bar_w = 10;
    int fill = (int)((float)bar_w * ls->progress01 + 0.5f);
    mvprintw(bar_y - 1, bar_x, "You");
    mvhline(bar_y, bar_x, ' ', bar_w);
    for (int i = 0; i < fill; ++i)
        mvaddch(bar_y, bar_x + i, '=');
    mvaddch(bar_y, bar_x + fill < bar_x + bar_w ? bar_x + fill : bar_x + bar_w - 1, '>');

    // Peer progress bar
    float peer_prog = g_peer.seen ? g_peer.last_progress01 : 0.0f;
    int bar_y2 = bar_y + 2;
    int fill2 = (int)((float)bar_w * peer_prog + 0.5f);
    mvprintw(bar_y2 - 1, bar_x, "Peer%s%s",
             g_peer.seen && g_peer.name[0] ? " (" : "",
             g_peer.seen && g_peer.name[0] ? g_peer.name : "");
    if (g_peer.seen && g_peer.name[0])
        mvaddch(bar_y2 - 1, bar_x + 5 + (int)strlen(g_peer.name), ')');
    mvhline(bar_y2, bar_x, ' ', bar_w);
    for (int i = 0; i < fill2; ++i)
        mvaddch(bar_y2, bar_x + i, '#');
    mvaddch(bar_y2, bar_x + fill2 < bar_x + bar_w ? bar_x + fill2 : bar_x + bar_w - 1, '>');

    // Tiny playfield for flair
    ui_draw_player_and_legend(&ls->pl);

    // Peer status
    if (!g_peer.seen)
    {
        mvprintw(bar_y2 + 2, bar_x, "(Waiting for peer... use --peer IP:PORT on both sides)");
    }
    else if (g_peer.last_status == NET_STATUS_WIN)
    {
        mvprintw(bar_y2 + 2, bar_x, "Peer has already finished!");
    }

    refresh();
}

static void level_destroy(Scene *s) { (void)s; }

Scene *create_level_scene(int scene_id)
{
    LevelScene *ls = (LevelScene *)calloc(1, sizeof(LevelScene));
    ls->base.init = level_init;
    ls->base.handle_input = level_handle;
    ls->base.update = level_update;
    ls->base.render = level_render;
    ls->base.destroy = level_destroy;
    ls->base.next_scene = -1;
    ls->scene_id = scene_id;
    return (Scene *)ls;
}
