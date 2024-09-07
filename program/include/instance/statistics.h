#ifndef INSTANCE_DATA_H
#define INSTANCE_DATA_H

#include <stdbool.h>

typedef struct statistics {
    double backtrack_count;
    double bad_forward_check_count;
    double bad_arc_consistency_count;

    double dfs_iteration_count;
    double dfs_stack_max_size;
} Statistics;

Statistics * get_stat_singleton(void);

bool invalid_state_backtrack_stat    (bool is_invalid_state);
bool invalid_state_forward_check_stat(bool is_invalid_state);
bool invalid_state_look_ahead_stat   (bool is_invalid_state);

void set_dfs_stack_max_size(size_t size);

void print_statistics(void);

#endif /* INSTANCE_DATA_H */