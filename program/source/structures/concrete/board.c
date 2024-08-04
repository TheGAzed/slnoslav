#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <structures/concrete/board.h>

board_grid_s   _init_grid       (FILE * kakuro_file);
void    _free_grid       (board_grid_s * grid);
void    _kakuro_alloc    (board_s * board, FILE * kakuro_file);
void    _kakuro_setup    (board_s * board);
void    _setup_coords    (board_s * board, ulookup_t row, ulookup_t col, ulookup_t index);
void    _setup_blocks    (board_s * board, ulookup_t row, ulookup_t col, ulookup_t index);
void    _setup_sums      (board_s * board, ulookup_t row, ulookup_t col, ulookup_t index);
ulookup_t _empty_cell_count(board_grid_s from);

board_s create_board(FILE * kakuro_file) {
    assert(kakuro_file && "KAKURO FILE POINTER IS NULL");

    board_s k = { 0 };
    _kakuro_alloc(&k, kakuro_file);
    _kakuro_setup(&k);

    return k;
}

void destroy_board(board_s * board) {
    for (size_t i = 0; i < GRID_DIMENTIONS * U_LOOKUP_COUNT; i++) {
        free(board->u_lookups[i]);
        board->u_lookups[i] = NULL;
    }

    for (size_t i = 0; i < board->game.size[ROW_E]; i++) free(board->grid[i]);
    free(board->grid);

    _free_grid(&(board->game));
}

bool is_wall_hit(board_s board, ulookup_t row, ulookup_t col) {
    return
        (row >= board.game.size[ROW_E] || col >= board.game.size[COLUMN_E]) ||
        board.grid[row][col] == -1;
}

void print_board(board_s board) {
    for (size_t i = 0; i < GRID_DIMENTIONS; i++) {
        for (size_t j = 0; j < board.game.size[ROW_E]; j++) {
            for (size_t k = 0; k < board.game.size[COLUMN_E]; k++) {
                printf("%02hhd ", board.game.grids[i][j][k]);
            }
            putchar('\n');
        }
        putchar('\n');
    }
    fflush(stdout);
}

void add_check(board_s board, check_e * checks, ulookup_t index) {
    add_row_check(board, checks, index);
    add_col_check(board, checks, index);
}

void sub_check(board_s board, check_e * checks, ulookup_t index) {
    sub_row_check(board, checks, index);
    sub_col_check(board, checks, index);
}

void add_row_check(board_s board, check_e * checks, ulookup_t index) {
    if (checks[index] & ROWCHECK) return;

    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    while (!is_wall_hit(board, row, col - 1)) col--;
    for (ulookup_t i = 0; i < board.blocks[ROW_E][index]; i++) checks[board.grid[row][col + i]] |= ROWCHECK;
}

void add_col_check(board_s board, check_e * checks, ulookup_t index) {
    if (checks[index] & COLCHECK) return;

    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    while (!is_wall_hit(board, row - 1, col)) row--;
    for (ulookup_t i = 0; i < board.blocks[COLUMN_E][index]; i++) checks[board.grid[row + i][col]] |= COLCHECK;
}

void sub_row_check(board_s board, check_e * checks, ulookup_t index) {
    if (!(checks[index] & ROWCHECK)) return;

    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    while (!is_wall_hit(board, row, col - 1)) col--;
    for (ulookup_t i = 0; i < board.blocks[ROW_E][index]; i++) checks[board.grid[row][col + i]] &= ~(ROWCHECK);
}

void sub_col_check(board_s board, check_e * checks, ulookup_t index) {
    if (!(checks[index] & ROWCHECK)) return;

    ulookup_t row = board.coords[ROW_E][index], col = board.coords[COLUMN_E][index];
    while (!is_wall_hit(board, row - 1, col)) row--;
    for (ulookup_t i = 0; i < board.blocks[COLUMN_E][index]; i++) checks[board.grid[row + i][col]] &= ~(COLCHECK);
}

board_grid_s _init_grid(FILE * kakuro_file) {
    assert(kakuro_file && "KAKURO FILE POINTER IS NULL");

    size_t read_size = 0;
    board_grid_s g = { 0 };

    read_size = fread(g.size, sizeof(ulookup_t), GRID_DIMENTIONS, kakuro_file);
    assert(read_size == 2);

    g.count = g.size[ROW_E] * g.size[COLUMN_E];
    g.grids[ROW_E] = malloc(g.size[ROW_E] * sizeof(lookup_t*));
    assert(g.grids[ROW_E] && "ALLOCATION TO ROW ARRAY FAILED");
    g.grids[COLUMN_E] = malloc(g.size[ROW_E] * sizeof(lookup_t*));
    assert(g.grids[COLUMN_E] && "ALLOCATION TO COLUMN ARRAY FAILED");

    for (size_t i = 0; i < g.size[ROW_E]; i++) {
        g.grids[ROW_E][i]    = malloc(g.size[COLUMN_E] * sizeof(lookup_t));
        g.grids[COLUMN_E][i] = malloc(g.size[COLUMN_E] * sizeof(lookup_t));

        assert(g.grids[ROW_E][i] && "ALLOCATION TO ROW ARRAY FAILED");
        assert(g.grids[COLUMN_E][i] && "ALLOCATION TO COLUMN ARRAY FAILED");
    }

    ulookup_t buffer[GRID_DIMENTIONS * (sizeof(ulookup_t) << 8) * (sizeof(ulookup_t) << 8)] = { 0 };
    size_t read_number = GRID_DIMENTIONS * g.size[ROW_E] * g.size[COLUMN_E], index = 0;
    read_size = fread(buffer, sizeof(lookup_t), read_number, kakuro_file);
    assert((read_size == (sizeof(lookup_t) * read_number)) && "READ FAILED");
    for (ulookup_t i = 0; i < GRID_DIMENTIONS; i++) {
        for (ulookup_t j = 0; j < g.size[ROW_E]; j++) {
            for (ulookup_t k = 0; k < g.size[COLUMN_E]; k++) {
                g.grids[i][j][k] = buffer[index];
                index++;
            }
        }
    }

    g.empty_count = _empty_cell_count(g);

    return g;
}

