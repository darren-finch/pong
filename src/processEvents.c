#include <stdbool.h>
#include <SDL2/SDL.h>
#include "objects.h"
#include "constants.h"
#include "resetGameState.h"

bool processEvents(SDL_Window *window, GameState *gameState, GameOptions *gameOptions)
{
    bool done = false;
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = true;
            }
        }
        break;
        case SDL_QUIT:
        {
            done = true;
        }
        break;
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_1 && gameState->gameOver)
            {
                gameOptions->difficulty = EASY;
                resetGameState(gameState, false);
            }
            else if (event.key.keysym.sym == SDLK_2 && gameState->gameOver)
            {
                gameOptions->difficulty = MEDIUM;
                resetGameState(gameState, false);
            }
            else if (event.key.keysym.sym == SDLK_3 && gameState->gameOver)
            {
                gameOptions->difficulty = HARD;
                resetGameState(gameState, false);
            }
        }
        break;
        }
    }

    if (!gameState->gameOver)
    {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W])
        {
            gameState->paddles[PLAYER_1].vy = -PADDLE_VELOCITY;
        }
        else if (state[SDL_SCANCODE_S])
        {
            gameState->paddles[PLAYER_1].vy = PADDLE_VELOCITY;
        }
        else
        {
            gameState->paddles[PLAYER_1].vy = 0;
        }

        if (state[SDL_SCANCODE_I])
        {
            gameState->paddles[PLAYER_2].vy = -PADDLE_VELOCITY;
        }
        else if (state[SDL_SCANCODE_K])
        {
            gameState->paddles[PLAYER_2].vy = PADDLE_VELOCITY;
        }
        else
        {
            gameState->paddles[PLAYER_2].vy = 0;
        }
    }

    return done;
}