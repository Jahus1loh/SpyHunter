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

class Player {
    float carx = SCREEN_WIDTH / 2, cary = SCREEN_HEIGHT / 2, carxvelocity = 0, caryvelocity = 0;
    bool destroyed = false;
public:

    void handleEvent(SDL_Event& event, SDL_Surface* screen, int color1, int color2);
    bool isInBorder(float carx, float cary);
    void moveCar();
    void drawCar(SDL_Surface* screen, SDL_Surface* playerCar);
    bool detectOffRoad();
    void resetGame();
    int whereIsEnemy();
    void overrun();
    int getCarX();
    int getCarY();
    bool detectEnemyCollision();
    bool detectCivilCollision();
    void destroyPlayer();
    void repairPlayer();
    bool isDestroyed();

};
