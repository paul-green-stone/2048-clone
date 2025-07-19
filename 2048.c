#include "2048.h"

static SDL_Color colors[] = {
    {238, 228, 218, 96},
    {238, 228, 218, 255},
    {237, 224, 200, 255},
    {242, 177, 121, 255},
    {245, 149, 99, 255}, // 16
    {246, 124, 95, 255}, // 32
    {246, 94, 59, 255},
    {237, 207, 114, 255},
    {237, 204, 97, 255},
    {237, 200, 80, 255},
    {237, 197, 63, 255},
    {237, 194, 46, 255}
};

/* ================================================================ */

void board_init(size_t board[4][4]) {

    int t = 0, r, c;
    /* ======== */

    while (1) {

        r = rand() % 4; /* <= row */
        c = rand() % 4; /* <= column */

        if (board[c][r] == 0) {
            
            board[c][r] = ((rand() % 10) == 4) ? 4 : 2;
            t++;
        }

        if (t == 2) {
            break ;
        }
    }

    Manager_insert("0", colors + 0);
    Manager_insert("2", colors + 1);
    Manager_insert("4", colors + 2);
    Manager_insert("8", colors + 3);
    Manager_insert("16", colors + 4);
    Manager_insert("32", colors + 5);
    Manager_insert("64", colors + 6);
    Manager_insert("128", colors + 7);
    Manager_insert("256", colors + 8);
    Manager_insert("512", colors + 9);
    Manager_insert("1024", colors + 10);
    Manager_insert("2048", colors + 11);
}

/* ================================================================ */

void board_print(size_t board[4][4]) {

    int r, c;
    /* ======== */

    for (size_t r = 0; r < 4; r++) {
        for (size_t c = 0; c < 4; c++) {

            printf("%ld", board[r][c]);

            if (c + 1 != 4) {
                printf(", ");
            }
        }

        printf("\n");
    }
}

/* ================================================================ */

int Board_is_full(const size_t board[SIZE][SIZE]) {

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {

            if (!board[i][j]) {
                return 0;
            }
        }
    }

    /* ======== */
    return 1;
}

/* ================================================================ */

int Board_is_won(const size_t board[SIZE][SIZE]) {

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {

            if (board[i][j] == WIN_SCORE) {
                return 1;
            }
        }
    }

    /* ======== */
    return 0;
}

/* ================================================================ */

int Board_can_move(const size_t board[SIZE][SIZE]) {

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {

            /* === There are still cells to move === */
            if (!board[i][j]) {
                return 1;
            }

            /* === Can move left === */
            if ((j > 0) && (board[i][j] == board[i][j - 1])) {
                return 1;
            }

            /* === Can move right === */
            if ((j < (SIZE - 1)) && (board[i][j] == board[i][j + 1])) {
                return 1;
            }

            /* === Can move up === */
            if ((i > 0) && (board[i][j] == board[i - 1][j])) {
                return 1;
            }

            /* === Can move down === */
            if ((i < (SIZE - 1)) && (board[i][j] == board[i + 1][j])) {
                return 1;
            }
        }
    }

    /* ======== */
    return 0;
}

/* ================================================================ */

