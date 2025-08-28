/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#pragma once
#include <raylib.h>

typedef struct Alien {
    Texture2D image;
    Vector2 position;
    int type;
} Alien;

Alien* new_alien(int type, Vector2 position);
Rectangle hitboxAlien();


extern Alien* alien;