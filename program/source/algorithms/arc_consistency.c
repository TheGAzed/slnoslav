#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

    State B[BLOCK_LENGTH_REST]; State C[BLOCK_LENGTH_REST];
    State D[BLOCK_LENGTH_REST]; State E[BLOCK_LENGTH_REST];
    State F[BLOCK_LENGTH_REST]; State G[BLOCK_LENGTH_REST];
    State H[BLOCK_LENGTH_EIGHT];

    State * united[MAGIC_TABLE_LENGTH];
} MTable;

typedef struct reduce_combinations {
    SMatrix    e_array;
    uint32_t   e_count;
    ksize_t ** e_valid;
} RCombinations;

bool _reduce(Kakuro board, SArray * current_state);

State _check_magic_table(ksize_t blocks, ksize_t sum);
State _check_even_two_blocks(ksize_t block, ksize_t sum);
State _check_high_end(ksize_t block, ksize_t sum);
State _check_low_end(ksize_t block, ksize_t sum);
bool  _reduce_multi_values(Kakuro board, SArray * current_state);
bool  _reduce_row_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index);
bool  _reduce_col_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index);

bool _reduce_one_values(Kakuro board, SArray * current_state);
bool _reduce_row_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index);
bool _reduce_col_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index);

RCombinations _create_combinations(Kakuro board, SArray * current_state, ksize_t index, KGSizes type);
void          _destroy_combinations(RCombinations * combinations);
bool          _reduce_no_combination(Kakuro board, SArray * current_state);
bool          _reduce_no_col_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index);
bool          _reduce_no_row_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index);

bool arc_consistency(Kakuro board, SArray * current_state) {
    assert(current_state && "CURRENT STATE ARRAY IS NULL");
    
    while(_reduce(board, current_state));

    return valid_states(*current_state);
}

bool _reduce(Kakuro board, SArray * current_state) {
    return 
        _reduce_multi_values(board, current_state)
        | _reduce_one_values(board, current_state)
        | _reduce_no_combination(board, current_state)
        ;
}

State _check_magic_table(ksize_t blocks, ksize_t sum) {
    static MTable t = { 
        .blocks  = { 2, 3, 4, 5, 6, 7, 8 },
        .lengths = { 4, 4, 4, 4, 4, 4, 9 },

        .B = { 
            (State){ .mask = 0b000000011 }, (State){ .mask = 0b000000101 }, (State){ .mask = 0b101000000 }, (State){ .mask = 0b110000000 }, 
        },
        .C = { 
            (State){ .mask = 0b000000111 }, (State){ .mask = 0b000001011 }, (State){ .mask = 0b110100000 }, (State){ .mask = 0b111000000 }, 
        },
        .D = { 
            (State){ .mask = 0b000001111 }, (State){ .mask = 0b000010111 }, (State){ .mask = 0b111010000 }, (State){ .mask = 0b111100000 }, 
            },
        .E = { 
            (State){ .mask = 0b000011111 }, (State){ .mask = 0b000101111 }, (State){ .mask = 0b111101000 }, (State){ .mask = 0b111110000 }, 
        },
        .F = { 
            (State){ .mask = 0b000111111 }, (State){ .mask = 0b001011111 }, (State){ .mask = 0b111110100 }, (State){ .mask = 0b111111000 }, 
        },
        .G = { 
            (State){ .mask = 0b001111111 }, (State){ .mask = 0b010111111 }, (State){ .mask = 0b111111010 }, (State){ .mask = 0b111111100 }, 
        },
        .H = { 
            (State){ .mask = 0b011111111 }, (State){ .mask = 0b101111111 }, (State){ .mask = 0b110111111 }, (State){ .mask = 0b111011111 },
            (State){ .mask = 0b111101111 }, (State){ .mask = 0b111110111 }, (State){ .mask = 0b111111011 }, (State){ .mask = 0b111111101 },
            (State){ .mask = 0b111111110 },
        },

        .united[0] = t.B, .united[1] = t.C, .united[2] = t.D, .united[3] = t.E, .united[4] = t.F, .united[5] = t.G, .united[6] = t.H, 
    };

    assert(blocks >= 1 && blocks <= 9 && "BLOCK SIZE NOT IN RANGE");

    if (blocks == 1 || blocks == 9) return (State){ .mask = FULL_STATE, };
    State s = { .mask = FULL_STATE, };

    for (ksize_t i = 0; i < t.lengths[blocks - 2]; i++) {
        State temp = t.united[blocks - 2][i];
        if (state_to_sums(temp) == sum) { s = temp; break; }
    }
    return s;
}

