#define _CRT_SECURE_NO_WARNINGS
#include "Enemies.h"
#include "DrawDef.h"
#include "Player.h"
#include "OutRoad.h"
#include "Missile.h"
#include "stdlib.h"
#include "time.h"

DrawDef drawingen;
Enemy enemy;
Player play;
OutRoad eout;
Missile emissile;

Enemy::Enemy() {
    srand(time(NULL));
    enemy.enemyx = rand() % (eout.getWidth() + CAR_WIDTH) + (SCREEN_HEIGHT - eout.getWidth() - CAR_WIDTH);
    enemy.enemyy = SCORE_HEIGHT + CAR_HEIGHT / 2;
}

void Enemy::drawEnemy(SDL_Surface* screen, SDL_Surface* enemyCar) {
    if (!destroyed) {
        drawingen.DrawSurface(screen, enemyCar, (enemy.enemyx + CAR_WIDTH / 2), (enemy.enemyy + CAR_HEIGHT / 2));
    }
}

bool Enemy::isInBorder(float enemyx, float enemyy) {
    if (enemyy > CAR_HEIGHT && enemyy < SCREEN_HEIGHT - CAR_HEIGHT && enemyx < SCREEN_WIDTH - CAR_WIDTH && enemyx > CAR_WIDTH) return true;
    return false;
}

// determine where enemy is
int Enemy::whereIsPlayer() {
    int enemyCenterX = enemy.enemyx + CAR_WIDTH / 2;
    int playerCenterX = play.getCarX() + CAR_WIDTH / 2;
    int enemyCenterY = enemy.enemyy + CAR_HEIGHT / 2;
    int playerCenterY = play.getCarY() + CAR_HEIGHT / 2;
    if (enemyCenterX > playerCenterX && enemyCenterY > playerCenterY) return 4; // fourth quadrant - player is on right-down of enemy
    else if (enemyCenterX > playerCenterX && enemyCenterY < playerCenterY) return 1; // first quadrant - player is on right-up of enemy
    else if (enemyCenterX < playerCenterX && enemyCenterY > playerCenterY) return 3; // third quadrant - player is on left-bottom of enemy
    else if (enemyCenterX < playerCenterX && enemyCenterY < playerCenterY) return 2; // second quadrant - player is on left-up of enemy
    return 0;
}

void Enemy::overrun() {
    int enemyCenterX = enemy.enemyx + CAR_WIDTH / 2;
    int playerCenterX = play.getCarX() + CAR_WIDTH / 2;
    int enemyCenterY = enemy.enemyy + CAR_HEIGHT / 2;
    int playerCenterY = play.getCarY() + CAR_HEIGHT / 2;
    if (fabs(enemyCenterX - playerCenterX) < CAR_WIDTH + 20 && fabs(enemyCenterY - playerCenterY) < CAR_HEIGHT / 2 && enemy.enemyMode == 0) {
        if (whereIsPlayer() == 1 || whereIsPlayer() == 4) {
            enemy.enemyx += CAR_SPEED * 4;
        }
        else if (whereIsPlayer() == 2 || whereIsPlayer() == 3) {
            enemy.enemyx -= CAR_SPEED * 4;
        }
    }
}

bool Enemy::detectOffRoad() {
    int offRoadWidth = eout.getWidth();
    int offRoadHeight = eout.getHeight();
    int offRoadDirection = eout.getDirection();
    int offRoadDiff = eout.getDiff();
    if (enemy.enemyy > offRoadHeight + SCORE_HEIGHT) {
        if (offRoadDirection == 1) {
            if (enemy.enemyx < SCREEN_WIDTH - offRoadWidth && enemy.enemyx > offRoadWidth - CAR_WIDTH / 3) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            if (enemy.enemyx < SCREEN_WIDTH - offRoadWidth - offRoadDiff && enemy.enemyx > offRoadWidth + offRoadDiff - CAR_WIDTH / 3) {
                return false;
            }
            else {
                return true;
            }
        }
    }
    else if (enemy.enemyy < offRoadHeight + SCORE_HEIGHT) {
        if (offRoadDirection == 1) {
            if (enemy.enemyx < SCREEN_WIDTH - offRoadWidth && enemy.enemyx > offRoadWidth - CAR_WIDTH / 3) {
                return false;
            }
            else {
                return true;
            }
        }
        else {
            if (enemy.enemyx < SCREEN_WIDTH - offRoadWidth - offRoadDiff && enemy.enemyx > offRoadWidth + offRoadDiff - CAR_WIDTH / 3) {
                return false;
            }
            else {
                return true;
            }
        }
    }
    return false;
}

