#ifndef ALGORITHMS_FORWARD_CHECKING_H
#define ALGORITHMS_FORWARD_CHECKING_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool forward_checking(board_s board, SArray * current_state, ulookup_t index);

#endif /* ALGORITHMS_FORWARD_CHECKING_H */