void _free_grid(board_grid_s * grid) {
    for (size_t i = 0; i < grid->size[ROW_E]; i++) {
        free(grid->grids[ROW_E][i]);
        free(grid->grids[COLUMN_E][i]);
    }

    free(grid->grids[ROW_E]);
    free(grid->grids[COLUMN_E]);

    grid->grids[ROW_E] = NULL;
    grid->grids[COLUMN_E] = NULL;
}

void _kakuro_alloc(board_s * board, FILE * kakuro_file) {
    board->game = _init_grid(kakuro_file);

    board->grid = malloc(board->game.size[ROW_E] * sizeof(lookup_t*));
    assert(board->grid && "ALLOCATION TO ROW ARRAY FAILED");

    for (size_t i = 0; i < board->game.size[ROW_E]; i++) {
        board->grid[i] = malloc(board->game.size[COLUMN_E] * sizeof(lookup_t));
        assert(board->grid[i] && "ALLOCATION TO ROW ARRAY FAILED");
    }

    for (size_t i = 0; i < U_LOOKUP_COUNT * GRID_DIMENTIONS; i++) {
        board->u_lookups[i] = malloc(board->game.empty_count);
        assert(board->u_lookups[i] && "ALLOCATION TO UNSIGNED LOOKUP FAILED");
    }
}

void _kakuro_setup(board_s * board) {
    lookup_t index = 0;
    for (size_t r = 0; r < board->game.size[ROW_E]; r++) {
        for (size_t c = 0; c < board->game.size[COLUMN_E]; c++) {
            board->grid[r][c] = (board->game.grids[ROW_E][r][c] == 0) ? index++ : -1;
        }
    }

    index = 0;
    for (size_t r = 0; r < board->game.size[ROW_E]; r++) {
        for (size_t c = 0; c < board->game.size[COLUMN_E]; c++) {
            board->grid[r][c] = (board->game.grids[ROW_E][r][c] == 0) ? index : -1;
            if (board->grid[r][c] == -1) continue;

            _setup_coords(board, r, c, index);
            _setup_blocks(board, r, c, index);
            _setup_sums(board, r, c, index);

            index++;
        }
    }
}

void _setup_coords(board_s * board, ulookup_t row, ulookup_t col, ulookup_t index) {
    board->coords[ROW_E][index]    = row;
    board->coords[COLUMN_E][index] = col;
}

void _setup_blocks(board_s * board, ulookup_t row, ulookup_t col, ulookup_t index) {
    ulookup_t r = row, c = col;
    if (col && board->grid[row][col - 1] != -1 && board->blocks[ROW_E][board->grid[row][col - 1]]) {
        board->blocks[ROW_E][index] = board->blocks[ROW_E][board->grid[row][col - 1]];
    } else {
        ulookup_t c_blocks = 0;
        while (c != board->game.size[COLUMN_E] && (board->grid[row][c++] >= 0)) c_blocks++;
        board->blocks[ROW_E][index] = c_blocks;
    }

    if (row && board->grid[row - 1][col] != -1 && board->blocks[COLUMN_E][board->grid[row - 1][col]]) {
        board->blocks[COLUMN_E][index] = board->blocks[COLUMN_E][board->grid[row - 1][col]];
    } else {
        ulookup_t r_blocks = 0;
        while (r != board->game.size[ROW_E] && (board->grid[r++][col]) >= 0) r_blocks++;
        board->blocks[COLUMN_E][index] = r_blocks;
    }
}

void _setup_sums(board_s * board, ulookup_t row, ulookup_t col, ulookup_t index) {
    ulookup_t r = row, c = col;
    if (col && board->grid[row][col - 1] != -1 && board->sums[ROW_E][board->grid[row][col - 1]]) {
        board->sums[ROW_E][index] = board->sums[ROW_E][board->grid[row][col - 1]];
    } else {
        while (~c && !(board->game.grids[ROW_E][row][c])) c--;

        assert(~c && board->game.grids[ROW_E][row][c] > 0 && "NO SUMS FOUND");
        board->sums[ROW_E][index] = board->game.grids[ROW_E][row][c];
    }

    if (row && board->grid[row - 1][col] != -1 && board->sums[COLUMN_E][board->grid[row - 1][col]]) {
        board->sums[COLUMN_E][index] = board->sums[COLUMN_E][board->grid[row - 1][col]];
    } else {
        while (~r && !(board->game.grids[COLUMN_E][r][col])) r--;

        assert(~r && board->game.grids[COLUMN_E][r][col] > 0 && "NO SUMS FOUND");
        board->sums[COLUMN_E][index] = board->game.grids[COLUMN_E][r][col];
    }
}

ulookup_t _empty_cell_count(board_grid_s from) {
    assert(from.grids && "GRID POINTER IS NULL");

    ulookup_t count = 0;
    for (size_t i = 0; i < from.size[ROW_E]; i++) {
        for (size_t j = 0; j < from.size[COLUMN_E]; j++) {
            count += from.grids[ROW_E][i][j] == 0;
        }
    }

    return count;
}
