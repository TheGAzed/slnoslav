#ifndef KAKURO_ALGO_BACKTRACK_H
#define KAKURO_ALGO_BACKTRACK_H

#include <stdbool.h>

#include <ds/state.h>
#include <ds/board.h>

bool backtrack(Kakuro board, SArray current_state);

#ifdef EXPOSE_PRIVATE_FUNCTIONS

bool _backtrack_sum(Kakuro board, SArray current_state, Check * checks, size_t index, Check type);
bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index);
bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

#endif /* KAKURO_ALGO_BACKTRACK_H */
