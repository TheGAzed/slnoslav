#ifndef INSTANCE_EXPECT_H
#define INSTANCE_EXPECT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum error_mode_type {
    DEFAULT_E, ABORT_E, ASSERT_E, EXIT_E,
} error_mode_e;

static error_mode_e error_mode = DEFAULT_E;
static FILE *       error_log  = NULL;

#define NO_ACTION (void)(0)

#ifdef ERROR_LOG_FILE_PATH

static FILE *       error_log  = fopen(ERROR_LOG_FILE_PATH, "a");

#define expect(assertion, error_action, ...)                                   \
{                                                                              \
    if (!(assertion)) {                                                        \
        assert(error_log);                                                     \
        fprintf(error_log, __VA_ARGS__);                                       \
        fprintf(error_log, "\n");                                              \
        fclose(error_log)                                                      \
        switch (error_mode) {                                                  \
            case ABORT_E  : { error_action; abort(); break;                  } \
            case ASSERT_E : { error_action; assert(0 && (assertion)); break; } \
            default       : { error_action; break;                           } \
        }                                                                      \
    }                                                                          \
}

__attribute__((destructor)) static void close_error_log(void) {
    fclose(error_log);
}

#else

static FILE *       error_log;

#define expect(assertion, error_action, ...)                                  \
{                                                                             \
    if (!(assertion)) {                                                       \
        fprintf(error_log ? error_log : stderr, __VA_ARGS__);                 \
        fprintf(error_log ? error_log : stderr, "\n");                        \
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