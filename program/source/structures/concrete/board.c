#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <structures/concrete/board.h>

Kakuro init_kakuro(FILE * kakuro_file) {
    assert(kakuro_file && "KAKURO FILE POINTER IS NULL");

    Kakuro k = { 0 };
    _kakuro_alloc(&k, kakuro_file);
    _kakuro_setup(&k);

    return k;
}

void free_kakuro(Kakuro * board) {
    for (size_t i = 0; i < GRID_DIMENTIONS * U_LOOKUP_COUNT; i++) {
        free(board->u_lookups[i]);
        board->u_lookups[i] = NULL;
    }

    for (size_t i = 0; i < board->game.size[ROW]; i++) free(board->grid[i]);
    free(board->grid);

    _free_grid(&(board->game));
}

KGrid _init_grid(FILE * kakuro_file) {
    assert(kakuro_file && "KAKURO FILE POINTER IS NULL");

    KGrid g = { 0 };
    fread(g.size, sizeof(ksize_t), GRID_DIMENTIONS, kakuro_file);
    g.count = g.size[ROW] * g.size[COLUMN];

    assert((g.grids[ROW]    = malloc(g.size[ROW] * sizeof(lookup_t*))) && "ALLOCATION TO ROW ARRAY FAILED");
    assert((g.grids[COLUMN] = malloc(g.size[ROW] * sizeof(lookup_t*))) && "ALLOCATION TO COLUMN ARRAY FAILED");

    for (size_t i = 0; i < g.size[ROW]; i++) {
        assert((g.grids[ROW][i]    = malloc(g.size[COLUMN] * sizeof(lookup_t))) && "ALLOCATION TO ROW ARRAY FAILED");
        assert((g.grids[COLUMN][i] = malloc(g.size[COLUMN] * sizeof(lookup_t))) && "ALLOCATION TO COLUMN ARRAY FAILED");
    }

    for (ksize_t i = 0; i < GRID_DIMENTIONS; i++) {
        for (ksize_t j = 0; j < g.size[ROW]; j++) {
            for (ksize_t k = 0; k < g.size[COLUMN]; k++) {
                assert((fread(&(g.grids[i][j][k]), sizeof(lookup_t), 1, kakuro_file)) == sizeof(lookup_t) * 1 && "READ FAILED");
            }
        }
    }

    g.empty_count = _empty_cell_count(g);

    return g;
}

void _free_grid(KGrid * grid) {
    for (size_t i = 0; i < grid->size[ROW]; i++) {
        free(grid->grids[ROW][i]);
        free(grid->grids[COLUMN][i]);
    }

    free(grid->grids[ROW]);
    free(grid->grids[COLUMN]);

    grid->grids[ROW] = NULL;
    grid->grids[COLUMN] = NULL;
}

void _kakuro_alloc(Kakuro * board, FILE * kakuro_file) {
    board->game = _init_grid(kakuro_file);

    assert((board->grid = malloc(board->game.size[ROW] * sizeof(lookup_t*))) && "ALLOCATION TO ROW ARRAY FAILED");
    for (size_t i = 0; i < board->game.size[ROW]; i++) {
        assert((board->grid[i] = malloc(board->game.size[COLUMN] * sizeof(lookup_t))) && "ALLOCATION TO ROW ARRAY FAILED");
    }

    for (size_t i = 0; i < U_LOOKUP_COUNT * GRID_DIMENTIONS; i++) {
        assert((board->u_lookups[i] = malloc(board->game.empty_count)) && "ALLOCATION TO UNSIGNED LOOKUP FAILED");
    }
}

void _kakuro_setup(Kakuro * board) {
    assert(board && "KAKURO POINTER IS NULL");

    size_t index = 0;
    for (size_t r = 0; r < board->game.size[ROW]; r++) {
        for (size_t c = 0; c < board->game.size[COLUMN]; c++) {
            board->grid[r][c] = (board->game.grids[ROW][r][c] == 0) ? index++ : -1;
        }
    }

    index = 0;
    for (size_t r = 0; r < board->game.size[ROW]; r++) {
        for (size_t c = 0; c < board->game.size[COLUMN]; c++) {
            board->grid[r][c] = (board->game.grids[ROW][r][c] == 0) ? index : -1;
            if (board->grid[r][c] == -1) continue;

            _setup_coords(board, r, c, index);
            _setup_blocks(board, r, c, index);
            _setup_sums(board, r, c, index);

            index++;
        }
    }
}

void _setup_coords(Kakuro * board, ksize_t row, ksize_t col, ksize_t index) {
    assert(row < board->game.size[ROW] && "ROW IS OUT OF BOUNDS");
    assert(col < board->game.size[COLUMN] && "COLUMN IS OUT OF BOUNDS");
    assert(index < board->game.empty_count && "INDEX IS OUT OF BOUNDS");
    assert(board->coords[ROW] && "LOOKUP IS NULL");
    assert(board->coords[COLUMN] && "LOOKUP IS NULL");

    board->coords[ROW][index]    = row;
    board->coords[COLUMN][index] = col;
}

