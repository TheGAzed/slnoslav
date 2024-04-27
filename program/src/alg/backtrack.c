#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <alg/backtrack.h>

#ifndef SHOW_HIDDEN_FUNCTIONS

bool _backtrack_sum(Kakuro board, SArray current_state, Check * checks, size_t index, Check type);
bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index);
bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index);

#endif

bool backtrack(Kakuro board, SArray current_state) {
    Check * checks = malloc(board.game.empty_count * sizeof(Check));
    memset(checks, UNCHECKED, board.game.empty_count);

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        is_backtrack = _backtrack_sum(board, current_state, checks, i, checks[i]);
    }
    free(checks);

    return is_backtrack;
}

bool _backtrack_sum(Kakuro grid, SArray current_state, Check * checks, size_t index, Check type) {
    return 
        (type & ROWCHECK) ? _backtrack_row_sum(grid, current_state, checks, index) : false || 
        (type & COLCHECK) ? _backtrack_col_sum(grid, current_state, checks, index) : false;
}

bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index) {
    size_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    size_t filled_sums   = 0, sums   = board.sums[ROW][index];
    size_t filled_blocks = 0, blocks = board.blocks[ROW][index];

    for (size_t i = 0; i < blocks; i++) {
        checks[board.grid[row + i][col]] ^= ROWCHECK;
        State s = current_state.elements[board.grid[row + i][col]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s.mask) + 1; }
    }

    return
        (filled_blocks == blocks) ? filled_sums != sums                                            : false ||
        (filled_sums >= sums)                                                                              ||
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, HIGH) <= (sums - filled_sums) : false ||
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOW)  >= (sums - filled_sums) : false;
}

bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index) {
    size_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    size_t filled_sums   = 0, sums   = board.sums[COLUMN][index];
    size_t filled_blocks = 0, blocks = board.blocks[COLUMN][index];

    for (size_t i = 0; i < blocks; i++) {
        checks[board.grid[row][col + i]] ^= COLCHECK;
        State s = current_state.elements[board.grid[row][col + i]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s.mask) + 1; }
    }

    return 
        (blocks == filled_blocks) ? filled_sums != sums                                            : false ||
        (filled_sums >= sums)                                                                              ||
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, HIGH) <= (sums - filled_sums) : false ||
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOW)  >= (sums - filled_sums) : false;
}
