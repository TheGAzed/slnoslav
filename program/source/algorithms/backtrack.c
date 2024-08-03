#include <stdlib.h>
#include <string.h>

#include <structures/concrete/state.h>
#include <algorithms/backtrack.h>
#include <instance/settings.h>
#include <instance/statistics.h>
#include <instance/expect.h>

bool _backtrack_row_sum(board_s board, state_array_s current_state, size_t index);
bool _backtrack_col_sum(board_s board, state_array_s current_state, size_t index);

bool _backtrack_valid_sums(board_s board, state_array_s current_state);
bool _backtrack_valid_row_sums(board_s board, state_array_s current_state, size_t index);
bool _backtrack_valid_col_sums(board_s board, state_array_s current_state, size_t index);

bool _backtrack_row_repeat(board_s board, state_array_s current_state, size_t index);
bool _backtrack_col_repeat(board_s board, state_array_s current_state, size_t index);

bool backtrack(board_s board, state_array_s current_state) {
    if (!get_settings_singleton()->is_backtrack) return _backtrack_valid_sums(board, current_state);

    get_stat_singleton()->backtrack_call_count++;

    check_e * checks = calloc(board.game.empty_count, sizeof(check_e));

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        if (!(checks[i] & ROWCHECK)) is_backtrack = 
            is_backtrack || _backtrack_row_sum(board, current_state, i) || _backtrack_row_repeat(board, current_state, i);

        if (!(checks[i] & COLCHECK)) is_backtrack = 
            is_backtrack || _backtrack_col_sum(board, current_state, i) || _backtrack_col_repeat(board, current_state, i);

        add_check(board, checks, i);
    }

    free(checks);

    return (is_backtrack);
}

bool _backtrack_row_sum(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    ulookup_t filled_sums   = 0, sums   = board.sums[ROW_E][index];
    ulookup_t filled_blocks = 0, blocks = board.blocks[ROW_E][index];

    for (ulookup_t i = 0; i < blocks; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s) + 1; }
    }

    return
        ((filled_blocks == blocks) ? filled_sums != sums                                                 : false) ||
        (filled_sums > sums)                                                                                    || 
        ((blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, UPPER_EDGE_E) < (sums - filled_sums) : false) ||
        ((blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOWER_EDGE_E) > (sums - filled_sums) : false);
}

bool _backtrack_col_sum(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    ulookup_t filled_sums   = 0, sums   = board.sums[COLUMN_E][index];
    ulookup_t filled_blocks = 0, blocks = board.blocks[COLUMN_E][index];

    for (ulookup_t i = 0; i < blocks; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s) + 1; }
    }

    return 
        ((blocks == filled_blocks) ? filled_sums != sums                                                 : false) ||
        (filled_sums > sums)                                                                                      ||
        ((blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, UPPER_EDGE_E) < (sums - filled_sums) : false) ||
        ((blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOWER_EDGE_E) > (sums - filled_sums) : false);
}

bool _backtrack_valid_sums(board_s board, state_array_s current_state) {
    error_mode = DEFAULT_E;
    expect(is_end_state(current_state), return false, "current state is not an end state");

    check_e * checks = calloc(board.game.empty_count, sizeof(check_e));

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        if (!(checks[i] & ROWCHECK)) is_backtrack = 
            is_backtrack || _backtrack_valid_row_sums(board, current_state, i) || _backtrack_row_repeat(board, current_state, i);

        if (!(checks[i] & COLCHECK)) is_backtrack = 
            is_backtrack || _backtrack_valid_col_sums(board, current_state, i) || _backtrack_col_repeat(board, current_state, i);

        add_check(board, checks, i);
    }

    free(checks);
    
    return is_backtrack;
}

bool _backtrack_valid_row_sums(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    ulookup_t filled_sums = 0, sums = board.sums[ROW_E][index];

    for (ulookup_t i = 0; i < board.blocks[ROW_E][index]; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        filled_sums += __builtin_ctz(s) + 1;
    }

    return filled_sums != sums;
}

bool _backtrack_valid_col_sums(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    ulookup_t filled_sums = 0, sums = board.sums[COLUMN_E][index];

    for (ulookup_t i = 0; i < board.blocks[COLUMN_E][index]; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        filled_sums += __builtin_ctz(s) + 1;
    }

    return filled_sums != sums;
}

bool _backtrack_row_repeat(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    state_t state = INVALID_STATE;

    ulookup_t block = board.blocks[ROW_E][index];
    for (ulookup_t i = 0; i < block; i++) {
        state_t s = current_state.elements[board.grid[row][col + i]];
        if (!is_one_value(s)) continue;

        if (state & s) return true;
        else state |= s;
    }

    return false;
}

bool _backtrack_col_repeat(board_s board, state_array_s current_state, size_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    state_t state = INVALID_STATE;

    ulookup_t block = board.blocks[COLUMN_E][index];
    for (ulookup_t i = 0; i < block; i++) {
        state_t s = current_state.elements[board.grid[row + i][col]];
        if (!is_one_value(s)) continue;

        if (state & s) return true;
        else state |= s;
    }

    return false;
}
