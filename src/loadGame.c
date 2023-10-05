#include <SDL2/SDL.h>
#include "objects.h"
#include "constants.h"
#include "resetGameState.h"

SDL_Window *createWindow()
{
    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    return window;
}

SDL_Renderer *createRenderer(SDL_Window *window)
{
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    return renderer;
}

TTF_Font *loadFont(char *path, int size)
{
    TTF_Font *font = TTF_OpenFont(path, size);
    if (font == NULL)
    {
        printf("Failed to load font: %s\n", TTF_GetError());
        return NULL;
    }

    return font;
}

GameState *initializeGameState()
{
    GameState *gameState = malloc(sizeof(GameState));

    resetGameState(gameState);

    return gameState;
}

Game *loadGame()
{
    srandom(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    Game *game = malloc(sizeof(Game));

    game->window = createWindow();
    game->renderer = createRenderer(game->window);
    game->font = loadFont("assets/fonts/VT323-Regular.ttf", 48);
    game->gameState = initializeGameState();

    return game;
}