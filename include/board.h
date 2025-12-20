#ifndef INCLUDE_BOARD_H_
#define INCLUDE_BOARD_H_

#include <SDL3/SDL.h>
#include <stdint.h>
typedef enum boardCell { CELL_EMPTY, CELL_WHITE, CELL_BLACK } boardCell;
boardCell reverseCell(const boardCell cell);

typedef struct gameBoard {
  boardCell turn;

  int scoreBlack, scoreWhite, legal_count;
  bool legal_move[8][8];
  boardCell state[8][8];
} gameBoard;

gameBoard *boardInit();
void boardReCalcScore(gameBoard *board);
void boardCalcLegalMoves(gameBoard *board);
void boardHandleClick(gameBoard *board, int x, int y,
                      const SDL_FRect *board_pos);
void boardMove(gameBoard *board, int x, int y);
void boardDraw(gameBoard *board, SDL_Renderer *renderer,
               const SDL_FRect *board_pos);
boardCell boardWin(gameBoard *board);
void boardDestory(gameBoard *board);

#endif // INCLUDE_BOARD_H_
