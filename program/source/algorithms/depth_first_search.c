#include <string.h>

#include <algorithms/depth_first_search.h>
#include <algorithms/backtrack.h>
#include <algorithms/arc_consistency.h>
#include <algorithms/reduce.h>
#include <algorithms/forward_checking.h>

#include <instance/statistics.h>

#define STACK_DATA_TYPE state_array_s
#include <structures/abstract/stack.h>

state_array_s depth_first_search(board_s board) {
    stack_s stack = create_stack();

    state_array_s initial = create_state_array(board.game.empty_count);
    set_full_state_array(&initial);
    reduce(board, &initial);
    push_stack(&stack, initial);

    state_array_s solution = { 0 };
    while (!is_empty_stack(stack)) {
        get_stat_singleton()->dfs_iteration_count++;

        state_array_s guess = pop_stack(&stack);

        if (!look_ahead(board, &guess) || backtrack(board, guess)) {
            destroy_state_array(&guess);
            continue;
        }

        if (is_end_state(guess)) {
            solution = copy_state_array(guess);
            destroy_state_array(&guess);
            break;
        }

        ulookup_t index = shortest_multi_index(guess);

        state_matrix_s next = create_neighbors_state_matrix(guess, index);
        for (size_t i = 0; i < next.size; i++) {
            if (forward_checking(board, &next.elements[i], index)) push_stack(&stack, next.elements[i]);
            else destroy_state_array(&next.elements[i]);
        }

        set_dfs_stack_max_size(stack.size);

        destroy_state_array(&guess);
    }
    destroy_stack(&stack, destroy_state_array);

    return solution;
}
