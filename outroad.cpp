#define _CRT_SECURE_NO_WARNINGS
#include "Player.h"
#include "DrawDef.h"
#include "Constants.h"
#include "OutRoad.h"

DrawDef drw;
OutRoad outer;

void OutRoad::drawOutRoad(SDL_Surface* screen, int color) {
    int oneBlockHeight = SCREEN_HEIGHT - SCORE_HEIGHT - outer.height;
    int secondBlockHeight = SCREEN_HEIGHT - oneBlockHeight;

    int leftBotX = 0;
    int leftBotY = SCORE_HEIGHT + outer.height;
    int leftBotWidth = outer.offroadTopWidth;
    drw.DrawRectangle(screen, leftBotX, leftBotY, leftBotWidth, oneBlockHeight, color, color);
    // left bottom


    int leftTopX = 0;
    int leftTopY = SCORE_HEIGHT;
    int leftTopWidth = outer.offroadTopWidth + (outer.offroadDiff * outer.direction);
    drw.DrawRectangle(screen, leftTopX, leftTopY, leftTopWidth, secondBlockHeight, color, color);
    // left top


    int rightBotX = SCREEN_WIDTH - outer.offroadTopWidth + (outer.offroadDiff * outer.direction);
    int rightBotY = SCORE_HEIGHT + outer.height;
    int rightBotWidth = outer.offroadTopWidth + (outer.offroadDiff);
    drw.DrawRectangle(screen, rightBotX, rightBotY, rightBotWidth, oneBlockHeight, color, color);
    // right bot


    int rightTopX = SCREEN_WIDTH - outer.offroadTopWidth;
    int rightTopY = SCORE_HEIGHT;
    int rightTopWidth = outer.offroadTopWidth;
    drw.DrawRectangle(screen, rightTopX, rightTopY, rightTopWidth, secondBlockHeight, color, color);
    // right top

}

void OutRoad::moveOutRoad() {
    if (startY + outer.height < SCREEN_HEIGHT - SCORE_HEIGHT) {
        outer.height += OFFROAD_SPEED;
    }
    else {
        outer.height = 0;
        if (outer.offroadBotWidth <= MIN_OFFROAD_WIDTH || outer.offroadTopWidth <= MIN_OFFROAD_WIDTH) {
            outer.direction = 1;
            outer.previousDirection = -1;
        }
        else if (outer.offroadBotWidth >= SCREEN_WIDTH / 3 - 40 || outer.offroadTopWidth >= SCREEN_WIDTH / 3 - 40) {
            outer.direction = -1;
            outer.previousDirection = 1;
        }
        if (outer.direction == 1) {
            if (outer.previousDirection == 1) {
                outer.offroadBotWidth += 20;
                outer.offroadTopWidth += 20;
            }
            else {
                outer.offroadTopWidth += 20;
            }
        }
        else if (outer.direction == -1) {
            if (outer.previousDirection == -1) {
                outer.offroadBotWidth -= 20;
                outer.offroadTopWidth -= 20;
            }
            else {
                outer.offroadTopWidth -= 20;
            }
        }
    }


}

int OutRoad::getHeight() {
    return outer.height;
}

int OutRoad::getWidth() {
    return outer.offroadTopWidth + (outer.offroadDiff * outer.direction);
}

int OutRoad::getDirection() {
    return outer.direction;
}

int OutRoad::getDiff() {
    return outer.offroadDiff;
}