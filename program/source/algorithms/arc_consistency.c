#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <algorithms/arc_consistency.h>
#include <structures/concrete/board.h>
#include <instance/settings.h>
#include <instance/statistics.h>
#include <instance/expect.h>

#define STACK_DATA_TYPE ulookup_t
#include <structures/abstract/stack.h>

typedef struct valid {
    state_matrix_s  e_matrix;
    ulookup_t  e_valid[MAX_BLOCK_VALUES][MAX_BLOCK_VALUES];
} valid_s;

void _reduce_one_values(board_s board, state_array_s * current_state, check_e * checks);
void _reduce_row_one_values(board_s board, stack_s * ones, state_array_s * current_state, ulookup_t index, check_e * checks);
void _reduce_col_one_values(board_s board, stack_s * ones, state_array_s * current_state, ulookup_t index, check_e * checks);

valid_s _create_valid(board_s board, state_array_s * current_state, ulookup_t index, KGSizes type);
void  _destroy_valid(valid_s * combinations);
bool  _reduce_no_combination(board_s board, state_array_s * current_state, check_e * checks);
bool  _reduce_no_col_combination(board_s board, state_array_s * current_state, ulookup_t index);
bool  _reduce_no_row_combination(board_s board, state_array_s * current_state, ulookup_t index);

bool look_ahead(const board_s board, state_array_s * current_state) {
    error_mode = ASSERT_E;
    expect(current_state, NO_ACTION, "ERROR: current state parameter is NULL (%p)", (void*)current_state);

    if (!get_settings_singleton()->is_arc_consistency) return true;

    get_stat_singleton()->look_ahead_call_count++;

    check_e * checks = calloc(board.game.empty_count, sizeof(check_e));
    error_mode = ASSERT_E;
    expect(checks, NO_ACTION, "ERROR: 'checks' variable allocation failed (%p)", (void*)checks);
    do {
        _reduce_one_values(board, current_state, checks);
    } while (valid_states(*current_state) && _reduce_no_combination(board, current_state, checks));
    free(checks);

    return !invalid_state_look_ahead_stat(!valid_states(*current_state));
}

void _reduce_one_values(board_s board, state_array_s * current_state, check_e * checks) {
    stack_s ones = create_stack();

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

void _reduce_row_one_values(board_s board, stack_s * ones, state_array_s * current_state, ulookup_t index, check_e * checks) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    bool is_row_changed = false;

    for (size_t c = col - 1; !is_wall_hit(board, row, c); c--) {
        state_t * state = &(current_state->elements[board.grid[row][c]]);

        if (!(*state & current_state->elements[index])) continue;
        else is_row_changed = true;

        *state &= ~(current_state->elements[index]);
        if (is_one_value(*state)) push_stack(ones, (ulookup_t)board.grid[row][c]);
    }

    for (size_t c = col + 1; !is_wall_hit(board, row, c); c++) {
        state_t * state = &(current_state->elements[board.grid[row][c]]);

        if (!(*state & current_state->elements[index])) continue;
        else is_row_changed = true;

        *state &= ~(current_state->elements[index]);
        if (is_one_value(*state)) push_stack(ones, (ulookup_t)board.grid[row][c]);
    }

    if (is_row_changed) sub_row_check(board, checks, index);
}

void _reduce_col_one_values(board_s board, stack_s * ones, state_array_s * current_state, ulookup_t index, check_e * checks) {
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

valid_s _create_valid(board_s board, state_array_s * current_state, ulookup_t index, KGSizes type) {
    valid_s combinations = {
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
        combinations.e_matrix.elements[i] = split_state(s);
    }

    ulookup_t comb_index[MAX_BLOCK_VALUES] = { 0 };
    for (uint32_t c = 0; c < comb_count; c++) {
        state_t state = { 0 };

        for (ulookup_t s = 0; s < combinations.e_matrix.size; s++) {
            ulookup_t idx = comb_index[s];
            state |= combinations.e_matrix.elements[s].elements[idx];
        }

        for (ulookup_t s = 0; s < combinations.e_matrix.size; s++) {
            ulookup_t idx = comb_index[s];
            combinations.e_valid[s][idx] +=
                state_count(state)   == board.blocks[type][index] &&
                state_to_sums(state) == board.sums[type][index];
        }

        comb_index[0]++;
        for (ulookup_t s = 0; comb_index[s] / combinations.e_matrix.elements[s].size && s < combinations.e_matrix.size - 1; s++) {
            comb_index[s] = 0;
            comb_index[s + 1]++;
        }
    }

    return combinations;
}

void _destroy_valid(valid_s * combinations) {
    destroy_state_matrix(&(combinations->e_matrix));
}

bool _reduce_no_combination(board_s board, state_array_s * current_state, check_e * checks) {
    bool is_reduced = false;
    for (size_t i = 0; i < board.game.empty_count; i++) {
        if (!(checks[i] & ROWCHECK)) is_reduced |= _reduce_no_row_combination(board, current_state, i);
        if (!(checks[i] & COLCHECK)) is_reduced |= _reduce_no_col_combination(board, current_state, i);
        add_check(board, checks, i);
    }

    return is_reduced;
}

bool _reduce_no_row_combination(board_s board, state_array_s * current_state, ulookup_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    for (ulookup_t i = 0; i < board.blocks[ROW_E][index]; i++) {
        if (!current_state->elements[board.grid[row][col + i]]) return false;
    }

    valid_s combinations = _create_valid(board, current_state, index, ROW_E);
    bool is_reduced = false;

    for (ulookup_t i = 0; i < combinations.e_matrix.size; i++) {
        for (ulookup_t j = 0; j < combinations.e_matrix.elements[i].size; j++) {
            if (combinations.e_valid[i][j]) continue;

            current_state->elements[board.grid[row][col + i]] &= ~combinations.e_matrix.elements[i].elements[j];
            is_reduced = true;
        }
    }

    _destroy_valid(&combinations);

    return is_reduced;
}

bool _reduce_no_col_combination(board_s board, state_array_s * current_state, ulookup_t index) {
    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    for (ulookup_t i = 0; i < board.blocks[COLUMN_E][index]; i++) {
        if (current_state->elements[board.grid[row + i][col]] == INVALID_STATE) return false;
    }

    valid_s comb = _create_valid(board, current_state, index, COLUMN_E);
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
