#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <stdlib.h>

SDL_Window *window;
SDL_Renderer *renderer;

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to inintialise SDL: %s\n", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("reversi", 600, 800, 0, &window,
                                   &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR,
                 "ERROR: Failed to create window and renderer: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }
  SDL_Log("SDL initialised successfully!\n");

  SDL_SetRenderDrawColor(renderer, 0, 179, 33, 255);

  bool quit = false;
  while (!quit) {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_EVENT_QUIT)
        quit = true;
    }
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
  }

  SDL_Log("Quitting game...\n");
  SDL_Quit();
  return EXIT_SUCCESS;
}