void _setup_blocks(Kakuro * board, ksize_t row, ksize_t col, ksize_t index) {
    assert(row < board->game.size[ROW] && "ROW IS OUT OF BOUNDS");
    assert(col < board->game.size[COLUMN] && "COLUMN IS OUT OF BOUNDS");
    assert(index < board->game.empty_count && "INDEX IS OUT OF BOUNDS");
    assert(board->grid && "LOOKUP POINTER IS NULL");
    assert(board->blocks && "LOOKUP POINTER IS NULL");

    ksize_t r = row, c = col;
    if (col && board->grid[row][col - 1] != -1 && board->blocks[ROW][board->grid[row][col - 1]]) {
        board->blocks[ROW][index] = board->blocks[ROW][board->grid[row][col - 1]];
    } else {
        ksize_t c_blocks = 0;
        while (c != board->game.size[COLUMN] && (board->grid[row][c++] >= 0)) c_blocks++;
        board->blocks[ROW][index] = c_blocks;
    }

    if (row && board->grid[row - 1][col] != -1 && board->blocks[COLUMN][board->grid[row - 1][col]]) {
        board->blocks[COLUMN][index] = board->blocks[COLUMN][board->grid[row - 1][col]];
    } else {
        ksize_t r_blocks = 0;
        while (r != board->game.size[ROW] && (board->grid[r++][col]) >= 0) r_blocks++;
        board->blocks[COLUMN][index] = r_blocks;
    }
}

void _setup_sums(Kakuro * board, ksize_t row, ksize_t col, ksize_t index) {
    assert(row < board->game.size[ROW] && "ROW IS OUT OF BOUNDS");
    assert(col < board->game.size[COLUMN] && "COLUMN IS OUT OF BOUNDS");
    assert(index < board->game.empty_count && "INDEX IS OUT OF BOUNDS");
    assert(board->grid && "LOOKUP POINTER IS NULL");
    assert(board->sums && "LOOKUP POINTER IS NULL");

    ksize_t r = row, c = col;
    if (col && board->grid[row][col - 1] != -1 && board->sums[ROW][board->grid[row][col - 1]]) {
        board->sums[ROW][index] = board->sums[ROW][board->grid[row][col - 1]];
    } else {
        while (~c && !(board->game.grids[ROW][row][c])) c--;
        assert(~c && (board->sums[ROW][index] = board->game.grids[ROW][row][c]) > 0 && "NO SUMS FOUND");
    }

    if (row && board->grid[row - 1][col] != -1 && board->sums[COLUMN][board->grid[row - 1][col]]) {
        board->sums[COLUMN][index] = board->sums[COLUMN][board->grid[row - 1][col]];
    } else {
        while (~r && !(board->game.grids[COLUMN][r][col])) r--;
        assert(~r && (board->sums[COLUMN][index] = board->game.grids[COLUMN][r][col]) > 0 && "NO SUMS FOUND");
    }
}

ksize_t _empty_cell_count(KGrid from) {
    assert(from.grids && "GRID POINTER IS NULL");

    ksize_t count = 0;
    for (size_t i = 0; i < from.size[ROW]; i++) {
        for (size_t j = 0; j < from.size[COLUMN]; j++) {
            count += from.grids[ROW][i][j] == 0;
        }
    }

    return count;
}

bool is_wall_hit(Kakuro board, ksize_t row, ksize_t col) {
    return
        (row >= board.game.size[ROW] || col >= board.game.size[COLUMN]) ||
        board.grid[row][col] == -1;
}

void print_board(Kakuro board) {
    for (size_t i = 0; i < GRID_DIMENTIONS; i++) {
        for (size_t j = 0; j < board.game.size[ROW]; j++) {
            for (size_t k = 0; k < board.game.size[COLUMN]; k++) {
                printf("%02hhd ", board.game.grids[i][j][k]);
            }
            putchar('\n');
        }
        putchar('\n');
    }
}

IStack create_index_stack(void) {
    return (IStack) { 0 };
}

void destroy_index_stack(IStack * stack) {
    while (!is_empty_index_stack(*stack)) pop_index_stack(stack);
}

bool is_empty_index_stack(IStack stack) {
    return stack.size == 0;
}

void push_index_stack(IStack * stack, ksize_t array) {
    assert(stack && "STACK POINTER IS NULL");
    size_t idx = _get_index_index_stack(*stack, ISI_POSITION_NEXT);

    if (!idx) {
        ISLArray * temp = malloc(sizeof(ISLArray));
        temp->next = NULL;
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->next = stack->head;
        stack->head = temp;
    }
    stack->head->elements[idx] = array;

    stack->size++;
}

ksize_t pop_index_stack(IStack * stack) {
    assert(stack && "STACK POINTER IS NULL");
    assert(!is_empty_index_stack(*stack) && "CAN'T POP EMPTY STACK");

    ksize_t e = peek_index_stack(*stack);

    size_t idx = _get_index_index_stack(*stack, ISI_POSITION_CURRENT);
    stack->size--;

    if (!idx) {
        ISLArray * temp = stack->head->next;
        free(stack->head);
        stack->head = temp;
    }

    return e;
}

ksize_t peek_index_stack(IStack stack) {
    assert(!is_empty_index_stack(stack) && "CAN'T PEEK EMPTY STACK");
    size_t idx = _get_index_index_stack(stack, ISI_POSITION_CURRENT);

    return stack.head->elements[idx];
}

size_t _get_index_index_stack(IStack stack, ISIPosition type) {
    assert((!is_empty_index_stack(stack) || type != ISI_POSITION_CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY STACK");
    return (stack.size - type) % (STACK_LIST_ARRAY_SIZE - 1);
}