/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/



#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spaceship.h"


int main() {

    FLAG_FULLSCREEN_MODE;

    InitWindow(750, 700, "Space Invaders");
    SetTargetFPS(60);

    Spaceship* ship = newSpaceship();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        drawSpaceship(ship);

        EndDrawing();
    }

    deleteSpaceship(ship);
    CloseWindow();


    return 0;
}