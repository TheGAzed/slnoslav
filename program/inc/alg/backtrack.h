#ifndef KAKURO_ALGO_BACKTRACK_H
#define KAKURO_ALGO_BACKTRACK_H

#include <stdbool.h>

#include <ds/state.h>
#include <ds/board.h>

bool backtrack(Kakuro board, SArray current_state);

#ifdef SHOW_HIDDEN_FUNCTIONS

typedef enum check { UNCHECKED = 3, ROWCHECK = 1, COLCHECK = 2, CHEKCED = 0, } Check;
bool _backtrack_sum(Kakuro board, SArray current_state, Check * checks, size_t index, Check type);
bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index);
bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index);

#endif

#endif /* KAKURO_ALGO_BACKTRACK_H */
