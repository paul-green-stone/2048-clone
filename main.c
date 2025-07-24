#include <start/Start.h>
#include <time.h>
#include <sqlite3.h>

#include "2048.h"

#define DB_NAME "2048.db"

int new_game_callback(void* _self, va_list* args) {

    size_t** board = va_arg(*args, size_t**);
    int* score = va_arg(*args, int*);
    Text* _score = va_arg(*args, Text*);
    /* ======== */

    Text_update(_score, "0");
    *score = 0;
    board_init(board);

    /* ======== */
    return 1;
}

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
    Text* title = NULL;

    int width;

    board_init(board);

    Start();
    App_init();

    SDL_SetRenderDrawBlendMode(get_context(), SDL_BLENDMODE_BLEND);

    SDL_Event event;
    SDL_Renderer* ctx = get_context();
    TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", 64);

    SDL_GetWindowSize(get_window(), &width, NULL);

    title = Text_new(ctx, font, &(SDL_Color) {0, 0, 0, 255}, "2048");
    int title_x = width / 2 - 225;

    TTF_Font* font14 = TTF_OpenFont("Roboto-Regular.ttf", 14);

    /* === Button === */

    TTF_Font* font16 = TTF_OpenFont("Roboto-Regular.ttf", 16);
    SDL_Rect btn_bg = {title_x + 335, 140, 115, 40}; // 335 = 450 - 115
    void* new_game_btn = Widget_create(Button, 0, 0, font16, &(SDL_Color) {249, 246, 242, 255}, "New Game", NULL, NULL);
    Widget_bind_callback(new_game_btn, new_game_callback);

    int w, h;
    Widget_get_dimensions(new_game_btn, &w, &h);
    Widget_set_position(new_game_btn, btn_bg.x + (btn_bg.w / 2 - w / 2), btn_bg.y + (btn_bg.h / 2 - h / 2));

    /* ============== */

    /* === Score === */

    SDL_Rect score_rect = {title_x + 450 - 106, 64, 106, 58};
    SDL_Rect best_score_rect = {title_x + 450 - 225, 64, 106, 58};

    int score = 0;
    int best_score;

    Text* best_score_text = Text_new(ctx, font14, &(SDL_Color) {238, 228, 218, 255}, "BEST");
    Text* score_text = Text_new(ctx, font14, &(SDL_Color) {238, 228, 218, 255}, "SCORE");
    
    char buffer[16];
    TTF_Font* semiBold = TTF_OpenFont("Roboto-SemiBold.ttf", 16);
    Text* _score = Text_new(ctx, semiBold, &(SDL_Color) {255, 255, 255, 255}, "0");
    Text* _best_score = Text_new(ctx, semiBold, &(SDL_Color) {255, 255, 255, 255}, "0");

    /* ============= */

    Text* info_text = Text_new(ctx, font16, &(SDL_Color) {119, 110, 101, 255}, "Join the numbers and get to the 2048 tile!");

    for (size_t i = 0; i < SIZE; i++) {
        for (size_t j = 0; j < SIZE; j++) {
            text[i][j] = Text_new(ctx, font, &(SDL_Color) {119, 110, 101, 255}, "");
        }
    }

    int direction = -1;

    sqlite3* db;
    sqlite3_stmt* stmt;
    
    /* === Creating a database === */
    if (!file_exists("2048.db")) {

        if ((sqlite3_open(DB_NAME, &db)) != SQLITE_OK) {
            error(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            /* ======== */
            return 1;
        }

        /* === If database is new, create the scores table === */
        if ((sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS scores (id INTEGER PRIMARY KEY, best_score INTEGER);", 0, 0, 0)) != SQLITE_OK) {
            error(stderr, "Failed to create table: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            /* ======== */
            return 1;
        }

        /* === Inserting initial best score === */
        if ((sqlite3_exec(db, "INSERT INTO scores (best_score) VALUES (0);", 0, 0, 0)) != SQLITE_OK) {
            error(stderr, "Failed to initialize score: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            /* ======== */
            return 1;
        }
    }

        if ((sqlite3_open(DB_NAME, &db)) != SQLITE_OK) {
            error(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
            /* ======== */
            return 1;
        }

        if ((sqlite3_prepare_v2(db, "SELECT best_score FROM scores WHERE id = 1;", -1, &stmt, NULL)) != SQLITE_OK) {
            error(stderr, "Failed to prepare select statement: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            /* ======== */
            return 1;
        }

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            best_score = sqlite3_column_int(stmt, 0);
        }
        
        sqlite3_finalize(stmt);

        sprintf(buffer, "%d", best_score);
        Text_update(_best_score, buffer);

    while (App_isRunning()) {

        while (SDL_PollEvent(&event)) {

            switch (event.type) {

                case SDL_QUIT:
                    App_stop();
                    /* ======== */
                    break ;
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

        Widget_click(new_game_btn, board, &score, _score);

        direction = Input_wasKey_pressed(SDL_SCANCODE_UP) ? 0 : Input_wasKey_pressed(SDL_SCANCODE_RIGHT) ? 3 : Input_wasKey_pressed(SDL_SCANCODE_DOWN) ? 1 : Input_wasKey_pressed(SDL_SCANCODE_LEFT) ? 2 : -1;

        if (Board_move(board, direction, &score)) {

            sprintf(buffer, "%d", score);
            Text_update(_score, buffer);

            if (best_score <= score) {
                best_score = score;

                Text_update(_best_score, buffer);

                if (sqlite3_prepare_v2(db, "UPDATE scores SET best_score = ? WHERE id = 1;", -1, &stmt, NULL) != SQLITE_OK) {
                    error(stderr, "Failed to prepare update statement: %s\n", sqlite3_errmsg(db));
                    /* ======== */
                    App_quit();
                }

                sqlite3_bind_int(stmt, 1, score);

                if (sqlite3_step(stmt) != SQLITE_DONE) {
                    error(stderr, "Failed to update score: %s\n", sqlite3_errmsg(db));
                }

                sqlite3_finalize(stmt);
            }
        }

        /* ================ */

        Input_update();

        /* ================ */
        /* ==== Drawing === */
        /* ================ */

        SDL_SetRenderDrawColor(ctx, 255, 255, 255, 255);
        SDL_RenderClear(ctx);

        Board_draw(board, text);
	
        SDL_RenderSetViewport(ctx, NULL);
        Text_draw(title, &(SDL_Rect) {title_x, 16 + title->height, title->width, title->height});
        Text_draw(info_text, &(SDL_Rect) {title_x, btn_bg.y + btn_bg.h - info_text->height, info_text->width, info_text->height});

        SDL_SetRenderDrawColor(ctx, 143, 122, 102, 255);
        SDL_RenderFillRect(ctx, &btn_bg);

        Widget_draw(new_game_btn, NULL, NULL);

        /* === Score BG === */
        SDL_SetRenderDrawColor(ctx, 187, 173, 160, 255);
        
        SDL_RenderFillRect(ctx, &score_rect);
        SDL_RenderFillRect(ctx, &best_score_rect);

        Text_draw(score_text, &(SDL_Rect) {score_rect.x + (score_rect.w / 2 - score_text->width / 2), score_rect.y + (score_rect.h / 2 - score_text->height), score_text->width, score_text->height});
        Text_draw(best_score_text, &(SDL_Rect) {best_score_rect.x + (best_score_rect.w / 2 - best_score_text->width / 2), best_score_rect.y + (best_score_rect.h / 2 - best_score_text->height), best_score_text->width, best_score_text->height});

        /* === Score Text === */
        Text_draw(_score, &(SDL_Rect) {score_rect.x + (score_rect.w / 2 - _score->width / 2), score_rect.y + (score_rect.h / 2 + _score->height / 5), _score->width, _score->height});

        Text_draw(_best_score, &(SDL_Rect) {best_score_rect.x + (best_score_rect.w / 2 - _best_score->width / 2), best_score_rect.y + (best_score_rect.h / 2 + _best_score->height / 5), _best_score->width, _best_score->height});

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
    Text_destroy(&title);
    Text_destroy(&score_text);
    Text_destroy(&best_score_text);
    Text_destroy(&_score);
    Text_destroy(&_best_score);

    TTF_CloseFont(font);
    TTF_CloseFont(font16);
    TTF_CloseFont(font14);
    TTF_CloseFont(semiBold);

    Widget_destroy(new_game_btn);

    App_quit();
    Stop();

    sqlite3_close(db);
    
    /* ======== */
    return SSUCCESS;
}

/* ================================================================ */
