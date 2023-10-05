#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PADDLE_MARGIN = 20;

const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 150;

const int BALL_WIDTH = 20;
const int BALL_HEIGHT = 20;

const int PADDLE_VELOCITY = 6;
const int BALL_VELOCITY = 4;
const int BALL_VELOCITY_INCREMENT = 2;
const int MAX_BALL_VELOCITY = 16;

const int TARGET_SCORE = 10;

const int SEPARATOR_LINE_WIDTH = 5;
const int SEPARATOR_LINE_DASH_HEIGHT = 10;
const int SEPARATOR_LINE_DASH_MARGIN = 20;

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

void resetGame(GameState *game)
{
    game->gameOver = false;
    game->player1Score = 0;
    game->player2Score = 0;

    game->paddles[PLAYER_1].x = PADDLE_MARGIN;
    game->paddles[PLAYER_1].y = SCREEN_HEIGHT / 2;
    game->paddles[PLAYER_1].w = PADDLE_WIDTH;
    game->paddles[PLAYER_1].h = PADDLE_HEIGHT;
    game->paddles[PLAYER_1].vy = 0;

    game->paddles[PLAYER_2].x = SCREEN_WIDTH - PADDLE_MARGIN;
    game->paddles[PLAYER_2].y = SCREEN_HEIGHT / 2;
    game->paddles[PLAYER_2].w = PADDLE_WIDTH;
    game->paddles[PLAYER_2].h = PADDLE_HEIGHT;
    game->paddles[PLAYER_2].vy = 0;

    game->ball.x = SCREEN_WIDTH / 2;
    game->ball.y = SCREEN_HEIGHT / 2;
    game->ball.w = BALL_WIDTH;
    game->ball.h = BALL_HEIGHT;
    game->ball.vx = BALL_VELOCITY * (random() % 2 == 0 ? 1 : -1);
    game->ball.vy = BALL_VELOCITY * (random() % 2 == 0 ? 1 : -1);
}

bool processEvents(SDL_Window *window, GameState *game)
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
            if (event.key.keysym.sym == SDLK_SPACE)
            {
                resetGame(game);
            }
        }
        }
    }

    if (!game->gameOver)
    {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_W])
        {
            game->paddles[PLAYER_1].vy = -PADDLE_VELOCITY;
        }
        else if (state[SDL_SCANCODE_S])
        {
            game->paddles[PLAYER_1].vy = PADDLE_VELOCITY;
        }
        else
        {
            game->paddles[PLAYER_1].vy = 0;
        }

        if (state[SDL_SCANCODE_I])
        {
            game->paddles[PLAYER_2].vy = -PADDLE_VELOCITY;
        }
        else if (state[SDL_SCANCODE_K])
        {
            game->paddles[PLAYER_2].vy = PADDLE_VELOCITY;
        }
        else
        {
            game->paddles[PLAYER_2].vy = 0;
        }
    }

    return done;
}

void checkCollisions(GameState *game)
{
    // Check for collision with top and bottom of screen
    if (game->ball.y <= 0 || game->ball.y >= SCREEN_HEIGHT)
    {
        game->ball.vy *= -1;
    }

    // Check if ball went behind player 1 and increment score
    if (game->ball.x <= PADDLE_MARGIN)
    {
        game->player2Score++;
        game->ball.x = SCREEN_WIDTH / 2;
        game->ball.y = SCREEN_HEIGHT / 2;
        game->ball.vx = BALL_VELOCITY;
        game->ball.vy = BALL_VELOCITY;
    }

    // Check if ball went behind player 2 and increment score
    if (game->ball.x >= SCREEN_WIDTH - PADDLE_MARGIN)
    {
        game->player1Score++;
        game->ball.x = SCREEN_WIDTH / 2;
        game->ball.y = SCREEN_HEIGHT / 2;
        game->ball.vx = -BALL_VELOCITY;
        game->ball.vy = BALL_VELOCITY;
    }

    bool paddleCollision = false;
    // Check for collision with paddles
    if (game->ball.x <= game->paddles[PLAYER_1].x + (game->paddles[PLAYER_1].w / 2) &&
        game->ball.y >= game->paddles[PLAYER_1].y - (game->paddles[PLAYER_1].h / 2) &&
        game->ball.y <= game->paddles[PLAYER_1].y + (game->paddles[PLAYER_1].h / 2))
    {
        paddleCollision = true;
    }
    else if (game->ball.x >= game->paddles[PLAYER_2].x - (game->paddles[PLAYER_2].w / 2) &&
             game->ball.y >= game->paddles[PLAYER_2].y - (game->paddles[PLAYER_2].h / 2) &&
             game->ball.y <= game->paddles[PLAYER_2].y + (game->paddles[PLAYER_2].h / 2))
    {
        paddleCollision = true;
    }

    if (paddleCollision)
    {
        // Increment ball velocity, capping at max velocity. Then invert x velocity.
        if (game->ball.vx < 0)
        {
            game->ball.vx += -BALL_VELOCITY_INCREMENT;
        }
        else
        {
            game->ball.vx += BALL_VELOCITY_INCREMENT;
        }
        if (game->ball.vx < -MAX_BALL_VELOCITY)
        {
            game->ball.vx = -MAX_BALL_VELOCITY;
        }
        else if (game->ball.vx > MAX_BALL_VELOCITY)
        {
            game->ball.vx = MAX_BALL_VELOCITY;
        }
        game->ball.vx *= -1;
    }

    // Check for collision with top and bottom of paddles
    if (game->paddles[PLAYER_1].y - (game->paddles[PLAYER_1].h / 2) < 0)
    {
        game->paddles[PLAYER_1].y = game->paddles[PLAYER_1].h / 2;
    }
    else if (game->paddles[PLAYER_1].y + (game->paddles[PLAYER_1].h / 2) > SCREEN_HEIGHT)
    {
        game->paddles[PLAYER_1].y = SCREEN_HEIGHT - (game->paddles[PLAYER_1].h / 2);
    }

    if (game->paddles[PLAYER_2].y - (game->paddles[PLAYER_2].h / 2) < 0)
    {
        game->paddles[PLAYER_2].y = game->paddles[PLAYER_2].h / 2;
    }
    else if (game->paddles[PLAYER_2].y + (game->paddles[PLAYER_2].h / 2) > SCREEN_HEIGHT)
    {
        game->paddles[PLAYER_2].y = SCREEN_HEIGHT - (game->paddles[PLAYER_2].h / 2);
    }
}

