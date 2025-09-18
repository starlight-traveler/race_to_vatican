#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <curses.h>
#include "scene_manager.h"
#include "ui.h"
#include "config.h"

/* ----------------------------- CONFIG ----------------------------- */
#define MAX_STARS 220
#define STAR_MIN_SPEED 0.02f /* rows per second (normalized later) */
#define STAR_MAX_SPEED 0.20f
#define STAR_TWINKLE_HZ 1.5f

#define ASCII_BOB_HZ 0.4f
#define ASCII_SWAY_HZ 0.25f

/* Telephone-line look & motion */
#define TLINES_PER_SIDE 3    /* how many wires from each side */
#define TLINE_Y_TOP_OFFSET 2 /* how far from top border to start */
#define TLINE_SPACING_ROWS 2 /* vertical spacing between adjacent wires at start */
#define TLINE_INS_PERIOD 7   /* insulator spacing (in Bresenham steps) */
#define TLINE_INS_CHAR 'o'
#define TLINE_CHAR_FWD '\\'     /* down-right */
#define TLINE_CHAR_BACK '/'     /* down-left */
#define TLINE_CHAR_STRAIGHT '-' /* mostly horizontal (rare) */
#define TLINE_PHASE_SPEED 3.0f  /* insulator marching speed (steps/sec) */

#define TITLE_COLOR_PAIR 1
#define HILITE_COLOR_PAIR 2

// define M_PI for non mac os
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* ------------------------------------------------------------------ */

typedef struct
{
    float x01;   /* 0..1 horizontal (fraction of cols) */
    float y01;   /* 0..1 vertical   (fraction of rows) */
    float speed; /* rows per second (as fraction of rows) */
    float phase; /* 0..2pi for twinkle */
    int layer;   /* 0,1,2 for parallax intensity/char */
} Star;

typedef struct
{
    Scene base;
    int highlight;

    /* Animation state */
    Star stars[MAX_STARS];
    int star_count;
    uint64_t last_ms;
    float wire_phase; /* advances to move insulators along the wires */
    int last_rows, last_cols;

} MenuScene;

/* ----------------------------- ASCII ART ----------------------------- */
/* Width <= 76 cols to fit most terminals. Backslashes are escaped. */
static const char *VATICAN_ASCII[] = {
    "                               _.-^-.                                ",
    "                            .-\"  _   \"-.                             ",
    "                          .'   _/ \\_    '.                           ",
    "                         /    /  _  \\     \\                          ",
    "                        ;    / _/ \\_ \\     ;                         ",
    "                        |    |  ___  |     |                         ",
    "                        |    | |   | |     |                         ",
    "                    _.-.|    | |   | |     |.-._                     ",
    "                 .-\"    \\    | |___| |    /     \"-.                  ",
    "               .'         \\  |  ___  |  /          '.                ",
    "              /            \\ | |   | | /             \\               ",
    "             ;   _.-^-.     \\|_|   |_|/     .-^-._    ;              ",
    "             | .'  _   '.      .---.       .'  _  '.  |              ",
    "             |/   /_\\   \\_____/_____\\_____/   /_\\   \\ |              ",
    "            /|   /___\\  /  _   ___   _  \\  /  /___\\   |\\             ",
    "          .' |  |  _  |/  | | |___| | |  \\|  |  _  |  | '.           ",
    "      _.-'   |  | | | /|  | |  ___  | |\\  \\  | | | |  |   '-._       ",
    "   .-\"       |__|_|_|/__\\_|_|_|___|_|_|/__\\_|_|_|_|__|        \"-.    ",
    "  /---------------------------------------------------------------\\   ",
    " /___________________________ PIAZZA _______________________________\\  "};
static const int VATICAN_ASCII_LINES = (int)(sizeof(VATICAN_ASCII) / sizeof(VATICAN_ASCII[0]));

/* --------------------------- UTILS --------------------------- */

static float frand01(void)
{
    return (float)rand() / (float)RAND_MAX;
}

static void seed_rng_once(void)
{
    static int seeded = 0;
    if (!seeded)
    {
        unsigned long mix = (unsigned long)time(NULL) ^ (unsigned long)(uintptr_t)&seeded;
        srand((unsigned int)mix);
        seeded = 1;
    }
}

/* ---------------------- DRAW ASCII (with clipping) ---------------------- */
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

/* ------------------------- STARFIELD ------------------------- */

static void starfield_reseed(MenuScene *ms, int rows, int cols)
{
    (void)cols;
    ms->star_count = MAX_STARS;
    for (int i = 0; i < ms->star_count; ++i)
    {
        ms->stars[i].x01 = frand01();
        ms->stars[i].y01 = frand01();
        ms->stars[i].speed = STAR_MIN_SPEED + frand01() * (STAR_MAX_SPEED - STAR_MIN_SPEED);
        ms->stars[i].phase = frand01() * 2.0f * (float)M_PI;
        ms->stars[i].layer = (int)(frand01() * 3.0f); /* 0..2 */
    }
    ms->last_rows = rows;
    ms->last_cols = cols;
}

