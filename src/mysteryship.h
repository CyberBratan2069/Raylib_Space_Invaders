/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#pragma once
#include <raylib.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct MysteryShip {
    Texture2D image;
    Vector2 position;
    int speed;
    bool active;
} MysteryShip;


MysteryShip* new_mysteryShip();
void delete_mysteryShip();
void updateMysteryShip();
void drawMysteryShip();
void spawnMysteryShip();
Rectangle hitboxMysteryship();


extern MysteryShip* mysteryShip;