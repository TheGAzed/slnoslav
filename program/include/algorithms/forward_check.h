#ifndef ALGORITHMS_FORWARD_CHECK_H
#define ALGORITHMS_FORWARD_CHECK_H

#include <stdbool.h>

#include <structures/state.h>
#include <structures/board.h>

bool forward_check(Kakuro board, SArray * current_state);

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

State _check_magic_table(ksize_t blocks, ksize_t sum);
State _check_even_two_blocks(ksize_t block, ksize_t sum);
State _check_high_end(ksize_t block, ksize_t sum);
State _check_low_end(ksize_t block, ksize_t sum);

void _reduce_multi_values(Kakuro board, SArray * current_state);
void _reduce_row_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index);
void _reduce_col_multi_values(Kakuro board, SArray * current_state, Check * checks, ksize_t index);
void _reduce_one_values(Kakuro board, SArray * current_state);
void _reduce_row_one_values(Kakuro board, IStack * ones, SArray * current_state, ksize_t index);
void _reduce_col_one_values(Kakuro board, IStack * ones, SArray * current_state, ksize_t index);

#endif /* ALGORITHMS_FORWARD_CHECK_H */
