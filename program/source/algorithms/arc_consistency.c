#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <algorithms/arc_consistency.h>
#include <structures/concrete/board.h>
#include <instance/settings.h>
#include <instance/statistics.h>
#include <instance/expect.h>

#define STACK_DATA_TYPE ksize_t
#include <structures/abstract/stack.h>

typedef struct reduce_combinations {
    SMatrix  e_matrix;
    ulookup_t  e_valid[MAX_BLOCK_VALUES][MAX_BLOCK_VALUES];
} Valid;

void _reduce_one_values(board_s board, SArray * current_state, check_e * checks);
void _reduce_row_one_values(board_s board, Stack * ones, SArray * current_state, ulookup_t index, check_e * checks);
void _reduce_col_one_values(board_s board, Stack * ones, SArray * current_state, ulookup_t index, check_e * checks);

Valid _create_valid(board_s board, SArray * current_state, ulookup_t index, KGSizes type);
void  _destroy_valid(Valid * combinations);
bool  _reduce_no_combination(board_s board, SArray * current_state, check_e * checks);
bool  _reduce_no_col_combination(board_s board, SArray * current_state, ulookup_t index);
bool  _reduce_no_row_combination(board_s board, SArray * current_state, ulookup_t index);

bool look_ahead(board_s board, SArray * current_state) {
    error_mode = ASSERT_E;
    expect(current_state, NO_ACTION, "current state parameter is NULL (%p)", (void*)current_state);

    error_mode = DEFAULT_E;
    expect(get_settings_singleton()->is_arc_consistency, return true, "WARNING: arc consistency is off");

    get_stat_singleton()->look_ahead_call_count++;

    check_e checks = calloc(board.game.empty_count, sizeof(check_e));
    do {
        _reduce_one_values(board, current_state, checks);
    } while (valid_states(*current_state) && _reduce_no_combination(board, current_state, checks));
    free(checks);

    return !invalid_state_look_ahead_stat(!valid_states(*current_state));
}

void _reduce_one_values(board_s board, SArray * current_state, check_e * checks) {
    Stack ones = create_stack();

    for (ulookup_t i = 0; i < board.game.empty_count; i++) {
        if (is_one_value(current_state->elements[i])) push_stack(&ones, i);
    }

    while (!is_empty_stack(ones)) {
        ulookup_t index = pop_stack(&ones);

        _reduce_row_one_values(board, &ones, current_state, index, checks);
        _reduce_col_one_values(board, &ones, current_state, index, checks);
    }

    destroy_stack(&ones, NULL);
}