static void starfield_update(MenuScene *ms, float dt_sec, int rows, int cols)
{
    if (rows != ms->last_rows || cols != ms->last_cols)
    {
        ms->last_rows = rows;
        ms->last_cols = cols;
    }

    /* drift downward at each star's speed (normalized to rows) */
    for (int i = 0; i < ms->star_count; ++i)
    {
        Star *st = &ms->stars[i];
        st->y01 += st->speed * dt_sec;
        if (st->y01 > 1.05f)
        {
            st->y01 = -0.02f - frand01() * 0.05f;
            st->x01 = frand01();
            st->speed = STAR_MIN_SPEED + frand01() * (STAR_MAX_SPEED - STAR_MIN_SPEED);
            st->phase = frand01() * 2.0f * (float)M_PI;
            st->layer = (int)(frand01() * 3.0f);
        }
        st->phase += 2.0f * (float)M_PI * STAR_TWINKLE_HZ * dt_sec;
        if (st->phase > 2.0f * (float)M_PI)
            st->phase -= 2.0f * (float)M_PI;
    }
}

static void starfield_draw(MenuScene *ms)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    for (int i = 0; i < ms->star_count; ++i)
    {
        Star *st = &ms->stars[i];
        int x = (int)(st->x01 * (float)cols);
        int y = (int)(st->y01 * (float)rows);

        if (x < 1 || x >= cols - 1 || y < 1 || y >= rows - 2)
            continue; /* keep border clear */

        float s = (sinf(st->phase) + 1.0f) * 0.5f; /* 0..1 */
        char c;
        switch (st->layer)
        {
        case 0:
            c = (s > 0.66f) ? '+' : (s > 0.33f ? '*' : '.');
            break;
        case 1:
            c = (s > 0.66f) ? '*' : (s > 0.33f ? '.' : ' ');
            break;
        default:
            c = (s > 0.5f) ? '.' : ' ';
            break;
        }
        if (c != ' ')
            mvaddch(y, x, c);
    }
}

/* -------------------------- TELEPHONE LINES -------------------------- */

/* Simple Bresenham to draw a char along a line, with optional “insulators”
   (little 'o') placed every TLINE_INS_PERIOD steps, animated by phase. */
static void draw_line_with_insulators(int x0, int y0, int x1, int y1, float phase_steps)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    /* choose glyph by direction */
    char glyph;
    if (dx > -dy)
    { /* slope shallow-ish */
        glyph = (sx > 0) ? TLINE_CHAR_STRAIGHT : TLINE_CHAR_STRAIGHT;
    }
    else
    {
        glyph = (sx > 0) ? TLINE_CHAR_FWD : TLINE_CHAR_BACK;
    }

    /* move along the line */
    int step = 0;
    int x = x0, y = y0;

    /* convert phase (in steps) to an offset 0..period-1 */
    int offset = (int)floorf(phase_steps) % TLINE_INS_PERIOD;
    if (offset < 0)
        offset += TLINE_INS_PERIOD;

    while (1)
    {
        if (x >= 1 && x < cols - 1 && y >= 2 && y < rows - 3)
        {
            /* insulator placement */
            if (((step + offset) % TLINE_INS_PERIOD) == 0)
            {
                mvaddch(y, x, TLINE_INS_CHAR);
            }
            else
            {
                mvaddch(y, x, glyph);
            }
        }
        if (x == x1 && y == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y += sy;
        }
        ++step;
    }
}

/* Draws multiple wires from top-left and top-right toward a vanishing point.
   The vanishing point is near/above the cathedral, so lines “come in” at angles. */
static void telephone_lines_draw(int vanishing_x, int vanishing_y, float phase)
{
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* left fan */
    int start_y = TLINE_Y_TOP_OFFSET;
    for (int i = 0; i < TLINES_PER_SIDE; ++i)
    {
        int y0 = start_y + i * TLINE_SPACING_ROWS;
        int x0 = 2; /* just inside border */
        if (y0 >= vanishing_y)
            break; /* don’t invert the line */
        draw_line_with_insulators(x0, y0, vanishing_x, vanishing_y, phase + i * 1.7f);
    }

    /* right fan */
    start_y = TLINE_Y_TOP_OFFSET + 1; /* slight stagger for variety */
    for (int i = 0; i < TLINES_PER_SIDE; ++i)
    {
        int y0 = start_y + i * TLINE_SPACING_ROWS;
        int x0 = cols - 3; /* just inside border */
        if (y0 >= vanishing_y)
            break;
        draw_line_with_insulators(x0, y0, vanishing_x, vanishing_y, phase + i * 1.7f + 0.8f);
    }
}

/* --------------------------- SCENE --------------------------- */

