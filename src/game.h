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
    int        score;
    int        highScore;
    int        level;
    int        anyAlive;
    Music      music;
    Sound      shootLaserSound;
    Sound      alienKillingSound;

} Game;

Game* new_game();

void init_game();
void init_music();
void init_spaceship();
void init_obstacles();
void init_aliens();
void init_mysteryship();
void init_player();
void init_laser();


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
int  findBottomAlien(int col);
void alienShootLaser(void);

void checkForHitbox();

void gameOver();
void reset();

void checkForHighscore();
void safeHighscoreToFile(int highcore);
int loadHighscoreFromFile();

void currentLevel();
void nextLevel();


extern Game* game;
