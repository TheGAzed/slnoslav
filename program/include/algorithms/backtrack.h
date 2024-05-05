#ifndef ALGORITHMS_BACKTRACK_H
#define ALGORITHMS_BACKTRACK_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool backtrack(Kakuro board, SArray current_state);

bool _backtrack_row_sum(Kakuro board, SArray current_state, Check * checks, size_t index);
bool _backtrack_col_sum(Kakuro board, SArray current_state, Check * checks, size_t index);

#endif /* ALGORITHMS_BACKTRACK_H */
