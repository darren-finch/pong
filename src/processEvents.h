#ifndef PROCESS_EVENTS_H
#define PROCESS_EVENTS_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "objects.h"

bool processEvents(SDL_Window *window, GameState *gameState);

#endif