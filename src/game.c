/***********************************************************************************************************************
 * @author Christian Reiswich
 * @created on 27 Aug. 2025
 * @version 1.0
 **********************************************************************************************************************/

#include "game.h"
#include "alien.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>


#define ALIEN_ROWS 5
#define ALIEN_COLS 11

Game* game = NULL;

Game* new_game()
{
    init_game();
    init_music();
    init_spaceship();
    init_obstacles();
    init_aliens();
    init_mysteryship();
    init_player();
    init_laser();

    return game;
}


void init_game()
{
    game = malloc(sizeof *game);
    if(!game) return;
}


void init_music()
{
    if (!game) {
        TraceLog(LOG_ERROR, "unable to init music");
        return;
    }


    game->music = LoadMusicStream("../sounds/backgroundMusic.wav");
    PlayMusicStream(game->music);

    game->shootLaserSound   = LoadSound("../sounds/spaceshipLaserSound.wav");
    game->alienKillingSound = LoadSound("../sounds/alienKillingSound.wav");



    if(game->shootLaserSound.frameCount == 0) {
        TraceLog(LOG_WARNING, "shootLaserSound seems empty(check path");
    } else {
        SetSoundVolume(game->shootLaserSound, 0.7f);
    }

    if(game->alienKillingSound.frameCount == 0) {
        TraceLog(LOG_WARNING, "alienKillingSound seems empty(check path)");
    } else {
        SetSoundVolume(game->alienKillingSound, 2.0f);
    }
}


void init_spaceship()
{
    if(!new_spaceship()) {
        UnloadMusicStream(game->music);
        UnloadSound(game->shootLaserSound);
        UnloadSound(game->alienKillingSound);
        free(game);
        game = NULL;
    }
}


void init_obstacles()
{
    const size_t count = 4;
    const float obstacleWidth = OBSTACLE_W * 3.0f;
    const float screenWidth   = (float)GetScreenWidth();
    const float gap = (screenWidth - ((float)count * obstacleWidth)) / (float)(count + 1);
    const Vector2 start = (Vector2){gap, 550.0f};
    const float spacingX = obstacleWidth + gap;

    game->obstacles = createObstacles(start, count, spacingX);
    if(!game->obstacles) {
        delete_spaceship();
        free(game);
        game = NULL;
        return;
    }
    game->obstaclesCount = count;
}


void init_aliens()
{
    SetRandomSeed((unsigned int)time(NULL));
    game->aliens          = NULL;
    game->aliensCount     = 0;
    game->aliensDirection = 1;
    createAliens();
}


void init_mysteryship()
{
    if(!new_mysteryShip()) {
        delete_game();
        free(game);
        game = NULL;
        return;
    }
    game->timeLastSpawnMysteryShip = 0.0f;
    game->mysteryShipSpawnInterval = (float)GetRandomValue(10, 20);
}


void init_player()
{
    game->playerLives = 3;
    game->gameOver    = false;
    game->score       = 0;
    game->highScore   = loadHighscoreFromFile();
    game->level       = 1;
    game->anyAlive    = (int)game->aliensCount;
}

void init_laser()
{
    lasers_init(&game->lasers);
}



void delete_game() {
    TraceLog(LOG_INFO, "delete_game(): begin");

    UnloadMusicStream(game->music);
    UnloadSound(game->shootLaserSound);
    UnloadSound(game->alienKillingSound);

    delete_spaceship();
    lasers_free(&game->lasers);
    delete_Obstacles(game->obstacles, game->obstaclesCount);
    deleteAliens();
    delete_mysteryShip();
    free(game);
    game = NULL;
    TraceLog(LOG_INFO, "delete_game(): end");
}


void updateGame()
{
    if(!game || game->gameOver) return;

    const double currentTime = GetTime();
    if(currentTime - game->timeLastSpawnMysteryShip > game->mysteryShipSpawnInterval) {
        spawnMysteryShip();
        game->timeLastSpawnMysteryShip = (float)GetTime();
        game->mysteryShipSpawnInterval = (float)GetRandomValue(10, 20);
    }

    for(Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        updateLaser(it);
    }

    moveAliens();
    alienShootLaser();

    checkForHitbox();

    lasers_compact_inactive(&game->lasers);
    deleteInactiveLasers();

    updateMysteryShip();

    if(game->playerLives <= 0) {
        game->gameOver = true;
        return;
    }

    currentLevel();

    if(game->gameOver) {
        if(IsKeyDown(KEY_ENTER)) {
            reset();
        }
    }
}


