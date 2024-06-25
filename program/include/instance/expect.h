#ifndef INSTANCE_EXPECT_H
#define INSTANCE_EXPECT_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum error_mode_type {
    DEFAULT_E, ABORT_E, ASSERT_E,
} error_mode_e;

static error_mode_e error_mode = DEFAULT_E;
static FILE *       error_log  = NULL;

#define NO_ACTION (void)(0)

#ifdef ERROR_LOG_FILE_PATH

#define expect(assertion, error_action, ...)                          \
{                                                                     \
    if (!(assertion)) {                                               \
        assert(error_log = fopen(ERROR_LOG_FILE_PATH, "a"));          \
        fprintf(error_log, __VA_ARGS__);                              \
        fprintf(error_log, "\n");                                     \
        fclose(error_log)                                             \
        switch (error_mode) {                                         \
            case ABORT_E  : { error_action; abort();                } \
            case ASSERT_E : { error_action; assert(0 && assertion); } \
            default       : { error_action;                         } \
        }                                                             \
    }                                                                 \
}

#else

#define expect(assertion, error_action, ...)                          \
{                                                                     \
    if (!(assertion)) {                                               \
        fprintf(error_log ? error_log : stderr, __VA_ARGS__);         \
        fprintf(error_log ? error_log : stderr, "\n");                \
        switch (error_mode) {                                         \
            case ABORT_E  : { error_action; abort();                } \
            case ASSERT_E : { error_action; assert(0 && assertion); } \
            default       : { error_action;                         } \
        }                                                             \
    }                                                                 \
}

#endif /* ERROR_LOG_FILE_PATH */

#endif /* INSTANCE_EXPECT_H */