#include <greatest.h>

extern SUITE(stack_test);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(stack_test);

    GREATEST_MAIN_END();
    return 0;
}