static bool menu_init(Scene *s)
{
    MenuScene *ms = (MenuScene *)s;
    seed_rng_once();

    /* Optional: enable color for the border/menu if terminal supports it */
    if (has_colors())
    {
        start_color();
        /* Pair 1: border/title, Pair 2: highlight */
        init_pair(TITLE_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
        init_pair(HILITE_COLOR_PAIR, COLOR_BLACK, COLOR_CYAN);
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    starfield_reseed(ms, rows, cols);
    ms->last_ms = 0;
    ms->wire_phase = 0.0f;
    return true;
}

static void menu_handle(Scene *s, int ch)
{
    MenuScene *ms = (MenuScene *)s;
    const int N = 4;
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
            s->next_scene = SCENE_HOME;
        else if (ms->highlight == 1)
            s->next_scene = SCENE_HELP;
        else if (ms->highlight == 2)
            s->next_scene = SCENE_LEVEL_PRACTICE_440;
        else if (ms->highlight == 3)
            s->next_scene = SCENE_LEVEL_BLUEBOX;
        else if (ms->highlight == 4)
            s->next_scene = SCENE_VICTORY;

    }
}

static void menu_update(Scene *s, float f, float p, uint64_t now_ms)
{
    (void)f;
    (void)p;
    MenuScene *ms = (MenuScene *)s;

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    if (ms->last_ms == 0)
        ms->last_ms = now_ms;

    float dt_sec = (float)(now_ms - ms->last_ms) / 1000.0f;
    if (dt_sec < 0.f)
        dt_sec = 0.f;
    if (dt_sec > 0.1f)
        dt_sec = 0.1f; /* clamp to avoid huge jumps on pause */

    /* Update starfield */
    starfield_update(ms, dt_sec, rows, cols);

    /* Advance phase so insulators appear to “march” along wires */
    ms->wire_phase += dt_sec * TLINE_PHASE_SPEED;
    if (ms->wire_phase > 10000.0f)
        ms->wire_phase = fmodf(ms->wire_phase, (float)TLINE_INS_PERIOD);

    ms->last_ms = now_ms;
}

static void menu_render(Scene *s)
{
    MenuScene *ms = (MenuScene *)s;
    erase();
    ui_draw_border();

    /* Title & help */
    attr_t old;
    short old_pair;
    attr_get(&old, &old_pair, NULL);

    if (has_colors())
        attron(COLOR_PAIR(TITLE_COLOR_PAIR));
    mvprintw(1, 2, "RACE TO THE VATICAN");
    if (has_colors())
        attroff(COLOR_PAIR(TITLE_COLOR_PAIR));

    // mvprintw(2, 2, "Launch on two machines and pass --peer IP:PORT to race.");
    // mvprintw(3, 2, "Examples:");
    // mvprintw(4, 4, "./mic_term_game --name Alice --port 55555 --peer 192.168.1.42:55556");
    // mvprintw(5, 4, "./mic_term_game --name Bob   --port 55556 --peer 192.168.1.41:55555");

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    /* Background stars first (under everything else) */
    starfield_draw(ms);

    /* Cathedral bob/sway */
    int ascii_center_y = 9; /* your anchor */
    float t = (float)(ms->last_ms % 1000000ULL) / 1000.0f;
    int bob = (int)roundf(sinf(t * 2.0f * (float)M_PI * ASCII_BOB_HZ) * 1.5f);
    int sway = (int)roundf(sinf(t * 2.0f * (float)M_PI * ASCII_SWAY_HZ) * 2.0f);

    /* Where the wires converge (vanishing point) — place just above the art */
    int vanishing_y = ascii_center_y + 5 + bob + VATICAN_ASCII_LINES / 2 - 2;
    if (vanishing_y < 7)
        vanishing_y = 7;
    int vanishing_x = cols / 2 + 5 + sway; /* bias toward art’s x_offset */

    /* Draw telephone lines that angle in toward the vanishing point */
    telephone_lines_draw(vanishing_x, vanishing_y, ms->wire_phase * (float)TLINE_INS_PERIOD);

    /* Draw the cathedral */
    draw_ascii_centered(VATICAN_ASCII, VATICAN_ASCII_LINES, ascii_center_y, 5 + bob, 5 + sway);

    /* Menu items */
    const char *items[] = {
        "Begin Race",
        "How To Play",
        "Practice: Match Hz Dynamically",
        "Debug/Settings"};
    const int item_count = (int)(sizeof(items) / sizeof(items[0]));

    /* Menu anchor below the art */
    int menu_start_y = vanishing_y + 2;
    if (menu_start_y < 7)
        menu_start_y = 7;
    if (menu_start_y > rows - (item_count + 3))
        menu_start_y = rows - (item_count + 3);

    for (int i = 0; i < item_count; ++i)
    {
        if (i == ms->highlight)
        {
            if (has_colors())
                attron(COLOR_PAIR(HILITE_COLOR_PAIR));
            mvprintw(menu_start_y + i, 4, "> %s", items[i]);
            if (has_colors())
                attroff(COLOR_PAIR(HILITE_COLOR_PAIR));
        }
        else
        {
            mvprintw(menu_start_y + i, 4, "  %s", items[i]);
        }
    }

    mvprintw(LINES - 2, 2, "Arrows: navigate, Enter: select, 'q': quit.");
    attr_set(old, old_pair, NULL);
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
    ms->star_count = 0;
    ms->last_ms = 0;
    ms->last_rows = ms->last_cols = -1;
    ms->wire_phase = 0.0f;
    return (Scene *)ms;
}

