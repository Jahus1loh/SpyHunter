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

class Missile {
    float missiley = 0;
    float missilex = 0;
    bool fired = false;
public:

    void missileStartY();
    void shootMissile();
    void destroyMissile();
    bool enemyHit();
    bool civilHit();
    void drawMissile(SDL_Surface* screen, SDL_Surface* missileTexture);
    void moveMissile();
};