State _check_one_blocks(ksize_t blocks, ksize_t sum) {
    State s = blocks == 1 ? get_one_state(sum) : (State) { .mask = FULL_STATE };
    return s;
}

State _check_even_two_blocks(ksize_t block, ksize_t sum) {
    State s = { .mask = (block == 2) && !(sum & 1) ? FULL_STATE ^ (1 << ((sum >> 1) - 1)) : FULL_STATE, };
    return s;
}

State _check_high_end(ksize_t block, ksize_t sum) {
    ksize_t block_sum = get_sums(block - 1, UPPER_EDGE);
    State s = (LOWER_EDGE + block_sum < sum) ? get_edge_state((MAX_BLOCK_VALUES + 1) - (sum - block_sum), UPPER_EDGE) : (State) { .mask = FULL_STATE };
    return s;
}

State _check_low_end(ksize_t block, ksize_t sum) {
    ksize_t block_sum = get_sums(block - 1, LOWER_EDGE);
    State s = (UPPER_EDGE + block_sum > sum) ? get_edge_state(sum - block_sum, LOWER_EDGE) : (State) { .mask = FULL_STATE };
    return s;
}

bool _reduce_multi_values(Kakuro board, SArray * current_state) {
    Check * checks = calloc(board.game.empty_count, sizeof(Check));

    bool is_reduced = false;
    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!valid_states(*current_state)) return false;
        if (!(checks[i] & ROWCHECK)) is_reduced |= _reduce_row_multi_values(board, current_state, checks, i);

        if (!valid_states(*current_state)) return false;
        if (!(checks[i] & COLCHECK)) is_reduced |= _reduce_col_multi_values(board, current_state, checks, i);
    }
    
    free(checks);

    return is_reduced;
}

bool _reduce_row_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[ROW][index], sums = board.sums[ROW][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    Stack multi = create_stack();

    for (size_t i = 0; i < block; i++) {
        checks[board.grid[row][col + i]] |= ROWCHECK;
        State s = current_state->elements[board.grid[row][col + i]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_stack(&multi, (ksize_t)board.grid[row][col + i]);
    }
    
    bool is_reduced = false;
    while (!is_empty_stack(multi)) {
        ksize_t i   = pop_stack(&multi);
        State magic = _check_magic_table(empty_blocks, empty_sums);
        State s     = current_state->elements[i];

        if (magic.mask != FULL_STATE) current_state->elements[i].mask &= magic.mask;
        else current_state->elements[i].mask &= 
            _check_one_blocks(empty_blocks, empty_sums).mask & _check_even_two_blocks(empty_blocks, empty_sums).mask & 
            _check_high_end(empty_blocks, empty_sums).mask   & _check_low_end(empty_blocks, empty_sums).mask;
        
        if (s.mask != current_state->elements[i].mask) is_reduced = true;
    }

    destroy_stack(&multi, NULL);

    return is_reduced;
}

bool _reduce_col_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    ksize_t block = board.blocks[COLUMN][index], sums = board.sums[COLUMN][index];
    ksize_t empty_blocks = block, empty_sums = sums;
    Stack multi = create_stack();

    for (size_t i = 0; i < block; i++) {
        checks[board.grid[row + i][col]] |= COLCHECK;
        State s = current_state->elements[board.grid[row + i][col]];
        if (is_one_value(s)) { empty_blocks--; empty_sums -= state_to_sums(s); }
        else push_stack(&multi, (ksize_t)board.grid[row + i][col]);
    }
    
    bool is_reduced = false;
    while (!is_empty_stack(multi)) {
        ksize_t i   = pop_stack(&multi);
        State magic = _check_magic_table(empty_blocks, empty_sums);
        State s     = current_state->elements[i];

        if (magic.mask != FULL_STATE) current_state->elements[i].mask &= magic.mask;
        else current_state->elements[i].mask &= 
            _check_one_blocks(empty_blocks, empty_sums).mask & _check_even_two_blocks(empty_blocks, empty_sums).mask & 
            _check_high_end(empty_blocks, empty_sums).mask   & _check_low_end(empty_blocks, empty_sums).mask;

        if (s.mask != current_state->elements[i].mask) is_reduced = true;
    }

    destroy_stack(&multi, NULL);

    return is_reduced;
}

