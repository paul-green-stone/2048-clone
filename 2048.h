#include <start/Start.h>

#define SIZE 4
#define WIN_SCORE 2048

#define GRID_CELL_SIZE 97

typedef enum Colors {
    _0,
    _2,
    _4,
    _8,
};

/* ================================================================ */

void board_init(size_t board[4][4]);

void board_print(size_t board[4][4]);

int Board_is_full(const size_t board[SIZE][SIZE]);

int Board_is_won(const size_t board[SIZE][SIZE]);

int Board_can_move(const size_t board[SIZE][SIZE]);

int Board_move(size_t board[SIZE][SIZE], int direction);

int Board_draw(const size_t board[SIZE][SIZE], Text* text[SIZE][SIZE]);

/* ================================================================ */
