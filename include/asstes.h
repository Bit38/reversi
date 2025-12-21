#ifndef INCLUDE_ASSTES_H_
#define INCLUDE_ASSTES_H_

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

typedef struct {
  SDL_Texture *tex;
  SDL_FRect rect;
} Image;

extern TTF_Font *scoreFont;
extern Image restartButton;
extern TTF_Font *turnFont;

bool loadAssets(SDL_Renderer *renderer);
void deloadAssets();

#endif // INCLUDE_ASSTES_H_
