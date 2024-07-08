#ifndef INSTANCE_DATA_H
#define INSTANCE_DATA_H

#include <stdbool.h>

typedef struct statistics {
    size_t backtrack_call_count;
    size_t forward_check_call_count;
    size_t look_ahead_call_count;

    size_t invalid_state_backtrack_count;
    size_t invalid_state_forward_check_count;
    size_t invalid_state_look_ahead_count;

    size_t dfs_iteration_count;
    size_t dfs_stack_max_size;
} Statistics;

Statistics * get_stat_singleton(void);

bool invalid_state_backtrack_stat    (bool is_invalid_state);
bool invalid_state_forward_check_stat(bool is_invalid_state);
bool invalid_state_look_ahead_stat   (bool is_invalid_state);

void set_dfs_stack_max_size(size_t size);

void print_statistics(void);

#endif /* INSTANCE_DATA_H */