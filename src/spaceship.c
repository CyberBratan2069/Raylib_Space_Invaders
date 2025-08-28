/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/


#include "spaceship.h"
#include "game.h"
#include "laser.h"

#include <raylib.h>
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

    spaceship->position.x = (float)(GetScreenWidth()  - spaceship->image.width)  / 2.0f;
    spaceship->position.y = (float)(GetScreenHeight() - spaceship->image.height) - 100.0f;

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
    spaceship->position.x -= 7.0f;
    if(spaceship->position.x < 25.0f) {
        spaceship->position.x = 25.0f;
    }
}


void moveRight() {
    spaceship->position.x += 7.0f;
    if(spaceship->position.x > (float)GetScreenWidth() - (float)spaceship->image.width - 25) {
        spaceship->position.x = (float)GetScreenWidth() - (float)spaceship->image.width - 25;
    }
}


void fireLaser() {
    if(GetTime() - spaceship->lastFireTime >= 0.35f) {
        Laser laser;
        laser.position = (Vector2){spaceship->position.x + (float)spaceship->image.width / 2.0f - 2.0f, spaceship->position.y};
        laser.speed = -10;
        laser.active = true;
        lasers_push(&game->lasers, laser);
        spaceship->lastFireTime = GetTime();
    }
}


Rectangle hitboxSpaceship() {
    if(!spaceship) return (Rectangle){0};
    return (Rectangle) {
        spaceship->position.x,
        spaceship->position.y,
        (float)spaceship->image.width,
        (float)spaceship->image.height
    };
}