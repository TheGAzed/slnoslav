#ifndef ALGORITHMS_ARC_CONSISTENCY_H
#define ALGORITHMS_ARC_CONSISTENCY_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool look_ahead(const board_s board, state_array_s * current_state);

#endif /* ALGORITHMS_ARC_CONSISTENCY_H */
