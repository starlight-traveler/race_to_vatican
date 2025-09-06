#include <stdlib.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"

typedef struct
{
    Scene base;
    char winner_name[32];
    uint64_t enter_ms;
} DefeatScene;

static bool def_init(Scene *s)
{
    DefeatScene *d = (DefeatScene *)s;
    d->enter_ms = cfg_now_ms();
    return true;
}

static void def_handle(Scene *s, int ch)
{
    (void)s;
    (void)ch;
}

static void def_update(Scene *s, float f, float p, uint64_t now_ms)
{
    (void)f;
    (void)p;
    DefeatScene *d = (DefeatScene *)s;
    if (now_ms - d->enter_ms >= 2000)
        s->next_scene = SCENE_MENU;
}

static void def_render(Scene *s)
{
    DefeatScene *d = (DefeatScene *)s;
    erase();
    ui_draw_border();
    mvprintw(2, 2, ">>> YOU LOSE <<<");
    mvprintw(4, 2, "Peer won the race%s%s.",
             d->winner_name[0] ? " — winner: " : "",
             d->winner_name[0] ? d->winner_name : "");
    mvprintw(6, 2, "Returning to menu...");
    refresh();
}

static void def_destroy(Scene *s) { (void)s; }

Scene *create_defeat_scene(const char *winner_name)
{
    DefeatScene *d = (DefeatScene *)calloc(1, sizeof(DefeatScene));
    d->base.init = def_init;
    d->base.handle_input = def_handle;
    d->base.update = def_update;
    d->base.render = def_render;
    d->base.destroy = def_destroy;
    d->base.next_scene = -1;
    if (winner_name)
    {
        size_t i = 0;
        for (; winner_name[i] && i < sizeof(d->winner_name) - 1; ++i)
            d->winner_name[i] = winner_name[i];
        d->winner_name[i] = 0;
    }
    return (Scene *)d;
}
