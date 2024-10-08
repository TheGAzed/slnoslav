#include <gui/interface/grid.h>

#include <string.h>

#include <structures/concrete/board.h>
#include <structures/concrete/state.h>

#include <instance/expect.h>

#include <gui/interface/solver.h>
#include <gui/interface/biggest_rectangle.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define RGB_WHITE (nk_rgb(0xD9, 0xD9, 0xD9))
#define RGB_BLACK (nk_rgb(0x1B, 0x1B, 0x1B))
#define RGB_GRAY  (nk_rgb(0x82, 0x82, 0x82))

#define SUM_SQUARE(block_size) {                                     \
    .x = 0.15 * ((block_size) / 2),  .y = 0.15 * ((block_size) / 2), \
    .w = ((block_size) / 2) * 0.7, .h = ((block_size) / 2) * 0.7,    \
}

#define EMPTY_SQUARE(block_size) {                     \
    .x = 0.1 * (block_size),  .y = 0.1 * (block_size), \
    .w = (block_size) * 0.8, .h = (block_size) * 0.8,  \
}

struct nk_rect _background(struct nk_context * context, board_s board);

void _lines(struct nk_context * context, board_s board, struct nk_rect background);

void _unset_blocks(struct nk_context * context, board_s board, struct nk_rect background);
void _sum_values(struct nk_context * context, board_s board, struct nk_rect background);
void _upper_sum_value(struct nk_context * context, struct nk_rect background, struct nk_rect sum_square, ulookup_t row, ulookup_t col, float block_size, lookup_t value);
void _lower_sum_value(struct nk_context * context, struct nk_rect background, struct nk_rect sum_square, ulookup_t row, ulookup_t col, float block_size, lookup_t value);

void _empty_values(struct nk_context * context, board_s board, struct nk_rect background);
void _single_value(struct nk_context * context, struct nk_rect background, struct nk_rect empty_square, ulookup_t row, ulookup_t col, float block_size, state_t state_value);
void _multi_value(struct nk_context * context, struct nk_rect background, struct nk_rect empty_square, ulookup_t row, ulookup_t col, float block_size, state_t state_value);

void _draw_value(struct nk_context * context, struct nk_rect position, struct nk_color bg, struct nk_color fg, int value);

void grid(struct nk_context * context, board_s board) {
    struct nk_rect background = _background(context, board);
    _lines(context, board, background);
    _unset_blocks(context, board, background);
    _sum_values(context, board, background);
    _empty_values(context, board, background);
}

struct nk_rect _background(struct nk_context * context, board_s board) {
    ulookup_t row_size = board.game.size[ROW_E], col_size = board.game.size[COLUMN_E];
    struct nk_panel * layout = context->current->layout;

    float block_size = MIN((layout->clip.w / col_size), ((WINDOW_HEIGHT - 120) / row_size));

    nk_layout_row_dynamic(context, block_size * row_size, 1);
    struct nk_command_buffer * canvas = &context->current->buffer;

    struct nk_rect background = { 0 };
    nk_widget(&background, context);
    background.w = block_size * col_size;
    background.h = block_size * row_size;
    background.x += (layout->clip.w / 2) - (background.w / 2);
    nk_fill_rect(canvas, background, 0, RGB_WHITE);

    return background;
}

void _lines(struct nk_context * context, board_s board, struct nk_rect background) {
    ulookup_t row_size = board.game.size[ROW_E], col_size = board.game.size[COLUMN_E];
    float block_size = background.w / board.game.size[COLUMN_E];

    for(ulookup_t i = 0; i <= col_size; i++) {
        float x0 = background.x + (block_size * i), y0 = background.y;
        float x1 = background.x + (block_size * i), y1 = background.y + background.h;
        nk_stroke_line(&context->current->buffer, x0, y0, x1, y1, 0.5f, RGB_GRAY);
    }
    for(ulookup_t j = 0; j <= row_size; j++) {
        float x0 = background.x,                y0 = background.y + (block_size * j);
        float x1 = background.x + background.w, y1 = background.y + (block_size * j);
        nk_stroke_line(&context->current->buffer, x0, y0, x1, y1, 0.5f, RGB_GRAY);
    }
}

void _unset_blocks(struct nk_context * context, board_s board, struct nk_rect background) {
    ulookup_t col_size = board.game.size[COLUMN_E];
    float block_size = (background.w / col_size);

    struct nk_rect_array array = create_max_overlap_rectangles(board);
    for (size_t s = 0; s < array.size; s++) {
        struct nk_rect temp = array.elements[s];
        struct nk_rect trans = {
            .x = (temp.x * block_size) + background.x, .y = (temp.y * block_size) + background.y,
            .w = (temp.w * block_size) + 0.25, .h = (temp.h * block_size) + 0.25,
        };
        nk_fill_rect(&context->current->buffer, trans, 0, RGB_BLACK);
    }
    destroy_max_overlap_rectangles(&array);
}

