#include <stdlib.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"
#include <string.h>
static const char *VATICAN_WIN_ASCII[] = {
"    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⡠⠤⠲⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀ ",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡴⢛⣁⣤⣤⣤⡤⠤⠤⠄⠤⢄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⠿⡿⡿⠉⠀⠈⠀⠙⡀⠀⠀⠀⠀⠈⠙⠲⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣧⠀⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡕⢄⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⢸⣿⠀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣌⢆⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣧⣼⣿⡆⠙⠓⠀⠘⡢⣄⡀⠀⢀⡀⠀⠀⠀⠀⠀⢻⡿⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⣹⣿⣿⣿⣿⣿⠁⠚⢥⣑⡒⠊⠑⠒⠒⠂⠬⢍⡂⠀⠀⣠⣼⠃⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣼⣿⣿⣿⣿⣿⣷⣦⣽⣦⠐⢪⠒⠤⠀⠀⠉⠀⠀⣹⢻⡀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣿⡟⠻⣿⣟⠋⠻⢿⣿⠀⢰⣿⣿⣿⣿⣷⡆⠀⢃⢰⠃⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⡇⠀⠈⠉⠉⢩⣾⣯⠀⠀⠙⠿⢷⠌⠙⠃⠀⢻⡾⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣷⡆⠀⠀⣠⣾⣿⠁⠀⠀⣄⠑⢀⠀⠀⠀⢰⣻⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⣿⣿⣿⡿⣶⠞⣱⣿⣿⣦⣢⡠⠙⢷⣄⠢⠄⢀⡰⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣴⣶⣿⣿⣿⣿⣿⡄⢹⣿⣿⣽⣟⠛⠁⠀⢀⣈⣿⠂⠀⡞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣾⣿⣿⡁⢻⣿⣿⣿⣿⣷⡀⢿⣿⢿⣯⡍⢉⠉⠁⠋⠀⢀⡼⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣾⣿⣿⣿⣿⣦⣹⣿⣿⣿⣿⣿⣦⠙⣌⠉⠉⠉⠀⠀⢀⣠⠞⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⠀⠀⠀⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⡿⠿⣿⣾⣿⣒⣶⣶⣤⢀⣿⢹⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
"⠀⠀⠀⣠⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡄⠀⠉⠙⠛⣿⠛⠒⢉⡅⢺⣿⣿⣷⣦⣤⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀",
"⣀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⠀⢀⠇⢀⠀⠊⣀⠀⠹⣿⣿⣿⣿⣿⣿⣦⡀⠀⠀⠀⠀⠀⠀",
"⣿⣿⣿⣿⣿⣿⣿⡿⠏⣿⣿⠻⣿⣿⣿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀⢾⠀⠀⠙⢧⡀⠙⣿⣿⣿⣟⠿⣏⢷⡄⠀⠀⠀⠀⠀",
"⣿⣿⣿⣿⣿⣿⣿⠃⠀⣿⣿⠨⠍⣿⣿⣿⣿⣿⣯⡁⠀⠀⠀⠀⠀⠀⠀⢸⡄⠀⠀⠀⠱⡀⠘⢿⣿⣿⣿⣾⠏⢻⣆⠀⠀⠀⠀",
"⣿⣿⣿⣿⣿⡿⠋⠀⠲⣿⣿⠐⠂⣿⣿⣿⣾⣿⢻⡷⣄⠀⠀⠀⠀⠀⠀⠀⢡⢠⠀⠀⠀⠙⡀⠘⡿⣿⣿⣿⣷⣄⠹⣦⠀⠀⠀",
"⣿⣿⣿⣿⣿⠉⠀⠀⢰⣿⣿⢈⢸⣿⣿⣿⣿⡇⠀⢻⡻⣧⡀⠀⠀⠀⠀⠀⠈⡆⢀⠀⠀⠀⠱⡀⣧⠈⠿⣿⣿⣿⣦⠹⣧⠀⠀",
"⣿⣿⣿⣿⣿⠀⠀⠀⣾⣿⣿⢼⢸⣿⣿⣿⣿⡇⠀⠀⠻⢯⣷⡀⠀⠀⠀⠀⠀⢠⡈⢆⠀⠀⠀⢣⡏⠀⠀⠙⢿⣿⣿⣧⠹⣧⠀",
"⣿⣿⣿⣿⡟⠀⠀⠀⣿⣿⣿⠈⢸⡿⢻⣿⣿⡇⠀⠀⠀⠀⠀⠙⢦⡀⠀⠀⠀⠸⡙⢶⣧⡀⠀⠐⡷⡄⠀⠀⠀⢻⣿⣿⣿⣻⡇"};

static const int VATICAN_WIN_ASCII_LINES = (int)(sizeof(VATICAN_WIN_ASCII) / sizeof(VATICAN_WIN_ASCII[0]));

typedef struct
{
    Scene base;
    uint64_t enter_ms;
} VictoryScene;

static void draw_ascii_centered(
    const char **art, int nlines,
    int center_y_offset,
    int y_offset, /* additional down-shift  */
    int x_offset) /* additional right-shift */
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* Measure max width of the provided art */
    int maxw = 0;
    for (int i = 0; i < nlines; ++i)
    {
        int w = (int)strlen(art[i]);
        if (w > maxw)
            maxw = w;
    }

    /* Intended (unclamped) top-left position */
    int sy = center_y_offset - nlines / 2 + y_offset;
    int sx = (cols - maxw) / 2 + x_offset;

    /* Vertical clipping: if sy < 0, skip -sy lines from the source */
    int src_i0 = 0;  /* first source line index to draw */
    int dst_y0 = sy; /* screen y for that first line    */
    if (dst_y0 < 0)
    {
        src_i0 = -dst_y0;
        if (src_i0 > nlines)
            return; /* everything is above the screen */
        dst_y0 = 0;
    }
    if (dst_y0 >= rows)
        return; /* everything is below the screen */

    /* For each visible source line */
    for (int i = src_i0; i < nlines; ++i)
    {
        int y = dst_y0 + (i - src_i0);
        if (y >= rows)
            break; /* bottom clipped */

        const char *line = art[i];
        int len = (int)strlen(line);
        if (len <= 0)
            continue;

        /* Horizontal clipping */
        int draw_x = sx; /* where on screen we try to start */
        int src_x0 = 0;  /* how many chars we skip from the left of the line */

        /* If starting before column 0, skip that many chars from the source */
        if (draw_x < 0)
        {
            src_x0 = -draw_x;
            if (src_x0 >= len)
                continue; /* whole line is left of screen */
            draw_x = 0;
        }

        /* Now compute how many chars we can draw */
        int draw_len = len - src_x0;

        /* If starting beyond right edge, nothing to draw */
        if (draw_x >= cols)
            continue;

        /* Clip right edge */
        if (draw_x + draw_len > cols)
        {
            draw_len = cols - draw_x;
            if (draw_len <= 0)
                continue;
        }

        mvaddnstr(y, draw_x, line + src_x0, draw_len);
    }
}

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
    if (now_ms - v->enter_ms >= 10000)
    {
        s->next_scene = SCENE_MENU; // auto-return
    }
}

static void vic_render(Scene *s)
{
    erase();
    ui_draw_border();
    mvprintw(2, 2, ">>> LINK ESTABLISHED <<<");
    mvprintw(4, 2, "Congratulations on winning the race!\n");
    mvprintw(6, 2, "Returning to menu...\n");
    draw_ascii_centered(VATICAN_WIN_ASCII, VATICAN_WIN_ASCII_LINES, 9, 5, 45);
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
