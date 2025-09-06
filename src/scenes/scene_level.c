#include <stdlib.h>
#include <curses.h>
#include <math.h>
#include <string.h>

#include "scene_manager.h"
#include "config.h"
#include "ui.h"
#include "dsp.h"

typedef struct {
    Scene base;
    int scene_id;               // which level definition to use
    Player pl;
    float last_freq;
    float last_peak;
    uint64_t hold_start_ms;     // when we entered the in-window state
    bool in_window;             // currently within tolerance
    uint64_t start_ms;          // scene start for time limit
} LevelScene;

static bool level_init(Scene *s) {
    LevelScene *ls = (LevelScene*)s;
    ui_getmax(&ls->pl);
    ls->last_freq = -1.0f;
    ls->last_peak = 0.0f;
    ls->hold_start_ms = 0;
    ls->in_window = false;
    ls->start_ms = cfg_now_ms();
    return true;
}

static void level_handle(Scene *s, int ch) {
    (void)s;
    (void)ch;
    // Keyboard movement allowed for flavor
    LevelScene *ls = (LevelScene*)s;
    switch (ch) {
        case KEY_LEFT: case 'a': case 'A': ls->pl.x -= 1; break;
        case KEY_RIGHT: case 'd': case 'D': ls->pl.x += 1; break;
        case KEY_UP: case 'w': case 'W': ls->pl.y -= 1; break;
        case KEY_DOWN: case 's': case 'S': ls->pl.y += 1; break;
        default: break;
    }
    ui_clamp(&ls->pl);
}

static bool within_window(float freq, float target, float tol) {
    return fabsf(freq - target) <= tol;
}

static void level_update(Scene *s, float freq, float peak, uint64_t now_ms) {
    LevelScene *ls = (LevelScene*)s;
    const Level *lv = &g_levels[ls->scene_id];

    ls->last_freq = freq;
    ls->last_peak = peak;

    // Optional player drift by bins
    if (freq > 0.0f) {
        ui_move_player_bins(&ls->pl, freq);
    }

    if (freq > 0.0f && within_window(freq, lv->target_hz, lv->tolerance_hz)) {
        if (!ls->in_window) {
            ls->in_window = true;
            ls->hold_start_ms = now_ms;
        }
        if (lv->hold_ms > 0 && (now_ms - ls->hold_start_ms) >= lv->hold_ms) {
            s->next_scene = lv->next_scene_id; // success -> next
        }
    } else {
        ls->in_window = false;
    }

    if (lv->time_limit_ms > 0) {
        if ((now_ms - ls->start_ms) >= lv->time_limit_ms) {
            // Could route to a failure scene; for now return to menu
            s->next_scene = SCENE_MENU;
        }
    }
}

static void level_render(Scene *s) {
    LevelScene *ls = (LevelScene*)s;
    const Level *lv = &g_levels[ls->scene_id];

    erase();
    ui_draw_border();
    ui_header_pitch(ls->last_freq, ls->last_peak);

    // Title & story
    mvprintw(2, 2, "[Level] %s", lv->name);
    int row = 3;
    // Print blurb (multi-line)
    {
        const char *p = lv->blurb;
        while (*p) {
            const char *nl = strchr(p, '\n');
            if (!nl) { mvprintw(row++, 2, "%s", p); break; }
            char buf[256];
            size_t len = (size_t)(nl - p);
            if (len > sizeof(buf)-1) len = sizeof(buf)-1;
            memcpy(buf, p, len);
            buf[len] = 0;
            mvprintw(row++, 2, "%s", buf);
            p = nl + 1;
        }
    }

    // Target info
    mvprintw(row+1, 2, "Target: %.1f Hz  ±%.1f Hz  | Hold: %u ms",
             lv->target_hz, lv->tolerance_hz, lv->hold_ms);

    // Feedback line
    bool good = (ls->last_freq > 0.0f) &&
                within_window(ls->last_freq, lv->target_hz, lv->tolerance_hz);
    if (ls->last_freq > 0.0f) {
        mvprintw(row+3, 2, "Your pitch: %.1f Hz -> %s",
                 ls->last_freq, good ? "IN WINDOW" : "out of range");
        if (ls->in_window) {
            mvprintw(row+4, 2, "Holding... %llu / %u ms",
                     (unsigned long long)(cfg_now_ms() - ls->hold_start_ms),
                     lv->hold_ms);
        }
    } else {
        mvprintw(row+3, 2, "Listening... (hum a tone)");
    }

    // Tiny playfield for flair
    ui_draw_player_and_legend(&ls->pl);

    refresh();
}

static void level_destroy(Scene *s) { (void)s; }

Scene* create_level_scene(int scene_id) {
    LevelScene *ls = (LevelScene*)calloc(1, sizeof(LevelScene));
    ls->base.init = level_init;
    ls->base.handle_input = level_handle;
    ls->base.update = level_update;
    ls->base.render = level_render;
    ls->base.destroy = level_destroy;
    ls->base.next_scene = -1;
    ls->scene_id = scene_id;
    return (Scene*)ls;
}
