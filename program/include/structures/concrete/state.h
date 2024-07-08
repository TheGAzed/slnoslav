#ifndef STRUCTURES_CONCRETE_STATE_H

#define STRUCTURES_CONCRETE_STATE_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint8_t ulookup_t;
#define KAKURO_SIZE_MAX (sizeof(ksize_t) << 8)

#define MAX_BLOCK_VALUES 9

#define FULL_STATE    0x1FF // 0b111111111
#define INVALID_STATE 0x000 // 0b000000000

typedef uint16_t state_t;

typedef struct state_array {
    state_t * elements;
    ulookup_t  size;
} SArray;

typedef struct state_array_array {
    SArray  elements[MAX_BLOCK_VALUES];
    ulookup_t size;
} SMatrix;

typedef enum edge_type {
    UPPER_EDGE = 9,
    LOWER_EDGE = 1
} EType;

SArray   create_state_array  (ulookup_t size);
void     set_full_state_array(SArray * array);
void     destroy_state_array (SArray * array);

SArray   copy_state_array (SArray array);
bool     compare_states   (SArray array_a, SArray array_b);
bool     valid_states     (SArray array);
bool     is_end_state     (SArray array);
SArray   split_state      (state_t state);
state_t  merge_state_array(SArray array);

bool     is_one_value  (state_t state);
ulookup_t  get_sums      (ulookup_t start, EType type);
state_t  get_edge_state(ulookup_t count, EType type);
ulookup_t  get_one_value (state_t state);

ulookup_t state_to_sums       (state_t state);
state_t get_one_state       (ulookup_t value);
ulookup_t shortest_multi_index(SArray array);
ulookup_t state_count(state_t state);

SMatrix generate_neighbor(SArray array, ulookup_t index);
void    destroy_state_matrix(SMatrix * matrix);

void print_state(state_t s);
void print_state_array(SArray s);
void print_solution(SArray solved);

#endif /* STRUCTURES_CONCRETE_STATE_H */
