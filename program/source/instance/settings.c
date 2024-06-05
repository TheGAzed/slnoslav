#include <instance/settings.h>

Settings * get_settings_singleton(void) {
    static Settings setup = {
        .filepath           = "test/data/4x4s.kkr",
        .is_backtrack       = false,
        .is_forward_check   = false,
        .is_arc_consistency = false,
    };

    return &setup;
}
