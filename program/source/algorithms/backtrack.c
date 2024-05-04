#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <algorithms/backtrack.h>

bool backtrack(Kakuro board, SArray current_state) {
    Check * checks = calloc(board.game.empty_count, sizeof(Check));

    bool is_backtrack = false;
    for (size_t i = 0; i < board.game.empty_count && !is_backtrack; i++) {
        if (!(checks[i] & ROWCHECK)) is_backtrack = is_backtrack || _backtrack_row_sum(board, current_state, checks, i);
        if (!(checks[i] & COLCHECK)) is_backtrack = is_backtrack || _backtrack_col_sum(board, current_state, checks, i);
    }
    free(checks);

    return is_backtrack;
}

bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums   = 0, sums   = board.sums[ROW][index];
    ksize_t filled_blocks = 0, blocks = board.blocks[ROW][index];

    for (ksize_t i = 0; i < blocks; i++) {
        checks[board.grid[row][col + i]] |= ROWCHECK;
        State s = current_state.elements[board.grid[row][col + i]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s.mask) + 1; }
    }

    return
        (filled_blocks == blocks) ? filled_sums != sums                                            : false || 
        (filled_sums > sums)                                                                               || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, HIGH) < (sums - filled_sums) : false  || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOW)  > (sums - filled_sums) : false;
}

bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t filled_sums   = 0, sums   = board.sums[COLUMN][index];
    ksize_t filled_blocks = 0, blocks = board.blocks[COLUMN][index];

    for (ksize_t i = 0; i < blocks; i++) {
        checks[board.grid[row + i][col]] |= COLCHECK;
        State s = current_state.elements[board.grid[row + i][col]];
        if (is_one_value(s)) { filled_blocks++; filled_sums += __builtin_ctz(s.mask) + 1; }
    }

    return 
        (blocks == filled_blocks) ? filled_sums != sums                                            : false || 
        (filled_sums > sums)                                                                               || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, HIGH) < (sums - filled_sums) : false  || 
        (blocks  - filled_blocks) ? get_sums(blocks - filled_blocks, LOW)  > (sums - filled_sums) : false;
}
