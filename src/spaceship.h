/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 26 Aug. 2025
 **********************************************************************************************************************/


#pragma once
#include <raylib.h>
#include <stdlib.h>


typedef struct Spaceship {
    Texture2D image;
    Vector2   position;
    double    lastFireTime;
} Spaceship;


Spaceship* new_spaceship();
void delete_spaceship();
void drawSpaceship();
void moveLeft();
void moveRight();
void fireLaser();

extern Spaceship* spaceship;