int Board_move(size_t board[SIZE][SIZE], int dir) {
    int moved = 0;

    // Store the current board state
    int prev_board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            prev_board[i][j] = board[i][j];
        }
    }

    // Move upwards
    if (dir == 0) {
        for (int j = 0; j < SIZE; j++) {
            for (int i = 1; i < SIZE; i++) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k > 0 && board[k - 1][j] == 0) {
                        board[k - 1][j] = board[k][j];
                        board[k][j] = 0;
                        k--;
                        moved = 1;
                    }
                    if (k > 0 && board[k - 1][j] == board[k][j]) {
                        board[k - 1][j] *= 2;
                        board[k][j] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }

    // Move downwards
    else if (dir == 1) {
        for (int j = 0; j < SIZE; j++) {
            for (int i = SIZE - 2; i >= 0; i--) {
                if (board[i][j] != 0) {
                    int k = i;
                    while (k < SIZE - 1 && board[k + 1][j] == 0) {
                        board[k + 1][j] = board[k][j];
                        board[k][j] = 0;
                        k++;
                        moved = 1;
                    }
                    if (k < SIZE - 1 && board[k + 1][j] == board[k][j]) {
                        board[k + 1][j] *= 2;
                        board[k][j] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }

    // Move left
    else if (dir == 2) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 1; j < SIZE; j++) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k > 0 && board[i][k - 1] == 0) {
                        board[i][k - 1] = board[i][k];
                        board[i][k] = 0;
                        k--;
                        moved = 1;
                    }
                    if (k > 0 && board[i][k - 1] == board[i][k]) {
                        board[i][k - 1] *= 2;
                        board[i][k] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }

    // Move right
    else if (dir == 3) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = SIZE - 2; j >= 0; j--) {
                if (board[i][j] != 0) {
                    int k = j;
                    while (k < SIZE - 1 && board[i][k + 1] == 0) {
                        board[i][k + 1] = board[i][k];
                        board[i][k] = 0;
                        k++;
                        moved = 1;
                    }
                    if (k < SIZE - 1 && board[i][k + 1] == board[i][k]) {
                        board[i][k + 1] *= 2;
                        board[i][k] = 0;
                        moved = 1;
                    }
                }
            }
        }
    }

    // Check if the move was successful
    if (moved) {
        // Generate a new random number
        int i = rand() % SIZE;
        int j = rand() % SIZE;
        while (board[i][j] != 0) {
            i = rand() % SIZE;
            j = rand() % SIZE;
        }
        board[i][j] = (rand() % 2 + 1) * 2; // Generate 2 or 4
    }

    // Check if the move was successful
    if (moved) {
        return 1;
    } else {
        // If the move failed, restore the previous board state
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = prev_board[i][j];
            }
        }
        return 0;
    }
}

/* ================================================================ */

int Board_draw(const size_t _board[SIZE][SIZE], Text* _text[SIZE][SIZE]) {

    SDL_Rect board = {0, 0, 450, 450};
    int width = 0;
    int height = 0;

    int x_offset = 12;
    char buffer[16] = "";
    /* ======== */

    SDL_GetWindowSize(get_window(), &width, &height);
    board.x = width / 2 - board.w / 2;
    board.y = height / 2 - board.h / 2;
    
    SDL_RenderSetViewport(get_context(), &board);

    SDL_SetRenderDrawColor(get_context(), 187, 173, 160, 255);
    SDL_RenderFillRect(get_context(), NULL);

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {

            SDL_Rect cell = {j * GRID_CELL_SIZE + x_offset * (j + 1), i * GRID_CELL_SIZE + x_offset * (i + 1), GRID_CELL_SIZE, GRID_CELL_SIZE};

            sprintf(buffer, "%ld", _board[i][j]);
	    Text_update(_text[i][j], buffer);
	    if (_board[i][j] >= 8) {
		Text_set_color(_text[i][j], &(SDL_Color) {255, 255, 255, 255});
	    }
	    else {
		    Text_set_color(_text[i][j], &(SDL_Color) {0, 0, 0, 255});
	    }

	    SDL_Color* color = Manager_lookup(buffer);
            /* === Setting the color === */
            SDL_SetRenderDrawColor(get_context(), color->r, color->g, color->b, color->a);

            SDL_RenderFillRect(get_context(), &cell);

            // if (_board[i][j] == 0) {
            //     SDL_SetRenderDrawColor(get_context(), 238, 228, 218, 96);
            //     SDL_RenderFillRect(get_context(), &cell);
            // }
            // else if (_board[i][j] == 2) {
            //     SDL_SetRenderDrawColor(get_context(), 238, 228, 218, 255);
            //     SDL_RenderFillRect(get_context(), &cell);
            // }
            // else if (_board[i][j] == 4) {
            //     SDL_SetRenderDrawColor(get_context(), 237, 224, 200, 255);
            //     SDL_RenderFillRect(get_context(), &cell);
            // }
            // else if (_board[i][j] == 8) {
            //     SDL_SetRenderDrawColor(get_context(), 242, 177, 121, 255);
                
            // }
            // else {
            //     SDL_SetRenderDrawColor(get_context(), 238, 228, 218, 96);
            //     SDL_RenderFillRect(get_context(), &cell);
            // }

            /* === Drawing text === */
            if (_board[i][j] != 0) {
                Text_draw(_text[i][j], &(SDL_Rect) {cell.x + (GRID_CELL_SIZE / 2) - _text[i][j]->width / 2, cell.y + (GRID_CELL_SIZE / 2) - _text[i][j]->height / 2, _text[i][j]->width, _text[i][j]->height});
            }
        }
    } 

    SDL_RenderSetViewport(NULL, &board);
}

/* ================================================================ */
