/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/

#include "game.h"
#include "alien.h"

#include <stdlib.h>
#include <string.h>
#include <printf.h>

#define ALIEN_ROWS 5
#define ALIEN_COLS 11

Game* game = NULL;

Game* new_game() {
    /// Game initialization
    game = malloc(sizeof *game);
    if(!game) return NULL;

    /// Spaceship initialization
    if(!new_spaceship()) {
        free(game);
        game = NULL;
        return NULL;
    }

    /// Obstacles initialization
    size_t count = 4;
    float obstacleWidth = OBSTACLE_W * 3.0f;
    float screenWidth   = (float)GetScreenWidth();
    float gap = (screenWidth - (count * obstacleWidth)) / (count + 1);
    Vector2 start = (Vector2){gap, 550.0f};
    float spacingX = obstacleWidth + gap;

    game->obstacles = createObstacles(start, count, spacingX);
    if(!game->obstacles) {
        delete_spaceship();
        free(game);
        game = NULL;
        return NULL;
    }

    /// Alien initialization
    game->aliens = NULL;
    game->aliensCount = 0;
    createAliens();


    game->obstaclesCount = count;
    lasers_init(&game->lasers);
    return game;
}


void delete_game() {
    delete_spaceship();
    lasers_free(&game->lasers);
    delete_Obstacles(game->obstacles, game->obstaclesCount);
    //delete_alien();
    deleteAliens();
    free(game);
    game = NULL;
}


void updateGame() {
    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        updateLaser(it);
    }

    lasers_compact_inactive(&game->lasers);
    deleteInactiveLasers();
}


void drawGame() {
    drawSpaceship();

    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        drawLaser(it);
    }

    for(size_t i=0; i< game->obstaclesCount; i++) {
        drawObstacle(game->obstacles[i]);
    }

    drawAliens();
}


void handleInput() {
    if(IsKeyDown(KEY_LEFT))  moveLeft();
    if(IsKeyDown(KEY_RIGHT)) moveRight();
    if(IsKeyDown(KEY_SPACE)) fireLaser();
}


void deleteInactiveLasers() {
    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        if(!it->active) {
            size_t idx = (size_t)(it - game->lasers.data);
            lasers_remove_unordered(&game->lasers, idx);
        } else {
            it++;
        }
    }
}


Obstacle** createObstacles(Vector2 start, size_t count, float spacingX) {
    if(count == 0) return NULL;

    Obstacle** obstaclesArr = (Obstacle**)malloc(count * sizeof * obstaclesArr);
    if(!obstaclesArr) return NULL;

    for(int i=0; i< count; i++) {
        obstaclesArr[i] = new_obstacle((Vector2){start.x + (float)i * spacingX, start.y});
        if(!obstaclesArr[i]) {
            for(size_t j=0; j< i; j++) {
                free(obstaclesArr[j]);
                //free(obstaclesArr);
                //return NULL;
            }

            free(obstaclesArr);
            return NULL;
        }
    }
    return obstaclesArr;
}


void delete_Obstacles(Obstacle** obstaclesArr, size_t count) {
    if(!obstaclesArr) return;
    for(int i=0; i< count; i++) {
        free(obstaclesArr[i]);
    }
    free(obstaclesArr);
    obstaclesArr = NULL;
}


void createAliens(void) {
    const int rows = ALIEN_ROWS;
    const int cols = ALIEN_COLS;
    float startX = 100.0f;
    float startY = 80.0f;
    float spacingX = 50.0f;
    float spacingY = 40.0f;

    size_t total = (size_t)(rows * cols);
    game->aliens = (Alien**)malloc(total * sizeof *game->aliens);
    if(!game->aliens) {
        game->aliensCount = 0;
        return;
    }

    size_t idx = 0;
    for(int i=0; i< rows; i++) {
        int type = (i % 3) + 1;
        for(int j=0; j< cols; j++) {
            Vector2 pos = (Vector2){startX + (float)j * spacingX, startY + (float)i * spacingY};
            Alien* ali = new_alien(type, pos);
            if(!ali) {
                for(size_t k=0; k< idx; k++) {
                    if(game->aliens[k]) {
                        UnloadTexture(game->aliens[k]->image);
                        free(game->aliens[k]);
                    }
                }
                free(game->aliens);
                game->aliens = NULL;
                game->aliensCount = 0;
                return;
            }
            game->aliens[idx++] = ali;
        }
    }
    game->aliensCount = idx;
}


void drawAliens(void) {
    if(!game->aliens || game->aliensCount == 0) return;
    for(size_t i=0; i< game->aliensCount; i++) {
        Alien* ali = game->aliens[i];
        if(ali && ali->image.id != 0) {
            DrawTextureV(ali->image, ali->position, WHITE);
        }
    }
}


void deleteAliens(void) {
    if(!game->aliens) return;
    for(size_t i=0; i< game->aliensCount; i++) {
        if(game->aliens[i]) {
            UnloadTexture(game->aliens[i]->image);
            free(game->aliens[i]);
        }
    }
    free(game->aliens);
    game->aliens = NULL;
    game->aliensCount = 0;
}
