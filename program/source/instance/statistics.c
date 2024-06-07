#include <stdio.h>

#include <instance/statistics.h>
#include <instance/settings.h>

Statistics * get_stat_singleton(void) {
    static Statistics setup = {
        .backtrack_call_count     = 0,
        .forward_check_call_count = 0,
        .look_ahead_call_count    = 0,

        .invalid_state_backtrack_count     = 0,
        .invalid_state_forward_check_count = 0,
        .invalid_state_look_ahead_count    = 0,

        .dfs_iteration_count = 0,
        .dfs_stack_max_size  = 0,
    };

    return &setup;
}

bool invalid_state_backtrack_stat(bool is_invalid_state) {
    get_stat_singleton()->invalid_state_backtrack_count += is_invalid_state;
    return is_invalid_state;
}

bool invalid_state_forward_check_stat(bool is_invalid_state) {
    get_stat_singleton()->invalid_state_forward_check_count += is_invalid_state;
    return is_invalid_state;
}

bool invalid_state_look_ahead_stat(bool is_invalid_state) {
    get_stat_singleton()->invalid_state_look_ahead_count += is_invalid_state;
    return is_invalid_state;
}

void set_dfs_stack_max_size(size_t size) {
    size_t max = get_stat_singleton()->dfs_stack_max_size;
    if (size > max) get_stat_singleton()->dfs_stack_max_size = size;
}

void print_statistics(void) {
    puts("SETTINGS");
    printf("\t - FILEPATH         : %s\n", get_settings_singleton()->filepath);
    printf("\t - BACKTRACKING     : %s\n", get_settings_singleton()->is_backtrack ? "true" : "false");
    printf("\t - FORWARD CHECKING : %s\n", get_settings_singleton()->is_forward_check ? "true" : "false");
    printf("\t - ARC CONSISTENCY  : %s\n", get_settings_singleton()->is_arc_consistency ? "true" : "false");

    puts("STATISTICS");
    puts("\tDEPTH FIRST SEARCH:");
    printf("\t\t - DFS ITERATIONS   : %lu\n", get_stat_singleton()->dfs_iteration_count);
    printf("\t\t - MAX STACK LENGTH : %lu\n", get_stat_singleton()->dfs_stack_max_size);

    puts("\tBACKTRACK:");
    printf("\t\t - CALLS            : %lu\n", get_stat_singleton()->backtrack_call_count);
    printf("\t\t - VALID BACKTRACKS : %lu\n", get_stat_singleton()->invalid_state_backtrack_count);

    puts("\tFORWARD CHECK:");
    printf("\t\t - CALLS                  : %lu\n", get_stat_singleton()->forward_check_call_count);
    printf("\t\t - INVALID FORWARD CHECKS : %lu\n", get_stat_singleton()->invalid_state_forward_check_count);

    puts("\tARC CONSISTENCY (LOOK AHEAD):");
    printf("\t\t - CALLS              : %lu\n", get_stat_singleton()->look_ahead_call_count);
    printf("\t\t - INVALID LOOK AHEAD : %lu\n", get_stat_singleton()->invalid_state_look_ahead_count);
}