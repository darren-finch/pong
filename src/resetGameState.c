#include "objects.h"
#include "constants.h"

void resetGameState(GameState *gameState, bool gameOver)
{
    gameState->gameOver = gameOver;
    gameState->player1Score = 0;
    gameState->player2Score = 0;

    gameState->paddles[PLAYER_1].x = PADDLE_MARGIN;
    gameState->paddles[PLAYER_1].y = SCREEN_HEIGHT / 2;
    gameState->paddles[PLAYER_1].w = PADDLE_WIDTH;
    gameState->paddles[PLAYER_1].h = PADDLE_HEIGHT;
    gameState->paddles[PLAYER_1].vy = 0;

    gameState->paddles[PLAYER_2].x = SCREEN_WIDTH - PADDLE_MARGIN;
    gameState->paddles[PLAYER_2].y = SCREEN_HEIGHT / 2;
    gameState->paddles[PLAYER_2].w = PADDLE_WIDTH;
    gameState->paddles[PLAYER_2].h = PADDLE_HEIGHT;
    gameState->paddles[PLAYER_2].vy = 0;

    gameState->ball.x = SCREEN_WIDTH / 2;
    gameState->ball.y = SCREEN_HEIGHT / 2;
    gameState->ball.w = BALL_WIDTH;
    gameState->ball.h = BALL_HEIGHT;

    if (gameOver)
    {
        gameState->ball.vx = 0;
        gameState->ball.vy = 0;
    }
    else
    {
        gameState->ball.vx = BALL_VELOCITY * (random() % 2 == 0 ? 1 : -1);
        gameState->ball.vy = BALL_VELOCITY * (random() % 2 == 0 ? 1 : -1);
    }
}