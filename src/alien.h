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
void delete_alien(Alien* alien);
int getAlienType(const Alien* alien);
void updateAlien(int direction);
void drawAlien();


extern Alien* alien;