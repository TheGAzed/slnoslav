#ifndef DATA_STRUCTURES_BOARD_H

#define DATA_STRUCTURES_BOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define GRID_DIMENTIONS 2

typedef int8_t   lookup_t;
typedef uint8_t ulookup_t;
typedef uint8_t ksize_t;
typedef enum kakuro_grid_sizes { ROW = 0, COLUMN = 1, } KGSizes;
typedef struct kakuro_grid {
    ksize_t count;
    ksize_t empty_count;
    ksize_t size[GRID_DIMENTIONS];

    lookup_t **grids[GRID_DIMENTIONS];
} KGrid;

#define U_LOOKUP_COUNT 3
typedef struct kakuro {
    KGrid game;

    lookup_t ** grid;
    union {
        struct {
            ulookup_t * coords[GRID_DIMENTIONS];
            ulookup_t * blocks[GRID_DIMENTIONS];
            ulookup_t * sums  [GRID_DIMENTIONS];
        };
        ulookup_t *u_lookups[U_LOOKUP_COUNT * GRID_DIMENTIONS];
    };
} Kakuro;

typedef enum check {
    UNCHECKED = 0,
    ROWCHECK  = 1,
    COLCHECK  = 2,
    CHEKCED   = 3,
} Check;

#define STACK_LIST_ARRAY_SIZE (1 << 8)

typedef struct index_stack_list_array {
    ksize_t                         elements[STACK_LIST_ARRAY_SIZE];
    struct index_stack_list_array * next;
} ISLArray;

typedef struct index_stack {
    size_t     size;
    ISLArray * head;
} IStack;

typedef enum index_stack_index_position {
    ISI_POSITION_CURRENT = 1,
    ISI_POSITION_NEXT    = 0,
} ISIPosition;

IStack   create_index_stack    (void);
void     destroy_index_stack   (IStack * stack);
bool     is_empty_index_stack  (IStack   stack);
void     push_index_stack      (IStack * stack, ksize_t array);
ksize_t  pop_index_stack       (IStack * stack);
ksize_t  peek_index_stack      (IStack   stack);
size_t   _get_index_index_stack(IStack   stack, ISIPosition type);

Kakuro init_kakuro(FILE * kakuro_file);
void   free_kakuro(Kakuro * board);
bool   is_wall_hit(Kakuro board, ksize_t row, ksize_t col);
void   print_board(Kakuro board);

KGrid   _init_grid(FILE * kakuro_file);
void    _free_grid(KGrid * grid);
void    _kakuro_alloc(Kakuro * board, FILE * kakuro_file);
void    _kakuro_setup(Kakuro * board);
void    _setup_coords(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
void    _setup_blocks(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
void    _setup_sums(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
ksize_t _empty_cell_count(KGrid from);

#endif /* DATA_STRUCTURES_BOARD_H */
