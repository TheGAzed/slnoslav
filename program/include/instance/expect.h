#ifndef INSTANCE_EXPECT_H
#define INSTANCE_EXPECT_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef enum error_mode_type {
    DEFAULT_E, ABORT_E, ASSERT_E, EXIT_E,
} error_mode_e;

extern error_mode_e error_mode;
extern  FILE * error_log;

#define NO_ACTION (void)(0)

#ifdef ERROR_LOG_FILE_PATH

#define expect(assertion, error_action, ...)                                   \
{                                                                              \
    if (!(assertion)) {                                                        \
        fprintf(error_log, __VA_ARGS__);                                       \
        fprintf(error_log, "\n");                                              \
        fflush(error_log ? error_log : stderr);                                \
        switch (error_mode) {                                                  \
            case ABORT_E  : { error_action; abort(); break;                  } \
            case ASSERT_E : { error_action; assert(0 && (assertion)); break; } \
            default       : { error_action; break;                           } \
        }                                                                      \
    }                                                                          \
}

#else

#define expect(assertion, error_action, ...)                                  \
{                                                                             \
    if (!(assertion)) {                                                       \
        fprintf(error_log ? error_log : stderr, __VA_ARGS__);                 \
        fprintf(error_log ? error_log : stderr, "\n");                        \
        fflush(error_log ? error_log : stderr);                               \
        switch (error_mode) {                                                 \
            case ABORT_E  : { error_action; abort(); break;                 } \
            case ASSERT_E : { error_action; assert(0 && (assertion)); break;} \
            case EXIT_E   : { error_action; exit(EXIT_FAILURE); break;      } \
            default       : { error_action; break;                          } \
        }                                                                     \
    }                                                                         \
}

#endif /* ERROR_LOG_FILE_PATH */

#endif /* INSTANCE_EXPECT_H */