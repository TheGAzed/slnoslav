#ifndef ALGORITHMS_ARC_CONSISTENCY_H
#define ALGORITHMS_ARC_CONSISTENCY_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool look_ahead(Kakuro board, SArray * current_state);

#endif /* ALGORITHMS_ARC_CONSISTENCY_H */
