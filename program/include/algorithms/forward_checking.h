#ifndef ALGORITHMS_FORWARD_CHECKING_H
#define ALGORITHMS_FORWARD_CHECKING_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool forward_checking(board_s board, state_array_s * current_state, ulookup_t index);

#endif /* ALGORITHMS_FORWARD_CHECKING_H */
