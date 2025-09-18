#include "ui.h"
#include <string.h>

void ui_init(void)
{
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
}

void ui_shutdown(void)
{
    endwin();
}

void ui_draw_border(void)
{
    box(stdscr, 0, 0);
}

void ui_header_pitch(float freq, float peak)
{
    mvprintw(0, 2, " RACE TO VATICAN ");
    if (freq > 0.0f)
    {
        mvprintw(0, 18, " | freq: %7.1f Hz  peak: %.2f  (WASD/Arrows: move, 'q': quit)", freq, peak);
    }
    else
    {
        mvprintw(0, 18, "| freq: ---.- Hz  peak: %.2f  (WASD/Arrows: move, 'q': quit)", peak);
    }
}

void ui_getmax(Player *pl)
{
    getmaxyx(stdscr, pl->maxy, pl->maxx);
    pl->x = pl->maxx / 4;
    pl->y = pl->maxy / 2;
    ui_clamp(pl);
}

void ui_clamp(Player *p)
{
    if (p->x < 1)
        p->x = 1;
    if (p->x > p->maxx - 2)
        p->x = p->maxx - 2;
    if (p->y < 1)
        p->y = 1;
    if (p->y > p->maxy - 2)
        p->y = p->maxy - 2;
}

void ui_move_player_bins(Player *pl, float freq)
{
    if (freq <= 0.0f)
        return;
    if (freq < BIN_LEFT_MAX)
    {
        pl->x -= 1;
    }
    else if (freq < BIN_UP_MAX)
    {
        pl->y -= 1;
    }
    else if (freq < BIN_RIGHT_MAX)
    {
        pl->x += 1;
    }
    else
    {
        pl->y += 1;
    }
    ui_clamp(pl);
}

void ui_draw_player_and_legend(Player *pl)
{
    mvaddch(pl->y, pl->x, 'O' | A_BOLD);
    mvaddch(pl->maxy / 2, pl->maxx / 2, '*' | A_STANDOUT);
    mvprintw(pl->maxy - 1, 2, " Bins: <%.0f=LEFT  <%.0f=UP  <%.0f=RIGHT  >=%.0f=DOWN ",
             BIN_LEFT_MAX, BIN_UP_MAX, BIN_RIGHT_MAX, BIN_RIGHT_MAX);
}
