#include <greatest.h>

extern SUITE(easy_puzzles);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(easy_puzzles);

    GREATEST_MAIN_END();
    return 0;
}
