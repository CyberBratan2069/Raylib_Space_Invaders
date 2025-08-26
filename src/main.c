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

    InitWindow(755, 700, "Space Invaders");
    SetTargetFPS(60);

    Vector2 startPos = {100, 100};
    Spaceship* ship = newSpaceship("../graphics/spaceship.png", startPos);



    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(GRAY);

        DrawTexture(ship->image, ship->position.x, ship->position.y, WHITE);

        EndDrawing();
    }

    CloseWindow();


    return 0;
}