#include "board.h"
#include "SDL3/SDL_log.h"
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
  boardCalcLegalMoves(board);
  return board;
}

void boardScore(gameBoard *board, int *scoreBlack, int *scoreWhite) {}

#define _BOARD_NUM_DIRECTIONS 8

boardCell reverseCell(boardCell cell) {
  switch (cell) {
  case CELL_WHITE:
    return CELL_BLACK;
  case CELL_BLACK:
    return CELL_WHITE;
  default:
    return CELL_EMPTY;
  };
}

bool isDirectionCorrect(const gameBoard *board, int x, int y, int dx, int dy) {
  boardCell revTurn = reverseCell(board->turn);

  int delta = 0;
  int nx = x + dx;
  int ny = y + dy;
  while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
    if (board->state[ny][nx] == CELL_EMPTY)
      break;
    if (board->state[ny][nx] == revTurn)
      goto _cell_while_cont;
    if (board->state[ny][nx] == board->turn) {
      if (delta == 0)
        return false;
      else
        return true;
    }

  _cell_while_cont:
    nx += dx;
    ny += dy;
    delta++;
  }
  return false;
}

bool isMoveCorrect(const gameBoard *board, int x, int y) {
  const int directions[_BOARD_NUM_DIRECTIONS][2] = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
  };

  if (board->state[y][x] != CELL_EMPTY) {
    return false;
  }

  boardCell revTurn = reverseCell(board->turn);

  for (int i = 0; i < _BOARD_NUM_DIRECTIONS; ++i) {
    int delta_x = directions[i][0];
    int delta_y = directions[i][1];

    if (isDirectionCorrect(board, x, y, delta_x, delta_y))
      return true;
  }
  return false;
}

void boardCalcLegalMoves(gameBoard *board) {
  for (int x = 0; x < 8; ++x) {
    for (int y = 0; y < 8; ++y) {
      board->legal[y][x] = isMoveCorrect(board, x, y);
    }
  }
}

void boardHandleClick(gameBoard *board, int x, int y,
                      const SDL_FRect *board_pos) {
  const float CELL_WIDTH = board_pos->w / 8.f;
  const float CELL_HALF_WIDTH = board_pos->w / 16.f;
  const float CELL_HEIGHT = board_pos->h / 8.f;
  const float CELL_HALF_HEIGHT = board_pos->h / 16.f;
  if (x < board_pos->x || y < board_pos->y || x > board_pos->x + board_pos->w ||
      y > board_pos->y + board_pos->h)
    return;
  int cell_x = (x - board_pos->x) / CELL_WIDTH;
  int cell_y = (y - board_pos->y) / CELL_HEIGHT;

  if (board->state[cell_y][cell_x] != CELL_EMPTY ||
      !board->legal[cell_y][cell_x])
    return;

  boardMove(board, cell_x, cell_y);
}

void boardMove(gameBoard *board, int x, int y) {
  if (!board->legal[y][x])
    return;

  const int directions[_BOARD_NUM_DIRECTIONS][2] = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1},
  };
  bool valid_direction[_BOARD_NUM_DIRECTIONS] = {false, false, false, false,
                                                 false, false, false, false};

  for (int i = 0; i < _BOARD_NUM_DIRECTIONS; ++i) {
    int delta_x = directions[i][0];
    int delta_y = directions[i][1];

    valid_direction[i] = isDirectionCorrect(board, x, y, delta_x, delta_y);
  }

  for (int i = 0; i < _BOARD_NUM_DIRECTIONS; ++i) {
    if (!valid_direction[i])
      continue;

    boardCell revTurn = reverseCell(board->turn);
    int dx = directions[i][0];
    int dy = directions[i][1];
    int nx = x + dx;
    int ny = y + dy;
    while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
      if (board->state[ny][nx] == revTurn)
        board->state[ny][nx] = board->turn;
      else
        break;

      nx += dx;
      ny += dy;
    }
  }
  board->state[y][x] = board->turn;
  board->turn = reverseCell(board->turn);
  boardCalcLegalMoves(board);
}

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
        if (board->legal[y][x])
          SDL_SetRenderDrawColor(renderer, 0x17, 0x55, 0x87, 0xff);
        else
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