void drawGame()
{

    drawSpaceship();

    for(const Laser* it = game->lasers.data; it< game->lasers.data + game->lasers.size; it++) {
        drawLaser(it);
    }

    for(size_t i=0; i< game->obstaclesCount; i++) {
        drawObstacle(game->obstacles[i]);
    }

    drawAliens();
    drawMysteryShip();
    drawGameOver();
}


void handleInput()
{
    if(game && game->gameOver) {
        if(IsKeyDown(KEY_ENTER)) {
            reset();
        }
        return;
    }

    if(IsKeyDown(KEY_LEFT))  moveLeft();
    if(IsKeyDown(KEY_RIGHT)) moveRight();
    if(IsKeyDown(KEY_SPACE)) fireLaser();
}


void deleteInactiveLasers()
{
    size_t i = 0;
    while (i < game->lasers.size) {
        if (!game->lasers.data[i].active) {
            lasers_remove_unordered(&game->lasers, i);
        } else {
            i++;
        }
    }


}


Obstacle** createObstacles(const Vector2 start, const size_t count, const float spacingX)
{
    if(count == 0) return NULL;

    Obstacle** obstaclesArr = malloc(count * sizeof * obstaclesArr);
    if(!obstaclesArr) return NULL;

    for(int i=0; i< count; i++) {
        obstaclesArr[i] = new_obstacle((Vector2){start.x + (float)i * spacingX, start.y + 50});
        if(!obstaclesArr[i]) {
            for(size_t j=0; j< i; j++) {
                free(obstaclesArr[j]);
            }

            free(obstaclesArr);
            return NULL;
        }
    }
    return obstaclesArr;
}


void delete_Obstacles(Obstacle** obstaclesArr,const size_t count)
{
    if(!obstaclesArr) return;
    for(int i=0; i< count; i++) {
        free(obstaclesArr[i]);
    }
    free(obstaclesArr);
    obstaclesArr = NULL;
}


void createAliens(void)
{
    const int rows = ALIEN_ROWS;
    const int cols = ALIEN_COLS;
    float startX = 100.0f;
    float startY = 80.0f;
    float spacingX = 50.0f;
    float spacingY = 50.0f;

    const size_t total = (size_t)rows * (size_t)cols;
    game->aliens = (Alien**)malloc(total * sizeof *game->aliens);
    if(!game->aliens) {
        game->aliensCount = 0;
        game->anyAlive    = 0;
        return;
    }

    size_t index = 0;
    for(int i=0; i< rows; i++) {
        int type;
        if(i == 0) type = 3;
        else if(i <= 2) type = 2;
        else type = 1;
        for(int j=0; j< cols; j++) {
            Vector2 pos = (Vector2){startX + (float)j * spacingX, startY + (float)i * spacingY};
            alien = new_alien(type, pos);
            if(!alien) {
                for(size_t k=0; k< index; k++) {
                    if(game->aliens[k]) {
                        UnloadTexture(game->aliens[k]->image);
                        free(game->aliens[k]);
                    }
                }
                free(game->aliens);
                game->aliens = NULL;
                game->aliensCount = 0;
                game->anyAlive    = 0;
                return;
            }
            game->aliens[index++] = alien;
        }
    }
    game->aliensCount = index;
    game->anyAlive    = (int)index;
}


void drawAliens(void)
{
    if(!game->aliens || game->aliensCount == 0) return;
    for(size_t i=0; i< game->aliensCount; i++) {
        alien = game->aliens[i];
        if(alien && alien->image.id != 0) {
            DrawTextureV(alien->image, alien->position, WHITE);
        }
    }
}


