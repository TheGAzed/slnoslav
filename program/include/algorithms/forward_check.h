#ifndef ALGORITHMS_FORWARD_CHECK_H
#define ALGORITHMS_FORWARD_CHECK_H

#include <stdbool.h>

#include <structures/concrete/state.h>
#include <structures/concrete/board.h>

bool forward_check(Kakuro board, SArray * current_state);

#endif /* ALGORITHMS_FORWARD_CHECK_H */
