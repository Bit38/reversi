#include "draw.h"
#include <string.h>

// FIX: Some pixels are drawn multiple times (probobly use second fix from
// drawFillCircle)
void drawCircle(SDL_Renderer *renderer, int cx, int cy, int r) {
  int x = r, y = 0;

  int E = 3 - 2 * r;
  while (x >= y) {
    SDL_RenderPoint(renderer, cx + x, cy + y);
    SDL_RenderPoint(renderer, cx - x, cy + y);
    SDL_RenderPoint(renderer, cx - x, cy - y);
    SDL_RenderPoint(renderer, cx + x, cy - y);
    SDL_RenderPoint(renderer, cx + y, cy + x);
    SDL_RenderPoint(renderer, cx - y, cy + x);
    SDL_RenderPoint(renderer, cx - y, cy - x);
    SDL_RenderPoint(renderer, cx + y, cy - x);
    if (E > 0) {
      E += 2 * (y - x) + 5;
      x -= 1;
    } else
      E += 2 * y + 3;
    y += 1;
  }
}

void drawFillCircle(SDL_Renderer *renderer, int cx, int cy, int r) {
  int x = r, y = 0;

  int E = 3 - 2 * r;
  while (x >= y) {
    SDL_RenderLine(renderer, cx + x, cy + y, cx - x, cy + y);

    // When y == 0 the diameter is drawn for the second time
    if (y != 0)
      SDL_RenderLine(renderer, cx + x, cy - y, cx - x, cy - y);

    // When x does not change (E < 0) given chord is drawn multiple times
    // When x == y parts drawn earlier overlap with those drawn now
    if (E >= 0 && x != y) {
      SDL_RenderLine(renderer, cx + y, cy + x, cx - y, cy + x);
      SDL_RenderLine(renderer, cx + y, cy - x, cx - y, cy - x);
    }

    if (E >= 0) {
      E += 2 * (y - x) + 5;
      x -= 1;
    } else
      E += 2 * y + 3;
    y += 1;
  }
}

SDL_Texture *getTextTexture(SDL_Renderer *renderer, TTF_Font *font, char *text,
                            SDL_FRect *frect) {
  SDL_Color col;
  SDL_GetRenderDrawColor(renderer, &col.r, &col.g, &col.b, &col.a);

  SDL_Surface *surf = TTF_RenderText_Blended(font, text, strlen(text), col);
  if (surf == NULL)
    return NULL;
  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);

  if (frect != NULL) {
    SDL_Rect r;
    SDL_GetSurfaceClipRect(surf, &r);

    SDL_RectToFRect(&r, frect);
  }
  SDL_DestroySurface(surf);
  return tex;
}

SDL_FRect drawTextureAlignScale(SDL_Renderer *renderer, SDL_Texture *tex,
                                SDL_FRect *texRect, int x, int y,
                                Align alignment, float scale) {
  SDL_FRect destRect;
  switch (alignment) {
  case AlignLeftTop:
    destRect = (SDL_FRect){
        .x = x, .y = y, .h = (texRect->h * scale), .w = (texRect->w * scale)};
    break;
  // case AlignRightTop:
  //   destRect = (SDL_FRect){
  //       .x = x - (texRect->w * scale), .y = y, .h = (texRect->h * scale), .w
  //       = (texRect->w * scale)};
  //   break;
  case AlignCenterTop:
    destRect = (SDL_FRect){.x = x - ((texRect->w * scale) / 2.f),
                           .y = y,
                           .h = (texRect->h * scale),
                           .w = (texRect->w * scale)};
    break;
  case AlignLeftCenter:
    destRect = (SDL_FRect){.x = x,
                           .y = y - ((texRect->h * scale) / 2.f),
                           .h = (texRect->h * scale),
                           .w = (texRect->w * scale)};
    break;
  case AlignRightCenter:
    destRect = (SDL_FRect){.x = x - (texRect->w * scale),
                           .y = y - ((texRect->h * scale) / 2.f),
                           .h = (texRect->h * scale),
                           .w = (texRect->w * scale)};
    break;
  case AlignCenter:
    destRect = (SDL_FRect){.x = x - ((texRect->w * scale) / 2.f),
                           .y = y - ((texRect->h * scale) / 2.f),
                           .h = (texRect->h * scale),
                           .w = (texRect->w * scale)};
    break;
  // case AlignLeftBottom:
  //   destRect = (SDL_FRect){
  //       .x = x, .y = y - (texRect->h * scale), .h = (texRect->h * scale),
  //       .w = (texRect->w * scale)};
  //   break;
  // case AlignRightBottom:
  //   destRect = (SDL_FRect){.x = x - (texRect->w * scale),
  //                          .y = y - (texRect->h * scale),
  //                          .h = (texRect->h * scale),
  //                          .w = (texRect->w * scale)};
  //   break;
  case AlignCenterBottom:
    destRect = (SDL_FRect){.x = x - ((texRect->w * scale) / 2.f),
                           .y = y - (texRect->h * scale),
                           .h = (texRect->h * scale),
                           .w = (texRect->w * scale)};
    break;
  }

  SDL_RenderTexture(renderer, tex, texRect, &destRect);
  return destRect;
}

void drawAlign(SDL_Renderer *renderer, TTF_Font *font, char *text, int x, int y,
               Align alignment) {
  SDL_FRect originRect;
  SDL_Texture *tex = getTextTexture(renderer, font, text, &originRect);
  if (tex == NULL)
    return;

  drawTextureAlign(renderer, tex, &originRect, x, y, alignment);
  SDL_DestroyTexture(tex);
}

void updateBufferedText(BufferedText *bf, SDL_Renderer *renderer,
                        TTF_Font *font, char *text) {
  if (bf->tex != NULL)
    SDL_DestroyTexture(bf->tex);
  bf->tex = getTextTexture(renderer, font, text, &bf->rect);
}

inline SDL_FRect drawBufferedTextAlignScale(BufferedText *bf,
                                            SDL_Renderer *renderer, int x,
                                            int y, Align alignment,
                                            float scale) {
  return drawTextureAlignScale(renderer, bf->tex, &bf->rect, x, y, alignment,
                               scale);
}

void destroyBufferedText(BufferedText *bf) {
  if (bf->tex != NULL)
    SDL_DestroyTexture(bf->tex);
};
