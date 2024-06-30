#ifndef INSTANCE_SETTINGS_H
#define INSTANCE_SETTINGS_H

#include <stdlib.h>
#include <stdbool.h>

typedef enum playstate {
    STOP_E, PLAY_E, PAUSE_E, 
} playstate_e;

typedef struct settings {
    char * filepath;

    bool is_backtrack;
    bool is_forward_check;
    bool is_arc_consistency;
    bool is_reduce;

    playstate_e state;
} Settings;

Settings * get_settings_singleton(void);

#endif /* INSTANCE_SETTINGS_H */