#include <greatest.h>

extern SUITE(easy_unset);
extern SUITE(easy_fch);
extern SUITE(easy_bt);
extern SUITE(easy_bt_fch);
extern SUITE(easy_ac);
extern SUITE(easy_ac_fch);
extern SUITE(easy_ac_bt);
extern SUITE(easy_ac_bt_fch);

extern SUITE(medium_backtrack_forward_check);
extern SUITE(medium_arc_consistency_backtrack_forward_check);

GREATEST_MAIN_DEFS();

int main(const int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(easy_unset);
    RUN_SUITE(easy_fch);
    RUN_SUITE(easy_bt);
    RUN_SUITE(easy_bt_fch);
    RUN_SUITE(easy_ac);
    RUN_SUITE(easy_ac_fch);
    RUN_SUITE(easy_ac_bt);
    RUN_SUITE(easy_ac_bt_fch);

    RUN_SUITE(medium_backtrack_forward_check);
    RUN_SUITE(medium_arc_consistency_backtrack_forward_check);

    GREATEST_MAIN_END();
    return 0;
}
