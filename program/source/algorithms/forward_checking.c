#include <assert.h>

#include <algorithms/forward_checking.h>
#include <instance/settings.h>
#include <instance/statistics.h>
#include <instance/expect.h>

bool _row_forward_check(board_s board, state_array_s * current_state, ulookup_t index);
bool _col_forward_check(board_s board, state_array_s * current_state, ulookup_t index);

bool forward_checking(board_s board, state_array_s * current_state, ulookup_t index) {
    error_mode = EXIT_E;
    expect(current_state, DEBUG_ACTION, "current state parameter is NULL (%p)", (void*)current_state);
    expect(is_one_value(current_state->elements[index]), DEBUG_ACTION, "current state element at index %u is not a one value", index);
    expect(index < current_state->size, DEBUG_ACTION, "index '%u' is out of bounds of current state size '%u'", index, current_state->size);

    if (!get_settings_singleton()->is_forward_check) return true;

    bool valid_forward_check = _row_forward_check(board, current_state, index) && _col_forward_check(board, current_state, index);
    if (!valid_forward_check) get_stat_singleton()->bad_forward_check_count++;
    return valid_forward_check;
}

bool _row_forward_check(board_s board, state_array_s * current_state, ulookup_t index) {   
    state_t s = current_state->elements[index];
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index], c;

    c = col;
    while (!is_wall_hit(board, row, --c)) {
        current_state->elements[board.grid[row][c]] &= ~s;
        if (current_state->elements[board.grid[row][c]] == INVALID_STATE) return false;
    }

    c = col;
    while (!is_wall_hit(board, row, ++c)) {
        current_state->elements[board.grid[row][c]] &= ~s;
        if (current_state->elements[board.grid[row][c]] == INVALID_STATE) return false;
    }

    return true;
}

bool _col_forward_check(board_s board, state_array_s * current_state, ulookup_t index) {
    state_t s = current_state->elements[index];
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index], r;

    r = row;
    while (!is_wall_hit(board, --r, col)) {
        current_state->elements[board.grid[r][col]] &= ~s;
        if (current_state->elements[board.grid[r][col]] == INVALID_STATE) return false;
    }

    r = row;
    while (!is_wall_hit(board, ++r, col)) {
        current_state->elements[board.grid[r][col]] &= ~s;
        if (current_state->elements[board.grid[r][col]] == INVALID_STATE) return false;
    }

    return true;
}
