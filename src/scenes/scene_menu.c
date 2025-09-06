#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"

typedef struct
{
    Scene base;
    int highlight; // 0..N-1
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
            s->next_scene = SCENE_VICTORY; // demo
    }
}

static void menu_update(Scene *s, float freq, float peak, uint64_t now_ms)
{
    (void)freq;
    (void)peak;
    (void)now_ms;
    (void)s;
}

static void menu_render(Scene *s)
{
    (void)s;
    erase();
    ui_draw_border();
    mvprintw(1, 2, "M I C  T E R M  G A M E");
    mvprintw(2, 2, "Use your voice (tone) to play. Press 'q' anytime to quit.");
    mvprintw(4, 2, "Menu:");

    const char *items[] = {
        "Practice: Match 440 Hz",
        "Level: Blue Box (2600 Hz seize)",
        "Victory (demo)"};
    MenuScene *ms = (MenuScene *)s;
    for (int i = 0; i < 3; ++i)
    {
        if (i == ms->highlight)
            mvprintw(6 + i, 4, "> %s", items[i]);
        else
            mvprintw(6 + i, 4, "  %s", items[i]);
    }
    mvprintw(LINES - 2, 2, "Arrows: navigate, Enter: select. WASD/arrows move a dot for fun.");
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
