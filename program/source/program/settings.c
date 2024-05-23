#include <program/settings.h>

Settings * get_settings_singleton(void) {
    static Settings setup = {
        .filepath           = "test/data/11x11s.kkr",
        .is_backtrack       = true,
        .is_forward_check   = false,
        .is_arc_consistency = true,
    };

    return &setup;
}
