#include "ascii.h"
#include <curses.h>
#include <string.h>
#include <stdio.h>

static uint32_t clamp_u32(uint32_t v, uint32_t lo, uint32_t hi)
{
    return (v < lo) ? lo : (v > hi ? hi : v);
}

uint32_t ascii_total_cycle_ms(const AsciiArt *art)
{
    if (!art || !art->frames || art->frame_count <= 0)
        return 0;
    uint64_t sum = 0;
    for (int i = 0; i < art->frame_count; ++i)
    {
        sum += (uint64_t)(art->frames[i].duration_ms ? art->frames[i].duration_ms : 1u);
    }
    if (sum > 0xFFFFFFFFULL)
        sum = 0xFFFFFFFFULL;
    return (uint32_t)sum;
}

int ascii_pick_frame(const AsciiArt *art,
                     uint64_t start_ms,
                     uint64_t now_ms,
                     uint32_t *out_frame_elapsed_ms)
{
    if (!art || !art->frames || art->frame_count <= 0)
    {
        if (out_frame_elapsed_ms)
            *out_frame_elapsed_ms = 0;
        return 0;
    }
    uint32_t cycle = ascii_total_cycle_ms(art);
    if (cycle == 0)
    {
        if (out_frame_elapsed_ms)
            *out_frame_elapsed_ms = 0;
        return 0;
    }

    uint64_t elapsed = now_ms - start_ms;
    uint32_t in_cycle = (uint32_t)(elapsed % cycle);

    uint32_t acc = 0;
    for (int i = 0; i < art->frame_count; ++i)
    {
        uint32_t d = art->frames[i].duration_ms ? art->frames[i].duration_ms : 1u;
        if (in_cycle < acc + d)
        {
            if (out_frame_elapsed_ms)
                *out_frame_elapsed_ms = in_cycle - acc;
            return i;
        }
        acc += d;
    }
    // Fallback: last frame
    if (out_frame_elapsed_ms)
        *out_frame_elapsed_ms = 0;
    return art->frame_count - 1;
}

/* Count total characters (including newlines) in frame text */
static int frame_total_chars(const AsciiFrame *f)
{
    if (!f || !f->text)
        return 0;
    return (int)strlen(f->text);
}

/* Render up to max_chars characters of a multi-line text block at (y0,x0),
 * clipping to the current terminal size.
 */
static void render_block_clipped(const char *text,
                                 int width,
                                 int height,
                                 int y0,
                                 int x0,
                                 int max_chars, // -1 -> infinite
                                 bool center)
{
    if (!text || width <= 0 || height <= 0)
        return;

    int term_rows = LINES;
    int term_cols = COLS;

    // compute top-left if centered
    int draw_y = y0;
    int draw_x = x0;
    if (center)
    {
        draw_y = y0 - (height / 2);
        draw_x = x0 - (width / 2);
    }

    const char *p = text;
    int rendered = 0;
    for (int row = 0; row < height; ++row)
    {
        if (p[0] == '\0')
            break;

        int out_y = draw_y + row;
        if (out_y < 0)
        {
            // Skip line until newline
            while (*p && *p != '\n')
            {
                if (max_chars < 0 || rendered < max_chars)
                {
                    ++rendered;
                }
                ++p;
            }
            if (*p == '\n')
            {
                if (max_chars < 0 || rendered < max_chars)
                {
                    ++rendered;
                }
                ++p;
            }
            continue;
        }
        if (out_y >= term_rows)
            break; // beyond terminal

        // Render up to "width" chars (or until newline)
        int col = 0;
        while (*p && *p != '\n' && col < width)
        {
            if (max_chars >= 0 && rendered >= max_chars)
                return;

            int out_x = draw_x + col;
            if (out_x >= 0 && out_x < term_cols)
            {
                mvaddch(out_y, out_x, (unsigned char)*p);
            }
            ++p;
            ++col;
            ++rendered;
        }

        // consume the rest of the line to newline if width shorter than text
        while (*p && *p != '\n')
        {
            if (max_chars >= 0 && rendered >= max_chars)
                return;
            ++p;
            ++rendered;
        }

        // consume newline
        if (*p == '\n')
        {
            if (max_chars >= 0 && rendered >= max_chars)
                return;
            ++p;
            ++rendered;
        }
    }
}

void ascii_draw(const AsciiArt *art,
                uint64_t start_ms,
                uint64_t now_ms,
                int origin_y,
                int origin_x,
                int center_override)
{
    if (!art || !art->frames || art->frame_count <= 0)
        return;

    int idx = ascii_pick_frame(art, start_ms, now_ms, NULL);
    if (idx < 0 || idx >= art->frame_count)
        idx = 0;

    const AsciiFrame *f = &art->frames[idx];
    if (!f->text || f->width <= 0 || f->height <= 0)
        return;

    int oy = (origin_y >= 0) ? origin_y : art->origin_y;
    int ox = (origin_x >= 0) ? origin_x : art->origin_x;
    bool ctr = (center_override >= 0) ? (center_override != 0) : art->center;

    int max_chars = -1;
    if (art->typing_cps > 0)
    {
        uint64_t el = now_ms - start_ms;
        uint64_t chars = (el * (uint64_t)art->typing_cps) / 1000ULL;
        int total = frame_total_chars(f);
        if ((uint64_t)chars < (uint64_t)total)
            max_chars = (int)chars;
        else
            max_chars = -1; // show all once complete
    }

    render_block_clipped(f->text, f->width, f->height, oy, ox, max_chars, ctr);
}
