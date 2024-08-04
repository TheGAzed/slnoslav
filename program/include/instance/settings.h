#ifndef INSTANCE_SETTINGS_H
#define INSTANCE_SETTINGS_H

#include <stdbool.h>
#include <time.h>

#define MAX_DELAY_MS 500
typedef struct settings {
    char * filepath;

    bool is_backtrack;
    bool is_forward_check;
    bool is_arc_consistency;
    bool is_reduce;

    clock_t time_ms;
} settings_s;

settings_s * get_settings_singleton(void);

#endif /* INSTANCE_SETTINGS_H */