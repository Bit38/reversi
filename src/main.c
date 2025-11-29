#include "SDL3/SDL_init.h"
#include "board.h"
#include "draw.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;

gameBoard *board;

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to inintialise SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("reversi", 800, 900, 0, &window,
                                   &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to create window and renderer: %s",
                 SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_Log("SDL initialised successfully!\n");

  board = boardInit();
  const SDL_FRect boardRect = {20.f, 120.f, 760.f, 760.f};

  bool quit = false;
  while (!quit) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT)
        quit = true;
    }

    // Board
    SDL_SetRenderDrawColor(renderer, 15, 82, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 28, 150, 0, 255);

    // const SDL_FRect rect = {20.f, 120.f, 760.f, 760.f};
    // SDL_RenderFillRect(renderer, &rect);
    // SDL_SetRenderDrawColor(renderer, 5, 28, 0, 255);
    // SDL_RenderRect(renderer, &rect);

    boardDraw(board, renderer, &boardRect);

    SDL_RenderPresent(renderer);
  }

  SDL_Log("Quitting game...\n");
  SDL_Quit();
  return EXIT_SUCCESS;
}
