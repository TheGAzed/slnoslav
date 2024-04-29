#ifndef KAKURO_ALGO_DEPTH_FIRST_SEARCH_H
#define KAKURO_ALGO_DEPTH_FIRST_SEARCH_H

#include <ds/state.h>
#include <ds/board.h>

SArray depth_first_search(Kakuro board);

#ifdef EXPOSE_PRIVATE_FUNCTIONS

void   _push_neighbors(SStack * stack, SArray current);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

#endif /* KAKURO_ALGO_DEPTH_FIRST_SEARCH_H */
