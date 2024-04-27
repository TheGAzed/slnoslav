#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include <alg/forward_check.h>

bool forward_check(Kakuro board, SArray * current_state) {   
    _reduce_multi_values(board, current_state);
    _reduce_one_values(board, current_state);
    
    SArray copy = copy_state(*current_state);
    while (!compare_states(copy, *current_state)) {
        free_state(&copy);

        copy = copy_state(*current_state);
        _reduce_multi_values(board, current_state);
        _reduce_one_values(board, current_state);
    }
    free_state(&copy);
    
    return valid_states(*current_state);
}

State _check_magic_table(ksize_t blocks, ksize_t sum) {
    static MTable t = { 
        .blocks  = { 2, 3, 4, 5, 6, 7, 8 },
        .lengths = { 4, 4, 4, 4, 4, 4, 9 },

        .B = { 
            (State){ .mask = 0b000000011 }, (State){ .mask = 0b000000101 }, 
            (State){ .mask = 0b101000000 }, (State){ .mask = 0b110000000 }, 
        },
        .C = { 
            (State){ .mask = 0b000000111 }, (State){ .mask = 0b000001011 }, 
            (State){ .mask = 0b110100000 }, (State){ .mask = 0b111000000 }, 
        },
        .D = { 
            (State){ .mask = 0b000001111 }, (State){ .mask = 0b000010111 }, 
            (State){ .mask = 0b111010000 }, (State){ .mask = 0b111100000 }, 
            },
        .E = { 
            (State){ .mask = 0b000011111 }, (State){ .mask = 0b000101111 }, 
            (State){ .mask = 0b111101000 }, (State){ .mask = 0b111110000 }, 
        },
        .F = { 
            (State){ .mask = 0b000111111 }, (State){ .mask = 0b001011111 }, 
            (State){ .mask = 0b111110100 }, (State){ .mask = 0b111111000 }, 
        },
        .G = { 
            (State){ .mask = 0b001111111 }, (State){ .mask = 0b010111111 }, 
            (State){ .mask = 0b111111010 }, (State){ .mask = 0b111111100 }, 
        },
        .H = { 
            (State){ .mask = 0b011111111 }, (State){ .mask = 0b101111111 }, 
            (State){ .mask = 0b110111111 }, (State){ .mask = 0b111011111 },
            (State){ .mask = 0b111101111 }, (State){ .mask = 0b111110111 }, 
            (State){ .mask = 0b111111011 }, (State){ .mask = 0b111111101 },
            (State){ .mask = 0b111111110 },
        },

        .united[0] = t.B, .united[1] = t.C, .united[2] = t.D, .united[3] = t.E, 
        .united[4] = t.F, .united[5] = t.G, .united[6] = t.H, 
    };

    assert(blocks >= 1 && blocks <= 9 && "BLOCK SIZE NOT IN RANGE");

    if (blocks == 1 || blocks == 9) return (State){ .mask = FULL_STATE, };
    
    for (ksize_t a = 0, b = t.lengths[blocks - 2] - 1, d = (a + (b - a)) >> 1; a <= b; d = (a + (b - a)) >> 1) {
        State s = t.united[blocks - 2][d];
        if (state_to_sums(s) == sum) return s;
        else if (state_to_sums(s) < sum) b = d - 1;
        else a = d + 1;
    }
    
    return (State){ .mask = FULL_STATE, };
}

State _check_even_two_blocks(ksize_t block, ksize_t sum) {
    State s = { .mask = (block == 2) && (sum & 1) ? FULL_STATE ^ (1 << ((sum >> 1) - 1)) : FULL_STATE };
    return s;
}

State _check_end(ksize_t block, ksize_t sum, SType type) {
    ksize_t dif = sum - get_sums(block - 1, type);
    bool cond = type == HIGH ? (dif > LOW) : (dif < HIGH);
    return cond ? get_state(dif, type) : (State) { .mask = FULL_STATE };
}

