#ifndef ALGORITHMS_DEPTH_FIRST_SEARCH_H
#define ALGORITHMS_DEPTH_FIRST_SEARCH_H

#include <structures/state.h>
#include <structures/board.h>

SArray depth_first_search(Kakuro board);
void   push_neighbors(SStack * stack, SArray current);

#endif /* ALGORITHMS_DEPTH_FIRST_SEARCH_H */
