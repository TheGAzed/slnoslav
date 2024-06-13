#ifndef STRUCTURES_CONCRETE_STATE_H

#define STRUCTURES_CONCRETE_STATE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t ksize_t;
#define KAKURO_SIZE_MAX (sizeof(ksize_t) << 8)

#define MAX_BLOCK_VALUES 9

#define FULL_STATE    0b111111111
#define INVALID_STATE 0b000000000

typedef uint16_t state_t;

typedef struct state_array {
    state_t * elements;
    ksize_t  size;
} SArray;

typedef struct state_array_array {
    SArray  elements[MAX_BLOCK_VALUES];
    ksize_t size;
} SMatrix;

typedef enum edge_type {
    UPPER_EDGE = 9,
    LOWER_EDGE = 1
} EType;

SArray   create_state_array  (ksize_t size);
void     set_full_state_array(SArray * array);
void     destroy_state_array (SArray * array);

SArray   copy_state_array (SArray array);
bool     compare_states   (SArray array_a, SArray array_b);
bool     valid_states     (SArray array);
bool     is_end_state     (SArray array);
SArray   split_state      (state_t state);
state_t  merge_state_array(SArray array);

bool     is_one_value  (state_t state);
ksize_t  get_sums      (ksize_t start, EType type);
state_t  get_edge_state(ksize_t count, EType type);
ksize_t  get_one_value (state_t state);

ksize_t state_to_sums       (state_t state);
state_t get_one_state       (ksize_t value);
ksize_t shortest_multi_index(SArray array);
ksize_t state_count(state_t state);

SMatrix generate_neighbor(SArray array, ksize_t index);
void    destroy_state_matrix(SMatrix * matrix);

void print_state(state_t s);
void print_state_array(SArray s);
void print_solution(SArray solved);

#endif /* STRUCTURES_CONCRETE_STATE_H */
