#include <instance/settings.h>

Settings * get_settings_singleton(void) {
    static Settings setup = {
        .filepath           = "program/0.kkr",
        .is_backtrack       = true,
        .is_forward_check   = true,
        .is_arc_consistency = true,
        .is_reduce          = true,
        .state              = STOP_E,
    };

    return &setup;
}
