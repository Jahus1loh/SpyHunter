#define _CRT_SECURE_NO_WARNINGS
#include "Enemies.h"
#include "DrawDef.h"
#include "Player.h"
#include "Civils.h"
#include "Missile.h"

DrawDef drawingmis;
Missile missile;
Player mplayer;
Enemy menemy;
Civil mcivil;

void Missile::missileStartY() {
    missile.missiley = mplayer.getCarY();   // set missile y pos to currents car y pos
}

void Missile::shootMissile() {
    missileStartY();
    missile.fired = true; // set that missile was fired
}

bool Missile::enemyHit() {
    int enemyCenter = menemy.getEnemyX() + CAR_WIDTH / 2;
    if (fabs(enemyCenter - missile.missilex) < CAR_WIDTH / 2 + MISSILE_WIDTH && missile.missiley - menemy.getEnemyY() < CAR_HEIGHT) { // check if missile hit enemy
        destroyMissile();   // destroy missile
        menemy.reduceHP();  // reduce enemy hp
        return true;  
    }
    return false;
}

bool Missile::civilHit() {
    int civilCenter = mcivil.getCivilX() + CAR_WIDTH / 2;
    if (fabs(civilCenter - missile.missilex) < CAR_WIDTH / 2 + MISSILE_WIDTH && missile.missiley - mcivil.getCivilY() < CAR_HEIGHT) { // check if missile hit civil
        destroyMissile();   // destroy missile
        mcivil.reduceHP();  // reduce civil hp
        return true;
    }
    return false;
}

void Missile::destroyMissile() {
    missile.fired = false;  // set that missile is no longer fired
    missile.missiley = SCREEN_HEIGHT - MISSILE_HEIGHT;
}

void Missile::drawMissile(SDL_Surface* screen, SDL_Surface* missileTexture) {
    missile.missilex = mplayer.getCarX() + CAR_WIDTH / 2;
    if (missile.fired && !enemyHit() && !civilHit() && missile.missiley > SCORE_HEIGHT + MISSILE_HEIGHT) {  // check if missile is fired, is in border and didnt hit enemy nor civil
        drawingmis.DrawSurface(screen, missileTexture, missile.missilex, missile.missiley); // draw missile to the screen
    }
}

void Missile::moveMissile() {
    if (missile.missiley > MISSILE_HEIGHT) {    // mmvoe missile if its in border
        missile.missiley -= 5;
    }
}