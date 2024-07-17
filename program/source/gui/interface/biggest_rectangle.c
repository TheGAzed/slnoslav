#include <gui/interface/biggest_rectangle.h>

#include <stdbool.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_KEYSTATE_BASED_INPUT
#include <nuklear.h>
#include <nuklear_glfw_gl3.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

bool                 _is_inner_rectangle(struct nk_rect inner, struct nk_rect outer);
bool                 _is_outer_rectangle(struct nk_rect inner, struct nk_rect outer);
bool                 _replace_max_rectangle(struct nk_rect_array * array, struct nk_rect guess);

struct nk_rect_array create_max_overlap_rectangles(board_s board) {
    ulookup_t row_size = board.game.size[ROW_E], col_size = board.game.size[COLUMN_E];
    size_t * matrix_row = calloc(col_size, sizeof(size_t));

    struct nk_rect_array array = {
        .elements = malloc((((row_size * col_size) / 2) + 1) * sizeof(struct nk_rect)),
        .size = 0,
    };

    for (size_t r = 0; r < row_size; r++) {
        for (size_t c = 0; c < col_size; c++) {
            matrix_row[c] = board.grid[r][c] == -1 ? matrix_row[c] + 1 : 0;
        }

        for (size_t c = 0; c < col_size; c++) {
            if (matrix_row[c] == 0) continue;
            float height = matrix_row[c], width = 0, x = c;

            while (matrix_row[c] != 0 && c < col_size) {
                height = MIN(height, matrix_row[c]);
                width++;
                c++;
            }

            struct nk_rect temp = { .x = x, .y = r - (height - 1), .w = width, .h = height, };
            if (!_replace_max_rectangle(&array, temp)) array.elements[array.size++] = temp;
        }

        for (size_t c = 0; c < col_size; c++) {
            if (matrix_row[c] == 0) continue;
            struct nk_rect temp = { .x = c, .y = r - (matrix_row[c] - 1), .w = 1, .h = matrix_row[c], };
            if (!_replace_max_rectangle(&array, temp)) array.elements[array.size++] = temp;
        }
    }

    return array;
}

void destroy_max_overlap_rectangles(struct nk_rect_array * array) {
    free(array->elements);
    array->elements = NULL;
    array->size = 0;
}

bool _is_inner_rectangle(struct nk_rect inner, struct nk_rect outer) {
    return
        (inner.x >= outer.x) && (inner.y >= outer.y) &&
        ((inner.w + inner.x) <= (outer.w + outer.x)) &&
        ((inner.h + inner.y) <= (outer.h + outer.y));
}

bool _is_outer_rectangle(struct nk_rect inner, struct nk_rect outer) {
    return _is_inner_rectangle(outer, inner);
}

bool _replace_max_rectangle(struct nk_rect_array * array, struct nk_rect guess) {
    bool replaced = false;
    for (size_t s = 0; s < array->size; s++) {
        if (_is_inner_rectangle(array->elements[s], guess)) {
            array->elements[s] = guess;
            replaced = true;
        }
        if (_is_outer_rectangle(array->elements[s], guess)) replaced = true;
    }

    return replaced;
}