void deleteAliens(void)
{
    if(!game->aliens) {
        TraceLog(LOG_INFO, "deleteAliens(): game->aliens is NULL");
        return;
    }
    TraceLog(LOG_INFO, "deleteAliens(): game->aliens is not NULL", game->aliensCount);

    for(size_t i=0; i< game->aliensCount; i++) {
        if(game->aliens[i]) {
            const Texture2D tex = game->aliens[i]->image;
            if(tex.id != 0) {
                TraceLog(LOG_INFO, "deleteAliens(): UnloadTexture id=%u(%ux%u)", tex.id, tex.width, tex.height);
                UnloadTexture(tex);
            }
            free(game->aliens[i]);
        }
    }
    free(game->aliens);
    game->aliens      = NULL;
    game->aliensCount = 0;
    game->anyAlive    = 0;
    TraceLog(LOG_INFO, "deleteAliens(): end");
}


void moveAliens(void)
{
    if(!game->aliens || game->aliensCount == 0) return;

    const float speedFactor = 1.0f + 0.10f * (float)(game->level - 1); //10% Steigerung pro Level
    const float direction_x = 1.0f * speedFactor;
    const float direction_y = 10.0f;

    float minX = 1e9f;
    float maxX = -1e9f;
    int any = 0;

    for(size_t i=0; i< game->aliensCount; i++) {
        alien = game->aliens[i];
        if(!alien || alien->image.id == 0) {
            continue;
        }

        any = 1;
        if(alien->position.x < minX) {
            minX = alien->position.x;
        }

        float right = alien->position.x + (float)alien->image.width;
        if(right > maxX) maxX = right;
    }

    if(!any) return;

    const float screenWidth = (float)GetScreenWidth();

    const bool hitRight = (maxX + direction_x * (float)game->aliensDirection >= screenWidth - 25);
    const bool hitLeft  = (minX + direction_x * (float)game->aliensDirection <= 25.0f);

    if(hitRight || hitLeft) {
        game->aliensDirection *= -1;
        for(size_t i=0; i< game->aliensCount; i++) {
            alien = game->aliens[i];
            if(!alien) continue;
            alien->position.y += direction_y;
        }
    }

    for(size_t i=0; i< game->aliensCount; i++) {
        alien = game->aliens[i];
        if(!alien) continue;
        alien->position.x += direction_x * (float)game->aliensDirection;
    }

    const Rectangle ship = hitboxSpaceship();
    if (ship.width > 0 && ship.height > 0) {
        for (size_t i = 0; i < game->aliensCount; ++i) {
            Alien* a = game->aliens[i];
            if (!a || a->image.id == 0) continue;

            const Rectangle ar = (Rectangle){
                    a->position.x,
                    a->position.y,
                    (float)a->image.width,
                    (float)a->image.height
            };

            if (CheckCollisionRecs(ar, ship)) {
                game->playerLives = 0;
                game->gameOver = true;
                return;
            }
        }
    }

    for (size_t ai = 0; ai < game->aliensCount; ++ai) {
        Alien* a = game->aliens[ai];
        if (!a || a->image.id == 0) continue;

        const Rectangle ar = (Rectangle){
                a->position.x,
                a->position.y,
                (float)a->image.width,
                (float)a->image.height
        };

        for (size_t ob = 0; ob < game->obstaclesCount; ++ob) {
            Obstacle* obs = game->obstacles[ob];
            if (!obs) continue;

            const Rectangle orc = (Rectangle){
                    obs->position.x,
                    obs->position.y,
                    OBSTACLE_W * 3.0f,
                    OBSTACLE_H * 3.0f
            };

            if (!CheckCollisionRecs(orc, ar)) continue;


            const float relX0 = ar.x - obs->position.x;
            const float relY0 = ar.y - obs->position.y;
            const float relX1 = relX0 + ar.width;
            const float relY1 = relY0 + ar.height;


            const float maxW = OBSTACLE_W * 3.0f;
            const float maxH = OBSTACLE_H * 3.0f;
            const float clX0 = relX0 < 0 ? 0 : (relX0 > maxW ? maxW : relX0);
            const float clY0 = relY0 < 0 ? 0 : (relY0 > maxH ? maxH : relY0);
            const float clX1 = relX1 < 0 ? 0 : (relX1 > maxW ? maxW : relX1);
            const float clY1 = relY1 < 0 ? 0 : (relY1 > maxH ? maxH : relY1);


            int x0 = (int)(clX0 / 3.0f);
            int y0 = (int)(clY0 / 3.0f);
            int x1 = (int)((clX1 - 0.001f) / 3.0f);
            int y1 = (int)((clY1 - 0.001f) / 3.0f);

            if (x0 < 0) x0 = 0;
            if (y0 < 0) y0 = 0;
            if (x1 >= OBSTACLE_W) x1 = OBSTACLE_W - 1;
            if (y1 >= OBSTACLE_H) y1 = OBSTACLE_H - 1;

            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    if (obs->grid[y][x]) {
                        obs->grid[y][x] = 0;
                    }
                }
            }
        }
    }
}


