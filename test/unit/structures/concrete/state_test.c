#include <greatest.h>

#include <structures/concrete/state.h>

TEST is_one_value_all_valid_states(void) {
    for (size_t i = 0; i < MAX_BLOCK_VALUES; ++i) {
        ASSERTm(
            is_one_value(1 << i),
            "[ERROR] EXPECTED STATE TO BE A ONE VALUE"
        );
    }
    PASS();
}

TEST is_one_value_invalid_state(void) {
    for (size_t i = 0; i < MAX_BLOCK_VALUES; ++i) {
        ASSERTm(
            is_one_value(1 << i),
            "[ERROR] EXPECTED STATE TO BE A ONE VALUE"
        );
    }
    PASS();
}