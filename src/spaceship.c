/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/


#include "spaceship.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Spaceship* newSpaceship() {
    Spaceship* spaceship = malloc(sizeof *spaceship);
    if(!spaceship) return NULL;

    spaceship->image = LoadTexture("../graphics/spaceship.png");
    if(spaceship->image.id == 0) {
        free(spaceship);
        return NULL;
    }


    spaceship->position.x = (GetScreenWidth()  - spaceship->image.width)  / 2.0f;
    spaceship->position.y = (GetScreenHeight() - spaceship->image.height) - 20.0f;

    return spaceship;
}


void deleteSpaceship(Spaceship* spaceship) {
    if(!spaceship) return;

    if(spaceship->image.id) {
        UnloadTexture(spaceship->image);
        free(spaceship);
    }
}


void drawSpaceship(Spaceship* spaceship) {
    if(!spaceship) return;

    DrawTextureV(spaceship->image, spaceship->position, WHITE);
}






