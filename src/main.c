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


    Vector2 startPos = {100, 100};
    Spaceship* spaceship = newSpaceship("../graphics/spaceship.png", startPos);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GRAY);

        draw(spaceship);

        EndDrawing();
    }

    deleteSpaceship(spaceship);
    CloseWindow();


    return 0;
}