#include <instance/settings.h>

Settings * get_settings_singleton(void) {
    static Settings setup = {
        .filepath           = "program/0.kkr",
        .is_backtrack       = false,
        .is_forward_check   = false,
        .is_arc_consistency = false,
        .is_reduce          = false,
        .time_ms            = MAX_DELAY_MS / 2,
    };

    return &setup;
}
