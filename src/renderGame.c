#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "objects.h"
#include "constants.h"

void renderPlayer1(SDL_Renderer *renderer, GameState *gameState)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect player1Rect = {
        gameState->paddles[PLAYER_1].x - (gameState->paddles[PLAYER_1].w / 2),
        gameState->paddles[PLAYER_1].y - (gameState->paddles[PLAYER_1].h / 2),
        gameState->paddles[PLAYER_1].w,
        gameState->paddles[PLAYER_1].h};
    SDL_RenderFillRect(renderer, &player1Rect);
}

void renderPlayer2(SDL_Renderer *renderer, GameState *gameState)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect player2Rect = {
        gameState->paddles[PLAYER_2].x - (gameState->paddles[PLAYER_2].w / 2),
        gameState->paddles[PLAYER_2].y - (gameState->paddles[PLAYER_2].h / 2),
        gameState->paddles[PLAYER_2].w,
        gameState->paddles[PLAYER_2].h};
    SDL_RenderFillRect(renderer, &player2Rect);
}

void renderBall(SDL_Renderer *renderer, GameState *gameState)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect ballRect = {
        gameState->ball.x - (gameState->ball.w / 2),
        gameState->ball.y - (gameState->ball.h / 2),
        gameState->ball.w,
        gameState->ball.h};
    SDL_RenderFillRect(renderer, &ballRect);
}

// Draw a dashed separator line with some thickness. Ensure there is margin between dashes.
void renderDashedSeparatorLine(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    int dashCount = SCREEN_HEIGHT / (SEPARATOR_LINE_DASH_HEIGHT + SEPARATOR_LINE_DASH_MARGIN);

    for (int i = 0; i < dashCount; i++)
    {
        SDL_Rect dashRect = {
            SCREEN_WIDTH / 2 - (SEPARATOR_LINE_WIDTH / 2),
            i * (SEPARATOR_LINE_DASH_HEIGHT + SEPARATOR_LINE_DASH_MARGIN),
            SEPARATOR_LINE_WIDTH,
            SEPARATOR_LINE_DASH_HEIGHT};
        SDL_RenderFillRect(renderer, &dashRect);
    }
}

void renderText(SDL_Renderer *renderer, TTF_Font *font, GameState *gameState)
{
    char *scoreString = NULL;
    if (gameState->gameOver)
    {
        if (gameState->player1Score > gameState->player2Score)
        {
            int scoreStringLength = snprintf(NULL, 0, "P1 Wins! Press 1, 2, or 3 to restart.");
            scoreString = malloc(scoreStringLength + 1);
            sprintf(scoreString, "P1 Wins! Press 1, 2, or 3 to restart.");
        }
        else if (gameState->player1Score < gameState->player2Score)
        {
            int scoreStringLength = snprintf(NULL, 0, "P2 Wins! Press 1, 2, or 3 to restart.");
            scoreString = malloc(scoreStringLength + 1);
            sprintf(scoreString, "P2 Wins! Press 1, 2, or 3 to restart.");
        }
        else
        {
            int startGameStringLength = snprintf(NULL, 0, "Press 1 for ez, 2 for med, 3 for hd.");
            scoreString = malloc(startGameStringLength + 1);
            sprintf(scoreString, "Press 1 for ez, 2 for med, 3 for hd.");
        }
    }
    else
    {
        int scoreStringLength = snprintf(NULL, 0, "%d   %d", gameState->player1Score, gameState->player2Score);
        scoreString = malloc(scoreStringLength + 1);
        sprintf(scoreString, "%d   %d", gameState->player1Score, gameState->player2Score);
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(font, scoreString, (SDL_Color){255, 255, 255, 255});
    if (textSurface == NULL)
    {
        printf("Failed to render text: %s\n", TTF_GetError());
        exit(1);
    }
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, textTexture, NULL,
                   &(SDL_Rect){
                       SCREEN_WIDTH / 2 - textSurface->w / 2,
                       20,
                       textSurface->w,
                       textSurface->h});

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    free(scoreString);
}

void renderGame(Game *game)
{
    SDL_Renderer *renderer = game->renderer;
    TTF_Font *font = game->font;
    GameState *gameState = game->gameState;

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    renderPlayer1(renderer, gameState);
    renderPlayer2(renderer, gameState);
    renderBall(renderer, gameState);

    renderDashedSeparatorLine(renderer);

    renderText(renderer, font, gameState);

    SDL_RenderPresent(renderer);
}