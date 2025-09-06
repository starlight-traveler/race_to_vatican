#include <stdlib.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"

typedef struct
{
    Scene base;
    uint64_t enter_ms;
} VictoryScene;

static bool vic_init(Scene *s)
{
    VictoryScene *v = (VictoryScene *)s;
    v->enter_ms = cfg_now_ms();
    return true;
}

static void vic_handle(Scene *s, int ch)
{
    (void)s;
    (void)ch;
}

static void vic_update(Scene *s, float freq, float peak, uint64_t now_ms)
{
    (void)freq;
    (void)peak;
    VictoryScene *v = (VictoryScene *)s;
    if (now_ms - v->enter_ms >= 1500)
    {
        s->next_scene = SCENE_MENU; // auto-return
    }
}

static void vic_render(Scene *s)
{
    erase();
    ui_draw_border();
    mvprintw(2, 2, ">>> LINK ESTABLISHED <<<");
    mvprintw(4, 2, "A quiet click. Then a distant voice: \"Pronto... Vaticano.\"");
    mvprintw(6, 2, "Returning to menu...");
    refresh();
}

static void vic_destroy(Scene *s) { (void)s; }

Scene *create_victory_scene(void)
{
    VictoryScene *v = (VictoryScene *)calloc(1, sizeof(VictoryScene));
    v->base.init = vic_init;
    v->base.handle_input = vic_handle;
    v->base.update = vic_update;
    v->base.render = vic_render;
    v->base.destroy = vic_destroy;
    v->base.next_scene = -1;
    return (Scene *)v;
}
