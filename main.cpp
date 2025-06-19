#define _CRT_SECURE_NO_WARNINGS

#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}


#include "Player.h"
#include "Constants.h"
#include "DrawDef.h"
#include "OutRoad.h"
#include "Enemies.h"
#include "Civils.h"
#include "Missile.h"

SDL_Event event;

DrawDef draw;
Player user;
OutRoad out;
Enemy opponents;
Civil neutral;
Missile bullet;

int pauseTime;

void ResetGame() {
    double worldTime = 0;
    double score = 0;
    user.resetGame();
    opponents.resetGame();
    neutral.resetGame();
}

int getCurrentTime(double worldTime) {
    return worldTime;
}


void CalculateScore(double worldTime, double score) {
    if (user.detectOffRoad()) {
        pauseTime = getCurrentTime(worldTime) + NOSCORE_TIME;
    }
    else {
        if (getCurrentTime(worldTime) != pauseTime) {

        }
        score += 50;
    }
}

#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char** argv) {
    int t1, t2, quit, frames, rc, prevWorldTime;
    int randomNumber = 100;
    int lives = START_LIFES;
    double delta, worldTime, fpsTimer, fps, distance, score;
    bool paused = false, lost = false;
    SDL_Event event;
    SDL_Surface* screen, * charset;
    SDL_Surface* playerCar;
    SDL_Surface* enemyCar;
    SDL_Surface* missileTexture;
    SDL_Surface* civilCar;
    SDL_Texture* scrtex;
    SDL_Window* window;
    SDL_Renderer* renderer;

    srand(time(NULL));

    printf("printf output goes here\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
        &window, &renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_SetWindowTitle(window, "SpyHunter, Jan Pastucha 193662");


    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_ShowCursor(SDL_DISABLE);

    charset = SDL_LoadBMP("./cs8x8.bmp");
    if (charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };
    SDL_SetColorKey(charset, true, 0x000000);

    // load enemy car bitmap
    enemyCar = SDL_LoadBMP("./enemycar.bmp");
    if (enemyCar == NULL) {
        printf("SDL_LoadBMP(enemycar.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };

    // load player car bitmap
    playerCar = SDL_LoadBMP("./playercar.bmp");
    if (playerCar == NULL) {
        printf("SDL_LoadBMP(playercar.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };


    // load civil car bitmap
    civilCar = SDL_LoadBMP("./civilcar.bmp");
    if (civilCar == NULL) {
        printf("SDL_LoadBMP(civilcar.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };

    // load missile bitmap
    missileTexture = SDL_LoadBMP("./missile.bmp");
    if (missileTexture == NULL) {
        printf("SDL_LoadBMP(missile.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };

    char text[128];
    int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    int green = SDL_MapRGB(screen->format, 0x00, 0x62, 0x21);
    int red = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    int gray = SDL_MapRGB(screen->format, 0x70, 0x80, 0x90);

    t1 = SDL_GetTicks();

    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;
    score = 0;
    distance = 0;

    while (!quit) {
        if (paused) {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = 1;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                    else if (event.key.keysym.sym == SDLK_p) {
                        paused = false;
                    }
                }
            }
            t1 = SDL_GetTicks();
            t2 = SDL_GetTicks();
        }
        else if (lost) {
            sprintf(text, "You lost");
            draw.DrawString(screen, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, text, charset);
            SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
            SDL_RenderCopy(renderer, scrtex, NULL, NULL);
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    quit = 1;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                    else if (event.key.keysym.sym == SDLK_n) {
                        worldTime = 0;
                        score = 0;
                        pauseTime = 0;
                        user.resetGame();
                        opponents.resetGame();
                        neutral.resetGame();
                    }
                }
            }
        }
        else {
            t2 = SDL_GetTicks();

            // here t2-t1 is the time in milliseconds since
            // the last screen was drawn
            // delta is the same time in seconds
            delta = (t2 - t1) * 0.001;
            t1 = t2;

            prevWorldTime = worldTime;
            worldTime += delta;


            if (user.isDestroyed()) {
                if (worldTime > IMMUNE_TIME) {
                    if (lives == 0) {
                        lost = true;
                    }
                    else {
                        lives--;
                        user.repairPlayer();
                    }
                }
                else {
                    pauseTime = worldTime + NOSCORE_TIME;
                    user.repairPlayer();
                }
            }
            else {
                if (worldTime >= pauseTime && floor(worldTime) != prevWorldTime) { // check if score isnt paused and if new second started 
                    if (worldTime > IMMUNE_TIME && int(score) % 1000 == 0) { // if score is divisible by 1000 and time when player is immune expired add one life to player
                        lives++;
                    }
                    if (neutral.isDestroyed()) {    // if civil is not on the screen, show him again
                        neutral.repairCivil();
                    }
                    if (opponents.isDestroyed()) {  // if enemy is not on the screen, show him again
                        opponents.repairEnemy();
                    }
                    if (opponents.destroyedByPlayer()) {   // check if enemy was destroyed by the player
                        score += 100;   // add 100 to score
                        opponents.resetHP();    // reset enemy hp
                    }
                    if (neutral.destroyedByPlayer()) {  // check if civil was destroyed by the player
                        pauseTime = worldTime + NOSCORE_TIME;   // set pause time to current world time + no score time
                    }
                    score += 50;          // add 50 points to score
                    randomNumber = rand() % 6 + 2;  // generate random number between 2 and 6
                }
            }

            if (randomNumber % 2 == 0) {
                opponents.switchDirection();
            }

            if (prevWorldTime % randomNumber) {
                opponents.switchMode();
            }


            SDL_FillRect(screen, NULL, black);

            fpsTimer += delta;
            if (fpsTimer > 0.5) {
                fps = frames * 2;
                frames = 0;
                fpsTimer -= 0.5;
            };

            out.drawOutRoad(screen, green);     // draw an offroad
            out.moveOutRoad();              // move an offroad
            draw.DrawLegend(screen, black, red, score, worldTime, charset, fps, lives); // draw a legend
            user.drawCar(screen, playerCar);    // draw a player to the screen
            user.moveCar();     // move a player
            user.overrun();     // overrun the player by enemy
            user.destroyPlayer();   // check if one of conditions when player is destroy is fullfilled

            bullet.drawMissile(screen, missileTexture);     // draw missile to the screen
            bullet.moveMissile();       // move the missile


            if (!opponents.isDestroyed()) {     // check if enemy is not destroyed
                opponents.drawEnemy(screen, enemyCar);  // draw an enemy to the screen
                opponents.moveEnemy();      // move an enemy
                opponents.destroyEnemy();   // check if one of conditions when enemy is destroyed is fullfilled
                if (opponents.detectOffRoad()) {    // check if enemy went off road
                    opponents.destroyEnemy();   // destroy an enemy
                }
            }


            if (!neutral.isDestroyed()) {   // check if civil is not destroyed
                neutral.drawCivil(screen, civilCar);    // draw civil to the screen
                neutral.moveCivil();    // move civil
                neutral.destroyCivil(); // check if one of conditions when civil is destroyed is fullfilled
                neutral.moveFromOffRoad();  // move civil if its offroad
                if (neutral.detectEnemyCollision()) {   // check if civil collided with an enemy
                    neutral.destroyCivil(); // destroy civil
                }
            }

            if (user.detectCivilCollision()) {  // check if player collided with civil
                pauseTime = worldTime + NOSCORE_TIME;   // set pause time to current world time + no score time
                neutral.destroyCivil(); // destroy civil
                user.resetGame();   // reset player position
            }

            if (user.detectEnemyCollision()) {  // check if player collided with an enemy
                opponents.destroyEnemy(); // destroy enemy
                user.resetGame();   // reset player position
            }

            SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
            //        SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, scrtex, NULL, NULL);
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    quit = true;
                }
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                    else if (event.key.keysym.sym == SDLK_n) {
                        worldTime = 0;
                        score = 0;
                        pauseTime = 0;
                        user.resetGame();
                        opponents.resetGame();
                        neutral.resetGame();
                    }
                    else if (event.key.keysym.sym == SDLK_p) paused = true;
                }

                user.handleEvent(event, screen, black, gray);
            }
            frames++;
        }
    };

    // zwolnienie powierzchni / freeing all surfaces
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
};