void _reduce_row_one_values(board_s board, Stack * ones, SArray * current_state, ulookup_t index, check_e * checks) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    bool is_row_changed = false;

    for (size_t c = col - 1; !is_wall_hit(board, row, c); c--) {
        state_t * s = &(current_state->elements[board.grid[row][c]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_row_changed = true;

        *s &= ~(current_state->elements[index]);
        if (is_one_value(*s)) push_stack(ones, (ulookup_t)board.grid[row][c]);
    }

    for (size_t c = col + 1; !is_wall_hit(board, row, c); c++) {
        state_t * s = &(current_state->elements[board.grid[row][c]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_row_changed = true;

        *s &= ~(current_state->elements[index]);
        if (is_one_value(*s)) push_stack(ones, (ulookup_t)board.grid[row][c]);
    }

    if (is_row_changed) sub_row_check(board, checks, index);
}

void _reduce_col_one_values(board_s board, Stack * ones, SArray * current_state, ulookup_t index, check_e * checks) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    bool is_col_changed = false;

    for (size_t r = row - 1; !is_wall_hit(board, r, col); r--) {
        state_t * s = &(current_state->elements[board.grid[r][col]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_col_changed = true;

        *s &= ~current_state->elements[index];
        if (is_one_value(*s)) push_stack(ones, (ulookup_t)board.grid[r][col]);
    }

    for (size_t r = row + 1; !is_wall_hit(board, r, col); r++) {
        state_t * s = &(current_state->elements[board.grid[r][col]]);

        if (!(*s & current_state->elements[index])) continue;
        else is_col_changed = true;

        *s &= ~current_state->elements[index];
        if (is_one_value(*s)) push_stack(ones, (ulookup_t)board.grid[r][col]);
    }

    if (is_col_changed) sub_col_check(board, checks, index);
}

Valid _create_valid(board_s board, SArray * current_state, ulookup_t index, KGSizes type) {
    Valid comb = { 
        .e_matrix  = { .size = board.blocks[type][index], }
    };

    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    uint32_t comb_count = 1;
    for (ulookup_t i = 0; i < board.blocks[type][index]; i++) {
        state_t s = type == ROW_E ? 
            current_state->elements[board.grid[row][col + i]] : 
            current_state->elements[board.grid[row + i][col]];
        
        ulookup_t s_count = state_count(s);

        comb_count             *= s_count;
        comb.e_matrix.elements[i] = split_state(s);
    }

    ulookup_t c_lookup[MAX_BLOCK_VALUES] = { 0 };
    for (uint32_t i = 0; i < comb_count; i++) {
        state_t s = { 0 };

        for (ulookup_t j = 0; j < comb.e_matrix.size; j++) {
            ulookup_t a = c_lookup[j];
            s |= comb.e_matrix.elements[j].elements[a];
        }

        for (ulookup_t j = 0; j < comb.e_matrix.size; j++) {
            ulookup_t a = c_lookup[j];
            comb.e_valid[j][a] += 
                state_count(s)   == board.blocks[type][index] && 
                state_to_sums(s) == board.sums[type][index];
        }
        
        c_lookup[0]++;
        for (ulookup_t j = 0; c_lookup[j] / comb.e_matrix.elements[j].size && j < comb.e_matrix.size - 1; j++) {
            c_lookup[j] = 0;
            c_lookup[j + 1]++;
        }
    }

    return comb;
}

void _destroy_valid(Valid * combinations) {
    destroy_state_matrix(&(combinations->e_matrix));
}

bool _reduce_no_combination(board_s board, SArray * current_state, check_e * checks) {
    bool is_reduced = false;
    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!(checks[i] & ROWCHECK)) is_reduced |= _reduce_no_row_combination(board, current_state, i);
        if (!(checks[i] & COLCHECK)) is_reduced |= _reduce_no_col_combination(board, current_state, i);
        add_check(board, checks, i);
    }

    return is_reduced;
}

bool _reduce_no_row_combination(board_s board, SArray * current_state, ulookup_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    for (ulookup_t i = 0; i < board.blocks[ROW_E][index]; i++) {
        if (!current_state->elements[board.grid[row][col + i]]) return false;
    }

    Valid comb = _create_valid(board, current_state, index, ROW_E);
    bool is_reduced = false;

    for (ulookup_t i = 0; i < comb.e_matrix.size; i++) {
        for (ulookup_t j = 0; j < comb.e_matrix.elements[i].size; j++) {
            if (comb.e_valid[i][j]) continue;

            current_state->elements[board.grid[row][col + i]] &= ~comb.e_matrix.elements[i].elements[j];
            is_reduced = true;
        }
    }

    _destroy_valid(&comb);

    return is_reduced;
}

bool _reduce_no_col_combination(board_s board, SArray * current_state, ulookup_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    for (ulookup_t i = 0; i < board.blocks[COLUMN_E][index]; i++) {
        if (current_state->elements[board.grid[row + i][col]] == INVALID_STATE) return false;
    }

    Valid comb = _create_valid(board, current_state, index, COLUMN_E);
    bool is_reduced = false;

    for (ulookup_t i = 0; i < comb.e_matrix.size; i++) {
        for (ulookup_t j = 0; j < comb.e_matrix.elements[i].size; j++) {
            if (comb.e_valid[i][j]) continue;

            current_state->elements[board.grid[row + i][col]] &= ~comb.e_matrix.elements[i].elements[j];
            is_reduced = true;
        }
    }

    _destroy_valid(&comb);

    return is_reduced;
}
