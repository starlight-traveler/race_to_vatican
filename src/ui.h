#ifndef UI_H
#define UI_H

#include <ncursesw/ncurses.h>
#include "config.h"

// UI helpers
void ui_init(void);
void ui_shutdown(void);
void ui_draw_border(void);

// Status/header
void ui_header_pitch(float freq, float peak);

// Player-ish visualization (optional)
typedef struct
{
    int maxy, maxx;
    int y, x;
} Player;

void ui_getmax(Player *pl);
void ui_clamp(Player *pl);
void ui_move_player_bins(Player *pl, float freq);
void ui_draw_player_and_legend(Player *pl);

#endif