void _sum_values(struct nk_context * context, board_s board, struct nk_rect background) {
    ulookup_t row_size = board.game.size[ROW_E];
    ulookup_t col_size = board.game.size[COLUMN_E];
    float block_size = (background.w / col_size);

    struct nk_rect sum_square = SUM_SQUARE(block_size);
    for (ulookup_t r = 0; r < row_size; r++) {
        for (ulookup_t c = 0; c < col_size; c++) {
            lookup_t row_value = board.game.grids[ROW_E][r][c];
            lookup_t col_value = board.game.grids[COLUMN_E][r][c];

            if (row_value > 0) _upper_sum_value(context, background, sum_square, r, c, block_size, row_value);
            if (col_value > 0) _lower_sum_value(context, background, sum_square, r, c, block_size, col_value);
        }
    }
}

void _upper_sum_value(struct nk_context * context, struct nk_rect background, struct nk_rect sum_square, ulookup_t row, ulookup_t col, float block_size, lookup_t value) {
    struct nk_rect upper_sum = {
        .x = sum_square.x + background.x + (block_size * col) + (block_size / 2),
        .y = sum_square.y + background.y + (block_size * row),
        .w = sum_square.w,
        .h = sum_square.h,
    };

    _draw_value(context, upper_sum, RGB_GRAY, RGB_WHITE, value);
}

void _lower_sum_value(struct nk_context * context, struct nk_rect background, struct nk_rect sum_square, ulookup_t row, ulookup_t col, float block_size, lookup_t value) {
    struct nk_rect lower_sum = {
        .x = sum_square.x + background.x + (block_size * col),
        .y = sum_square.y + background.y + (block_size * row) + (block_size / 2),
        .w = sum_square.w,
        .h = sum_square.h,
    };

    _draw_value(context, lower_sum, RGB_GRAY, RGB_WHITE, value);
}

void _empty_values(struct nk_context * context, board_s board, struct nk_rect background) {
    const ulookup_t col_size = board.game.size[COLUMN_E];
    float block_size = (background.w / col_size);
    struct nk_rect empty_square = EMPTY_SQUARE(block_size);
    state_array_s guess = { 0 };

    if (!(get_player_singleton()->play_state & IS_PLAY)) {
        guess = state_provider(CURRENT_VALUE_DS);
    } else if (get_player_singleton()->play_state == PLAY_FORWARDS_E) {
        guess = state_provider(NEXT_VALUE_TIMED_DS);
    } else if (get_player_singleton()->play_state == PLAY_BACKWARDS_E) {
        guess = state_provider(PREV_VALUE_TIMED_DS);
    }

    for (ulookup_t s = 0; s < guess.size; s++) {
        ulookup_t row = board.coords[ROW_E][s];
        ulookup_t col = board.coords[COLUMN_E][s];
        state_t state = guess.elements[s];

        if (is_one_value(state)) _single_value(context, background, empty_square, row, col, block_size, state);
        else _multi_value(context, background, empty_square, row, col, block_size, state);
    }
}

void _single_value(struct nk_context * context, struct nk_rect background, struct nk_rect empty_square, ulookup_t row, ulookup_t col, float block_size, state_t state_value) {
    struct nk_rect single = {
        .x = empty_square.x + background.x + (block_size * col),
        .y = empty_square.y + background.y + (block_size * row),
        .w = empty_square.w,
        .h = empty_square.h,
    };

    _draw_value(context, single, RGB_WHITE, RGB_BLACK, get_one_value(state_value));
}

void _multi_value(struct nk_context * context, struct nk_rect background, struct nk_rect empty_square, ulookup_t row, ulookup_t col, float block_size, state_t state_value) {
    for (int i = 0; i < MAX_BLOCK_VALUES; ++i) {
        if (!(state_value & (1 << i))) continue;

        struct nk_rect multi = {
            .x = empty_square.x + background.x + (block_size * col) + ((i % 3) * (empty_square.w / 3)),
            .y = empty_square.y + background.y + (block_size * row) + ((i / 3) * (empty_square.h / 3)),
            .w = empty_square.w / 3,
            .h = empty_square.h / 3,
        };

        _draw_value(context, multi, RGB_WHITE, RGB_BLACK, get_one_value(state_value & (1 << i)));
    }
}

void _draw_value(struct nk_context * context, struct nk_rect position, struct nk_color bg, struct nk_color fg, int value) {
    char string_num[sizeof("-2147483647")] = { 0 };
    float temp_height = context->style.font->height;
    struct nk_user_font * font = context->style.font;

    sprintf(string_num, "%d", value);
    font->height = position.h;
    position.x += (position.w / 2) - (font->width(font->userdata, font->height, string_num, strnlen(string_num, sizeof("-2147483647"))) / 2);
    nk_draw_text(&context->current->buffer, position, string_num, strnlen(string_num, sizeof("-2147483647")), font, bg, fg);
    font->height = temp_height;
}
