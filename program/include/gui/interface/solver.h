#ifndef GUI_INTERFACE_SOLVER_H
#define GUI_INTERFACE_SOLVER_H

#include <gui/default.h>

#include <structures/concrete/state.h>

#include <pthread.h>

typedef enum ds_action {
    CREATE_DS, DESTROY_DS, NEXT_VALUE_DS, NEXT_VALUE_TIMED_DS,
    PREV_VALUE_DS, RESET_DS, CURRENT_VALUE_DS, PREV_VALUE_TIMED_DS,
    START_VALUE_DS, END_VALUE_DS,
} ds_action_e;

#define IS_PLAY 1

typedef enum player_state {
    PLAY_FORWARDS_E  = 0b00 | IS_PLAY,
    PLAY_BACKWARDS_E = 0b10 | IS_PLAY,

    STOP_UNSET_E  = 0b00 << 1,
    STOP_START_E  = 0b01 << 1,
    STOP_MIDDLE_E = 0b10 << 1,
    STOP_END_E    = 0b11 << 1,
} player_state_e;

typedef enum solver_state {
    SOLVE_UNSET_E, SOLVE_RUNNING_E, SOLVE_FINISHED_E, SOLVE_STOPPED_E,
} solver_state_e;

typedef struct player {
    player_state_e play_state;
    solver_state_e solve_state;
} player_s;

player_s *    get_player_singleton(void);
pthread_t     solve(void);
state_array_s state_provider(const ds_action_e action);

#endif /* GUI_INTERFACE_SOLVER_H */
