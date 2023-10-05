#include <SDL2/SDL.h>
#include "objects.h"

void teardownGame(Game *game)
{
    free(game->gameOptions);
    free(game->gameState);
    free(game->font);
    SDL_DestroyRenderer(game->renderer);
    SDL_DestroyWindow(game->window);
    free(game);
    SDL_Quit();
}