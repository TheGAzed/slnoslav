#include <assert.h>
#include <stdlib.h>

#include <ds/state.h>

#include <ds/stack.h>

bool is_empty_istack(IStack stack) {
    return stack.count == 0;
}

bool is_empty_sstack(SStack stack) {
    return !stack.count;
}

void push_istack(IStack * stack, ksize_t index) {
    assert(stack && "STACK POINTER IS NULL");

    ksize_t idx = _get_index_istack(*stack, NEXT);

    if (!idx) {
        IAList * temp = malloc(sizeof(IAList));
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->bottom = stack->top;
        stack->top = temp;
    }

    stack->top->indexes[idx] = index;
    stack->count++;
}

void push_sstack(SStack * stack, SArray state) {
    assert(stack && "STACK POINTER IS NULL");
    assert(state.blocks && "STATE BLOCKS POINTER IS NULL");

    ksize_t idx = _get_index_sstack(*stack, NEXT);
    
    if (!idx) {
        SAList * temp = malloc(sizeof(SAList));
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->bottom = stack->top;
        stack->top = temp;
    }

    stack->top->kstates[idx] = state;
    stack->count++;
}

ksize_t pop_istack(IStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    assert(!is_empty_istack(*stack) && "CAN'T POP FROM EMPTY STACK");

    ksize_t i = _get_index_istack(*stack, CURRENT);
    
    if (!_get_index_istack(*stack, CURRENT)) {
        IAList * temp = stack->top->bottom;
        free(stack->top);
        stack->top = temp;
    }

    stack->count--;

    return i;
}

SArray pop_sstack(SStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    assert(!is_empty_sstack(*stack) && "CAN'T POP FROM EMPTY STACK");

    SArray gs = stack->top->kstates[_get_index_sstack(*stack, CURRENT)];
    
    if (!_get_index_sstack(*stack, CURRENT)) {
        SAList * temp = stack->top->bottom;
        free(stack->top);
        stack->top = temp;
    }

    stack->count--;

    return gs;
}

void free_istack(IStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    while (!is_empty_istack(*stack)) pop_istack(stack);
}

void free_sstack(SStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    while (!is_empty_sstack(*stack)) free(pop_sstack(stack).blocks);
}

ksize_t _get_index_istack(IStack from, Position type) {
    assert((!is_empty_istack(from) || type != CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY STACK");
    return (from.count - type) & (IA_SIZE - 1);
}

ksize_t _get_index_sstack(SStack from, Position type) {
    assert((!is_empty_sstack(from) || type != CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY STACK");
    return (from.count - type) & (KSA_SIZE - 1);
}
