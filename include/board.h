#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <stdint.h>
typedef enum boardCell { CELL_EMPTY, CELL_WHITE, CELL_BLACK } boardCell;

typedef struct gameBoard {
  boardCell turn;

  size_t legal_size;
  bool legal[8][8]; // To store legal moves
  boardCell state[8][8];
} gameBoard;

gameBoard *boardInit();
void boardScore(gameBoard *board, int *scoreBlack, int *scoreWhite);
void boardCalcLegalMoves(gameBoard *board);
void boardHandleClick(gameBoard *board, int x, int y,
                      const SDL_FRect *board_pos);
void boardMove(gameBoard *board, int x, int y);
void boardDraw(gameBoard *board, SDL_Renderer *renderer,
               const SDL_FRect *board_pos);
void boardDestory(gameBoard *board);

#endif // INCLUDE_BOARD_H_
