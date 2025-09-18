#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "scene.h"

typedef struct
{
    Scene *current;
} SceneManager;

// Factory for scenes
Scene *create_menu_scene(void);
Scene *create_help_scene(void);
Scene *create_level_scene(int scene_id); // uses g_levels[scene_id]
Scene *create_victory_scene(void);
Scene *create_defeat_scene(const char *winner_name);

void scene_manager_init(SceneManager *sm, Scene *initial);
void scene_manager_switch(SceneManager *sm, Scene *next);
void scene_manager_destroy(SceneManager *sm);

#endif
