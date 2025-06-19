#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include "DrawDef.h"
#include "Missile.h"
#include "Enemies.h"
#include "OutRoad.h"
#include "Civils.h"

DrawDef drawing;
Player player;
Missile pShoot;
Enemy pEnemy;
Civil pCivil;
OutRoad pOut;

void Player::handleEvent(SDL_Event& event, SDL_Surface* screen, int color1, int color2) {

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_UP && player.cary > CAR_HEIGHT) caryvelocity = -CAR_SPEED;                                // up key
        else if (event.key.keysym.sym == SDLK_DOWN && player.cary < SCREEN_HEIGHT - CAR_HEIGHT) caryvelocity = CAR_SPEED;        // down key
        else if (event.key.keysym.sym == SDLK_RIGHT && player.carx < SCREEN_WIDTH - CAR_WIDTH) carxvelocity = CAR_SPEED;        // right key
        else if (event.key.keysym.sym == SDLK_LEFT && player.cary > CAR_WIDTH) carxvelocity = -CAR_SPEED;                        // left key
        else if (event.key.keysym.sym == SDLK_SPACE) pShoot.shootMissile();                                                // space
    }
    if (event.type == SDL_KEYUP) {
        carxvelocity = 0;
        caryvelocity = 0;
    }
}

void Player::drawCar(SDL_Surface* screen, SDL_Surface* playerCar) {
    drawing.DrawSurface(screen, playerCar, (player.carx + CAR_WIDTH / 2), (player.cary + CAR_HEIGHT / 2));
}

