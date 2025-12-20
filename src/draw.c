#include "draw.h"
#include <string.h>

void drawCircle(SDL_Renderer *renderer, int cx, int cy, int r) {
  int x = r, y = 0;

  int E = 3 - 2 * r;
  while (x > y) {
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
    SDL_RenderLine(renderer, cx + x, cy - y, cx - x, cy - y);
    SDL_RenderLine(renderer, cx + y, cy + x, cx - y, cy + x);
    SDL_RenderLine(renderer, cx + y, cy - x, cx - y, cy - x);
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

void drawTextAlign(SDL_Renderer *renderer, TTF_Font *font, char *text, int x,
                   int y, TextAlign alignment) {
  SDL_FRect originRect;
  SDL_Texture *tex = getTextTexture(renderer, font, text, &originRect);
  if (tex == NULL)
    return;

  SDL_FRect destRect;
  switch (alignment) {
  case AlignLeftTop:
    destRect =
        (SDL_FRect){.x = x, .y = y, .h = originRect.h, .w = originRect.w};
    break;
  // case AlignRightTop:
  //   destRect = (SDL_FRect){
  //       .x = x - originRect.w, .y = y, .h = originRect.h, .w = originRect.w};
  //   break;
  // case AlignCenterTop:
  //   destRect = (SDL_FRect){.x = x - (originRect.w / 2.f),
  //                          .y = y,
  //                          .h = originRect.h,
  //                          .w = originRect.w};
  //   break;
  case AlignLeftCenter:
    destRect = (SDL_FRect){.x = x,
                           .y = y - (originRect.h / 2.f),
                           .h = originRect.h,
                           .w = originRect.w};
    break;
  case AlignRightCenter:
    destRect = (SDL_FRect){.x = x - originRect.w,
                           .y = y - (originRect.h / 2.f),
                           .h = originRect.h,
                           .w = originRect.w};
    break;
    // case AlignCenter:
    //   destRect = (SDL_FRect){.x = x - (originRect.w / 2.f),
    //                          .y = y - (originRect.h / 2.f),
    //                          .h = originRect.h,
    //                          .w = originRect.w};
    //   break;
    // case AlignLeftBottom:
    //   destRect = (SDL_FRect){
    //       .x = x, .y = y - originRect.h, .h = originRect.h, .w =
    //       originRect.w};
    //   break;
    // case AlignRightBottom:
    //   destRect = (SDL_FRect){.x = x - originRect.w,
    //                          .y = y - originRect.h,
    //                          .h = originRect.h,
    //                          .w = originRect.w};
    //   break;
    // case AlignCenterBottom:
    //   destRect = (SDL_FRect){.x = x - (originRect.w / 2.f),
    //                          .y = y - originRect.h,
    //                          .h = originRect.h,
    //                          .w = originRect.w};
    //   break;
  }

  SDL_RenderTexture(renderer, tex, &originRect, &destRect);
  SDL_DestroyTexture(tex);
}
