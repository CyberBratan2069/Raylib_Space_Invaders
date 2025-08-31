/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include "laser.h"


void updateLaser(Laser* laser) {
    if(!laser || !laser->active) return;
    laser->position.y += (float)laser->speed;
    if(laser->position.y > (float)GetScreenHeight() - 100 || laser->position.y < 25) {
        laser->active = false;

    }
}


void drawLaser(const Laser* laser) {
    if(!laser || !laser->active) return;
    DrawRectangle((int)laser->position.x, (int)laser->position.y, 4, 15, (Color){243, 216, 63, 255});
}


Rectangle hitboxLaser(const Laser* laser) {
    if(!laser || !laser->active) return (Rectangle){0};

    Rectangle rectangle;
    rectangle.x = laser->position.x;
    rectangle.y = laser->position.y;
    rectangle.width = 4;
    rectangle.height = 15;
    return rectangle;
}

