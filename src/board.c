#include "board.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "draw.h"
#include <stdlib.h>

gameBoard *boardInit() {
  gameBoard *board = malloc(sizeof(*board));
  if (board == NULL)
    return NULL;

  board->turn = CELL_WHITE;

  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      if ((x == 3 && y == 3) || (x == 4 && y == 4))
        board->state[y][x] = CELL_WHITE;
      else if ((x == 4 && y == 3) || (x == 3 && y == 4))
        board->state[y][x] = CELL_BLACK;
      else
        board->state[y][x] = CELL_EMPTY;
    }
  }
  return board;
}

void boardScore(gameBoard *board, int *scoreBlack, int *scoreWhite) {}

void boardMove(gameBoard *board, int x, int y) {}

void boardDraw(gameBoard *board, SDL_Renderer *renderer,
               const SDL_FRect *board_pos) {
  if (board == NULL || renderer == NULL || board_pos == NULL)
    return;

  const float CELL_WIDTH = board_pos->w / 8.f;
  const float CELL_HALF_WIDTH = board_pos->w / 16.f;
  const float CELL_HEIGHT = board_pos->h / 8.f;
  const float CELL_HALF_HEIGHT = board_pos->h / 16.f;
  const float PIECE_RADIUS = SDL_min(CELL_WIDTH, CELL_HEIGHT) * 0.8f * 0.5f;

  SDL_SetRenderDrawColor(renderer, 0x00, 0xa3, 0x10, 0xff);
  SDL_RenderFillRect(renderer, board_pos);

  float mx, my;
  SDL_GetMouseState(&mx, &my);
  int hover_x = -1, hover_y = -1;
  if (mx > board_pos->x && my > board_pos->y &&
      mx < board_pos->x + board_pos->w && my < board_pos->y + board_pos->h) {
    hover_x = (mx - board_pos->x) / CELL_WIDTH;
    hover_y = (my - board_pos->y) / CELL_HEIGHT;
  }

  for (int x = 0; x < 8; ++x) {
    const float cell_pos_x = board_pos->x + (float)(x)*CELL_WIDTH;
    for (int y = 0; y < 8; ++y) {
      const float cell_pos_y = board_pos->y + (float)(y)*CELL_HEIGHT;
      if (hover_x == x && hover_y == y) {
        SDL_FRect cellR = {cell_pos_x, cell_pos_y, CELL_WIDTH, CELL_HEIGHT};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x59, 0x09, 0xff);
        SDL_RenderFillRect(renderer, &cellR);
      }

      SDL_SetRenderDrawColor(renderer, 0x00, 0x17, 0x02, 0xff);
      SDL_RenderLine(renderer, board_pos->x, cell_pos_y,
                     board_pos->x + board_pos->w, cell_pos_y);

      switch (board->state[y][x]) {
      case CELL_WHITE:
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        break;
      case CELL_BLACK:
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
        break;
      case CELL_EMPTY:
      default:
        continue;
      }
      drawFillCircle(renderer, cell_pos_x + CELL_HALF_WIDTH,
                     cell_pos_y + CELL_HALF_HEIGHT, PIECE_RADIUS);
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0x17, 0x02, 0xff);
    SDL_RenderLine(renderer, cell_pos_x, board_pos->y, cell_pos_x,
                   board_pos->y + board_pos->w);
  }
  SDL_SetRenderDrawColor(renderer, 0x00, 0x17, 0x02, 0xff);
  SDL_RenderRect(renderer, board_pos);
}

void boardDestory(gameBoard *board) {
  if (board == NULL)
    return;
  free(board);
}
