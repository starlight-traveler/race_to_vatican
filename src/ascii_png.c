// ascii_png.c
// Load a PNG and render it as ASCII art into an ncurses window.
// Build with: -lncursesw -lpng

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <png.h>
#include <curses.h>

/* Map luminance [0..255] to ASCII characters (dark->bright). */
static const char *ASCII_RAMP = " .:-=+*#%@";
static const int ASCII_RAMP_LEN = 10;

/* Simple clamp */
static inline int clampi(int v, int lo, int hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

/* Convert RGBA to perceived luminance (BT.709) in [0..255]. */
static inline unsigned rgb_to_luma(unsigned r, unsigned g, unsigned b)
{
    double y = 0.2126 * r + 0.7152 * g + 0.0722 * b;
    if (y < 0)
        y = 0;
    if (y > 255)
        y = 255;
    return (unsigned)(y + 0.5);
}

/* Read a PNG file into an 8-bit RGBA buffer using libpng.
 * On success, returns 1 and sets out_*; caller must free(*out_rgba).
 * On failure, returns 0.
 */
static int load_png_rgba(const char *path,
                         unsigned char **out_rgba,
                         int *out_w, int *out_h, int *out_stride)
{
    FILE *fp = fopen(path, "rb");
    if (!fp)
        return 0;

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fclose(fp);
        return 0;
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 0;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    png_uint_32 width = png_get_image_width(png, info);
    png_uint_32 height = png_get_image_height(png, info);
    png_uint_32 bit_depth = png_get_bit_depth(png, info);
    png_uint_32 color_type = png_get_color_type(png, info);

    // Expand to 8-bit RGBA
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);

    // Ensure we have RGB
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png);
    }

    // Ensure we have alpha
    if (!(color_type & PNG_COLOR_MASK_ALPHA))
    {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    png_read_update_info(png, info);

    png_size_t rowbytes = png_get_rowbytes(png, info);
    int stride = (int)rowbytes;
    int w = (int)width;
    int h = (int)height;

    unsigned char *rgba = (unsigned char *)malloc((size_t)stride * h);
    if (!rgba)
    {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 0;
    }

    png_bytep *rowptrs = (png_bytep *)malloc(sizeof(png_bytep) * h);
    if (!rowptrs)
    {
        free(rgba);
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        return 0;
    }

    for (int y = 0; y < h; ++y)
        rowptrs[y] = rgba + (size_t)y * stride;
    png_read_image(png, rowptrs);
    png_read_end(png, info);

    free(rowptrs);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    *out_rgba = rgba;
    *out_w = w;
    *out_h = h;
    *out_stride = stride;
    return 1;
}

/* Draw the PNG (converted to ASCII) into stdscr at (top,left).
 * max_cols/max_rows bound the printed size (in character cells).
 * If either max_cols or max_rows is <= 0, it auto-fits to available screen
 * from (top,left).
 *
 * Returns 1 on success, 0 on failure.
 */
int draw_png_ascii(const char *png_path, int top, int left, int max_cols, int max_rows)
{
    unsigned char *rgba = NULL;
    int img_w = 0, img_h = 0, stride = 0;

    if (!load_png_rgba(png_path, &rgba, &img_w, &img_h, &stride))
    {
        return 0;
    }

    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    // Available printing region
    int avail_cols = clampi(cols - left, 0, cols);
    int avail_rows = clampi(rows - top, 0, rows);

    if (max_cols <= 0 || max_cols > avail_cols)
        max_cols = avail_cols;
    if (max_rows <= 0 || max_rows > avail_rows)
        max_rows = avail_rows;

    if (max_cols <= 0 || max_rows <= 0)
    {
        free(rgba);
        return 0; // no space to draw
    }

    // Preserve aspect ratio: terminal character cells are taller than they are wide.
    // A common heuristic is to scale image height by ~0.5 when mapping pixels->rows.
    // We'll adjust target rows to keep aspect pleasant.
    double cell_aspect = 0.5; // tweak as desired (0.5..0.6 looks decent)
    double sx = (double)img_w / (double)max_cols;
    double sy = (double)img_h / (double)max_rows / cell_aspect;

    double scale = sx > sy ? sx : sy; // choose the limiting scale to fit
    int out_w = (int)floor((double)img_w / scale);
    int out_h = (int)floor((double)img_h / scale * cell_aspect);

    if (out_w < 1)
        out_w = 1;
    if (out_h < 1)
        out_h = 1;
    if (out_w > max_cols)
        out_w = max_cols;
    if (out_h > max_rows)
        out_h = max_rows;

    // Render line by line
    // We’ll reuse a buffer to hold one line of ASCII chars (+NUL)
    char *linebuf = (char *)malloc((size_t)out_w + 1);
    if (!linebuf)
    {
        free(rgba);
        return 0;
    }

    for (int y = 0; y < out_h; ++y)
    {
        // Map output row y to source row
        double src_y = (double)y * scale / cell_aspect;
        int iy = (int)floor(src_y);
        if (iy >= img_h)
            iy = img_h - 1;
        const unsigned char *rowp = rgba + (size_t)iy * stride;

        for (int x = 0; x < out_w; ++x)
        {
            double src_x = (double)x * scale;
            int ix = (int)floor(src_x);
            if (ix >= img_w)
                ix = img_w - 1;

            const unsigned char *px = rowp + ix * 4; // RGBA
            unsigned r = px[0], g = px[1], b = px[2], a = px[3];

            if (a == 0)
            {
                linebuf[x] = ' '; // transparent -> space
            }
            else
            {
                unsigned y255 = rgb_to_luma(r, g, b);
                int idx = (int)((y255 * (ASCII_RAMP_LEN - 1)) / 255.0 + 0.5);
                linebuf[x] = ASCII_RAMP[clampi(idx, 0, ASCII_RAMP_LEN - 1)];
            }
        }
        linebuf[out_w] = '\0';

        // Clip if needed and print
        if (top + y >= 0 && top + y < rows)
        {
            // If line would run off the right edge, ncurses will clip mvaddnstr
            mvaddnstr(top + y, left, linebuf, out_w);
        }
    }

    free(linebuf);
    free(rgba);
    return 1;
}
