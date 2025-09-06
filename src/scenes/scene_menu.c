#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"

typedef struct
{
    Scene base;
    int highlight;
} MenuScene;

static bool menu_init(Scene *s)
{
    (void)s;
    return true;
}

static void menu_handle(Scene *s, int ch)
{
    MenuScene *ms = (MenuScene *)s;
    const int N = 3;
    if (ch == KEY_UP)
    {
        ms->highlight = (ms->highlight + N - 1) % N;
    }
    else if (ch == KEY_DOWN)
    {
        ms->highlight = (ms->highlight + 1) % N;
    }
    else if (ch == '\n' || ch == KEY_ENTER)
    {
        if (ms->highlight == 0)
            s->next_scene = SCENE_LEVEL_PRACTICE_440;
        else if (ms->highlight == 1)
            s->next_scene = SCENE_LEVEL_BLUEBOX;
        else if (ms->highlight == 2)
            s->next_scene = SCENE_VICTORY;
    }
}

static void menu_update(Scene *s, float f, float p, uint64_t now_ms)
{
    (void)s;
    (void)f;
    (void)p;
    (void)now_ms;
}

static void menu_render(Scene *s)
{
    erase();
    ui_draw_border();
    mvprintw(1, 2, "M I C  T E R M  G A M E  —  LAN Race");
    mvprintw(2, 2, "Launch on two machines and pass --peer IP:PORT to race.");
    mvprintw(3, 2, "Examples:");
    mvprintw(4, 4, "./mic_term_game --name Alice --port 55555 --peer 192.168.1.42:55556");
    mvprintw(5, 4, "./mic_term_game --name Bob   --port 55556 --peer 192.168.1.41:55555");

    const char *items[] = {
        "Practice: Match 440 Hz (race-enabled)",
        "Level: Blue Box (2600 Hz seize) (race-enabled)",
        "Victory (demo)"};
    MenuScene *ms = (MenuScene *)s;
    for (int i = 0; i < 3; ++i)
    {
        if (i == ms->highlight)
            mvprintw(7 + i, 4, "> %s", items[i]);
        else
            mvprintw(7 + i, 4, "  %s", items[i]);
    }

    mvprintw(LINES - 2, 2, "Arrows: navigate, Enter: select, 'q': quit.");
    refresh();
}

static void menu_destroy(Scene *s) { (void)s; }

Scene *create_menu_scene(void)
{
    MenuScene *ms = (MenuScene *)calloc(1, sizeof(MenuScene));
    ms->base.init = menu_init;
    ms->base.handle_input = menu_handle;
    ms->base.update = menu_update;
    ms->base.render = menu_render;
    ms->base.destroy = menu_destroy;
    ms->base.next_scene = -1;
    ms->highlight = 0;
    return (Scene *)ms;
}
