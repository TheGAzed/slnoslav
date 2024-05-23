#ifndef PROGRAM_SETTINGS_H
#define PROGRAM_SETTINGS_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct settings {
    char * filepath;

    bool is_backtrack;
    bool is_forward_check;
    bool is_arc_consistency;
} Settings;

Settings * get_settings_singleton(void);

#endif /* PROGRAM_SETTINGS_H */