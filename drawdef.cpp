#define _CRT_SECURE_NO_WARNINGS
#include "DrawDef.h"



// narysowanie napisu txt na powierzchni screen, zaczynaj?c od punktu (x, y)
// charset to bitmapa 128x128 zawieraj?ca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawDef::DrawString(SDL_Surface* screen, int x, int y, const char* text,
    SDL_Surface* charset) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = 8;
    d.h = 8;
    while (*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitSurface(charset, &s, screen, &d);
        x += 8;
        text++;
    };
};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt ?rodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawDef::DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
    SDL_Rect dest;
    dest.x = x - sprite->w / 2;
    dest.y = y - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};



// rysowanie pojedynczego pixela
// draw a single pixel
void DrawDef::DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32*)p = color;
};


// rysowanie linii o d?ugo?ci l w pionie (gdy dx = 0, dy = 1)
// b?d? poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawDef::DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
    for (int i = 0; i < l; i++) {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};


// rysowanie prostok?ta o d?ugo?ci bok?w l i k
// draw a rectangle of size l by k
void DrawDef::DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
    Uint32 outlineColor, Uint32 fillColor) {
    int i;
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for (i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};


void DrawDef::DrawRoad(SDL_Surface* screen, int color) {
    DrawRectangle(screen, 0, 0, 100, SCREEN_HEIGHT / 4, color, color);
}

void DrawDef::DrawLegend(SDL_Surface* screen, int czerwony, int niebieski, double score, double worldTime, SDL_Surface* charset, double fps, double lives) {
    char text[128];
    DrawRectangle(screen, 0, 0, SCREEN_WIDTH, 50, czerwony, niebieski);
    //            "template for the second project, elapsed time = %.1lf s  %.0lf frames / s"
    sprintf(text, "Score = %.0lf, Elapsed time = %.1lf s  %.0lf frames / s, lives %.0lf", score, worldTime, fps, lives);
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
    sprintf(text, "Jan Pastucha, 193662");
    DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 30, text, charset);

    sprintf(text, "n - start a new game");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 10, text, charset);
    sprintf(text, "Esc - exit");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 40, text, charset);
    sprintf(text, "\033 - right");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 50, text, charset);
    sprintf(text, "\032 - left");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 60, text, charset);
    sprintf(text, "\031 - down");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 70, text, charset);
    sprintf(text, "\030 - up");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 80, text, charset);
    sprintf(text, "space - shoot");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 30, text, charset);
    sprintf(text, "p - pause/continue");
    DrawString(screen, screen->w - strlen(text) * 8, SCREEN_HEIGHT - 20, text, charset);

}
