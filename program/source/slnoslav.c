#include <slnoslav.h>

#include <instance/expect.h>
#include <gui/graphics.h>

#include <stdio.h>

FILE * error_log;

void _open_error_log(void);
void _close_error_log(void);

void run_program(void) {
    _open_error_log();
    gui();
    _close_error_log();
}

#ifdef ERROR_LOG_FILE_PATH

void _open_error_log(void) {
    remove(ERROR_LOG_FILE_PATH);
    error_log = fopen(ERROR_LOG_FILE_PATH, "a");
    assert(error_log && "Failed to open error_log");
}

#else

void _open_error_log(void) {
    remove("debug_log.txt");
    error_log = fopen("debug_log.txt", "a");
    assert(error_log && "Failed to open error_log");
}

#endif

void _close_error_log(void) {
    fclose(error_log);
}
