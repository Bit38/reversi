#include "Knewave_Regular_ttf.h"
#include "SDL3/SDL_surface.h"
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
  SDL_Log("SDL initialised successfully!\n");
  return true;
}

int main(int argc, char *argv[]) {
  if (!sdlInit())
    return EXIT_FAILURE;

  TTF_Font *f1 = TTF_OpenFontIO(
      SDL_IOFromMem(Knewave_Regular_ttf_data, Knewave_Regular_ttf_size), true,
      50);

  board = boardInit();
  const SDL_FRect boardRect = {20.f, 120.f, 760.f, 760.f};

  bool quit = false;
  while (!quit) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT)
        quit = true;
      if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        boardHandleClick(board, ev.button.x, ev.button.y, &boardRect);
      }
    }

    SDL_SetRenderDrawColor(renderer, 15, 82, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    char scoreW[100];
    sprintf(scoreW, "White: %d", board->scoreWhite);
    drawTextAlign(renderer, f1, scoreW, boardRect.x, 60, AlignLeftCenter);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    char scoreB[100];
    sprintf(scoreB, "Black: %d", board->scoreBlack);
    drawTextAlign(renderer, f1, scoreB, boardRect.x + boardRect.w, 60,
                  AlignRightCenter);

    // Consider using event driven redraw
    boardDraw(board, renderer, &boardRect);

    SDL_RenderPresent(renderer);
  }

  SDL_Log("Quitting game...\n");
  SDL_Quit();
  return EXIT_SUCCESS;
}
