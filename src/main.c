#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "constants.h"
#include "objects.h"
#include "loadGame.h"
#include "teardownGame.h"
#include "resetGameState.h"
#include "processEvents.h"
#include "doGameStateCalculations.h"
#include "renderGame.h"

int main(int argc, char *argv[])
{
    Game *game = loadGame();

    bool done = false;
    while (!done)
    {
        // Events
        done = processEvents(game->window, game->gameState);

        // Logic
        doGameStateCalculations(game->gameState);

        // Render
        renderGame(game);
    }

    teardownGame(game);

    return 0;
}