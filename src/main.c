#include <stdio.h>
#include <stdbool.h>
#include <curses.h>
#include <time.h>

#include "config.h"
#include "audio.h"
#include "dsp.h"
#include "ui.h"
#include "scene_manager.h"

int main(void)
{
    // ---- Audio ----
    AudioSystem audio;
    if (!audio_init(&audio))
        return 1;
    if (!audio_start(&audio))
    {
        audio_shutdown(&audio);
        return 1;
    }

    // ---- UI ----
    ui_init();

    // ---- Scenes ----
    SceneManager sm;
    Scene *scene = create_menu_scene();
    scene_manager_init(&sm, scene);

    // Player feedback (bins) is used inside level scenes; in menu we just render
    float window[WINDOW_SIZE];
    float freq_hz = -1.0f, peak_norm = 0.0f;

    uint64_t next_tick = cfg_now_ms();
    bool running = true;

    while (running)
    {
        // Input
        int ch = getch();
        if (ch != ERR)
        {
            if (ch == 'q' || ch == 'Q')
            {
                running = false;
                break;
            }
            if (sm.current && sm.current->handle_input)
            {
                sm.current->handle_input(sm.current, ch);
            }
        }

        uint64_t now_ms = cfg_now_ms();
        if (now_ms >= next_tick)
        {
            next_tick = now_ms + HOP_INTERVAL_MS;

            bool ok = audio_copy_latest_window(&audio, window, WINDOW_SIZE);
            if (ok)
            {
                PitchResult pr = dsp_estimate_pitch_autocorr(window, WINDOW_SIZE, (float)SAMPLE_RATE);
                freq_hz = pr.freq_hz;
                peak_norm = pr.peak_norm;
            }
            else
            {
                freq_hz = -1.0f;
                peak_norm = 0.0f;
            }

            if (sm.current && sm.current->update)
                sm.current->update(sm.current, freq_hz, peak_norm, now_ms);

            if (sm.current && sm.current->next_scene != -1)
            {
                int ns = sm.current->next_scene;
                sm.current->next_scene = -1;
                Scene *next = NULL;
                switch (ns)
                {
                case SCENE_MENU:
                    next = create_menu_scene();
                    break;
                case SCENE_LEVEL_BLUEBOX:
                    next = create_level_scene(SCENE_LEVEL_BLUEBOX);
                    break;
                case SCENE_LEVEL_PRACTICE_440:
                    next = create_level_scene(SCENE_LEVEL_PRACTICE_440);
                    break;
                case SCENE_VICTORY:
                    next = create_victory_scene();
                    break;
                default:
                    next = create_menu_scene();
                    break;
                }
                scene_manager_switch(&sm, next);
            }

            if (sm.current && sm.current->render)
                sm.current->render(sm.current);
        }

        // tiny sleep
        struct timespec ts = {0, 3 * 1000 * 1000};
        nanosleep(&ts, NULL);
    }

    ui_shutdown();
    scene_manager_destroy(&sm);
    audio_shutdown(&audio);
    return 0;
}
