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
  AlignCenterTop,
  AlignLeftCenter,
  AlignRightCenter,
  AlignCenter,
  // AlignLeftBottom,
  // AlignRightBottom,
  AlignCenterBottom
} Align;

SDL_FRect drawTextureAlignScale(SDL_Renderer *renderer, SDL_Texture *tex,
                                SDL_FRect *texRect, int x, int y,
                                Align alignment, float scale);
void drawTextAlign(SDL_Renderer *renderer, TTF_Font *font, char *text, int x,
                   int y, Align alignment);

#define drawText(renderer, font, text, x, y)                                   \
  drawTextAlign((renderer), (font), (text), (x), (y), AlignLeftTop)
#define drawTextureAlign(renderer, tex, texRect, x, y, alignment)              \
  drawTextureAlignScale((renderer), (tex), (texRect), (x), (y), (alignment),   \
                        1.f)

typedef struct {
  SDL_Texture *tex;
  SDL_FRect rect;
} BufferedText;

void updateBufferedText(BufferedText *bf, SDL_Renderer *renderer,
                        TTF_Font *font, char *text);
SDL_FRect drawBufferedTextAlignScale(BufferedText *bf, SDL_Renderer *renderer,
                                     int x, int y, Align alignment,
                                     float scale);
#define drawBufferedTextAlign(bf, renderer, x, y, alignment)                   \
  drawBufferedTextAlignScale(bf, renderer, x, y, alignment, 1.f)
void destroyBufferedText(BufferedText *bf);

#endif // INCLUDE_DRAW_H_
