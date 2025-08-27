/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/


#include "spaceship.h"
#include "game.h"
#include "laser.h"

#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Spaceship* spaceship = NULL;

Spaceship* new_spaceship() {
    if(spaceship) return spaceship;
    spaceship = malloc(sizeof *spaceship);
    if(!spaceship) return NULL;

    spaceship->image = LoadTexture("../graphics/spaceship.png");
    if(spaceship->image.id == 0) {
        free(spaceship);
        spaceship = NULL;
        return NULL;
    }

    spaceship->position.x = (GetScreenWidth()  - spaceship->image.width)  / 2.0f;
    spaceship->position.y = (GetScreenHeight() - spaceship->image.height) - 20.0f;

    spaceship->lastFireTime = 0.0;

    return spaceship;
}


void delete_spaceship() {
    if(!spaceship) return;

    UnloadTexture(spaceship->image);
    free(spaceship);
    spaceship = NULL;
}


void drawSpaceship() {
    if(!spaceship) return;
    DrawTextureV(spaceship->image, spaceship->position, WHITE);
}

void moveLeft() {
    spaceship->position.x -= 10.0f;
    if(spaceship->position.x < 0.0f) {
        spaceship->position.x = 0.0f;
    }
}


void moveRight() {
    spaceship->position.x += 10.0f;
    if(spaceship->position.x > GetScreenWidth() - spaceship->image.width) {
        spaceship->position.x = GetScreenWidth() - spaceship->image.width;
    }
}

void fireLaser() {
    if(GetTime() - spaceship->lastFireTime >= 0.35f) {
        Laser laser;
        laser.position = (Vector2){spaceship->position.x + spaceship->image.width / 2.0f - 2.0f, spaceship->position.y};
        laser.speed = -10;
        laser.active = true;
        lasers_push(&game->lasers, laser);
        spaceship->lastFireTime = GetTime();
    }
}






