#include <stdio.h>

#include <instance/statistics.h>
#include <instance/settings.h>

Statistics * get_stat_singleton(void) {
    static Statistics setup = {
        .backtrack_count     = 0,
        .bad_forward_check_count = 0,
        .bad_arc_consistency_count    = 0,

        .dfs_iteration_count = 0,
        .dfs_stack_max_size  = 0,
    };

    return &setup;
}

bool invalid_state_backtrack_stat(bool is_invalid_state) {
    get_stat_singleton()->backtrack_count += is_invalid_state ? 1 : 0;
    return is_invalid_state;
}

bool invalid_state_forward_check_stat(bool is_invalid_state) {
    get_stat_singleton()->bad_forward_check_count += is_invalid_state ? 1 : 0;
    return is_invalid_state;
}

bool invalid_state_look_ahead_stat(bool is_invalid_state) {
    get_stat_singleton()->bad_arc_consistency_count += is_invalid_state ? 1 : 0;
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
    printf("\t - REDUCE           : %s\n", get_settings_singleton()->is_reduce ? "true" : "false");

    puts("STATISTICS");
    puts("\tDEPTH FIRST SEARCH:");
    printf("\t\t - DFS ITERATIONS : %g\n", get_stat_singleton()->dfs_iteration_count);
    printf("\t\t - MAX STACK SIZE : %g\n", get_stat_singleton()->dfs_stack_max_size);

    puts("\tBACKTRACK:");
    printf("\t\t - VALID BACKTRACKS : %g\n", get_stat_singleton()->backtrack_count);

    puts("\tFORWARD CHECK:");
    printf("\t\t - INVALID FORWARD CHECKS : %g\n", get_stat_singleton()->bad_forward_check_count);

    puts("\tARC CONSISTENCY (LOOK AHEAD):");
    printf("\t\t - INVALID LOOK AHEAD : %g\n", get_stat_singleton()->bad_arc_consistency_count);
}