/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include <stdlib.h>
#include "alien.h"


Alien* alien = NULL;


Alien* new_alien(const int type,const Vector2 position) {
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


Rectangle hitboxAlien() {
    if(!alien) return (Rectangle){0};
    return (Rectangle) {
            alien->position.x,
            alien->position.y,
            (float)alien->image.width,
            (float)alien->image.height
    };
}