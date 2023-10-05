#ifndef OBJECTS_H
#define OBJECTS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

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
    int player1Score;
    int player2Score;
    bool gameOver;
} GameState;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    TTF_Font *font;
    GameState *gameState;
} Game;

#endif