void moveGameObjects(GameState *game)
{
    game->paddles[PLAYER_1].y += game->paddles[PLAYER_1].vy;
    game->paddles[PLAYER_2].y += game->paddles[PLAYER_2].vy;

    game->ball.x += game->ball.vx;
    game->ball.y += game->ball.vy;
}

bool checkGameOver(GameState *game)
{
    if (game->player1Score >= TARGET_SCORE || game->player2Score >= TARGET_SCORE)
    {
        game->gameOver = true;
        return true;
    }
    return false;
}

void doGameStateCalculations(GameState *game)
{
    if (checkGameOver(game))
    {
        return;
    }

    checkCollisions(game);
    moveGameObjects(game);
}

void renderPlayer1(SDL_Renderer *renderer, GameState *game)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect player1Rect = {
        game->paddles[PLAYER_1].x - (game->paddles[PLAYER_1].w / 2),
        game->paddles[PLAYER_1].y - (game->paddles[PLAYER_1].h / 2),
        game->paddles[PLAYER_1].w,
        game->paddles[PLAYER_1].h};
    SDL_RenderFillRect(renderer, &player1Rect);
}

void renderPlayer2(SDL_Renderer *renderer, GameState *game)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect player2Rect = {
        game->paddles[PLAYER_2].x - (game->paddles[PLAYER_2].w / 2),
        game->paddles[PLAYER_2].y - (game->paddles[PLAYER_2].h / 2),
        game->paddles[PLAYER_2].w,
        game->paddles[PLAYER_2].h};
    SDL_RenderFillRect(renderer, &player2Rect);
}

void renderBall(SDL_Renderer *renderer, GameState *game)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect ballRect = {
        game->ball.x - (game->ball.w / 2),
        game->ball.y - (game->ball.h / 2),
        game->ball.w,
        game->ball.h};
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

void renderGameState(SDL_Renderer *renderer, TTF_Font *font, GameState *game)
{
    char *scoreString = NULL;
    if (game->gameOver)
    {
        if (game->player1Score > game->player2Score)
        {
            int scoreStringLength = snprintf(NULL, 0, "Player 1 Wins! Press Space to Restart.");
            scoreString = malloc(scoreStringLength + 1);
            sprintf(scoreString, "Player 1 Wins! Press Space to Restart.");
        }
        else
        {
            int scoreStringLength = snprintf(NULL, 0, "Player 2 Wins! Press Space to Restart.");
            scoreString = malloc(scoreStringLength + 1);
            sprintf(scoreString, "Player 2 Wins! Press Space to Restart.");
        }
    }
    else
    {
        int scoreStringLength = snprintf(NULL, 0, "%d   %d", game->player1Score, game->player2Score);
        scoreString = malloc(scoreStringLength + 1);
        sprintf(scoreString, "%d   %d", game->player1Score, game->player2Score);
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

//  SDL_Surface *textSurface, SDL_Texture *textTexture,
void render(SDL_Renderer *renderer, TTF_Font *font, GameState *game)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    renderPlayer1(renderer, game);
    renderPlayer2(renderer, game);
    renderBall(renderer, game);

    renderDashedSeparatorLine(renderer);

    renderGameState(renderer, font, game);

    SDL_RenderPresent(renderer);
}

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

void teardown(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, GameState *game)
{
    free(font);
    free(game);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

GameState *initializeGame()
{
    GameState *game = malloc(sizeof(GameState));

    resetGame(game);

    return game;
}

int main(int argc, char *argv[])
{
    srandom(time(NULL));

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = createWindow();
    SDL_Renderer *renderer = createRenderer(window);

    TTF_Font *font = loadFont("assets/fonts/VT323-Regular.ttf", 48);

    GameState *game = initializeGame();

    bool done = false;
    while (!done)
    {
        // Events
        done = processEvents(window, game);

        // Logic
        doGameStateCalculations(game);

        // Render
        render(renderer, font, game);
    }

    teardown(window, renderer, font, game);

    return 0;
}