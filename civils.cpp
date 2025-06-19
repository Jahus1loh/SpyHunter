#define _CRT_SECURE_NO_WARNINGS
#include "Enemies.h"
#include "DrawDef.h"
#include "Player.h"
#include "Civils.h"
#include "OutRoad.h"
#include "stdlib.h"
#include "time.h"

DrawDef drawingciv;
Enemy cenemy;
Player cplay;
Civil civil;
OutRoad cout;


Civil::Civil() {
    srand(time(NULL));
    civil.civilx = rand() % (cout.getWidth() + CAR_WIDTH) + (SCREEN_HEIGHT - cout.getWidth() - CAR_WIDTH);;
    civil.civily = SCREEN_HEIGHT - 100;
}

void Civil::drawCivil(SDL_Surface* screen, SDL_Surface* civilCar) {
    if (!destroyed) {
        drawingciv.DrawSurface(screen, civilCar, (civil.civilx + CAR_WIDTH / 2), (civil.civily + CAR_HEIGHT / 2));
    }
}

bool Civil::isInBorder(float civilx, float civily) {
    if (civily > CAR_HEIGHT && civily < SCREEN_HEIGHT - CAR_HEIGHT && civilx < SCREEN_WIDTH - CAR_WIDTH && civilx > CAR_WIDTH) return true;
    return false;
}

int Civil::distanceToEnemy() {
    return sqrt(pow(cenemy.getEnemyX() - civil.civilx, 2) + pow(cenemy.getEnemyY() - civil.civily, 2));
}

int Civil::distanceToPlayer() {
    return sqrt(pow(cplay.getCarX() - civil.civilx, 2) + pow(cplay.getCarY() - civil.civily, 2));
}

void Civil::moveCivil() {
    if (isInBorder(civil.civilx, civil.civily) && !destroyed) {
        if ((distanceToEnemy() < CAR_HEIGHT + 10 && fabs(cenemy.getEnemyY() - civil.civily) < SCREEN_WIDTH)) {
            civil.civily += 1;
        }
        else {
            civil.civily -= 0.2;
        }
    }
    else {
        destroyCivil();
    }
}

void Civil::resetGame() {
    srand(time(NULL));
    civil.civilx = rand() % (cout.getWidth() + CAR_WIDTH) + (SCREEN_HEIGHT - cout.getWidth() - CAR_WIDTH);
    civil.civily = SCREEN_HEIGHT - 100;
    civil.destroyed = false;
    if (fabs(civil.civilx - cplay.getCarX()) < CAR_WIDTH + 10 && fabs(civil.civily - cplay.getCarY()) < CAR_HEIGHT) {
        destroyCivil();
    }

}

int Civil::getCivilX() {
    return civil.civilx;
}

int Civil::getCivilY() {
    return civil.civily;
}

void Civil::moveFromOffRoad() {
    if (civil.civilx > SCREEN_WIDTH - cout.getWidth() - CAR_WIDTH) {
        civil.civilx -= 0.2;
    }
    else if (civil.civilx < cout.getWidth()) {
        civil.civilx += 0.2;
    }
}

bool Civil::isDestroyed() {
    return civil.destroyed;
}

void Civil::destroyCivil() {
    if (!isInBorder(civil.civilx, civil.civily) || civil.civilHP == 0 || cplay.detectCivilCollision()) {
        civil.destroyed = true;
        civil.civilx = 0;
        civil.civily = 0;
    }
}

void Civil::repairCivil() {
    srand(time(NULL));
    civil.civilx = rand() % cout.getWidth() + (SCREEN_HEIGHT - cout.getWidth());
    civil.civily = SCREEN_HEIGHT - 100;
    civil.destroyed = false;
}

bool Civil::detectEnemyCollision() {
    int enemyX = cenemy.getEnemyX();
    int enemyY = cenemy.getEnemyY();
    int civilXCenter = civil.civilx + CAR_HEIGHT / 2;
    int civilYCenter = civil.civily + CAR_WIDTH / 2;
    int enemyXCenter = enemyX + CAR_WIDTH / 2;
    int enemyYCenter = enemyY + CAR_HEIGHT / 2;

    if (enemyYCenter - civilYCenter < CAR_HEIGHT && enemyYCenter - civilYCenter > -CAR_HEIGHT && enemyXCenter - civilXCenter < CAR_WIDTH && enemyXCenter - civilXCenter > -CAR_WIDTH) {
        return true;

    }
    else {
        return false;
    }
}

void Civil::reduceHP() {
    civil.civilHP -= 10;
}

bool Civil::destroyedByPlayer() {
    if ((civil.civilHP == 0) || cplay.detectCivilCollision()) {
        return true;
    }
    else {
        return false;
    }
}
