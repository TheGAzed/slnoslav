#include <greatest.h>

extern SUITE(test_board_4x4);
extern SUITE(test_board_6x4);
extern SUITE(test_board_istack);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(test_board_4x4);
    RUN_SUITE(test_board_6x4);
    RUN_SUITE(test_board_istack);

    GREATEST_MAIN_END();
    return 0;
}