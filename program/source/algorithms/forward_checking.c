#include <assert.h>

#include <algorithms/forward_checking.h>
#include <instance/settings.h>

bool _row_forward_check(Kakuro board, SArray * current_state, ksize_t index);
bool _col_forward_check(Kakuro board, SArray * current_state, ksize_t index);

bool forward_checking(Kakuro board, SArray * current_state, ksize_t index) {
    if (!(get_settings_singleton()->is_forward_check)) return true;

    return 
        _row_forward_check(board, current_state, index) && 
        _col_forward_check(board, current_state, index);
}

bool _row_forward_check(Kakuro board, SArray * current_state, ksize_t index) {
    assert(index < current_state->size);
    assert(is_one_value(current_state->elements[index]));
    
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
    assert(index < current_state->size);
    assert(is_one_value(current_state->elements[index]));
    
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
