#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include <algorithms/arc_consistency.h>
#include <structures/concrete/board.h>

#define STACK_DATA_TYPE ksize_t
#include <structures/abstract/stack.h>

#define MAGIC_TABLE_LENGTH 7
#define BLOCK_LENGTH_REST  4
#define BLOCK_LENGTH_EIGHT 9
typedef struct magic_table {
    size_t blocks[MAGIC_TABLE_LENGTH];
    size_t lengths[MAGIC_TABLE_LENGTH];

    state_t B[BLOCK_LENGTH_REST]; state_t C[BLOCK_LENGTH_REST];
    state_t D[BLOCK_LENGTH_REST]; state_t E[BLOCK_LENGTH_REST];
    state_t F[BLOCK_LENGTH_REST]; state_t G[BLOCK_LENGTH_REST];
    state_t H[BLOCK_LENGTH_EIGHT];

    state_t * united[MAGIC_TABLE_LENGTH];
} MTable;

typedef uint32_t comb_t;
typedef struct reduce_combinations {
    SMatrix  e_matrix;
    comb_t e_count;
    ksize_t  e_valid[MAX_BLOCK_VALUES][MAX_BLOCK_VALUES];
} RCombinations;

state_t _check_magic_table(ksize_t blocks, ksize_t sum);
state_t _check_one_blocks(ksize_t blocks, ksize_t sum);
state_t _check_even_two_blocks(ksize_t block, ksize_t sum);
state_t _check_high_end(ksize_t block, ksize_t sum);
state_t _check_low_end(ksize_t block, ksize_t sum);
void  _reduce_multi_values(Kakuro board, SArray * initial_state);
void  _reduce_row_multi_values(Kakuro board, SArray * initial_state, Check * checks, ksize_t index);
void  _reduce_col_multi_values(Kakuro board, SArray * initial_state, Check * checks, ksize_t index);

void _reduce_one_values(Kakuro board, SArray * current_state, Check * checks);
void _reduce_row_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index, Check * checks);
void _reduce_col_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index, Check * checks);

RCombinations _create_combinations(Kakuro board, SArray * current_state, ksize_t index, KGSizes type);
void          _destroy_combinations(RCombinations * combinations);
bool          _reduce_no_combination(Kakuro board, SArray * current_state, Check * checks);
bool          _reduce_no_col_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index);
bool          _reduce_no_row_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index);

void reduce_tree(Kakuro board, SArray * initial_state) {
    assert(initial_state && "INITIAL STATE ARRAY IS NULL");
    
    _reduce_multi_values(board, initial_state);
}

bool look_ahead(Kakuro board, SArray * current_state) {
    assert(current_state && "CURRENT STATE ARRAY IS NULL");

    Check checks[KAKURO_SIZE_MAX] = { 0 };
    do {
        _reduce_one_values(board, current_state, checks);
    } while (valid_states(*current_state) && _reduce_no_combination(board, current_state, checks));

    return valid_states(*current_state);
}

state_t _check_magic_table(ksize_t blocks, ksize_t sum) {
    static MTable t = { 
        .blocks  = { 2, 3, 4, 5, 6, 7, 8 },
        .lengths = { 4, 4, 4, 4, 4, 4, 9 },

        .B = { 0b000000011, 0b000000101, 0b101000000, 0b110000000, },    .C = { 0b000000111, 0b000001011, 0b110100000, 0b111000000, },
        .D = { 0b000001111, 0b000010111, 0b111010000, 0b111100000, },    .E = { 0b000011111, 0b000101111, 0b111101000, 0b111110000, },
        .F = { 0b000111111, 0b001011111, 0b111110100, 0b111111000, },    .G = { 0b001111111, 0b010111111, 0b111111010, 0b111111100, },
        .H = { 0b011111111, 0b101111111, 0b110111111, 0b111011111, 0b111101111, 0b111110111, 0b111111011, 0b111111101, 0b111111110, },

        .united[0] = t.B, .united[1] = t.C, .united[2] = t.D, .united[3] = t.E, .united[4] = t.F, .united[5] = t.G, .united[6] = t.H, 
    };

    assert(blocks >= 1 && blocks <= 9 && "BLOCK SIZE NOT IN RANGE");

    state_t s = FULL_STATE;
    if (blocks == 1 || blocks == 9) return s;

    for (ksize_t i = 0; i < t.lengths[blocks - 2]; i++) {
        state_t temp = t.united[blocks - 2][i];
        if (state_to_sums(temp) == sum) { s = temp; break; }
    }
    return s;
}

