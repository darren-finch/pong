#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "objects.h"
#include "constants.h"

bool checkGameOver(GameState *gameState)
{
    if (gameState->player1Score >= TARGET_SCORE || gameState->player2Score >= TARGET_SCORE)
    {
        gameState->gameOver = true;
        return true;
    }
    return false;
}

void checkCollisions(GameState *gameState)
{
    // Check for collision with top and bottom of screen
    if (gameState->ball.y <= 0 || gameState->ball.y >= SCREEN_HEIGHT)
    {
        gameState->ball.vy *= -1;
    }

    // Check if ball went behind player 1 and increment score
    if (gameState->ball.x <= PADDLE_MARGIN)
    {
        gameState->player2Score++;
        gameState->ball.x = SCREEN_WIDTH / 2;
        gameState->ball.y = SCREEN_HEIGHT / 2;
        gameState->ball.vx = BALL_VELOCITY;
        gameState->ball.vy = BALL_VELOCITY;
    }

    // Check if ball went behind player 2 and increment score
    if (gameState->ball.x >= SCREEN_WIDTH - PADDLE_MARGIN)
    {
        gameState->player1Score++;
        gameState->ball.x = SCREEN_WIDTH / 2;
        gameState->ball.y = SCREEN_HEIGHT / 2;
        gameState->ball.vx = -BALL_VELOCITY;
        gameState->ball.vy = BALL_VELOCITY;
    }

    bool paddleCollision = false;
    // Check for collision with paddles
    if (gameState->ball.x <= gameState->paddles[PLAYER_1].x + (gameState->paddles[PLAYER_1].w / 2) &&
        gameState->ball.y >= gameState->paddles[PLAYER_1].y - (gameState->paddles[PLAYER_1].h / 2) &&
        gameState->ball.y <= gameState->paddles[PLAYER_1].y + (gameState->paddles[PLAYER_1].h / 2))
    {
        paddleCollision = true;
    }
    else if (gameState->ball.x >= gameState->paddles[PLAYER_2].x - (gameState->paddles[PLAYER_2].w / 2) &&
             gameState->ball.y >= gameState->paddles[PLAYER_2].y - (gameState->paddles[PLAYER_2].h / 2) &&
             gameState->ball.y <= gameState->paddles[PLAYER_2].y + (gameState->paddles[PLAYER_2].h / 2))
    {
        paddleCollision = true;
    }

    if (paddleCollision)
    {
        // Increment ball velocity, capping at max velocity. Then invert x velocity.
        if (gameState->ball.vx < 0)
        {
            gameState->ball.vx += -BALL_VELOCITY_INCREMENT;
        }
        else
        {
            gameState->ball.vx += BALL_VELOCITY_INCREMENT;
        }
        if (gameState->ball.vx < -MAX_BALL_VELOCITY)
        {
            gameState->ball.vx = -MAX_BALL_VELOCITY;
        }
        else if (gameState->ball.vx > MAX_BALL_VELOCITY)
        {
            gameState->ball.vx = MAX_BALL_VELOCITY;
        }
        gameState->ball.vx *= -1;

        // Randomly invert y velocity
        if (rand() % 2 == 0)
        {
            gameState->ball.vy *= -1;
        }
    }

    // Check for collision with top and bottom of paddles
    if (gameState->paddles[PLAYER_1].y - (gameState->paddles[PLAYER_1].h / 2) < 0)
    {
        gameState->paddles[PLAYER_1].y = gameState->paddles[PLAYER_1].h / 2;
    }
    else if (gameState->paddles[PLAYER_1].y + (gameState->paddles[PLAYER_1].h / 2) > SCREEN_HEIGHT)
    {
        gameState->paddles[PLAYER_1].y = SCREEN_HEIGHT - (gameState->paddles[PLAYER_1].h / 2);
    }

    if (gameState->paddles[PLAYER_2].y - (gameState->paddles[PLAYER_2].h / 2) < 0)
    {
        gameState->paddles[PLAYER_2].y = gameState->paddles[PLAYER_2].h / 2;
    }
    else if (gameState->paddles[PLAYER_2].y + (gameState->paddles[PLAYER_2].h / 2) > SCREEN_HEIGHT)
    {
        gameState->paddles[PLAYER_2].y = SCREEN_HEIGHT - (gameState->paddles[PLAYER_2].h / 2);
    }
}

void calculateAIMove(GameState *gameState, GameOptions *gameOptions)
{
    switch (gameOptions->difficulty)
    {
    case EASY:
    {
        if (gameState->ball.vx > 0 && gameState->ball.x > (SCREEN_WIDTH / 3) * 2)
        {
            if (gameState->ball.y < gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = -PADDLE_VELOCITY;
            }
            else if (gameState->ball.y > gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = PADDLE_VELOCITY;
            }
            else
            {
                gameState->paddles[PLAYER_2].vy = 0;
            }
        }
        else
        {
            gameState->paddles[PLAYER_2].vy = 0;
        }
    }
    break;
    case MEDIUM:
    {
        if (gameState->ball.vx > 0 && gameState->ball.x > SCREEN_WIDTH / 2)
        {
            if (gameState->ball.y < gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = -PADDLE_VELOCITY;
            }
            else if (gameState->ball.y > gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = PADDLE_VELOCITY;
            }
            else
            {
                gameState->paddles[PLAYER_2].vy = 0;
            }
        }
        else
        {
            gameState->paddles[PLAYER_2].vy = 0;
        }
    }
    break;
    case HARD:
    {
        if (gameState->ball.vx > 0 && gameState->ball.x > SCREEN_WIDTH / 6)
        {
            if (gameState->ball.y < gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = -PADDLE_VELOCITY;
            }
            else if (gameState->ball.y > gameState->paddles[PLAYER_2].y)
            {
                gameState->paddles[PLAYER_2].vy = PADDLE_VELOCITY;
            }
            else
            {
                gameState->paddles[PLAYER_2].vy = 0;
            }
        }
        else
        {
            gameState->paddles[PLAYER_2].vy = 0;
        }
    }
    break;
    }
}

void moveGameObjects(GameState *gameState)
{
    gameState->paddles[PLAYER_1].y += gameState->paddles[PLAYER_1].vy;
    gameState->paddles[PLAYER_2].y += gameState->paddles[PLAYER_2].vy;

    gameState->ball.x += gameState->ball.vx;
    gameState->ball.y += gameState->ball.vy;
}

void doGameStateCalculations(GameState *gameState, GameOptions *gameOptions)
{
    if (checkGameOver(gameState))
    {
        return;
    }

    checkCollisions(gameState);
    calculateAIMove(gameState, gameOptions);
    moveGameObjects(gameState);
}