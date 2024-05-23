#include <assert.h>

#include <algorithms/forward_checking.h>

void _row_check(Kakuro board, SArray * current_state, ksize_t index);
void _col_check(Kakuro board, SArray * current_state, ksize_t index);

void forward_checking(Kakuro board, SArray * current_state, ksize_t index) {
    _row_check(board, current_state, index);
    _col_check(board, current_state, index);
}

void _row_check(Kakuro board, SArray * current_state, ksize_t index) {
    assert(index < current_state->size);
    assert(is_one_value(current_state->elements[index]));
    
    State s = current_state->elements[index];
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index], c;

    c = col;
    while (!is_wall_hit(board, row, --c)) current_state->elements[board.grid[row][c]].mask &= ~s.mask;

    c = col;
    while (!is_wall_hit(board, row, ++c)) current_state->elements[board.grid[row][c]].mask &= ~s.mask;
}

void _col_check(Kakuro board, SArray * current_state, ksize_t index) {
    assert(index < current_state->size);
    assert(is_one_value(current_state->elements[index]));
    
    State s = current_state->elements[index];
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index], r;

    r = row;
    while (!is_wall_hit(board, --r, col)) current_state->elements[board.grid[r][col]].mask &= ~s.mask;

    r = row;
    while (!is_wall_hit(board, ++r, col)) current_state->elements[board.grid[r][col]].mask &= ~s.mask;  
}
