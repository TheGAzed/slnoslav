#include <greatest.h>

extern SUITE(test_board_istack);
extern SUITE(test_board_4x4s);
extern SUITE(test_board_6x4s);
extern SUITE(test_board_6x4u);
extern SUITE(test_board_10x10s);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(test_board_4x4s);
    RUN_SUITE(test_board_6x4s);
    RUN_SUITE(test_board_6x4u);
    RUN_SUITE(test_board_10x10s);

    GREATEST_MAIN_END();
    return 0;
}