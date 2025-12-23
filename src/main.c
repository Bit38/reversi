#include "asstes.h"
#include "board.h"
#include "draw.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;

gameBoard *board;

bool sdlInit() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to inintialise SDL: %s\n", SDL_GetError());
    return false;
  }

  if (!TTF_Init()) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to initialise SDL_ttf: %s\n", SDL_GetError());
    return false;
  }

  if (!SDL_CreateWindowAndRenderer("reversi", 800, 900, 0, &window,
                                   &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to create window and renderer: %s",
                 SDL_GetError());
    SDL_Quit();
    return false;
  }

  if (!loadAssets(renderer)) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return false;
  }

  SDL_Log("SDL initialised successfully!\n");
  return true;
}

int main(int argc, char *argv[]) {
  if (!sdlInit())
    return EXIT_FAILURE;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  board = boardInit();
  const SDL_FRect boardRect = {20.f, 120.f, 760.f, 760.f};

  BufferedText whiteScore;
  BufferedText blackScore;
  BufferedText turnInfo;
  SDL_FRect restartBtn;

  bool quit = false;
  while (!quit) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT)
        quit = true;
      if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_FPoint mouse = {ev.button.x, ev.button.y};
        boardHandleClick(board, &mouse, &boardRect);

        if (SDL_PointInRectFloat(&mouse, &restartBtn))
          boardReset(board);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        char scoreW[100];
        sprintf(scoreW, "White: %d", board->scoreWhite);
        updateBufferedText(&whiteScore, renderer, scoreFont, scoreW);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        char scoreB[100];
        sprintf(scoreB, "%d: Black", board->scoreBlack);
        updateBufferedText(&blackScore, renderer, scoreFont, scoreB);

        switch (board->turn) {
        case CELL_BLACK:
          SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
          updateBufferedText(&turnInfo, renderer, turnFont, "Turn: Black");
          break;
        case CELL_WHITE:
          SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
          updateBufferedText(&turnInfo, renderer, turnFont, "Turn: White");
          break;
        default:
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 15, 82, 0, 255);
    SDL_RenderClear(renderer);

    drawBufferedTextAlign(&whiteScore, renderer, boardRect.x, 60,
                          AlignLeftCenter);
    drawBufferedTextAlign(&blackScore, renderer, boardRect.x + boardRect.w, 60,
                          AlignRightCenter);
    restartBtn =
        drawTextureAlignScale(renderer, restartButton.tex, &restartButton.rect,
                              400, 5, AlignCenterTop, 0.2f);
    drawBufferedTextAlign(&turnInfo, renderer, 400, 115, AlignCenterBottom);

    // Consider using event driven redraw
    boardDraw(board, renderer, &boardRect);

    SDL_RenderPresent(renderer);
  }

  SDL_Log("Quitting game...\n");
  deloadAssets();
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  return EXIT_SUCCESS;
}
