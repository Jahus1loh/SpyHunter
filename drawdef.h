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

class DrawDef {
public:

    void DrawString(SDL_Surface* screen, int x, int y, const char* text,
        SDL_Surface* charse);
    void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
    void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
    void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
    void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
        Uint32 outlineColor, Uint32 fillColor);
    void DrawRoad(SDL_Surface* screen, int color);
    void DrawLegend(SDL_Surface* screen, int czerwony, int niebieski, double score, double worldTime, SDL_Surface* charset, double fps, double lives);
};