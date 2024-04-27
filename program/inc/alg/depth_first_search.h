#ifndef KAKURO_ALGO_DEPTH_FIRST_SEARCH_H
#define KAKURO_ALGO_DEPTH_FIRST_SEARCH_H

#include <ds/state.h>
#include <ds/board.h>

SArray depth_first_search(Kakuro board);

void   _push_neighbors(SStack * stack, SArray current);

#endif /* KAKURO_ALGO_DEPTH_FIRST_SEARCH_H */
