#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <ds/state.h>

SArray init_state(ksize_t state_length) {
    SArray state = { .elements = malloc(state_length * sizeof(State)) };
    memset(state.elements, FULL_STATE, state_length);
    return state;
}

void free_state(SArray * state) {
    assert(state && "STATE POINTER IS NULL");
    assert(state->elements && "STATE BLOCKS POINTER IS NULL");
    free(state->elements);
}

SArray copy_state(SArray to_copy) {
    SArray state = { .elements = malloc(sizeof(State) * to_copy.count) };
    assert(state.elements && "MEMORY ALLOCATION FAILED");
    memcpy(state.elements, to_copy.elements, to_copy.count);
    return state;
}

bool compare_states(SArray a, SArray b) {
    assert(a.count == b.count && "NOT EQUAL LENGTHS");
    for (ksize_t i = 0; i < a.count; i++) {
        if (a.elements[i].mask != b.elements[i].mask) return false;
    }
    return true;
}

bool valid_states(SArray to_validate) {
    for (ksize_t i = 0; i < to_validate.count; i++) {
        if (to_validate.elements[i].mask == INVALID_STATE) return false;
    }
    return true;
}

bool is_end_state(SArray state) {
    for (ksize_t i = 0; i < state.count; i++) {
        if (!is_one_value(state.elements[i])) return false;
    }
    return true;
}

ksize_t get_multi_index(SArray state) {
    for (ksize_t i = 0; i < state.count; i++) {
        if (!is_one_value(state.elements[i])) return i;
    }
    assert(false && "NO INDEX FOUND");
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
    return __builtin_ctz(state.mask);
}

ksize_t state_to_sums(State state) {
    ksize_t s = 0;
    for (ksize_t i = 0; i < MAX_BLOCK_VALUES; i++) if (state.mask & (1 << i)) s += i + 1;
    return s;
}

#define POPCOUNT(value) (__builtin_popcount((unsigned int)value))
kssize_t shortest_multi_index(SArray current) {
    kssize_t index = -1;

    for (kssize_t i = 0; i < current.count; i++) {
        if (
            !is_one_value(current.elements[i]) && 
            (index == -1 || POPCOUNT(current.elements[index].mask) > POPCOUNT(current.elements[i].mask))
        ) index = i;
    }
    
    return index;
}

SMatrix generate_neighbor(SArray current, size_t index) {
    assert(current.count > index && "INVALID INDEX");

    SMatrix array = { 
        .count    = POPCOUNT(current.elements[index].mask),
        .elements = malloc(array.count * sizeof(SArray))
    };

    assert(array.elements && "MEMORY ALLOCATION FAILED");

    size_t idx = 0;
    for (size_t i = 0; i < MAX_BLOCK_VALUES; i++) {
        if (!(current.elements[index].mask & (1 << i))) continue;

        SArray S_next_state = copy_state(current);
        S_next_state.elements[index].mask &= (1 << i);

        array.elements[idx++] = S_next_state;
    }

    return array;
}
#undef POPCOUNT

void free_matrix(SMatrix * array) {
    assert(array && "ARRAY POINTER IS NULL");
    assert(array->elements && "ELEMENST POINTER IS NULL");
    for (size_t i = 0; i < array->count; i++) free(array->elements[i].elements);
    free(array->elements);
}

SStack init_sstack() {
    return (SStack) { 0 };
}

bool is_empty_sstack(SStack stack) {
    return !stack.count;
}

void push_sstack(SStack * stack, SArray state) {
    assert(stack && "STACK POINTER IS NULL");
    assert(state.elements && "STATE BLOCKS POINTER IS NULL");

    ksize_t idx = _get_index_sstack(*stack, S_NEXT);
    
    if (!idx) {
        SAList * temp = malloc(sizeof(SAList));
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->bottom = stack->top;
        stack->top = temp;
    }

    stack->top->kstates[idx] = state;
    stack->count++;
}

SArray pop_sstack(SStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    assert(!is_empty_sstack(*stack) && "CAN'T POP FROM EMPTY STACK");

    SArray gs = stack->top->kstates[_get_index_sstack(*stack, S_CURRENT)];
    
    if (!_get_index_sstack(*stack, S_CURRENT)) {
        SAList * temp = stack->top->bottom;
        free(stack->top);
        stack->top = temp;
    }

    stack->count--;

    return gs;
}

void free_sstack(SStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    while (!is_empty_sstack(*stack)) free(pop_sstack(stack).elements);
}

ksize_t _get_index_sstack(SStack from, SPosition type) {
    assert((!is_empty_sstack(from) || type != S_CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY STACK");
    return (from.count - type) & (KSA_SIZE - 1);
}
