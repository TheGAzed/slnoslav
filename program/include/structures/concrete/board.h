#ifndef STRUCTURES_CONCRETE_BOARD_H

#define STRUCTURES_CONCRETE_BOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define GRID_DIMENTIONS 2

typedef int8_t   lookup_t;
typedef uint8_t ulookup_t;
typedef uint8_t ksize_t;
typedef enum kakuro_grid_sizes { ROW = 0, COLUMN = 1, } KGSizes;
typedef struct kakuro_grid {
    lookup_t **grids[GRID_DIMENTIONS];

    ksize_t size[GRID_DIMENTIONS];
    ksize_t count;
    ksize_t empty_count;
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

Kakuro init_kakuro(FILE * kakuro_file);
void   free_kakuro(Kakuro * board);
bool   is_wall_hit(Kakuro board, ksize_t row, ksize_t col);
void   print_board(Kakuro board);

void   add_check    (Kakuro board, Check * checks, ksize_t index);
void   add_row_check(Kakuro board, Check * checks, ksize_t index);
void   add_col_check(Kakuro board, Check * checks, ksize_t index);

void   sub_check    (Kakuro board, Check * checks, ksize_t index);
void   sub_row_check(Kakuro board, Check * checks, ksize_t index);
void   sub_col_check(Kakuro board, Check * checks, ksize_t index);

#endif /* STRUCTURES_CONCRETE_BOARD_H */