int findBottomAlien(const int col)
{
    for(int i= ALIEN_ROWS - 1; i>= 0; i--) {
        const size_t index = (size_t)(ALIEN_COLS * i + col);
        alien = game->aliens[index];
        if(alien && alien->image.id != 0) {
            return (int)index;
        }
    }
    return -1;
}


void alienShootLaser(void)
{
    if(!game || !game->aliens || game->aliensCount == 0) return;

    const int all5Levels = 5;
    const float frequencyFactor  = 1.0f + 0.50f * (float)(game->level - 1) / (float)all5Levels; //50% alle 5 Level steigerung
    static double lastShootTime  = 0.0;
    static int    lastShooter    = -1;
    const double  shootFrequency = 0.8 / frequencyFactor;

    const double currenTime = GetTime();
    if (currenTime - lastShootTime < shootFrequency) return;

    const int maxTries = 32;
    int index    = -1;
    for(int i=0; i< maxTries; i++) {
        const int col = GetRandomValue(0, ALIEN_COLS - 1);
        const int cand = findBottomAlien(col);
        if (cand >= 0 && cand != lastShooter) {
            index = cand;
            break;
        }
    }

    if (index < 0) return;

    const Alien* shooter = game->aliens[index];
    lastShooter    = index;
    lastShootTime  = currenTime;

    Laser laser;
    laser.position = (Vector2){
            shooter->position.x + (float)shooter->image.width * 0.5f - 2.0f,
            shooter->position.y + (float)shooter->image.height
    };

    laser.speed  = 6;
    laser.active = true;

    if(!lasers_push(&game->lasers, laser)) {
        TraceLog(LOG_WARNING, "alienShootLaser(): lasers_push() failed");
    } else {
        TraceLog(LOG_INFO, "alienShootLaser(): Shot von idx=%d", index);
    }
}


