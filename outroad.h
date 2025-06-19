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


class OutRoad {
    int startY = SCORE_HEIGHT;
    int offroadBotWidth = 160;
    int offroadTopWidth = 180;
    int offroadDiff = 20;
    float height = 0;
    int direction = 1;
    int previousDirection = 1;
public:

    void drawOutRoad(SDL_Surface* screen, int zielony);
    void moveOutRoad();
    int getWidth();
    int getHeight();
    int getDirection();
    int getDiff();
};