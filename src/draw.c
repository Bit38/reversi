#include "draw.h"

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