void checkForHitbox()
{

    /// Spaceship lasers
    if(!game) return;

    for(size_t i=0; i< game->lasers.size; i++) {
        Laser* laser = &game->lasers.data[i];
        if(!laser->active) continue;
        if(laser->speed >= 0) continue;

        if(game->aliens && game->aliensCount > 0) {
            for(size_t al=0; al< game->aliensCount; al++) {
                alien = game->aliens[al];
                if(!alien || alien->image.id == 0) continue;

                /// Hit Alien
                if(CheckCollisionRecs(hitboxAlien(), hitboxLaser(laser))) {
                    PlaySound(game->alienKillingSound);
                    TraceLog(LOG_INFO, "KILL: Alien got hit");
                    UnloadTexture(alien->image);
                    free(alien);
                    game->aliens[al] = NULL;

                    if(game->anyAlive > 0) {
                        game->anyAlive--;
                    }

                    laser->active = false;
                    game->score += 100;
                    checkForHighscore();
                    break;
                }
            }
        }

        /// Hit MysteryShip
        if(laser->active) {
            if(CheckCollisionRecs(hitboxMysteryship(), hitboxLaser(laser))) {
                TraceLog(LOG_INFO, "KILL: MysteryShip got hit");
                mysteryShip->active = false;
                game->score += 300;
                checkForHighscore();
                PlaySound(game->alienKillingSound);
            }
        }

        if(laser->active) {
            for(size_t ob=0; ob< game->obstaclesCount; ob++) {
                obstacle = game->obstacles[ob];
                if(!obstacle) continue;

                const Rectangle lr  = hitboxLaser(laser);
                const Rectangle orc = (Rectangle){
                        obstacle->position.x,
                        obstacle->position.y,
                        OBSTACLE_W * 3.0f,
                        OBSTACLE_H * 3.0f
                };

                if(!CheckCollisionRecs(orc, lr)) continue;

                const float relX0 = lr.x - obstacle->position.x;
                const float relY0 = lr.y - obstacle->position.y;
                const float relX1 = relX0 + lr.width;
                const float relY1 = relY0 + lr.height;

                const float maxW = OBSTACLE_W * 3.0f;
                const float maxH = OBSTACLE_H * 3.0f;
                const float clX0 = relX0 < 0 ? 0 : (relX0 > maxW ? maxW : relX0);
                const float clY0 = relY0 < 0 ? 0 : (relY0 > maxH ? maxH : relY0);
                const float clX1 = relX1 < 0 ? 0 : (relX1 > maxW ? maxW : relX1);
                const float clY1 = relY1 < 0 ? 0 : (relY1 > maxH ? maxH : relY1);

                int x0 = (int)(clX0 / 3.0f);
                int y0 = (int)(clY0 / 3.0f);
                int x1 = (int)((clX1 - 0.001f) / 3.0f);
                int y1 = (int)((clY1 - 0.001f) / 3.0f);

                if (x0 < 0) x0 = 0;
                if (y0 < 0) y0 = 0;
                if (x1 >= OBSTACLE_W) x1 = OBSTACLE_W - 1;
                if (y1 >= OBSTACLE_H) y1 = OBSTACLE_H - 1;

                bool hit = false;

                for (int y = y0; y <= y1 ; ++y) {
                    for (int x = x0; x <= x1; ++x) {
                        if (obstacle->grid[y][x]) {
                            obstacle->grid[y][x] = 0;
                            hit = true;
                        }
                    }
                }

                if (hit) {
                    laser->active = false;
                    break;
                }
            }
        }
    }

    /// Alien Lasers
    for(size_t i=0; i< game->lasers.size; i++) {
        Laser* laser = &game->lasers.data[i];
        if(!laser->active) continue;
        if(laser->speed <= 0) continue;

        if(CheckCollisionRecs(hitboxSpaceship(), hitboxLaser(laser))) {
            laser->active = false;

            if(game->playerLives > 0) {
                game->playerLives--;
                TraceLog(LOG_INFO, "HIT: playerLives=%d", game->playerLives);
            }
            if(game->playerLives <= 0) {
                game->playerLives = 0;
                gameOver();
                TraceLog(LOG_INFO, "GAME OVER");
            }

            continue;
        }

        for(size_t ob=0; ob< game->obstaclesCount; ob++) {
            Obstacle* obs = game->obstacles[ob];
            if(!obs) continue;

            const Rectangle lr = hitboxLaser(laser);
            const Rectangle orc = (Rectangle){
                    obs->position.x,
                    obs->position.y,
                    OBSTACLE_W * 3.0f,
                    OBSTACLE_H * 3.0f
            };

            if(!CheckCollisionRecs(orc, lr)) continue;

            const float relX0 = lr.x - obs->position.x;
            const float relY0 = lr.y - obs->position.y;
            const float relX1 = relX0 + lr.width;
            const float relY1 = relY0 + lr.height;


            const float maxW = OBSTACLE_W * 3.0f;
            const float maxH = OBSTACLE_H * 3.0f;
            const float clX0 = relX0 < 0 ? 0 : (relX0 > maxW ? maxW : relX0);
            const float clY0 = relY0 < 0 ? 0 : (relY0 > maxH ? maxH : relY0);
            const float clX1 = relX1 < 0 ? 0 : (relX1 > maxW ? maxW : relX1);
            const float clY1 = relY1 < 0 ? 0 : (relY1 > maxH ? maxH : relY1);

            int x0 = (int)(clX0 / 3.0f);
            int y0 = (int)(clY0 / 3.0f);

            int x1 = (int)((clX1 - 0.001f) / 3.0f);
            int y1 = (int)((clY1 - 0.001f) / 3.0f);

            if (x0 < 0) x0 = 0;
            if (y0 < 0) y0 = 0;
            if (x1 >= OBSTACLE_W) x1 = OBSTACLE_W - 1;
            if (y1 >= OBSTACLE_H) y1 = OBSTACLE_H - 1;

            bool hit = false;
            for (int y = y0; y <= y1; ++y) {
                for (int x = x0; x <= x1; ++x) {
                    if (obs->grid[y][x]) {
                        obs->grid[y][x] = 0;
                        hit = true;
                    }
                }
            }

            if (hit) {
                laser->active = false;
                break;
            }
        }
    }
}


