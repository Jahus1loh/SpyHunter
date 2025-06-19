#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#include "Constants.h"

class Enemy {
    float enemyx = SCREEN_WIDTH / 2 + 100;
    float enemyy = SCORE_HEIGHT + CAR_HEIGHT / 2, enemyxvelocity = 0, enemyyvelocity = 0;
    int enemyMode = 0, direction = 1;
    bool destroyed = false;
    int enemyHP = 100;
public:

    Enemy();

    bool isInBorder(float enemyx, float enemyy);
    void attackMode();
    void moveEnemy();
    void drawEnemy(SDL_Surface* screen, SDL_Surface* enemyCar);
    int whereIsPlayer();
    void overrun();
    bool detectOffRoad();
    void resetGame();
    int getEnemyX();
    int getEnemyY();
    void switchMode();
    void switchDirection();
    int getEnemyMode();
    bool isDestroyed();
    void destroyEnemy();
    void repairEnemy();
    void reduceHP();
    int getHP();
    void resetHP();
    bool destroyedByPlayer();
};

