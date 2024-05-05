#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

#include <structures/concrete/state.h>

#define POPCOUNT(value) (__builtin_popcount((unsigned int)value))

SArray create_state_array(size_t size) {
    assert(size != 0 && "SIZE OF ARRAY CAN'T BE ZERO");

    SArray sa = { .elements = malloc(sizeof(State) * size), .size = size, };
    assert(sa.elements && "MEMORY ALLOCATION FAILED");

    for (size_t i = 0; i < size; i++) sa.elements[i].mask = FULL_STATE;
    return sa;
}

void destroy_state_array(SArray * array) {
    free(array->elements);
    array->elements = NULL;
    array->size = 0;
}

SArray copy_state(SArray array) {
    SArray sa = { .elements = malloc(sizeof(State) * array.size), .size = array.size };
    assert(sa.elements && "MEMORY ALLOCATION FAILED");

    for (size_t i = 0; i < array.size; i++) sa.elements[i].mask = array.elements[i].mask;

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

kssize_t get_multi_index(SArray array) {
    for (ksize_t i = 0; i < array.size; i++) {
        if (!is_one_value(array.elements[i])) return i;
    }
    return -1;
}

ksize_t get_sums(ksize_t start, SType type) {
    assert(start <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    ksize_t sums = 0;
    if (type == LOW) for (ksize_t i = 1; i <= start; i++) sums += i;
    else for (ksize_t i = MAX_BLOCK_VALUES; i > MAX_BLOCK_VALUES - start; i--) sums += i;

    return sums;
}

State get_state(ksize_t start, SType type) {
    assert(start <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    State state = { 0 };
    if (type == LOW) for (uint8_t i = 0; i < start; i++) state.mask |= 1 << i;
    else for (uint8_t i = MAX_BLOCK_VALUES - 1; i >= MAX_BLOCK_VALUES - start; i--) state.mask |= 1 << i;
    
    return state;
}

bool is_one_value(State state) {
    return state.mask && !(state.mask & (state.mask - 1));
}

int get_one_value(State state) {
    assert(is_one_value(state) && "EXPECTED ONE VALUE STATE");
    return __builtin_ctz(state.mask) + 1;
}

ksize_t state_to_sums(State state) {
    ksize_t s = 0;
    for (ksize_t i = 0; i < MAX_BLOCK_VALUES; i++) if (state.mask & (1 << i)) s += i + 1;
    return s;
}

State one_sum_to_state(ksize_t sum) {
    assert(sum >= LOW && sum <= HIGH && "CAN'T TURN INTO MULTI VALUE STATE");
    return (State) { .mask = 1 << (sum - 1), };
}

kssize_t shortest_multi_index(SArray array) {
    kssize_t index = -1;

    for (kssize_t i = 0; i < array.size; i++) {
        if (
            !is_one_value(array.elements[i]) && 
            (index == -1 || POPCOUNT(array.elements[index].mask) > POPCOUNT(array.elements[i].mask))
        ) index = i;
    }
    
    return index;
}

SMatrix generate_neighbor(SArray array, size_t index) {
    assert(array.size > index && "INVALID INDEX");

    SMatrix sm = { 
        .size     = POPCOUNT(array.elements[index].mask),
        .elements = malloc(array.size * sizeof(SArray))
    };
    
    assert(sm.elements && "MEMORY ALLOCATION FAILED");

    size_t idx = 0;
    for (size_t i = 0; i < MAX_BLOCK_VALUES; i++) {
        if (!(array.elements[index].mask & (1 << i))) continue;

        SArray next_state = copy_state(array);
        next_state.elements[index].mask &= (1 << i);

        sm.elements[idx++] = next_state;
    }
    return sm;
}

void free_state_matrix(SMatrix * matrix) {
    assert(matrix && "MATRIX POINTER IS NULL");
    assert(matrix->elements && "ELEMENST POINTER IS NULL");
    free(matrix->elements);
}

void print_state(State s) {
    if (is_one_value(s)) printf("%*d ", 2 * get_one_value(s) - 1, get_one_value(s));
    else {
        for (size_t j = 0; j < MAX_BLOCK_VALUES; j++) {
            if (s.mask & (1 << j)) printf("%lu ", j + 1);
            else printf("  ");
        }
    }
    putchar('\n');
}

void print_state_array(SArray s) {
    for (size_t i = 0; i < s.size; i++) {
        printf("[ %3lu ] : ", i);
        print_state(s.elements[i]);
    }
    putchar('\n');
    fflush(stdout);
}
