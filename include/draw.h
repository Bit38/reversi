#ifndef INCLUDE_DRAW_H_
#define INCLUDE_DRAW_H_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

// TODO: Use floating point?
void drawCircle(SDL_Renderer *renderer, int cx, int cy, int r);
void drawFillCircle(SDL_Renderer *renderer, int cx, int cy, int r);

SDL_Texture *getTextTexture(SDL_Renderer *renderer, TTF_Font *font, char *text,
                            SDL_FRect *frect);

// Commented values are just unused in codebase
typedef enum TextAlign {
  AlignLeftTop,
  // AlignRightTop,
  // AlignCenterTop,
  AlignLeftCenter,
  AlignRightCenter,
  // AlignCenter,
  // AlignLeftBottom,
  // AlignRightBottom,
  // AlignCenterBottom
} TextAlign;

void drawTextAlign(SDL_Renderer *renderer, TTF_Font *font, char *text, int x,
                   int y, TextAlign alignment);

#define drawText(renderer, font, text, x, y)                                   \
  drawTextAlign((renderer), (font), (text), (x), (y), AlignLeftTop)

#endif // INCLUDE_DRAW_H_
