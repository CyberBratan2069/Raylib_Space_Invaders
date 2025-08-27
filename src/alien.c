/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include <stdlib.h>
#include "alien.h"


Alien* alien = NULL;

Alien* new_alien(int type, Vector2 position) {
    alien = malloc(sizeof *alien);
    if(!alien) return NULL;
    alien->type = type;
    alien->position = position;

    switch(type) {
        case 1:
            alien->image = LoadTexture("../graphics/alien_1.png");
            break;
        case 2:
            alien->image = LoadTexture("../graphics/alien_2.png");
            break;
        case 3:
            alien->image = LoadTexture("../graphics/alien_3.png");
            break;
        default:
            free(alien);
            alien = NULL;
            return NULL;
    }

    if(alien->image.id == 0) {
        free(alien);
        //alien = NULL;
        return NULL;
    }

    return alien;
}


void delete_alien() {
    if(!alien) return;

    UnloadTexture(alien->image);
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