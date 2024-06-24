#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum error_mode {
    DEFAULT_E,
    ABORT_E,
    ASSERT_E,
} error_mode_e;

error_mode_e mode = DEFAULT_E;
FILE *       error_log;

#define NO_ACTION (void)(0)

#ifdef ERROR_LOG_FILE_PATH

#define expect(assertion, error_action, ...)                 \
{                                                            \
    if (assertion) {                                         \
        assert(error_log = fopen(ERROR_LOG_FILE_PATH, "a")); \
        fprintf(error_log, __VA_ARGS__);                     \
        fprintf(error_log, "\n");                            \
        fclose(error_log)                                    \
        switch (mode) {                                      \
            case ABORT_E  : { abort();           }           \
            case ASSERT_E : { assert(assertion); }           \
            default       : { error_action;      }           \
        }                                                    \
    }                                                        \
}

#else

#define expect(assertion, error_action, ...)                  \
{                                                             \
    if (assertion) {                                          \
        fprintf(error_log ? error_log : stderr, __VA_ARGS__); \
        fprintf(error_log ? error_log : stderr, "\n");        \
        switch (mode) {                                       \
            case ABORT_E  : { abort();           }            \
            case ASSERT_E : { assert(assertion); }            \
            default       : { error_action;      }            \
        }                                                     \
    }                                                         \
}

#endif /* ERROR_LOG_FILE_PATH */