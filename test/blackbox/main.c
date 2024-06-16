#include <greatest.h>

extern SUITE(easy_unset);
extern SUITE(easy_forward_check);
extern SUITE(easy_backtrack);
extern SUITE(easy_backtrack_forward_check);
extern SUITE(easy_arc_consistency);
extern SUITE(easy_arc_consistency_forward_check);
extern SUITE(easy_arc_consistency_backtrack);
extern SUITE(easy_arc_consistency_backtrack_forward_check);

extern SUITE(medium_backtrack_forward_check);
extern SUITE(medium_arc_consistency_backtrack_forward_check);

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(easy_unset);
    RUN_SUITE(easy_forward_check);
    RUN_SUITE(easy_backtrack);
    RUN_SUITE(easy_backtrack_forward_check);
    RUN_SUITE(easy_arc_consistency);
    RUN_SUITE(easy_arc_consistency_forward_check);
    RUN_SUITE(easy_arc_consistency_backtrack);
    RUN_SUITE(easy_arc_consistency_backtrack_forward_check);

    RUN_SUITE(medium_backtrack_forward_check);
    RUN_SUITE(medium_arc_consistency_backtrack_forward_check);

    GREATEST_MAIN_END();
    return 0;
}
