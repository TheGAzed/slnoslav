#include <gui/interface/solver.h>

#include <string.h>
#include <errno.h>

#include <instance/expect.h>
#include <instance/statistics.h>
#include <instance/settings.h>

#include <algorithms/backtrack.h>
#include <algorithms/arc_consistency.h>
#include <algorithms/reduce.h>
#include <algorithms/forward_checking.h>

#include <gui/default.h>

#define STACK_DATA_TYPE state_array_s

#include <structures/abstract/stack.h>

#ifdef WIN32

#include <io.h>
#include <fcntl.h>

int pipe(int pipefd[2]) { return _pipe(pipefd, sizeof(stack_s), _O_BINARY); }

#else

#include <unistd.h>

#endif

__attribute__((constructor)) static void create_state_queue(void) {
    state_provider(CREATE_DS);
}

__attribute__((destructor)) static void destroy_state_queue(void) {
    state_provider(DESTROY_DS);
}

typedef enum pipe_type {
    READ_PIPE_E = 0, WRITE_PIPE_E = 1,
} pipe_type_e;

int pipefd[2];

void * _solver(void * data);

void _create_ds(stack_s * prev, stack_s * next);
void _destroy_ds(stack_s * prev, stack_s * next);
void _reset_ds(stack_s * prev, stack_s * next);
state_array_s _next_value_ds(stack_s * prev, stack_s * next);
state_array_s _next_value_timed_ds(stack_s * prev, stack_s * next);
state_array_s _prev_value_ds(stack_s * prev, stack_s * next);
state_array_s _current_value_ds(stack_s prev);
state_array_s _prev_value_timed_ds(stack_s * prev, stack_s * next);
void _start_value_ds(stack_s * prev, stack_s * next);
void _end_value_ds(stack_s * prev, stack_s * next);

player_s * get_player_singleton(void) {
    static player_s player = {
        .play_state = STOP_UNSET_E,
        .solve_state = SOLVE_UNSET_E,
    };

    return &player;
}

pthread_t solve(void) {
    pthread_t tid = { 0 };
    pthread_create(&tid, NULL, &_solver, NULL);

    return tid;
}

state_array_s state_provider(const ds_action_e action) {
    static stack_s prev;
    static stack_s next;

    switch (action) {
        case CREATE_DS            : { _create_ds(&prev, &next);  break;          }
        case DESTROY_DS           : { _destroy_ds(&prev, &next); break;          }
        case RESET_DS             : { _reset_ds(&prev, &next);   break;          }
        case NEXT_VALUE_DS        : { return _next_value_ds(&prev, &next);       }
        case NEXT_VALUE_TIMED_DS  : { return _next_value_timed_ds(&prev, &next); }
        case PREV_VALUE_DS        : { return _prev_value_ds(&prev, &next);       }
        case CURRENT_VALUE_DS     : { return _current_value_ds(prev);            }
        case PREV_VALUE_TIMED_DS  : { return _prev_value_timed_ds(&prev, &next); }
        case START_VALUE_DS       : { _start_value_ds(&prev, &next); break;      }
        case END_VALUE_DS         : { _end_value_ds(&prev, &next); break;        }
        default                   : { return (state_array_s) { 0 };              }
    }

    return (state_array_s) { 0 };
}

void * _solver(void * data) {
    get_player_singleton()->solve_state = SOLVE_RUNNING_E;

    board_s board = create_board(get_settings_singleton()->filepath);
    stack_s stack = create_stack();

    state_array_s initial = create_state_array(board.game.empty_count);
    set_full_state_array(&initial);
    reduce(board, &initial);
    push_stack(&stack, initial);

    while (!is_empty_stack(stack) && get_player_singleton()->solve_state != SOLVE_STOPPED_E) {
        get_stat_singleton()->dfs_iteration_count++;

        state_array_s guess = pop_stack(&stack);

        state_array_s temp = copy_state_array(guess);
        error_mode = EXIT_E;
        expect(-1 != write(pipefd[WRITE_PIPE_E], &temp, sizeof(state_array_s)), DEBUG_ACTION, "[ERROR] Write to pipe failed: %s", strerror(errno));

        if (!look_ahead(board, &guess) || backtrack(board, guess)) {
            destroy_state_array(&guess);
            continue;
        }

        if (is_end_state(guess)) {
            error_mode = EXIT_E;
            expect(-1 != write(pipefd[WRITE_PIPE_E], &guess, sizeof(state_array_s)), DEBUG_ACTION, "[ERROR] Write to pipe failed: %s", strerror(errno));

            break;
        }

        ulookup_t index = shortest_multi_index(guess);

        state_matrix_s next = create_neighbors_state_matrix(guess, index);
        for (size_t i = 0; i < next.size; i++) {
            if (forward_checking(board, &next.elements[i], index)) push_stack(&stack, next.elements[i]);
            else destroy_state_array(&next.elements[i]);
        }

        destroy_state_array(&guess);
        set_dfs_stack_max_size(stack.size);
    }

    destroy_stack(&stack, destroy_state_array);
    expect(0 == close(pipefd[WRITE_PIPE_E]), DEBUG_ACTION, "[ERROR] Write pipe failed to close with error: %s", strerror(errno));
    destroy_board(&board);
    get_player_singleton()->solve_state = SOLVE_FINISHED_E;
    return 0;
}

