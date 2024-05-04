#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithms/depth_first_search.h>
#include <algorithms/backtrack.h>
#include <algorithms/forward_check.h>

SArray depth_first_search(Kakuro board) {
    SStack stack = create_state_stack();
    push_state_stack(&stack, create_state_array(board.game.empty_count));

    SArray solution = { 0 };
    size_t look_count = 0;
    while (!is_empty_state_stack(stack)) {
        look_count++;
        SArray guess = pop_state_stack(&stack);

        if (!forward_check(board, &guess) || backtrack(board, guess)) {
            destroy_state_array(&guess);
            continue;
        }
        if (is_end_state(guess)) {
            solution = copy_state(guess);
            destroy_state_array(&guess);
            break;
        }
        push_neighbors(&stack, guess);

        destroy_state_array(&guess);
    }
    destroy_state_stack(&stack);

    printf("LOOP COUNT: %lu\n", look_count);

    return solution;
}

void push_neighbors(SStack * stack, SArray current) {
    kssize_t index = shortest_multi_index(current);

    assert(index != -1 && "NO STATE FOUND");

    SMatrix next = generate_neighbor(current, index);
    for (size_t i = 0; i < next.size; i++) push_state_stack(stack, next.elements[i]);
    free_state_matrix(&next);
}
