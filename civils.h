#pragma once

#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "stdlib.h"
#include "time.h"

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#include "Constants.h"

class Civil {

    float civilx = SCREEN_WIDTH / 2 + 100;
    float civily = SCREEN_HEIGHT - 100;
    int civilHP = 100;
    bool destroyed = false;
public:

    Civil();

    bool isInBorder(float civilx, float civily);
    void moveCivil();
    int distanceToEnemy();
    int distanceToPlayer();
    void drawCivil(SDL_Surface* screen, SDL_Surface* civilCar);
    void resetGame();
    int getCivilX();
    int getCivilY();
    void moveFromOffRoad();
    bool isDestroyed();
    void destroyCivil();
    void repairCivil();
    bool detectEnemyCollision();
    void reduceHP();
    bool destroyedByPlayer();
};
