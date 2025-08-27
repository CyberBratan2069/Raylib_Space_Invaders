/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/



#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "game.h"




int main() {

    FLAG_FULLSCREEN_MODE;

    Color grey = {29, 29, 27, 255};
    InitWindow(750, 700, "Space Invaders");
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
        ClearBackground(grey);
        drawGame();
        EndDrawing();
    }


    delete_game();
    CloseWindow();
    return 0;
}