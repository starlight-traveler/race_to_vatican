#ifndef ASCII_H
#define ASCII_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct AsciiFrame
    {
        const char *text;
        int width;
        int height;
        uint32_t duration_ms;
    } AsciiFrame;

    typedef struct AsciiArt
    {
        const AsciiFrame *frames;
        int frame_count;
        uint32_t typing_cps;
        int origin_y;
        int origin_x;
        bool center;
    } AsciiArt;

    uint32_t ascii_total_cycle_ms(const AsciiArt *art);

    int ascii_pick_frame(const AsciiArt *art,
                         uint64_t start_ms,
                         uint64_t now_ms,
                         uint32_t *out_frame_elapsed_ms);

    void ascii_draw(const AsciiArt *art,
                    uint64_t start_ms,
                    uint64_t now_ms,
                    int origin_y,
                    int origin_x,
                    int center);

#ifdef __cplusplus
}
#endif

#endif /* ASCII_H */