void checkForHighscore()
{
    if(game->score > game->highScore) {
        game->highScore = game->score;
        safeHighscoreToFile(game->highScore);
    }
}


void gameOver()
{
    if(!game || game->gameOver) return;
    game->gameOver = true;
    TraceLog(LOG_INFO, "gameOver(): triggered");

    for(size_t i=0; i< game->lasers.size; i++) {
        game->lasers.data[i].active = false;
    }

    if(mysteryShip) mysteryShip->active = false;
}


void drawGameOver()
{
    if (game && game->gameOver)
        {
        const char* msg  = "GAME OVER";
        const int   fs   = 48;
        const int   msgW = MeasureText(msg, fs);
        const int   mx   = (GetScreenWidth() - msgW) / 2;
        const int   my   = GetScreenHeight() / 2 - fs;
        DrawText(msg, mx+2, my+2, fs, BLACK);
        DrawText(msg, mx,   my,   fs, RED);

        const char* sub1    = "Press ESC to leave";
        const int   subFS   = 20;
        const int   sub1W   = MeasureText(sub1, subFS);
        const int   sub1X   = (GetScreenWidth() - sub1W) / 2;
        const int   sub1Y   = my + fs + 20;
        DrawText(sub1, sub1X+1, sub1Y+1, subFS, BLACK);
        DrawText(sub1, sub1X,   sub1Y,   subFS, RAYWHITE);

        const char* sub2  = "Press ENTER to restart";
        const int   sub2W = MeasureText(sub2, subFS);
        const int   sub2X = (GetScreenWidth() - sub2W) / 2;
        const int   sub2Y = sub1Y + subFS + 8;
        DrawText(sub2, sub2X+1, sub2Y+1, subFS, BLACK);
        DrawText(sub2, sub2X,   sub2Y,   subFS, RAYWHITE);
    }
}


void reset()
{
    if(game) {
        delete_game();
    }
    game = new_game();
    TraceLog(LOG_INFO, "reset(): new game");
}


void safeHighscoreToFile(int highscore)
{
    const char* path = "highscore.dat";
    FILE* f = fopen(path, "wb");
    if(!f) {
        TraceLog(LOG_WARNING, "safeHighscoreToFile(): cannot open '%' for writing", path);
        return;
    }

    const size_t w = fwrite(&highscore, sizeof(highscore), 1, f);
    if(w != 1) {
        TraceLog(LOG_WARNING, "safeHighscoreToFile(): write failed for '%s'", path);
    }
    fclose(f);
}


int loadHighscoreFromFile()
{
    const char* path = "highscore.dat";
    FILE* file = fopen(path, "rb");
    if(!file) {
        return 0;
    }

    int hs = 0;
    const size_t r = fread(&hs, sizeof(hs), 1, file);
    fclose(file);

    if(r != 1) {
        TraceLog(LOG_WARNING, "loadHighscoreFrom(): read failed for '%s'", path);
        return 0;
    }
    return hs;
}


void currentLevel()
{
    if(!game || game->gameOver) return;

    if(game->anyAlive == 0) {
        nextLevel();
    }
}


void nextLevel()
{
    game->level++;

    deleteAliens();
    createAliens();

    if(game->level % 3 == 0 && game->playerLives < 5) {
        game->playerLives++;
    }

    game->timeLastSpawnMysteryShip = (float)GetTime();
    game->mysteryShipSpawnInterval = (float)GetRandomValue(10, 20);
}