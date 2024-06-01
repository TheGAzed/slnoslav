#ifndef STRUCTURES_ABSTRACT_STACK_H
#define STRUCTURES_ABSTRACT_STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

//#define FINITE_QUEUE
//#undef FINITE_QUEUE

#ifndef STACK_DATA_TYPE 

#define STACK_DATA_TYPE void *

#endif /* STACK_DATA_TYPE */

#ifndef FINITE_STACK

#define STACK_LIST_ARRAY_SIZE (1 << 8)

typedef struct stack_list_array {
    STACK_DATA_TYPE           elements[STACK_LIST_ARRAY_SIZE];
    struct stack_list_array * next;
} SLArray;

#endif /* FINITE_STACK */

typedef struct stack {
    size_t size;

#ifdef FINITE_STACK
    size_t max;
    STACK_DATA_TYPE * elements;
#else
    SLArray * head;
#endif /* FINITE_STACK */

} Stack;

typedef enum stack_index_position {
    SI_POSITION_CURRENT = 1,
    SI_POSITION_NEXT    = 0,
} SIPosition;

#ifdef FINITE_STACK

static Stack create_stack(size_t max);
static bool  is_full_stack(Stack stack);

#else

static Stack create_stack(void);

#endif /* FINITE_STACK */

static bool            is_empty_stack(Stack stack);
static void            push_stack(Stack * stack, STACK_DATA_TYPE element);
static STACK_DATA_TYPE pop_stack(Stack * stack);
static STACK_DATA_TYPE peek_stack(Stack stack);
static Stack           copy_stack(Stack stack, STACK_DATA_TYPE (*copy_element)(STACK_DATA_TYPE));
static void            destroy_stack(Stack * stack, void (*free_element)(STACK_DATA_TYPE *));
static size_t          _get_index_stack(Stack stack, SIPosition type);

#ifdef FINITE_STACK

static inline Stack create_stack(size_t max) {
    Stack stack = { .elements = malloc(sizeof(STACK_DATA_TYPE) * max), .max = max, 0, };
    assert(stack.elements && "MEMORY ALLOCATION FAILED");
    return stack;
}

static inline bool is_full_stack(Stack stack) {
    return stack.size == stack.max;
}

#else

static inline Stack create_stack(void)  {
    return (Stack) { 0 };
}

#endif /* FINITE_STACK */

static inline bool is_empty_stack(Stack stack) {
    return !stack.size;
}

static inline void push_stack(Stack * stack, STACK_DATA_TYPE element) {
    assert(stack && "QUEUE POINTER IS NULL");

#ifdef FINITE_STACK
    assert(!is_full_stack(*stack) && "CAN'T PUSH FULL QUEUE");
#endif /* FINITE_STACK */

    size_t idx = _get_index_stack(*stack, SI_POSITION_NEXT);

#ifdef FINITE_STACK
    queue->elements[idx] = element;
#else
    if (!idx) {
        SLArray * temp = malloc(sizeof(SLArray));

        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->next = NULL;

        temp->next = stack->head;
        stack->head = temp;
    }

    stack->head->elements[idx] = element;
#endif /* FINITE_STACK */
    stack->size++;
}

static inline STACK_DATA_TYPE pop_stack(Stack * stack) {
    assert(stack && "QUEUE POINTER IS NULL");
    assert(!is_empty_stack(*stack) && "CAN'T POP EMPTY QUEUE");

    STACK_DATA_TYPE e = peek_stack(*stack);

#ifndef FINITE_STACK
    size_t idx = _get_index_stack(*stack, SI_POSITION_CURRENT);

    if (!idx) {
        SLArray * temp = stack->head->next;
        free(stack->head);
        stack->head = temp;
    }
#endif /* FINITE_STACK */

    stack->size--;
    return e;
}

static inline STACK_DATA_TYPE peek_stack(Stack stack) {
    assert(!is_empty_stack(stack) && "CAN'T PEEK EMPTY STACK");
    size_t idx = _get_index_stack(stack, SI_POSITION_CURRENT);

#ifdef FINITE_STACK
    return stack.elements[idx];
#else
    return stack.head->elements[idx];
#endif /* FINITE_STACK */

}

static inline Stack copy_stack(Stack stack, STACK_DATA_TYPE (*copy_element)(STACK_DATA_TYPE)) {

#ifdef FINITE_STACK
    Stack copy = stack;
    assert(copy.elements = malloc(sizeof(STACK_DATA_TYPE) * stack.max) && "MEMORY ALLOCATION FAILED");

    for (size_t i = 0; i < stack.size; i++) {
        copy.elements[i] = copy_element ? copy_element(stack.elements[i]) : stack.elements[i];
    }
#else
    Stack copy = create_stack();
    
    SLArray * current_array = stack.head;
    for (size_t i = stack.size - 1; i >= 0 && current_array;) {
        SLArray * temp = malloc(sizeof(SLArray));

        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->next = NULL;

        temp->next = copy.head;
        copy.head  = temp;

        for (size_t j = i % STACK_LIST_ARRAY_SIZE; j >= 0; i--, j--) {
            copy.head->elements[j] = copy_element ? copy_element(current_array->elements[j]) : current_array->elements[j];
            copy.size++;
        }
        current_array = current_array->next;
    }
    
#endif /* FINITE_STACK */

    return copy;
}

static inline void destroy_stack(Stack * stack, void (*free_element)(STACK_DATA_TYPE *)) {
    while (!is_empty_stack(*stack)) {
        STACK_DATA_TYPE e = pop_stack(stack);
        if (free_element) free_element(&e);
    }

#ifdef FINITE_STACK
    free(stack->elements);
    stack->max = 0;
#endif /* FINITE_STACK */

}

static inline size_t _get_index_stack(Stack stack, SIPosition type) {
    assert((!is_empty_stack(stack) || type != SI_POSITION_CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY STACK");

#ifdef FINITE_STACK
    return (stack.size - type) % (stack.max - 1);
#else
    return (stack.size - type) % (STACK_LIST_ARRAY_SIZE - 1);
#endif /* FINITE_STACK */

}

#endif /* STRUCTURES_ABSTRACT_STACK_H */
