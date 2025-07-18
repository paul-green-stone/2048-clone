#include <start/Start.h>
#include <time.h>

#include "2048.h"

/* ================================================================ */

int main(int argc, char** argv) {

    srand(time(NULL));

    size_t board[SIZE][SIZE] = {
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    };

    Text* text[SIZE][SIZE];

    board_init(board);
    board_print(board);

    Start();
    App_init();

    SDL_SetRenderDrawBlendMode(get_context(), SDL_BLENDMODE_BLEND);

    SDL_Event event;
    SDL_Renderer* ctx = get_context();
    TTF_Font* font = TTF_OpenFont("8bitOperatorPlus8-Regular.ttf", 64);

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            text[i][j] = Text_new(ctx, font, &(SDL_Color) {0, 0, 0, 255}, "");
        }
    }

    int direction = -1;

    while (App_isRunning()) {

        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_QUIT:
                    App_stop();
                    /* ======== */
                    break ;
            }

            if (Input_wasKey_pressed(SDL_SCANCODE_LEFT)) {
                
            }

            if (Board_is_won(board)) {

                App_stop();
                /* ======== */
                break ;
            }

            if (Board_is_full(board) && !Board_can_move(board)) {

                App_stop();
                /* ======== */
                break ;
            }
            
        }

        direction = Input_wasKey_pressed(SDL_SCANCODE_UP) ? 0 : Input_wasKey_pressed(SDL_SCANCODE_RIGHT) ? 3 : Input_wasKey_pressed(SDL_SCANCODE_DOWN) ? 1 : Input_wasKey_pressed(SDL_SCANCODE_LEFT) ? 2 : -1;

        if (Board_move(board, direction)) {
            board_print(board);
        }

        /* ================ */

        Input_update();

        /* ================ */
        /* ==== Drawing === */
        /* ================ */

        SDL_SetRenderDrawColor(ctx, 255, 255, 255, 255);
        SDL_RenderClear(ctx);

        Board_draw(board, text);

        /* ================ */
        /* === Rendering == */
        /* ================ */

        App_render();
    }

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            Text_destroy(&text[i][j]);
        }
    }

    /* ======== */
    TTF_CloseFont(font);

    App_quit();
    Stop();
    
    /* ======== */
    return SSUCCESS;
}

/* ================================================================ */
