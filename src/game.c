/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/

#include "game.h"
#include <stdlib.h>
#include <string.h>
#include <printf.h>


Game* game = NULL;

Game* new_game() {
    game = malloc(sizeof *game);
    if(!game) return NULL;
    if(!new_spaceship()) {
        free(game);
        game = NULL;
        return NULL;
    }
    lasers_init(&game->lasers);
    return game;
}


void delete_game() {
    delete_spaceship();
    lasers_free(&game->lasers);
    free(game);
    game = NULL;
}


void updateGame() {
    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        updateLaser(it);
    }
    lasers_compact_inactive(&game->lasers);
    deleteInactiveLasers();
    //printf("lasers: %d\n", game->lasers.size);
}

void drawGame() {
    drawSpaceship();

    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        drawLaser(it);
    }
}


void handleInput() {
    if(IsKeyDown(KEY_LEFT))  moveLeft();
    if(IsKeyDown(KEY_RIGHT)) moveRight();
    if(IsKeyDown(KEY_SPACE)) fireLaser();
}

void deleteInactiveLasers() {
    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        if(!it->active) {
            lasers_remove_unordered(&game->lasers, it - game->lasers.data);
        }
    }
}