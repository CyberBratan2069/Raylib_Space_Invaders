/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include <stdlib.h>
#include "alien.h"


Alien* alien = NULL;
Texture2D alienImages[3] = {};

Alien* new_alien(int type, Vector2 position) {
    if(type < 1 || type > 3) return NULL;

    alien = malloc(sizeof *alien);
    if(!alien) return NULL;

    alien->type     = type;
    alien->position = position;
    alien->image.id = 0;

    switch(type) {
        case 1: alien->image = LoadTexture("../graphics/alien_1.png"); break;
        case 2: alien->image = LoadTexture("../graphics/alien_2.png"); break;
        case 3: alien->image = LoadTexture("../graphics/alien_3.png"); break;
        default: alien->image = LoadTexture("../graphics/alien_1.png"); break;
    }

    if(alien->image.id == 0) {
        free(alien);
        return NULL;
    }

    return alien;
}


void updateAlien(int direction) {
    if(!alien) return;
    alien->position.x += (float)direction;

}


void delete_alien() {
    if(!alien) return;

    if(alien->image.id != 0) {
        TraceLog(LOG_INFO, "delete_alien(): UnloadTexture id=%u", alien->image.id);
        UnloadTexture(alien->image);
        alien->image.id = 0;
    }

    free(alien);
    alien = NULL;
}


void drawAlien() {
    if(!alien || alien->image.id == 0) return;
    DrawTextureV(alien->image, alien->position, WHITE);
}


int getAlienType() {
    if(!alien) return 0;
    return alien->type;
}