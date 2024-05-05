#ifndef STRUCTURES_ARGUMENT_H
#define STRUCTURES_ARGUMENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct settings {
    bool   is_default;
    char * filepath;
} Settings;

Settings init_settings(int argc, char **argv);

#endif /* STRUCTURES_ARGUMENT_H */
