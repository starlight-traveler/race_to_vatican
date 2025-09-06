#ifndef SCENE_H
#define SCENE_H

#include <stdbool.h>
#include "config.h"

// Forward decl for shared systems
struct AudioSystem;
struct Player;

// Base scene interface
typedef struct Scene
{
    bool (*init)(struct Scene *self);
    void (*handle_input)(struct Scene *self, int ch);
    void (*update)(struct Scene *self, float freq_hz, float peak_norm, uint64_t now_ms);
    void (*render)(struct Scene *self);
    void (*destroy)(struct Scene *self);
    int next_scene; // -1: stay, else request switch
} Scene;

#endif
