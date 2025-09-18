// src/scenes/scene_help.c
#include <stdlib.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"

#define TITLE_COLOR_PAIR 1

static bool help_init(Scene *s)
{
    (void)s;
    if (has_colors())
    {
        start_color();
        init_pair(TITLE_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
    }
    return true;
}

static void help_handle_input(Scene *s, int ch)
{
    if (ch == 'q' || ch == 'Q' || ch == KEY_BACKSPACE || ch == 27 /*ESC*/ || ch == '\n' || ch == KEY_ENTER)
    {
        s->next_scene = SCENE_MENU;
    }
}

static void help_update(Scene *s, float f, float p, uint64_t now_ms)
{
    (void)s;
    (void)f;
    (void)p;
    (void)now_ms;
}

static void help_render(Scene *s)
{
    (void)s;
    erase();
    ui_draw_border();

    if (has_colors())
        attron(COLOR_PAIR(TITLE_COLOR_PAIR));
    mvprintw(1, 2, "HOW TO PLAY");
    if (has_colors())
        attroff(COLOR_PAIR(TITLE_COLOR_PAIR));

    int y = 3, x = 2;

    mvprintw(y++, x, "Goal:");
    mvprintw(y++, x + 2, "- Use Phreaking device to dial to frequency or buttons for granular control.");
    mvprintw(y++, x + 2, "- Hold the device close to the comptuer's microphone, you need to find where that is.");
    y++;

    mvprintw(y++, x, "Tones:");
    mvprintw(y++, x + 2, "- The game logic supports 3 different tones: 1400Hz, 1600Hz, and 1800Hz.");
    mvprintw(y++, x + 2, "- At the top of each level, there is a frequency tracker to check what frequency you are at.");
    mvprintw(y++, x + 2, "- At each level, each of the tones will lead to a different scene (most of the time).");
    mvprintw(y++, x + 2, "- Each tone between scenes is a two way path, so using the same tone again immediately will take you back to the previous scene.");
    mvprintw(y++, x + 2, "- So, with 3 tones, each scene will generally have 2 new options to explore, and a way to go back");
    mvprintw(y++, x + 2, "- ");
    y++;



    mvprintw(y++, x, "Levels:");
    mvprintw(y++, x + 2, "- Practice 440 Hz: A4 Steadily.");
    y++;

    mvprintw(y++, x, "Tips:");
    mvprintw(y++, x + 2, "- A clear, stable tone works best. The DSP logic attemps to cut off voice, but will cause jitter.");
    mvprintw(y++, x + 2, "- If input seems dead, check your OS mic permissions and input device. It should asked on startup.");
    y++;

    mvprintw(y++, x, "Controls:");
    mvprintw(y++, x + 2, "Arrows : Navigate menus");
    mvprintw(y++, x + 2, "Enter  : Select/Return");
    mvprintw(y++, x + 2, "Q/ESC  : Back/Quit");
    y++;

    mvprintw(LINES - 2, 2, "Press Enter or ESC to return to the menu.");
    refresh();
}

static void help_destroy(Scene *s) { (void)s; }

Scene *create_help_scene(void)
{
    Scene *s = (Scene *)calloc(1, sizeof(Scene));
    s->init = help_init;
    s->handle_input = help_handle_input;
    s->update = help_update;
    s->render = help_render;
    s->destroy = help_destroy;
    s->next_scene = -1;
    return s;
}
