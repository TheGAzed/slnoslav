#ifndef ALGORITHMS_BACKTRACK_H
#define ALGORITHMS_BACKTRACK_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool backtrack(board_s board, state_array_s current_state);

#endif /* ALGORITHMS_BACKTRACK_H */
