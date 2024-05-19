#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <stdlib.h>

#include <algorithms/depth_first_search.h>
#include <algorithms/backtrack.h>
#include <algorithms/arc_consistency.h>

#define STACK_DATA_TYPE SArray
#include <structures/abstract/stack.h>

void _push_neighbors(Stack * stack, SArray current);

SArray depth_first_search(Kakuro board) {
    Stack stack = create_stack();

    {
        SArray initial = create_state_array(board.game.empty_count);
        set_full_state_array(&initial);
        push_stack(&stack, initial);
    }

    SArray solution = { 0 };
    size_t look_count = 0;
    while (!is_empty_stack(stack)) {
        look_count++;
        SArray guess = pop_stack(&stack);

        if (!arc_consistency(board, &guess) || backtrack(board, guess)) {
            destroy_state_array(&guess);
            continue;
        }
        if (is_end_state(guess)) {
            solution = copy_state_array(guess);
            destroy_state_array(&guess);
            break;
        }
        _push_neighbors(&stack, guess);

        destroy_state_array(&guess);
    }
    destroy_stack(&stack, destroy_state_array);

    printf("LOOP COUNT: %lu\n", look_count);

    return solution;
}

void _push_neighbors(Stack * stack, SArray current) {
    kssize_t index = shortest_multi_index(current);

    assert(index != -1 && "NO STATE FOUND");

    SMatrix next = generate_neighbor(current, index);
    for (size_t i = 0; i < next.size; i++) push_stack(stack, next.elements[i]);
    free_state_matrix(&next);
}
