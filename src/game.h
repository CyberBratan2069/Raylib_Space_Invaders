/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/


#include "spaceship.h"
#include "laser_vec.h"
#include "obstacle.h"
#include "alien.h"
#include "mysteryship.h"

#pragma once


typedef struct Game {
    LaserVec   lasers;

    Obstacle** obstacles;
    size_t     obstaclesCount;

    Alien**    aliens;
    size_t     aliensCount;
    int        aliensDirection;

    float      mysteryShipSpawnInterval;
    float      timeLastSpawnMysteryShip;

    int        playerLives;

    bool       gameOver;

} Game;

Game* new_game();
void delete_game();
void updateGame();
void drawGame();
void handleInput();

void deleteInactiveLasers();

Obstacle** createObstacles(Vector2 start, size_t count, float spacingX);
void delete_Obstacles(Obstacle** obstaclesArr, size_t count);

void createAliens(void);
void drawAliens(void);
void deleteAliens(void);
void moveAliens(void);
void alienShootLaser(void);

void checkForHitbox();

void gameOver();
void reset();


extern Game* game;
