#include <instance/expect.h>

FILE * error_log = NULL;
error_mode_e error_mode = DEFAULT_E;

#ifdef ERROR_LOG_FILE_PATH

__attribute__((constructor)) static void open_error_log(void) {
    remove(ERROR_LOG_FILE_PATH);
    error_log = fopen(ERROR_LOG_FILE_PATH, "a");
    assert(error_log && "Failed to open error_log");
}

__attribute__((destructor)) static void close_error_log(void) {
    fclose(error_log);
}

#endif