bool _reduce_one_values(Kakuro board, SArray * current_state) {
    Stack ones = create_stack();

    for (ksize_t i = 0; i < board.game.empty_count; i++) {
        if (is_one_value(current_state->elements[i])) push_stack(&ones, i);
    }

    bool is_reduced = false;
    while (!is_empty_stack(ones)) {
        ksize_t index = pop_stack(&ones);

        if (!valid_states(*current_state)) return false;
        is_reduced |= _reduce_row_one_values(board, &ones, current_state, index);

        if (!valid_states(*current_state)) return false;
        is_reduced |= _reduce_col_one_values(board, &ones, current_state, index);
    }

    destroy_stack(&ones, NULL);

    return is_reduced;
}

bool _reduce_row_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index) {
    assert(is_one_value(current_state->elements[index]) && "EXPECTED ONE VALUE STATE");

    bool is_reduced = false;
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];

    for (size_t c = col - 1; !is_wall_hit(board, row, c); c--) {
        State * s = &(current_state->elements[board.grid[row][c]]);

        if (!(s->mask & current_state->elements[index].mask)) continue;

        is_reduced = true;
        s->mask &= ~(current_state->elements[index].mask);
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[row][c]);
    }

    for (size_t c = col + 1; !is_wall_hit(board, row, c); c++) {
        State * s = &(current_state->elements[board.grid[row][c]]);

        if (!(s->mask & current_state->elements[index].mask)) continue;

        is_reduced = true;
        s->mask &= ~(current_state->elements[index].mask);
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[row][c]);
    }

    return is_reduced;
}

bool _reduce_col_one_values(Kakuro board, Stack * ones, SArray * current_state, ksize_t index) {
    assert(is_one_value(current_state->elements[index]) && "EXPECTED ONE VALUE STATE");

    bool is_reduced = false;
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];

    for (size_t r = row - 1; !is_wall_hit(board, r, col); r--) {
        State * s = &(current_state->elements[board.grid[r][col]]);

        if (!(s->mask & current_state->elements[index].mask)) continue;

        is_reduced = true;
        s->mask &= ~current_state->elements[index].mask;
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[r][col]);
    }

    for (size_t r = row + 1; !is_wall_hit(board, r, col); r++) {
        State * s = &(current_state->elements[board.grid[r][col]]);

        if (!(s->mask & current_state->elements[index].mask)) continue;

        is_reduced = true;
        s->mask &= ~current_state->elements[index].mask;
        if (is_one_value(*s)) push_stack(ones, (ksize_t)board.grid[r][col]);
    }

    return is_reduced;
}

