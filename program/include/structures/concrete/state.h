#ifndef STRUCTURES_CONCRETE_STATE_H

#define STRUCTURES_CONCRETE_STATE_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t ulookup_t;

#define MAX_BLOCK_VALUES 9

#define FULL_STATE    0x1FF // 0b111111111
#define INVALID_STATE 0x000 // 0b000000000

typedef uint16_t state_t;

typedef struct state_array {
    state_t * elements;
    ulookup_t  size;
} state_array_s;

typedef struct state_matrix {
    state_array_s  elements[MAX_BLOCK_VALUES];
    ulookup_t size;
} state_matrix_s;

typedef enum edge_type {
    UPPER_EDGE_E = 9,
    LOWER_EDGE_E = 1,
} edge_type_e;

state_array_s   create_state_array  (ulookup_t size);
void            set_full_state_array(state_array_s * array);
void            destroy_state_array (state_array_s * array);

state_array_s   copy_state_array (state_array_s array);
bool            compare_states   (state_array_s array_a, state_array_s array_b);
bool            valid_states     (state_array_s array);
bool            is_end_state     (state_array_s array);
state_array_s   split_state      (state_t state);
state_t         merge_state_array(state_array_s array);

bool      is_one_value  (state_t state);
ulookup_t get_sums      (ulookup_t start, edge_type_e type);
state_t   get_edge_state(ulookup_t count, edge_type_e type);
ulookup_t get_one_value (state_t state);

ulookup_t state_to_sums       (state_t state);
state_t   get_one_state       (ulookup_t value);
ulookup_t shortest_multi_index(state_array_s array);
ulookup_t state_count         (state_t state);

state_matrix_s create_neighbors_state_matrix(state_array_s array, ulookup_t index);
void           destroy_state_matrix         (state_matrix_s * matrix);

void print_state(state_t s);
void print_state_array(state_array_s s);
void print_solution(state_array_s solved);

#endif
