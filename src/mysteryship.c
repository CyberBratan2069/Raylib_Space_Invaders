/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include "mysteryship.h"

MysteryShip* mysteryShip = NULL;

MysteryShip* new_mysteryShip() {
    if(mysteryShip) return mysteryShip;

    mysteryShip = malloc(sizeof *mysteryShip);
    if(!mysteryShip) return NULL;

    mysteryShip->image = LoadTexture("../graphics/mystery.png");
    if(mysteryShip->image.id == 0) {
        free(mysteryShip);
        mysteryShip = NULL;
        return NULL;
    }

    mysteryShip->active   = false;
    mysteryShip->position = (Vector2){0.0f, 0.0f};
    mysteryShip->speed    = 0;

    return mysteryShip;
}


void delete_mysteryShip() {
    if(!mysteryShip) return;
    if(mysteryShip->image.id != 0) {
        UnloadTexture(mysteryShip->image);
        mysteryShip->image.id = 0;
    }

    free(mysteryShip);
    mysteryShip = NULL;
}


void spawnMysteryShip() {
    if(!mysteryShip) return;

    const int screenW = GetScreenWidth();
    const int baseSpeed = 3;
    const bool fromLeft = GetRandomValue(0, 1) == 0;

    mysteryShip->position.y = 50.0f;

    if(fromLeft) {
        mysteryShip->position.x = -(float)mysteryShip->image.width;
        mysteryShip->speed = +baseSpeed;
    } else {
        mysteryShip->position.x = (float)screenW;
        mysteryShip->speed = - baseSpeed;
    }

    mysteryShip->active = true;
}


void updateMysteryShip() {
    if(!mysteryShip || !mysteryShip->active) return;

    mysteryShip->position.x += (float)mysteryShip->speed;

    if(mysteryShip->speed > 0) {
        if(mysteryShip->position.x > (float)GetScreenWidth()) {
            mysteryShip->active = false;
        }
    } else {
        if (mysteryShip->position.x + (float)mysteryShip->image.width < 0.0f) {
            mysteryShip->active = false;
        }
    }
}


void drawMysteryShip() {
    if(!mysteryShip || !mysteryShip->active) return;
    DrawTextureV(mysteryShip->image, mysteryShip->position, WHITE);
}


Rectangle hitboxMysteryship() {
    if(!mysteryShip) return (Rectangle){0};

    if(mysteryShip->active) {
        return (Rectangle) {
                mysteryShip->position.x,
                mysteryShip->position.y,
                (float)mysteryShip->image.width,
                (float)mysteryShip->image.height
        };
    } else {
        return (Rectangle) {
                mysteryShip->position.x,
                mysteryShip->position.y,
                0.0f,
                0.0f
        };
    }
}