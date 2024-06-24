#include <assert.h>
#include <instance/expect.h>

#include <algorithms/forward_checking.h>
#include <instance/settings.h>
#include <instance/statistics.h>

bool _row_forward_check(Kakuro board, SArray * current_state, ksize_t index);
bool _col_forward_check(Kakuro board, SArray * current_state, ksize_t index);

bool forward_checking(Kakuro board, SArray * current_state, ksize_t index) {
    expect(
        current_state,
        assert(current_state),
        "current state parameter is NULL (%p)", current_state
    );
    expect(
        get_settings_singleton()->is_forward_check,
        return true,
        ""
    );

    get_stat_singleton()->forward_check_call_count++;

    return !invalid_state_forward_check_stat(!(
        _row_forward_check(board, current_state, index) && 
        _col_forward_check(board, current_state, index)
    ));
}

bool _row_forward_check(Kakuro board, SArray * current_state, ksize_t index) {
    mode = ASSERT_E;
    expect(
        index < current_state->size, NO_ACTION,
        "index '%u' is out of bounds of current state size '%u'", index, current_state->size
    );
    expect(
        is_one_value(current_state->elements[index]), NO_ACTION,
        "current state element at index %u is not a one value", index
    );
    mode = DEFAULT_E;
    
    state_t s = current_state->elements[index];
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index], c;

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

bool _col_forward_check(Kakuro board, SArray * current_state, ksize_t index) {
    expect(index < current_state->size, assert(index < current_state->size),
        "index '%u' is out of bounds of current state size '%u'", index, current_state->size
    );
    expect(is_one_value(current_state->elements[index]), assert(is_one_value(current_state->elements[index])),
        "current state element at index '%u' is not a one value", index
    );
    
    state_t s = current_state->elements[index];
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index], r;

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
