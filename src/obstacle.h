/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/

#pragma once
#include "block.h"

#include <stdint.h>
#include <raylib.h>

#define OBSTACLE_H 14
#define OBSTACLE_W 23


typedef struct {
    Vector2 position;
    uint8_t grid[OBSTACLE_H][OBSTACLE_W];
} Obstacle;


Obstacle* new_obstacle(Vector2 position);
void drawObstacle(const Obstacle* obstacle);


extern Obstacle* obstacle;