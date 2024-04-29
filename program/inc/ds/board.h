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

#define IA_SHIFT 4
#define IA_SIZE (1 << IA_SHIFT)
typedef struct index_array_list IAList;
struct index_array_list {
    ksize_t  indexes[IA_SIZE];
    IAList * bottom;
};

typedef struct index_stack {
    size_t count;
    IAList * top;
} IStack;

typedef enum check {
    UNCHECKED = 3,
    ROWCHECK  = 1,
    COLCHECK  = 2,
    CHEKCED   = 0,
} Check;

Kakuro init_kakuro(FILE * kakuro_file);
void   free_kakuro(Kakuro * board);
bool   is_wall_hit(Kakuro board, ksize_t row, ksize_t col);

IStack  init_istack(void);
bool    is_empty_istack(IStack stack);
void    push_istack(IStack * stack, ksize_t index);
ksize_t peek_istack(IStack stack);
ksize_t pop_istack(IStack * stack);
void    free_istack(IStack * stack);
void    print_board(Kakuro board);

#ifdef EXPOSE_PRIVATE_FUNCTIONS

typedef enum index_position {
    I_CURRENT = 1,
    I_NEXT = 0,
} IPosition;

KGrid   _init_grid(FILE * kakuro_file);
void    _free_grid(KGrid * grid);
void    _kakuro_alloc(Kakuro * board, FILE * kakuro_file);
void    _kakuro_setup(Kakuro * board);
void    _setup_coords(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
void    _setup_blocks(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
void    _setup_sums(Kakuro * board, ksize_t row, ksize_t col, ksize_t index);
ksize_t _empty_cell_count(KGrid from);
ksize_t _get_index_istack(IStack from, IPosition type);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

#endif /* DATA_STRUCTURES_BOARD_H */
