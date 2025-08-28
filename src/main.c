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
    Font font = LoadFontEx("../font/monogram.ttf", 64, 0, 0);
    SetTargetFPS(60);


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
        DrawRectangleRoundedLines((Rectangle){10, 10, 780, 780}, 0.18f, 20, YELLOW);
        DrawLineEx((Vector2){25, 730},
                    (Vector2){775, 730},
                    3.0f,
                    YELLOW);
        DrawTextEx(font, "Level 01", (Vector2){570, 740}, 34, 2, YELLOW);
        drawGame();

        EndDrawing();
    }


    delete_game();
    CloseWindow();
    return 0;
}