void _reduce_multi_values(Kakuro board, SArray * current_state) {
    Check * checks = malloc(board.game.empty_count * sizeof(Check));
    memset(checks, UNCHECKED, board.game.empty_count);

    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (checks[i] & ROWCHECK) _reduce_row_multi_values(board, current_state, checks, i);
        if (checks[i] & COLCHECK) _reduce_col_multi_values(board, current_state, checks, i);
    }
    
    free(checks);
}

void _reduce_row_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[ROW][index], sums = board.sums[ROW][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    IStack multi = init_istack();

    for (size_t i = 0; i < block; i++) {
        checks[board.grid[row + i][col]] ^= ROWCHECK;
        State s = current_state->elements[board.grid[row + 1][col]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_istack(&multi, board.grid[row + i][col]);
    }
    
    while (!is_empty_istack(multi)) {
        size_t i = pop_istack(&multi);
        current_state->elements[i].mask &= 
            _check_magic_table(empty_blocks, empty_sums).mask & 
            _check_even_two_blocks(empty_blocks, empty_sums).mask &
            _check_end(empty_blocks, empty_sums, HIGH).mask &
            _check_end(empty_blocks, empty_sums, LOW).mask;
    }
}

void _reduce_col_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[COLUMN][index], sums = board.sums[COLUMN][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    IStack multi = init_istack();

    for (size_t i = 0; i < block; i++) {
        checks[board.grid[row][col + i]] ^= COLCHECK;
        State s = current_state->elements[board.grid[row][col + i]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_istack(&multi, board.grid[row][col + i]);
    }
    
    while (!is_empty_istack(multi)) {
        size_t i = pop_istack(&multi);
        current_state->elements[i].mask &= 
            _check_magic_table(empty_blocks, empty_sums).mask & 
            _check_even_two_blocks(empty_blocks, empty_sums).mask &
            _check_end(empty_blocks, empty_sums, HIGH).mask &
            _check_end(empty_blocks, empty_sums, LOW).mask;
    }
}

void _reduce_one_values(Kakuro board, SArray * current_state) {
    IStack ones = init_istack();

    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (is_one_value(current_state->elements[i])) push_istack(&ones, i);
    }

    while (!is_empty_istack(ones)) {
        size_t index = pop_istack(&ones);
        _reduce_row_one_values(board, &ones, current_state, index);
        _reduce_col_one_values(board, &ones, current_state, index);
    }
    
}

void _reduce_row_one_values(Kakuro board, IStack * ones, SArray * current_state, ksize_t index) {
    assert(is_one_value(current_state->elements[index]) && "EXPECTED ONE VALUE STATE");

    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (size_t c = col - 1; !is_wall_hit(board, row, c); c--) {
        State * s = &(current_state->elements[board.grid[row][c]]);
        if (is_one_value(*s)) continue;
        (*s).mask &= ~(current_state->elements[index].mask);
        if (is_one_value(*s)) push_istack(ones, board.grid[row][c]);
    }

    for (size_t c = col + 1; !is_wall_hit(board, row, c); c++) {
        State * s = &(current_state->elements[board.grid[row][c]]);
        if (is_one_value(*s)) continue;
        (*s).mask &= ~(current_state->elements[index].mask);
        if (is_one_value(*s)) push_istack(ones, board.grid[row][c]);
    }
}

void _reduce_col_one_values(Kakuro board, IStack * ones, SArray * current_state, ksize_t index) {
    assert(is_one_value(current_state->elements[index]) && "EXPECTED ONE VALUE STATE");

    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (size_t r = row - 1; !is_wall_hit(board, row, r); r--) {
        State * s = &(current_state->elements[board.grid[r][col]]);
        if (is_one_value(*s)) continue;
        (*s).mask &= ~current_state->elements[index].mask;
        if (is_one_value(*s)) push_istack(ones, board.grid[r][col]);
    }

    for (size_t r = row + 1; !is_wall_hit(board, row, r); r++) {
        State * s = &(current_state->elements[board.grid[r][col]]);
        if (is_one_value(*s)) continue;
        (*s).mask &= ~current_state->elements[index].mask;
        if (is_one_value(*s)) push_istack(ones, board.grid[r][col]);
    }
}
