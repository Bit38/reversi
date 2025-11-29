#ifndef INCLUDE_DRAW_H_
#define INCLUDE_DRAW_H_

#include <SDL3/SDL.h>

// TODO: Use floating point?
void drawCircle(SDL_Renderer *renderer, int cx, int cy, int r);
void drawFillCircle(SDL_Renderer *renderer, int cx, int cy, int r);

#endif // INCLUDE_DRAW_H_
