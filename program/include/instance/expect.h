#ifndef INSTANCE_EXPECT_H
#define INSTANCE_EXPECT_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/**
 * Enum modes that determine what action should happen after executing error_action.
 */
typedef enum error_mode_type {
    DEFAULT_E, ABORT_E, EXIT_E,
} error_mode_e;

/**
 * Static error mode indicator.
 */
static error_mode_e error_mode;
/**
 * External global error log FILE pointer variable to write errors to file.
 */
extern FILE * error_log;

/**
 * Placeholder macro that doesn't execute anything.
 */
#define NO_ACTION (void)(0)
/**
 * Placeholder debug action that asserts 0 (false) for debug break.
 */
#define DEBUG_ACTION (assert(0))

/**
 * @brief An assert-esque macro with extra features like running
 * suer defined action code after assertion fails.
 * @param assertion statement that is expected to be true
 * @param error_action action to perform after assertion is false
 */
#define expect(assertion, error_action, ...)               \
{                                                          \
    if (!(assertion)) {                                    \
        FILE * temp = error_log ? error_log : stderr;      \
        fprintf(temp, #assertion "\n" __VA_ARGS__);        \
        fprintf(temp, "\n");                               \
        fflush(temp);                                      \
        error_action;                                      \
        switch (error_mode) {                              \
            case ABORT_E  : { abort(); break;            } \
            case EXIT_E   : { exit(EXIT_FAILURE); break; } \
            default       : { break;                     } \
        }                                                  \
    }                                                      \
}

#endif /* INSTANCE_EXPECT_H */