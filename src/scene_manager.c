#include "scene_manager.h"
#include <stdlib.h>

void scene_manager_init(SceneManager *sm, Scene *initial)
{
    sm->current = initial;
    if (sm->current && sm->current->init)
        sm->current->init(sm->current);
}

void scene_manager_switch(SceneManager *sm, Scene *next)
{
    if (!sm)
        return;
    if (sm->current && sm->current->destroy)
        sm->current->destroy(sm->current);
    free(sm->current);
    sm->current = next;
    if (sm->current && sm->current->init)
        sm->current->init(sm->current);
}

void scene_manager_destroy(SceneManager *sm)
{
    if (!sm)
        return;
    if (sm->current && sm->current->destroy)
        sm->current->destroy(sm->current);
    free(sm->current);
    sm->current = NULL;
}
