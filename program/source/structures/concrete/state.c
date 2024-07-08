#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <structures/concrete/state.h>
#include <instance/expect.h>

SArray create_state_array(ulookup_t size) {
    error_mode = ASSERT_E;
    expect(size != 0, NO_ACTION, "state array size can't be zero (%u)", size);

    SArray array = { .elements = calloc(size, sizeof(state_t)), .size = size, };
    expect(array.elements, NO_ACTION, "memory allocation for array failed/array element is NULL (%p)", (void*)array.elements);

    return array;
}

void set_full_state_array(SArray * array) {
    error_mode = ASSERT_E;
    expect(array, NO_ACTION, "state array pointer is NULL (%p)", (void*)array);

    for (ulookup_t i = 0; i < array->size; i++) array->elements[i] = FULL_STATE;
}

SArray split_state(state_t state) {
    state_t copy = state;
    SArray sub = create_state_array(state_count(copy));
    
    for (ulookup_t i = 0; i < sub.size; i++) {
        sub.elements[i] = copy & ~(copy - 1);
        copy ^= copy & ~(copy - 1);
    }

    return sub;
}

state_t merge_state_array(SArray array) {
    state_t merge = { 0 };

    for (ulookup_t i = 0; i < array.size; i++) {
        merge |= array.elements[i];
    }
    
    return merge;
}

void destroy_state_array(SArray * array) {
    error_mode = ASSERT_E;
    expect(array, NO_ACTION, "state array pointer is NULL (%p)", (void*)array);

    free(array->elements);
    array->elements = NULL;
    array->size = 0;
}

SArray copy_state_array(SArray array) {
    SArray sa = { .elements = malloc(sizeof(state_t) * array.size), .size = array.size };
    assert(sa.elements && "MEMORY ALLOCATION FAILED");

    for (ulookup_t i = 0; i < array.size; i++) sa.elements[i] = array.elements[i];

    return sa;
}

bool compare_states(SArray array_a, SArray array_b) {
    assert(array_a.size == array_b.size && "NOT EQUAL LENGTHS");
    for (ulookup_t i = 0; i < array_a.size; i++) {
        if (array_a.elements[i] != array_b.elements[i]) return false;
    }
    return true;
}

bool valid_states(SArray array) {
    for (ulookup_t i = 0; i < array.size; i++) {
        if (array.elements[i] == INVALID_STATE) return false;
    }
    return true;
}

bool is_end_state(SArray array) {
    for (ulookup_t i = 0; i < array.size; i++) {
        if (!is_one_value(array.elements[i])) return false;
    }
    return true;
}

ulookup_t get_sums(ulookup_t start, EType type) {
    assert(start <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    ulookup_t sums = 0;
    if (type == LOWER_EDGE) for (ulookup_t i = 1; i <= start; i++) sums += i;
    else for (ulookup_t i = MAX_BLOCK_VALUES; i > MAX_BLOCK_VALUES - start; i--) sums += i;

    return sums;
}

state_t get_edge_state(ulookup_t count, EType type) {
    assert(count <= MAX_BLOCK_VALUES && "VALUE IS TOO HIGH");

    return (type == LOWER_EDGE) ? (FULL_STATE >> (MAX_BLOCK_VALUES - count)) : ((FULL_STATE << (MAX_BLOCK_VALUES - count)) & FULL_STATE);
}

bool is_one_value(state_t state) {
    return state && !(state & (state - 1));
}

ulookup_t get_one_value(state_t state) {
    assert(is_one_value(state) && "EXPECTED ONE VALUE STATE");
    return __builtin_ctz(state) + 1;
}

ulookup_t state_to_sums(state_t state) {
    ulookup_t sum = 0;
    state_t copy_state = state;

    while (copy_state) {
        sum += __builtin_ctz(copy_state) + 1;
        copy_state &= ~(copy_state & -copy_state);
    }
    
    return sum;
}

state_t get_one_state(ulookup_t value) {
    assert(value >= LOWER_EDGE && value <= UPPER_EDGE && "CAN'T TURN INTO MULTI VALUE STATE");
    return 1 << (value - 1);
}

ulookup_t shortest_multi_index(SArray array) {
    int16_t index = -1;

    for (ulookup_t i = 0; i < array.size; i++) {
        if (
            !is_one_value(array.elements[i]) && 
            (index == -1 || state_count(array.elements[index]) > state_count(array.elements[i]))
        ) index = i;
    }
    assert(index != -1 && "NO MULTI STATE FOUND");
    
    return index;
}

ulookup_t state_count(state_t state) {
    return __builtin_popcount(state);
}

SMatrix generate_neighbor(SArray array, ulookup_t index) {
    assert(array.size > index && "INVALID INDEX");

    SMatrix sm = { 
        .size = state_count(array.elements[index]),
    };
    
    ulookup_t idx = 0;
    for (ulookup_t i = 0; i < MAX_BLOCK_VALUES; i++) {
        if (!(array.elements[index] & (1 << i))) continue;

        SArray next_state = copy_state_array(array);
        next_state.elements[index] &= (1 << i);

        sm.elements[idx++] = next_state;
    }
    return sm;
}

void destroy_state_matrix(SMatrix * matrix) {
    for (ulookup_t i = 0; i < matrix->size; i++) destroy_state_array(&matrix->elements[i]);
    matrix->size = 0;
}

void print_state(state_t s) {
    if (is_one_value(s)) printf("%*d ", 2 * get_one_value(s) - 1, get_one_value(s));
    else {
        for (ulookup_t j = 0; j < MAX_BLOCK_VALUES; j++) {
            if (s & (1 << j)) printf("%u ", j + 1);
            else printf("  ");
        }
    }
    putchar('\n');
}

void print_state_array(SArray s) {
    for (ulookup_t i = 0; i < s.size; i++) {
        printf("[ %3u ] : ", i);
        print_state(s.elements[i]);
    }
    putchar('\n');
    fflush(stdout);
}

void print_solution(SArray solved) {
    assert(is_end_state(solved) && "CAN ONLY PRINT END STATE");
    printf("[ ");
    for (ulookup_t i = 0; i < solved.size; i++) printf("%u ", __builtin_ctz(solved.elements[i]) + 1);
    puts("]");
}
