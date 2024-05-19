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

typedef enum edge_type {
    UPPER_EDGE = 9,
    LOWER_EDGE = 1
} EType;

SArray   create_state_array (size_t size);
void     set_full_state_array(SArray * array);
void     destroy_state_array(SArray * array);

SArray   copy_state_array       (SArray array);
bool     compare_states   (SArray array_a, SArray array_b);
bool     valid_states     (SArray array);
bool     is_end_state     (SArray array);
kssize_t get_multi_index  (SArray array);
SArray   split_state      (State state);
State    merge_state_array(SArray array);

bool     is_one_value  (State state);
ksize_t  get_sums      (ksize_t start, EType type);
State    get_edge_state(ksize_t count, EType type);
ksize_t  get_one_value (State state);

ksize_t  state_to_sums       (State state);
State    get_one_state       (ksize_t value);
kssize_t shortest_multi_index(SArray array);
ksize_t  state_count(State state);

SMatrix generate_neighbor(SArray array, size_t index);
void    free_state_matrix(SMatrix * matrix);

void    print_state(State s);
void    print_state_array(SArray s);

#endif
