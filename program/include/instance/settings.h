#ifndef INSTANCE_SETTINGS_H
#define INSTANCE_SETTINGS_H

#include <stdbool.h>
#include <time.h>

/**
 * Maximum delay when accesing states in gui's 'forward/backward play'
 * player mode.
 */
#define MAX_DELAY_MS 500

/**
 * The settings that can be set by arguments by the user
 */
typedef struct settings {
    char * filepath; // -fp or --filepath : the path of the file containing a kakuro board to solve

    bool is_backtrack;       // -bt --backtrack : sets 'backtracking' to true
    bool is_forward_check;   // -fch --forward-check : sets 'forward checking' to true
    bool is_arc_consistency; // -ac --arc-consistency : sets 'arc consistency' to true
    bool is_reduce;          // -r --reduce : sets 'initial state reduction' to true

    clock_t time_ms; // time delay to set next state if player is playing (only variable that gets set in gui)
} settings_s;

settings_s * get_settings_singleton(void);

#endif /* INSTANCE_SETTINGS_H */