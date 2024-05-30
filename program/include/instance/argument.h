#ifndef INSTANCE_ARGUMENT_H
#define INSTANCE_ARGUMENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Initializes program settings based on specified arguments.
 * 
 * @param argc Number of arguments.
 * @param argv Argument values as string array.
 */
void setup_program(int argc, char **argv);

#endif /* INSTANCE_ARGUMENT_H */
