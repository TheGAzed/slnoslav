#ifndef INSTANCE_DATA_H
#define INSTANCE_DATA_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct statistics {
    char * name;

    bool is_backtrack;
    bool is_forward_check;
    bool is_arc_consistency;
} Statistics;

Statistics * get_stat_singleton(void);

#endif /* INSTANCE_DATA_H */