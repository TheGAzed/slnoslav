#include <instance/settings.h>

settings_s * get_settings_singleton(void) {
    static settings_s setup = {
        .filepath           = "assets/puzzles/0.kkr",
        .is_backtrack       = false,
        .is_forward_check   = false,
        .is_arc_consistency = false,
        .is_reduce          = false,
        .time_ms            = MAX_DELAY_MS / 2,
    };

    return &setup;
}
