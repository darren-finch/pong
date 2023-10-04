#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

typedef enum
{
    PLAYER_1 = 0,
    PLAYER_2 = 1
} Player;

typedef struct
{
    int x, y;
    int w, h;
    int vy;
} Paddle;

typedef struct
{
    int x, y;
    int w, h;
    int vx, vy;
} Ball;

typedef struct
{
    Paddle paddles[2];
    Ball ball;
} GameState;

int main(int argc, char *argv[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "Hello SDL!",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool done = false;
    SDL_Event event;
    while (!done)
    {
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
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    done = true;
                    break;
                }
            }
            break;
            case SDL_QUIT:
                done = true;
                break;
            }
        }

        // Render
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_Rect rect = {220, 140, 200, 200};
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}