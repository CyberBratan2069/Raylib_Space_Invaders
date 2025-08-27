/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/

#ifndef SPACEINVADERS_SPACESHIP_H
#define SPACEINVADERS_SPACESHIP_H

#pragma once
#include <raylib.h>


typedef struct Spaceship {
    Texture2D image;
    Vector2   position;
} Spaceship;


Spaceship* newSpaceship();
void deleteSpaceship(Spaceship* s);
void drawSpaceship(Spaceship* s);
void moveLeft(Spaceship* s);
void moveRight(Spaceship* s);
void fireLaser(Spaceship* s);



#endif //SPACEINVADERS_SPACESHIP_H
