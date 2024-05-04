#ifndef KAKURO_STATE_H

#define KAKURO_STATE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t ksize_t;
typedef int16_t kssize_t;

#define MAX_BLOCK_VALUES 9

#define FULL_STATE    0b111111111
#define INVALID_STATE 0b000000000

typedef struct state {
    unsigned mask : MAX_BLOCK_VALUES;
} State;

typedef struct state_array {
    size_t  size;
    State * elements;
} SArray;

typedef struct state_array_array {
    ksize_t  size;
    SArray * elements;
} SMatrix;

typedef enum sum_type {
    HIGH = 9,
    LOW = 1
} SType;

#define STACK_LIST_ARRAY_SIZE (1 << 8)

typedef struct state_stack_list_array {
    SArray                          elements[STACK_LIST_ARRAY_SIZE];
    struct state_stack_list_array * next;
} SSLArray;

typedef struct state_stack {
    size_t     size;
    SSLArray * head;
} SStack;

typedef enum state_stack_index_position {
    SSI_POSITION_CURRENT = 1,
    SSI_POSITION_NEXT    = 0,
} SSIPosition;

SStack   create_state_stack    (void);
void     destroy_state_stack   (SStack * stack);
bool     is_empty_state_stack  (SStack   stack);
void     push_state_stack      (SStack * stack, SArray array);
SArray   pop_state_stack       (SStack * stack);
SArray   peek_state_stack      (SStack   stack);
size_t   _get_index_state_stack(SStack   stack, SSIPosition type);

SArray   create_state_array (size_t size);
void     destroy_state_array(SArray * array);

SArray   copy_state     (SArray array);
bool     compare_states (SArray array_a, SArray array_b);
bool     valid_states   (SArray array);
bool     is_end_state   (SArray array);
kssize_t get_multi_index(SArray array);

bool     is_one_value (State state);
ksize_t  get_sums     (ksize_t start, SType type);
State    get_state    (ksize_t start, SType type);
int      get_one_value(State state);

ksize_t  state_to_sums       (State state);
State    one_sum_to_state    (ksize_t sum);
kssize_t shortest_multi_index(SArray array);

SMatrix generate_neighbor(SArray array, size_t index);
void    free_state_matrix(SMatrix * matrix);

void print_state(State s);
void print_state_array(SArray s);

#endif