void _create_ds(stack_s * prev, stack_s * next) {
    *prev   = create_stack();
    *next   = create_stack();

    error_mode = EXIT_E;
    expect(0 == pipe(pipefd), DEBUG_ACTION, "[ERROR] Pipe initialization failed with error: %s", strerror(errno));
}

void _destroy_ds(stack_s * prev, stack_s * next) {
    destroy_stack(prev, destroy_state_array);
    destroy_stack(next, destroy_state_array);

    error_mode = EXIT_E;
    state_array_s state = { 0 };
    int read_return = read(pipefd[READ_PIPE_E], &state, sizeof(state_array_s));
    expect(-1 != read_return, DEBUG_ACTION, "[ERROR] Read from pipe failed: %s", strerror(errno));
    while (read_return) {
        push_stack(prev, state);
        read_return = read(pipefd[READ_PIPE_E], &state, sizeof(state_array_s));
        expect(-1 != read_return, DEBUG_ACTION, "[ERROR] Read from pipe failed: %s", strerror(errno));
    }
    close(pipefd[READ_PIPE_E]);
    close(pipefd[WRITE_PIPE_E]);
}

void _reset_ds(stack_s * prev, stack_s * next) {
    _destroy_ds(prev, next);
    _create_ds(prev, next);
}

state_array_s _next_value_ds(stack_s * prev, stack_s * next) {
    if (!is_empty_stack(*next)) {
        push_stack(prev, pop_stack(next));
    } else {
        state_array_s state;

        int read_return = read(pipefd[READ_PIPE_E], &state, sizeof(state_array_s));

        error_mode = EXIT_E;
        expect(-1 != read_return, DEBUG_ACTION, "[ERROR] Read from pipe failed: %s", strerror(errno));

        if (read_return > 0) push_stack(prev, state);
        else get_player_singleton()->play_state = STOP_END_E;
    }

    return _current_value_ds(*prev);
}

state_array_s _next_value_timed_ds(stack_s * prev, stack_s * next) {
    static clock_t start;
    state_array_s state = { 0 };

    if ((clock() - start) / (CLOCKS_PER_SEC / 1000) >= get_settings_singleton()->time_ms) {
        state = _next_value_ds(prev, next);
        start = clock();
    } else {
        state = _current_value_ds(*prev);
    }

    return state;
}

state_array_s _prev_value_ds(stack_s * prev, stack_s * next) {
    if (prev->size > 1) push_stack(next, pop_stack(prev));
    if (prev->size == 1) get_player_singleton()->play_state = STOP_START_E;

    return _current_value_ds(*prev);
}

state_array_s _current_value_ds(stack_s prev) {
    return is_empty_stack(prev) ? (state_array_s) { 0 } : peek_stack(prev);
}

state_array_s _prev_value_timed_ds(stack_s * prev, stack_s * next) {
    static clock_t start;
    state_array_s state = { 0 };

    if ((clock() - start) / (CLOCKS_PER_SEC / 1000) >= get_settings_singleton()->time_ms) {
        state = _prev_value_ds(prev, next);
        start = clock();
    } else {
        state = _current_value_ds(*prev);
    }

    return state;
}

void _start_value_ds(stack_s * prev, stack_s * next) {
    while (prev->size > 1) {
        push_stack(next, pop_stack(prev));
    }

    if (prev->size == 1) get_player_singleton()->play_state = STOP_START_E;
}

void _end_value_ds(stack_s * prev, stack_s * next) {
    while (!is_empty_stack(*next)) {
        push_stack(prev, pop_stack(next));
    }

    error_mode = EXIT_E;
    state_array_s state = { 0 };
    int read_return = read(pipefd[READ_PIPE_E], &state, sizeof(state_array_s));
    expect(-1 != read_return, DEBUG_ACTION, "[ERROR] Read from pipe failed: %s", strerror(errno));
    while (read_return) {
        push_stack(prev, state);
        read_return = read(pipefd[READ_PIPE_E], &state, sizeof(state_array_s));
        expect(-1 != read_return, DEBUG_ACTION, "[ERROR] Read from pipe failed: %s", strerror(errno));
    }

    get_player_singleton()->play_state = STOP_END_E;
}
