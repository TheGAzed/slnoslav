#ifndef ALGORITHMS_BACKTRACK_H
#define ALGORITHMS_BACKTRACK_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool backtrack(Kakuro board, SArray current_state);

#endif /* ALGORITHMS_BACKTRACK_H */
