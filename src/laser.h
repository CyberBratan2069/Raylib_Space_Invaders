/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/

#pragma once

#include "spaceship.h"
#include <stdbool.h>

typedef struct Laser {
    Vector2 position;
    int     speed;
    bool    active;
} Laser;

void updateLaser(Laser* laser);
void drawLaser(const Laser* laser);
Rectangle hitboxLaser(const Laser* laser);




