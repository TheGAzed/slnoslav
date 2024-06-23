#ifndef INSTANCE_ARGUMENT_H
#define INSTANCE_ARGUMENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define FILEPATH_FLAG_STRING_SHORT "-fp"
#define FILEPATH_FLAG_STRING_LONG  "--filepath"

#define BACKTRACK_FLAG_STRING_SHORT "-bt"
#define BACKTRACK_FLAG_STRING_LONG  "--backtrack"

#define FORWARD_CHECK_FLAG_STRING_SHORT "-fch"
#define FORWARD_CHECK_FLAG_STRING_LONG  "--forward-check"

#define ARC_CONSISTENCY_FLAG_STRING_SHORT "-ac"
#define ARC_CONSISTENCY_FLAG_STRING_LONG  "--arc-consistency"

#define REDUCE_FLAG_STRING_SHORT "-r"
#define REDUCE_FLAG_STRING_LONG  "--reduce"

#define INFO_FLAG_STRING_SHORT "-i"
#define INFO_FLAG_STRING_LONG  "--information"

#define HELP_FLAG_STRING_SHORT "-h"
#define HELP_FLAG_STRING_LONG  "--help"

#define LONGEST_FLAG_S  sizeof(ARC_CONSISTENCY_FLAG_STRING_LONG)

/**
 * @brief Initializes program settings based on specified arguments.
 * 
 * @param argc Number of arguments.
 * @param argv Argument values as string array.
 */
void setup_program(int argc, char **argv);

#endif /* INSTANCE_ARGUMENT_H */
