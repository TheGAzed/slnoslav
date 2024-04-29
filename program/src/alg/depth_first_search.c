#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#include <alg/depth_first_search.h>
#include <alg/backtrack.h>
#include <alg/forward_check.h>

#ifndef EXPOSE_PRIVATE_FUNCTIONS

void   _push_neighbors(SStack * stack, SArray current);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

SArray depth_first_search(Kakuro board) {
    SStack stack = init_sstack();
    push_sstack(&stack, init_state(board.game.empty_count));

    SArray solution = { 0 };
    while (!is_empty_sstack(stack)) {
        SArray guess = pop_sstack(&stack);

        if (!forward_check(board, &guess) || backtrack(board, guess)) continue;
        if (is_end_state(guess)) {
            solution = copy_state(guess);
            free_state(&guess);
            break;
        }
        _push_neighbors(&stack, guess);

        free_state(&guess);
    }
    free_sstack(&stack);

    return solution;
}

void _push_neighbors(SStack * stack, SArray current) {
    kssize_t index = shortest_multi_index(current);

    assert(index != -1 && "NO STATE FOUND");

    SMatrix next = generate_neighbor(current, index);
    for (size_t i = 0; i < next.count; i++) push_sstack(stack, next.elements[i]);
    free_matrix(&next);
}
