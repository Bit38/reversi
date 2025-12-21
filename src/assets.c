#include "Knewave_Regular_ttf.h"
#include "asstes.h"
#include "restart_icon_png.h"

TTF_Font *scoreFont;
TTF_Font *turnFont;
Image restartButton;

bool loadAssets(SDL_Renderer *renderer) {
  SDL_Log("Loading \"scoreFont\"\n");
  if ((scoreFont = TTF_OpenFontIO(
           SDL_IOFromMem(Knewave_Regular_ttf_data, Knewave_Regular_ttf_size),
           true, 50)) == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to load \"scoreFont\": %s\n", SDL_GetError());
    return false;
  }

  SDL_Log("Loading \"turnFont\"\n");
  if ((turnFont = TTF_OpenFontIO(
           SDL_IOFromMem(Knewave_Regular_ttf_data, Knewave_Regular_ttf_size),
           true, 40)) == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to load \"turnFont\": %s\n", SDL_GetError());
    return false;
  }

  SDL_Log("Loading \"restart_icon\"\n");
  SDL_Surface *surf;
  SDL_Rect r;
  if ((surf = SDL_LoadPNG_IO(
           SDL_IOFromMem(restart_icon_png_data, restart_icon_png_size),
           true)) == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to load \"restart_icon\": %s\n",
                 SDL_GetError());
    TTF_CloseFont(scoreFont);
    return false;
  }
  if ((restartButton.tex = SDL_CreateTextureFromSurface(renderer, surf)) ==
      NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to load \"restart_icon\": %s\n",
                 SDL_GetError());
    SDL_DestroySurface(surf);
    TTF_CloseFont(scoreFont);
    return false;
  }
  SDL_GetSurfaceClipRect(surf, &r);
  SDL_RectToFRect(&r, &restartButton.rect);
  SDL_DestroySurface(surf);

  SDL_Log("Assets loaded successfully!\n");
  return true;
}

void deloadAssets() {
  TTF_CloseFont(scoreFont);
  TTF_CloseFont(turnFont);
  SDL_DestroyTexture(restartButton.tex);
}
