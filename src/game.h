/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include "spaceship.h"
#include "laser_vec.h"

#pragma once

typedef struct Game {

    LaserVec lasers;
} Game;

Game* new_game();
void delete_game();
void updateGame();
void drawGame();
void handleInput();

void deleteInactiveLasers();

extern Game* game;