RCombinations _create_combinations(Kakuro board, SArray * current_state, ksize_t index, KGSizes type) {
    RCombinations c = { 
        .e_count  = 1,
        .e_valid  = malloc(board.blocks[type][index] * sizeof(ksize_t*)),
        .e_array  = {
            .elements = malloc(board.blocks[type][index] * sizeof(SArray)),
            .size     = board.blocks[type][index],
        }
    };

    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];
    for (ksize_t i = 0; i < board.blocks[type][index]; i++) {
        State s = type == ROW ? 
            current_state->elements[board.grid[row][col + i]] : 
            current_state->elements[board.grid[row + i][col]];
        
        ksize_t s_count = state_count(s);

        c.e_count            *= s_count;
        c.e_array.elements[i] = split_state(s);
        c.e_valid[i]          = calloc(s_count, sizeof(ksize_t));
    }

    ksize_t * c_lookup = calloc(board.blocks[type][index], sizeof(ksize_t));
    for (uint32_t i = 0; i < c.e_count; i++) {
        State s = { 0 };

        for (ksize_t j = 0; j < c.e_array.size; j++) {
            ksize_t a = c_lookup[j];
            s.mask |= c.e_array.elements[j].elements[a].mask;
        }

        for (ksize_t j = 0; j < c.e_array.size; j++) {
            ksize_t a = c_lookup[j];
            c.e_valid[j][a] += 
                state_count(s) == board.blocks[type][index] && 
                state_to_sums(s) == board.sums[type][index];
        }
        
        c_lookup[0]++;
        for (ksize_t j = 0; c_lookup[j] / c.e_array.elements[j].size && j < c.e_array.size - 1; j++) {
            c_lookup[j] = 0;
            c_lookup[j + 1]++;
        }
    }
    free(c_lookup);

    return c;
}

void _destroy_combinations(RCombinations * combinations) {
    for (ksize_t i = 0; i < combinations->e_array.size; i++) free(combinations->e_valid[i]);
    free(combinations->e_valid);
    combinations->e_valid = NULL;

    for (ksize_t i = 0; i < combinations->e_array.size; i++) free(combinations->e_array.elements[i].elements);
    free_state_matrix(&(combinations->e_array));
    combinations->e_array.elements = NULL;
    
    combinations->e_count = 0;
}

bool _reduce_no_combination(Kakuro board, SArray * current_state) {
    Check * checks = calloc(current_state->size, sizeof(Check));

    bool is_reduced = false;
    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!valid_states(*current_state)) return false;
        if (!(checks[i] & ROWCHECK)) is_reduced |= _reduce_no_row_combination(board, current_state, checks, i);

        if (!valid_states(*current_state)) return false;
        if (!(checks[i] & COLCHECK)) is_reduced |= _reduce_no_col_combination(board, current_state, checks, i);
    }

    free(checks);

    return is_reduced;
}

bool _reduce_no_row_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {        
    RCombinations c = _create_combinations(board, current_state, index, ROW);
    bool is_reduced = false;
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];

    for (ksize_t i = 0; i < board.blocks[ROW][index]; i++) checks[board.grid[row][col + i]] |= ROWCHECK;

    for (ksize_t i = 0; i < c.e_array.size; i++) {
        for (ksize_t j = 0; j < c.e_array.elements[i].size; j++) {
            if (c.e_valid[i][j]) continue;

            current_state->elements[board.grid[row][col + i]].mask &= ~c.e_array.elements[i].elements[j].mask;
            is_reduced = true;
        }
    }

    _destroy_combinations(&c);

    return is_reduced;
}

bool _reduce_no_col_combination(Kakuro board, SArray * current_state, Check * checks, ksize_t index) {
    RCombinations c = _create_combinations(board, current_state, index, COLUMN);
    bool is_reduced = false;
    ksize_t row = board.coords[ROW][index], col = board.coords[COLUMN][index];

    for (ksize_t i = 0; i < board.blocks[COLUMN][index]; i++) checks[board.grid[row + i][col]] |= COLCHECK;

    for (ksize_t i = 0; i < c.e_array.size; i++) {
        for (ksize_t j = 0; j < c.e_array.elements[i].size; j++) {
            if (c.e_valid[i][j]) continue;

            current_state->elements[board.grid[row + i][col]].mask &= ~c.e_array.elements[i].elements[j].mask;
            is_reduced = true;
        }
    }

    _destroy_combinations(&c);

    return is_reduced;
}
