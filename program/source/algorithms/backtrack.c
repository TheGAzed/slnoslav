#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <structures/concrete/state.h>
#include <algorithms/backtrack.h>
#include <instance/settings.h>
#include <instance/statistics.h>

#ifdef _MSC_VER

#include <intrin.h>
#define __builtin_ctz __lzcnt

#endif /* _MSC_VER */

bool _backtrack_row_sum(Kakuro board, SArray current_state, size_t index);
bool _backtrack_col_sum(Kakuro board, SArray current_state, size_t index);

bool _backtrack_valid_sums(Kakuro board, SArray current_state);
bool _backtrack_valid_row_sums(Kakuro board, SArray current_state, size_t index);
bool _backtrack_valid_col_sums(Kakuro board, SArray current_state, size_t index);

bool _backtrack_row_repeat(Kakuro board, SArray current_state, size_t index);
bool _backtrack_col_repeat(Kakuro board, SArray current_state, size_t index);

bool backtrack(Kakuro board, SArray current_state) {
    if (!(get_settings_singleton()->is_backtrack)) return _backtrack_valid_sums(board, current_state);
    get_stat_singleton()->backtrack_call_count++;

    Check checks[KAKURO_SIZE_MAX] = { 0 };

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        if (!(checks[i] & ROWCHECK)) is_backtrack = 
            is_backtrack || _backtrack_row_sum(board, current_state, i) || _backtrack_row_repeat(board, current_state, i);

        if (!(checks[i] & COLCHECK)) is_backtrack = 
            is_backtrack || _backtrack_col_sum(board, current_state, i) || _backtrack_col_repeat(board, current_state, i);

        add_check(board, checks, i);
    }

    return invalid_state_backtrack_stat(is_backtrack);
}

bool _backtrack_row_sum(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums   = 0, sums   = board.sums[ROW][index];
    ksize_t filled_blocks = 0, blocks = board.blocks[ROW][index];

    for (ksize_t i = 0; i < blocks; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s) + 1; }
    }

    return
        (filled_blocks == blocks) ? filled_sums != sums                                                 : false || 
        (filled_sums > sums)                                                                                    || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, UPPER_EDGE) < (sums - filled_sums) : false || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOWER_EDGE) > (sums - filled_sums) : false ||
        _backtrack_row_repeat(board, current_state, index);
}

bool _backtrack_col_sum(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums   = 0, sums   = board.sums[COLUMN][index];
    ksize_t filled_blocks = 0, blocks = board.blocks[COLUMN][index];

    for (ksize_t i = 0; i < blocks; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s) + 1; }
    }

    return 
        (blocks == filled_blocks) ? filled_sums != sums                                                 : false || 
        (filled_sums > sums)                                                                                    || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, UPPER_EDGE) < (sums - filled_sums) : false || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOWER_EDGE) > (sums - filled_sums) : false ||
        _backtrack_col_repeat(board, current_state, index);
}

bool _backtrack_valid_sums(Kakuro board, SArray current_state) {
    if (!is_end_state(current_state)) return false;

    Check checks[KAKURO_SIZE_MAX] = { 0 };

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        if (!(checks[i] & ROWCHECK)) is_backtrack = 
            is_backtrack || _backtrack_valid_row_sums(board, current_state, i) || _backtrack_row_repeat(board, current_state, i);

        if (!(checks[i] & COLCHECK)) is_backtrack = 
            is_backtrack || _backtrack_valid_col_sums(board, current_state, i) || _backtrack_col_repeat(board, current_state, i);

        add_check(board, checks, i);
    }
    
    return is_backtrack;
}

bool _backtrack_valid_row_sums(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums = 0, sums = board.sums[ROW][index];

    for (ksize_t i = 0; i < board.blocks[ROW][index]; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        filled_sums += __builtin_ctz(s) + 1;
    }

    return filled_sums != sums;
}

bool _backtrack_valid_col_sums(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums = 0, sums = board.sums[COLUMN][index];

    for (ksize_t i = 0; i < board.blocks[COLUMN][index]; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        filled_sums += __builtin_ctz(s) + 1;
    }

    return filled_sums != sums;
}

bool _backtrack_row_repeat(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    state_t state = INVALID_STATE;

    ksize_t block = board.blocks[ROW][index];
    for (ksize_t i = 0; i < block; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        if (!is_one_value(s)) continue;

        if (state & s) return true;
        else state |= s;
    }

    return false;
}

bool _backtrack_col_repeat(Kakuro board, SArray current_state, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    state_t state = INVALID_STATE;

    ksize_t block = board.blocks[COLUMN][index];
    for (ksize_t i = 0; i < block; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        if (!is_one_value(s)) continue;

        if (state & s) return true;
        else state |= s;
    }

    return false;
}