state_t _check_one_blocks(ksize_t blocks, ksize_t sum) {
    state_t s = blocks == 1 ? get_one_state(sum) : FULL_STATE;
    return s;
}

state_t _check_even_two_blocks(ksize_t block, ksize_t sum) {
    state_t s = ((block == 2) && !(sum & 1)) ? FULL_STATE ^ (1 << ((sum >> 1) - 1)) : FULL_STATE;
    return s;
}

state_t _check_high_end(ksize_t block, ksize_t sum) {
    ksize_t block_sum = get_sums(block - 1, UPPER_EDGE);
    state_t s = (LOWER_EDGE + block_sum < sum) ? get_edge_state((MAX_BLOCK_VALUES + 1) - (sum - block_sum), UPPER_EDGE) : FULL_STATE;
    return s;
}

state_t _check_low_end(ksize_t block, ksize_t sum) {
    ksize_t block_sum = get_sums(block - 1, LOWER_EDGE);
    state_t s = (UPPER_EDGE + block_sum > sum) ? get_edge_state(sum - block_sum, LOWER_EDGE) : FULL_STATE;
    return s;
}

void _reduce_multi_values(Kakuro board, SArray * current_state) {
    Check checks[KAKURO_SIZE_MAX] = { 0 };

    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!(checks[i] & ROWCHECK)) _reduce_row_multi_values(board, current_state, checks, i);
        if (!(checks[i] & COLCHECK)) _reduce_col_multi_values(board, current_state, checks, i);
        add_check(board, checks, i);
    }
}

