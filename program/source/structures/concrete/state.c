#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <structures/concrete/state.h>

SArray create_state_array(ksize_t size) {
    assert(size != 0 && "SIZE OF ARRAY CAN'T BE ZERO");

    SArray sa = { .elements = malloc(sizeof(State) * size), .size = size, };
    assert(sa.elements && "MEMORY ALLOCATION FAILED");

    return sa;
}

void set_full_state_array(SArray * array) {
    for (ksize_t i = 0; i < array->size; i++) array->elements[i].mask = FULL_STATE;
}

SArray split_state(State state) {
    State copy = state;
    SArray sub = create_state_array(state_count(copy));
    
    for (ksize_t i = 0; i < sub.size; i++) {
        sub.elements[i].mask = copy.mask & ~(copy.mask - 1);
        copy.mask ^= copy.mask & ~(copy.mask - 1);
    }

    return sub;
}

State merge_state_array(SArray array) {
    State merge = { 0 };

    for (ksize_t i = 0; i < array.size; i++) {
        merge.mask |= array.elements[i].mask;
    }
    
    return merge;
}

void destroy_state_array(SArray * array) {
    free(array->elements);
    array->elements = NULL;
    array->size = 0;
}

SArray copy_state_array(SArray array) {
    SArray sa = { .elements = malloc(sizeof(State) * array.size), .size = array.size };
    assert(sa.elements && "MEMORY ALLOCATION FAILED");

    for (ksize_t i = 0; i < array.size; i++) sa.elements[i].mask = array.elements[i].mask;

    return sa;
}

bool compare_states(SArray array_a, SArray array_b) {
    assert(array_a.size == array_b.size && "NOT EQUAL LENGTHS");
    for (ksize_t i = 0; i < array_a.size; i++) {
        if (array_a.elements[i].mask != array_b.elements[i].mask) return false;
    }
    return true;
}

bool valid_states(SArray array) {
    for (ksize_t i = 0; i < array.size; i++) {
        if (array.elements[i].mask == INVALID_STATE) return false;
    }
    return true;
}

bool is_end_state(SArray array) {
    for (ksize_t i = 0; i < array.size; i++) {
        if (!is_one_value(array.elements[i])) return false;
    }
    return true;
}

ksize_t get_sums(ksize_t start, EType type) {
    assert(start <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    ksize_t sums = 0;
    if (type == LOWER_EDGE) for (ksize_t i = 1; i <= start; i++) sums += i;
    else for (ksize_t i = MAX_BLOCK_VALUES; i > MAX_BLOCK_VALUES - start; i--) sums += i;

    return sums;
}

State get_edge_state(ksize_t count, EType type) {
    assert(count <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    return (State) {
        .mask = (type == LOWER_EDGE) ? (FULL_STATE >> (MAX_BLOCK_VALUES - count)) : ((FULL_STATE << (MAX_BLOCK_VALUES - count)) & FULL_STATE),
    };
}

bool is_one_value(State state) {
    return state.mask && !(state.mask & (state.mask - 1));
}

ksize_t get_one_value(State state) {
    assert(is_one_value(state) && "EXPECTED ONE VALUE STATE");
    return __builtin_ctz(state.mask) + 1;
}

ksize_t state_to_sums(State state) {
    ksize_t s = 0;
    for (ksize_t i = 0; i < MAX_BLOCK_VALUES; i++) if (state.mask & (1 << i)) s += i + 1;
    return s;
}

State get_one_state(ksize_t value) {
    assert(value >= LOWER_EDGE && value <= UPPER_EDGE && "CAN'T TURN INTO MULTI VALUE STATE");
    return (State) { .mask = 1 << (value - 1), };
}

ksize_t shortest_multi_index(SArray array) {
    int16_t index = -1;

    for (ksize_t i = 0; i < array.size; i++) {
        if (
            !is_one_value(array.elements[i]) && 
            (index == -1 || state_count(array.elements[index]) > state_count(array.elements[i]))
        ) index = i;
    }
    assert(index != -1 && "NO MULTI STATE FOUND");
    
    return index;
}

ksize_t state_count(State state) {
    return (ksize_t)__builtin_popcount(state.mask);
}

SMatrix generate_neighbor(SArray array, ksize_t index) {
    assert(array.size > index && "INVALID INDEX");

    SMatrix sm = { 
        .size     = state_count(array.elements[index]),
    };
    
    ksize_t idx = 0;
    for (ksize_t i = 0; i < MAX_BLOCK_VALUES; i++) {
        if (!(array.elements[index].mask & (1 << i))) continue;

        SArray next_state = copy_state_array(array);
        next_state.elements[index].mask &= (1 << i);

        sm.elements[idx++] = next_state;
    }
    return sm;
}

void print_state(State s) {
    if (is_one_value(s)) printf("%*d ", 2 * get_one_value(s) - 1, get_one_value(s));
    else {
        for (ksize_t j = 0; j < MAX_BLOCK_VALUES; j++) {
            if (s.mask & (1 << j)) printf("%u ", j + 1);
            else printf("  ");
        }
    }
    putchar('\n');
}

void print_state_array(SArray s) {
    for (ksize_t i = 0; i < s.size; i++) {
        printf("[ %3u ] : ", i);
        print_state(s.elements[i]);
    }
    putchar('\n');
    fflush(stdout);
}
