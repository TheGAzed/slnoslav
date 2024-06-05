#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithms/depth_first_search.h>
#include <algorithms/backtrack.h>
#include <algorithms/arc_consistency.h>
#include <algorithms/forward_checking.h>

#define STACK_DATA_TYPE SArray
#include <structures/abstract/stack.h>

SArray depth_first_search(Kakuro board) {
    Stack stack = create_stack();

    SArray initial = create_state_array(board.game.empty_count);
    set_full_state_array(&initial);
    reduce_tree(board, &initial);
    push_stack(&stack, initial);

    SArray solution = { 0 };
    while (!is_empty_stack(stack)) {
        SArray guess = pop_stack(&stack);

        if (!look_ahead(board, &guess) || backtrack(board, guess)) {
            destroy_state_array(&guess);
            continue;
        }

        if (is_end_state(guess)) {
            solution = copy_state_array(guess);
            destroy_state_array(&guess);
            break;
        }

        ksize_t index = shortest_multi_index(guess);

        SMatrix next = generate_neighbor(guess, index);
        for (size_t i = 0; i < next.size; i++) {
            if (forward_checking(board, &next.elements[i], index)) push_stack(&stack, next.elements[i]);
            else destroy_state_array(&next.elements[i]);
        }

        destroy_state_array(&guess);
    }
    destroy_stack(&stack, destroy_state_array);

    return solution;
}
