#ifndef KAKURO_STATE_H

#define KAKURO_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t ksize_t;
typedef int16_t kssize_t;

#define MAX_BLOCK_VALUES 9
#define FULL_STATE 0b111111111
#define INVALID_STATE 0b000000000

typedef struct state {
    unsigned mask : 10;
} State;

typedef struct state_array {
    ksize_t   count;
    State * elements;
} SArray;

typedef struct state_array_array {
    ksize_t   count;
    SArray * elements;
} SMatrix;

typedef enum sum_type {
    HIGH = 9,
    LOW = 1
} SType;

#define KSA_SHIFT 8
#define KSA_SIZE (1 << KSA_SHIFT)
typedef struct state_array_list SAList;
struct state_array_list {
    SArray   kstates[KSA_SIZE];
    SAList * bottom;
};

typedef struct state_stack {
    ksize_t  count;
    SAList * top;
} SStack;

SArray  init_state(ksize_t state_length);
void    free_state(SArray * state);
SArray  copy_state(SArray to_copy);
bool    compare_states(SArray a, SArray b);
bool    valid_states(SArray to_validate);
bool    is_end_state(SArray state);
ksize_t get_multi_index(SArray state);

bool    is_one_value(State state);
ksize_t get_sums(ksize_t start, SType type);
State   get_state(ksize_t start, SType type);
int     get_one_value(State state);

ksize_t  state_to_sums(State state);
kssize_t shortest_multi_index(SArray current);

SMatrix generate_neighbor(SArray current, size_t index);
void    free_matrix(SMatrix * array);

SStack  init_sstack();
bool    is_empty_sstack(SStack stack);
void    push_sstack(SStack * stack, SArray state);
SArray  pop_sstack(SStack * stack);
void    free_sstack(SStack * stack);

#ifdef EXPOSE_PRIVATE_FUNCTIONS

typedef enum state_position {
    S_CURRENT = 1,
    S_NEXT = 0,
} SPosition;

ksize_t _get_index_sstack(SStack from, SPosition type);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */


#endif