void _reduce_row_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[ROW][index], sums = board.sums[ROW][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    Stack multi = create_stack();

    for (size_t i = 0; i < block; i++) {
        state_t s = current_state->elements[board.grid[row][col + i]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_stack(&multi, (ksize_t)board.grid[row][col + i]);
    }
    
    while (!is_empty_stack(multi)) {
        ksize_t i   = pop_stack(&multi);
        state_t magic = _check_magic_table(empty_blocks, empty_sums);

        if (magic != FULL_STATE) current_state->elements[i] &= magic;
        else current_state->elements[i] &= 
            _check_one_blocks(empty_blocks, empty_sums) & _check_even_two_blocks(empty_blocks, empty_sums) & 
            _check_high_end(empty_blocks, empty_sums)   & _check_low_end(empty_blocks, empty_sums);
    }

    destroy_stack(&multi, NULL);
}

void _reduce_col_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[COLUMN][index], sums = board.sums[COLUMN][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    Stack multi = create_stack();

    for (size_t i = 0; i < block; i++) {
        state_t s = current_state->elements[board.grid[row + i][col]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_stack(&multi, (ksize_t)board.grid[row + i][col]);
    }
    
    while (!is_empty_stack(multi)) {
        ksize_t i   = pop_stack(&multi);
        state_t magic = _check_magic_table(empty_blocks, empty_sums);

        if (magic != FULL_STATE) current_state->elements[i] &= magic;
        else current_state->elements[i] &= 
            _check_one_blocks(empty_blocks, empty_sums) & _check_even_two_blocks(empty_blocks, empty_sums) & 
            _check_high_end(empty_blocks, empty_sums)   & _check_low_end(empty_blocks, empty_sums);
    }

    destroy_stack(&multi, NULL);
}

void _reduce_one_values(Kakuro board, SArray * current_state, Check * checks) {
    Stack ones = create_stack();

    for (ksize_t i = 0; i < board.game.empty_count; i++) {
        if (is_one_value(current_state->elements[i])) push_stack(&ones, i);
    }

    while (!is_empty_stack(ones)) {
        ksize_t index = pop_stack(&ones);

        _reduce_row_one_values(board, &ones, current_state, index, checks);
        _reduce_col_one_values(board, &ones, current_state, index, checks);
    }

    destroy_stack(&ones, NULL);
}

void _reduce_row_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index, Check * checks) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    bool is_row_changed = false;

    for (size_t c = col - 1; !is_wall_hit(board, row, c); c--) {
        state_t * s = &(current_state->elements[board.grid[row][c]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_row_changed = true;

        *s &= ~(current_state->elements[index]);
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[row][c]);
    }

    for (size_t c = col + 1; !is_wall_hit(board, row, c); c++) {
        state_t * s = &(current_state->elements[board.grid[row][c]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_row_changed = true;

        *s &= ~(current_state->elements[index]);
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[row][c]);
    }

    if (is_row_changed) sub_row_check(board, checks, index);
}

void _reduce_col_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index, Check * checks) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    bool is_col_changed = false;

    for (size_t r = row - 1; !is_wall_hit(board, r, col); r--) {
        state_t * s = &(current_state->elements[board.grid[r][col]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_col_changed = true;

        *s &= ~current_state->elements[index];
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[r][col]);
    }

    for (size_t r = row + 1; !is_wall_hit(board, r, col); r++) {
        state_t * s = &(current_state->elements[board.grid[r][col]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_col_changed = true;

        *s &= ~current_state->elements[index];
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[r][col]);
    }

    if (is_col_changed) sub_col_check(board, checks, index);
}

RCombinations _create_combinations(Kakuro board, SArray * current_state, ksize_t index, KGSizes type) {
    RCombinations comb = { 
        .e_count  = 1,
        .e_matrix  = {
            .size = board.blocks[type][index],
        }
    };

    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (ksize_t i = 0; i < board.blocks[type][index]; i++) {
        state_t s = type == ROW ? 
            current_state->elements[board.grid[row][col + i]] : 
            current_state->elements[board.grid[row + i][col]];
        
        ksize_t s_count = state_count(s);

        comb.e_count             *= s_count;
        comb.e_matrix.elements[i] = split_state(s);
    }

    ksize_t c_lookup[MAX_BLOCK_VALUES] = { 0 };
    for (comb_t i = 0; i < comb.e_count; i++) {
        state_t s = { 0 };

        for (ksize_t j = 0; j < comb.e_matrix.size; j++) {
            ksize_t a = c_lookup[j];
            s |= comb.e_matrix.elements[j].elements[a];
        }

        for (ksize_t j = 0; j < comb.e_matrix.size; j++) {
            ksize_t a = c_lookup[j];
            comb.e_valid[j][a] += 
                state_count(s)   == board.blocks[type][index] && 
                state_to_sums(s) == board.sums[type][index];
        }
        
        c_lookup[0]++;
        for (ksize_t j = 0; c_lookup[j] / comb.e_matrix.elements[j].size && j < comb.e_matrix.size - 1; j++) {
            c_lookup[j] = 0;
            c_lookup[j + 1]++;
        }
    }

    return comb;
}

void _destroy_combinations(RCombinations * combinations) {
    for (ksize_t i = 0; i < combinations->e_matrix.size; i++) destroy_state_array(&combinations->e_matrix.elements[i]);
    
    combinations->e_count = 0;
}

bool _reduce_no_combination(Kakuro board, SArray * current_state, Check * checks) {
    bool is_reduced = false;
    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!(checks[i] & ROWCHECK)) is_reduced |= _reduce_no_row_combination(board, current_state, checks, i);
        if (!(checks[i] & COLCHECK)) is_reduced |= _reduce_no_col_combination(board, current_state, checks, i);
        add_check(board, checks, i);
    }

    return is_reduced;
}

bool _reduce_no_row_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (ksize_t i = 0; i < board.blocks[ROW][index]; i++) {
        if (!current_state->elements[board.grid[row][col + i]]) return false;
    }

    RCombinations comb = _create_combinations(board, current_state, index, ROW);
    bool is_reduced = false;

    for (ksize_t i = 0; i < comb.e_matrix.size; i++) {
        for (ksize_t j = 0; j < comb.e_matrix.elements[i].size; j++) {
            if (comb.e_valid[i][j]) continue;

            current_state->elements[board.grid[row][col + i]] &= ~comb.e_matrix.elements[i].elements[j];
            is_reduced = true;
        }
    }

    _destroy_combinations(&comb);

    return is_reduced;
}

bool _reduce_no_col_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (ksize_t i = 0; i < board.blocks[COLUMN][index]; i++) {
        if (!current_state->elements[board.grid[row + i][col]]) return false;
    }

    RCombinations c = _create_combinations(board, current_state, index, COLUMN);
    bool is_reduced = false;

    for (ksize_t i = 0; i < c.e_matrix.size; i++) {
        for (ksize_t j = 0; j < c.e_matrix.elements[i].size; j++) {
            if (c.e_valid[i][j]) continue;

            current_state->elements[board.grid[row + i][col]] &= ~c.e_matrix.elements[i].elements[j];
            is_reduced = true;
        }
    }

    _destroy_combinations(&c);

    return is_reduced;
}
