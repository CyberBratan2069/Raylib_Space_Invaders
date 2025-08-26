/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/


#include "spaceship.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Spaceship* newSpaceship(const char* path, Vector2 position) {
    Spaceship* spaceship = malloc(sizeof *spaceship);
    if(!spaceship) return NULL;

    spaceship->image = LoadTexture(path);
    if(spaceship->image.id == 0) {
        free(spaceship);
        return NULL;
    }

    spaceship->position = position;

    return spaceship;
}


void deleteSpaceship(Spaceship* spaceship) {
    if(spaceship->image.id) {
        UnloadTexture(spaceship->image);
        free(spaceship);
    }
}


void draw(Spaceship* spaceship) {
    DrawTexture(spaceship->image, spaceship->position.x, spaceship->position.y, WHITE);
}