void Enemy::moveEnemy() {
    int offRoadWidth = eout.getWidth();
    int enemyCenterX = enemy.enemyx + CAR_WIDTH / 2;
    int playerCenterX = play.getCarX() + CAR_WIDTH / 2;
    int enemyCenterY = enemy.enemyy + CAR_HEIGHT / 2;
    int playerCenterY = play.getCarY() + CAR_HEIGHT / 2;
    if (enemy.enemyMode == 1) {
        if (playerCenterX > enemyCenterX) {
            enemy.enemyx += ENEMY_SPEED;
        }
        else {
            enemy.enemyx -= ENEMY_SPEED;
        }
        if (playerCenterY > enemyCenterY) {
            enemy.enemyy += ENEMY_SPEED;
        }
        else {
            enemy.enemyy -= ENEMY_SPEED;
        }
    }
    else if (enemy.enemyMode == 0) {
        if (fabs(enemyCenterX - playerCenterX) < (CAR_WIDTH + 15) && fabs(enemyCenterY - playerCenterY) < CAR_HEIGHT + 10) {
            if (whereIsPlayer() == 1) {
                enemy.enemyx += ENEMY_SPEED * 2;
                enemy.enemyy += ENEMY_SPEED * 2;
                enemy.enemyMode = 0;
            }
            else if (whereIsPlayer() == 2) {
                enemy.enemyx -= ENEMY_SPEED * 2;
                enemy.enemyy -= ENEMY_SPEED * 2;
                enemy.enemyMode = 0;
            }
            else if (whereIsPlayer() == 3) {
                enemy.enemyx -= 0.4;
                enemy.enemyy -= 0.4;
                enemy.enemyMode = 0;
            }
            else if (whereIsPlayer() == 4) {
                enemy.enemyx += 0.4;
                enemy.enemyy += 0.4;
                enemy.enemyMode = 0;
            }
        }
        else {
            if (enemy.enemyx > SCREEN_WIDTH - offRoadWidth - 3 * CAR_WIDTH) {
                enemy.enemyx -= 0.1;
                enemy.direction = -1;
            }
            else if (enemy.enemyx < offRoadWidth) {
                enemy.enemyx += 0.1;
                enemy.direction = 1;
            }
            else {
                enemy.enemyx += 0.1 * enemy.direction;
            }
        }
    }
}


void Enemy::resetGame() {
    srand(time(NULL));
    enemy.enemyx = rand() % (eout.getWidth() + CAR_WIDTH) + (SCREEN_HEIGHT - eout.getWidth() - CAR_WIDTH);
    enemy.enemyy = SCORE_HEIGHT + CAR_HEIGHT / 2;
    enemy.enemyHP = 100;
}

int Enemy::getEnemyX() {
    return enemy.enemyx;
}

int Enemy::getEnemyY() {
    return enemy.enemyy;
}

void Enemy::switchMode() {
    if (enemy.enemyMode == 0) {
        enemy.enemyMode = 1;
    }
    else {
        enemy.enemyMode = 0;
    }
}

void Enemy::switchDirection() {
    if (enemy.direction == 1) {
        enemy.direction = -1;
    }
    else {
        enemy.direction = 1;
    }
}

int Enemy::getEnemyMode() {
    return enemy.enemyMode;
}

bool Enemy::isDestroyed() {
    return enemy.destroyed;
}

void Enemy::destroyEnemy() {
    if ((!isInBorder(enemy.enemyx, enemy.enemyy)) || (enemy.enemyHP == 0) || play.detectEnemyCollision() || detectOffRoad()) {
        enemy.destroyed = true;
        enemy.enemyx = 0;
        enemy.enemyy = CAR_HEIGHT;
    }
}

void Enemy::repairEnemy() {
    enemy.destroyed = false;
    srand(time(NULL));
    enemy.enemyx = rand() % eout.getWidth() + (SCREEN_HEIGHT - eout.getWidth());
    enemy.enemyy = rand() % SCORE_HEIGHT + (SCREEN_HEIGHT / 2);
    enemy.enemyHP = 100;
}

void Enemy::reduceHP() {
    enemy.enemyHP -= 10;
}

int Enemy::getHP() {
    return enemy.enemyHP;
}

void Enemy::resetHP() {
    enemy.enemyHP = 100;
}

bool Enemy::destroyedByPlayer() {
    if ((enemy.enemyHP == 0) || play.detectEnemyCollision() || detectOffRoad()) {
        return true;
    }
    else {
        return false;
    }
}