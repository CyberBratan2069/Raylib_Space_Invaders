/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include "laser.h"
#include <stdio.h>


void updateLaser(Laser* laser) {
    if(!laser || !laser->active) return;
    laser->position.y += laser->speed;
    if(laser->position.y > GetScreenHeight() || laser->position.y < 0) {
        laser->active = false;
        //printf("laser inactive\n");
    }
}

void drawLaser(const Laser* laser) {
    DrawRectangle((int)laser->position.x, (int)laser->position.y, 4, 15, (Color){243, 216, 63, 255});
}