bool Player::detectOffRoad() {
    int offRoadWidth = pOut.getWidth();
    int offRoadHeight = pOut.getHeight();
    int offRoadDirection = pOut.getDirection();
    int offRoadDiff = pOut.getDiff();
    if (player.cary > offRoadHeight + SCORE_HEIGHT) {            // bottom part
        if (offRoadDirection == 1) {                            // road getting smaller, upper part bigger than lower
            if (player.carx > SCREEN_WIDTH - offRoadWidth + offRoadDiff + CAR_WIDTH || player.carx < offRoadWidth - offRoadDiff - CAR_WIDTH) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if (player.carx > SCREEN_WIDTH - offRoadWidth - offRoadDiff - CAR_WIDTH || player.carx < offRoadWidth + offRoadDiff - CAR_WIDTH) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    else if (player.cary < offRoadHeight + SCORE_HEIGHT) {    // top part
        if (offRoadDirection == 1) {                            // road getting wider, lower part bigger than upper
            if (player.carx > SCREEN_WIDTH - offRoadWidth + offRoadDiff || player.carx < offRoadWidth - offRoadDiff) {
                return true;
            }
            else {
                return false;
            }
        }
        else {
            if (player.carx > SCREEN_WIDTH - offRoadWidth - offRoadDiff || player.carx < offRoadWidth - CAR_WIDTH) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool Player::isInBorder(float carx, float cary) {
    if (cary > CAR_HEIGHT && cary < SCREEN_HEIGHT - CAR_HEIGHT && carx < SCREEN_WIDTH - CAR_WIDTH && carx > CAR_WIDTH) return true;
    return false;
}

void Player::moveCar() {
    if (detectOffRoad()) {
        resetGame();
    }
    else {
        if (isInBorder(player.carx, player.cary)) {
            player.carx += carxvelocity;
            player.cary += caryvelocity;
        }
        else {
            player.carx -= carxvelocity;
            player.cary -= caryvelocity;
            carxvelocity = 0;
            caryvelocity = 0;
        }
    }
}

void Player::resetGame() {
    player.carx = SCREEN_WIDTH / 2;
    player.carxvelocity = 0;
    player.cary = SCREEN_HEIGHT - 100;
    player.caryvelocity = 0;
}


// determine where enemy is
int Player::whereIsEnemy() {
    if (player.carx > pEnemy.getEnemyX() && player.cary > pEnemy.getEnemyY()) return 4;                // fourth quadrant - player is on right-down of enemy
    else if (player.carx > pEnemy.getEnemyX() && player.cary < pEnemy.getEnemyY()) return 1;        // first quadrant - player is on right-up of enemy
    else if (player.carx < pEnemy.getEnemyX() && player.cary > pEnemy.getEnemyY()) return 3;        // third quadrant - player is on left-bottom of enemy
    else if (player.carx < pEnemy.getEnemyX() && player.cary < pEnemy.getEnemyY()) return 2;        // second quadrant - player is on left-up of enemy
    return 0;
}

void Player::overrun() {
    if (pEnemy.getEnemyMode() == 1) {
        if (fabs(player.carx - pEnemy.getEnemyX()) < (CAR_WIDTH + 20) && fabs(player.cary - pEnemy.getEnemyY()) < CAR_HEIGHT / 3) {
            if (whereIsEnemy() == 1) {
                if (isInBorder(player.carx + 0.2, player.cary + 0.2)) {
                    player.carx += 0.25;
                    player.cary += 0.25;
                }
                else {
                    player.destroyPlayer();
                }
            }
            else if (whereIsEnemy() == 2) {
                if (isInBorder(player.carx - 0.2, player.cary - 0.2)) {
                    player.carx -= 0.25;
                    player.cary -= 0.25;
                }
                else {
                    player.destroyPlayer();
                }
            }
            else if (whereIsEnemy() == 3) {
                if (isInBorder(player.carx - 0.2, player.cary + 0.2)) {
                    player.carx -= 0.25;
                    player.cary += 0.25;
                }
                else {
                    player.destroyPlayer();
                }
            }
            else if (whereIsEnemy() == 4) {
                if (isInBorder(player.carx + 0.2, player.cary - 0.2)) {
                    player.carx += 0.25;
                    player.cary -= 0.25;
                }
                else {
                    player.destroyPlayer();
                }
            }
        }
    }
}

int Player::getCarX() {
    return player.carx;
}

int Player::getCarY() {
    return player.cary;
}

bool Player::detectEnemyCollision() {
    int enemyX = pEnemy.getEnemyX();
    int enemyY = pEnemy.getEnemyY();
    int carYCenter = player.cary + CAR_HEIGHT / 2;
    int carXCenter = player.carx + CAR_WIDTH / 2;
    int enemyXCenter = enemyX + CAR_WIDTH / 2;
    int enemyYCenter = enemyY + CAR_HEIGHT / 2;

    if (enemyYCenter - carYCenter < CAR_HEIGHT && enemyYCenter - carYCenter > -CAR_HEIGHT && enemyXCenter - carXCenter < CAR_WIDTH && enemyXCenter - carXCenter > -CAR_WIDTH) {
        return true;

    }
    else {
        return false;
    }
}

bool Player::detectCivilCollision() {
    int civilX = pCivil.getCivilX();
    int civilY = pCivil.getCivilY();
    int carYCenter = player.cary + CAR_HEIGHT / 2;
    int carXCenter = player.carx + CAR_WIDTH / 2;
    int civilXCenter = civilX + CAR_WIDTH / 2;
    int civilYCenter = civilY + CAR_HEIGHT / 2;

    if (civilYCenter - carYCenter < CAR_HEIGHT && civilYCenter - carYCenter > -CAR_HEIGHT && civilXCenter - carXCenter < CAR_WIDTH && civilXCenter - carXCenter > -CAR_WIDTH) {
        return true;
    }
    else {
        return false;
    }
}

void Player::destroyPlayer() {
    if (detectCivilCollision() || detectEnemyCollision() || detectOffRoad()) {
        player.destroyed = true;
    }
}

void Player::repairPlayer() {
    player.destroyed = false;
}

bool Player::isDestroyed() {
    return player.destroyed;
}