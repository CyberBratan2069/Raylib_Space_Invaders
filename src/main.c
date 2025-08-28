/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/

#include <raylib.h>
#include <stdio.h>
#include "game.h"

#define OFFSET 50
#define WINDOW_WIDTH 750
#define WINDOW_HEIGHT 700
#define GREY   (Color){29, 29, 27, 255}
#define YELLOW (Color){243, 216, 63, 255}


int main() {

    SetTraceLogLevel(LOG_INFO);

    FLAG_FULLSCREEN_MODE;

    InitWindow(WINDOW_WIDTH + OFFSET, WINDOW_HEIGHT + 2 * OFFSET, "Space Invaders");
    SetTargetFPS(60);

    Image bgImg = LoadImage("../graphics/BackGround.png");
    Texture2D bg = LoadTextureFromImage(bgImg);
    UnloadImage(bgImg);
    Font font = LoadFontEx("../font/monogram.ttf", 64, 0, 0);
    Texture2D spaceshipImages = LoadTexture("../graphics/spaceship.png");


    game = new_game();
    if(!game) {
        fprintf(stderr, "Failed to initialize game\n");
        CloseWindow();
        return 1;
    }

    while (!WindowShouldClose()) {
        handleInput();
        updateGame();
        BeginDrawing();
        ClearBackground(GREY);
        Rectangle src = (Rectangle){0,0, (float)bg.width, (float)bg.height};
        Rectangle dst = (Rectangle){0,0, (float)GetScreenWidth(), (float)GetScreenHeight()};
        DrawTexturePro(bg, src, dst, (Vector2){0, 0}, 0.0f, WHITE);

        DrawRectangleRoundedLines((Rectangle){10, 10,780, 780},
                                  0.18f,
                                  20,
                                  YELLOW);

        DrawLineEx((Vector2){25, 730},
                    (Vector2){775, 730},
                    3.0f,
                    YELLOW);

        DrawTextEx(font,
                   "Level 01",
                   (Vector2){570, 740},
                   34,
                   2,
                   YELLOW);

        float x = 50.0f;
        for(int i=0; i< game->playerLives; i++) {
            DrawTextureV(spaceshipImages, (Vector2){x, 745}, WHITE);
            x += 50;
        }


        char scoreText[16];
        snprintf(scoreText, sizeof(scoreText), "%05d", game->score);
        DrawTextEx(font, "SCORE:", (Vector2){50, 15}, 34, 2, YELLOW);
        DrawTextEx(font, scoreText, (Vector2){170, 15}, 34, 2, YELLOW);

        char highscoreText[16];
        snprintf(highscoreText, sizeof(highscoreText), "%05d", game->highScore);
        DrawTextEx(font, "HIGHSCORE:", (Vector2){480, 15}, 34, 2, YELLOW);
        DrawTextEx(font, highscoreText, (Vector2){670, 15}, 34, 2, YELLOW);

        drawGame();
        EndDrawing();
    }


    delete_game();
    CloseWindow();
    CloseAudioDevice();
    return 0;
}