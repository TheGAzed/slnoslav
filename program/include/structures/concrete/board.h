#ifndef STRUCTURES_CONCRETE_BOARD_H

#define STRUCTURES_CONCRETE_BOARD_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define GRID_DIMENTIONS 2

typedef int8_t   lookup_t;
typedef uint8_t ulookup_t;
typedef enum kakuro_grid_sizes { ROW_E = 0, COLUMN_E = 1, } KGSizes;
typedef struct board_grid {
    lookup_t **grids[GRID_DIMENTIONS];
    ulookup_t size[GRID_DIMENTIONS], count, empty_count;
} board_grid_s;

#define U_LOOKUP_COUNT 3
typedef struct board {
    board_grid_s game;

    lookup_t ** grid;
    union {
        struct {
            ulookup_t * coords[GRID_DIMENTIONS];
            ulookup_t * blocks[GRID_DIMENTIONS];
            ulookup_t * sums  [GRID_DIMENTIONS];
        };
        ulookup_t * u_lookups[U_LOOKUP_COUNT * GRID_DIMENTIONS];
    };
} board_s;

typedef enum check {
    UNCHECKED = 0, ROWCHECK  = 1,
    COLCHECK  = 2, CHEKCED   = 3,
} check_e;

board_s create_board(FILE * kakuro_file);
void    destroy_board(board_s * board);
bool    is_wall_hit(board_s board, ulookup_t row, ulookup_t col);
void    print_board(board_s board);

void add_check    (board_s board, check_e * checks, ulookup_t index);
void add_row_check(board_s board, check_e * checks, ulookup_t index);
void add_col_check(board_s board, check_e * checks, ulookup_t index);

void sub_check    (board_s board, check_e * checks, ulookup_t index);
void sub_row_check(board_s board, check_e * checks, ulookup_t index);
void sub_col_check(board_s board, check_e * checks, ulookup_t index);

#endif /* STRUCTURES_CONCRETE_BOARD_H */
