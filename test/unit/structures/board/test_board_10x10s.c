#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <greatest.h>

#include <structures/concrete/board.h>

#define FILEPATH "test/data/10x10s.kkr"
#define GAME_GRID_ROW_SIZE 10
#define GAME_GRID_COL_SIZE 10
#define GAME_GRID_COUNT (GAME_GRID_ROW_SIZE * GAME_GRID_COL_SIZE)
#define GAME_GRID_EMPTY_COUNT 49

Kakuro b10x10s;

void construct_10x10s() {
    FILE * fp =  fopen(FILEPATH, "rb");
    b10x10s = init_kakuro(fp);
    fclose(fp);
}

void destruct_10x10s() {
    free_kakuro(&b10x10s);
}

void __attribute__((constructor)) construct_10x10s();
void __attribute__((destructor)) destruct_10x10s();

TEST game_grid_row_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID ROW SIZE DOESN'T MATCH", 
        GAME_GRID_ROW_SIZE, b10x10s.game.size[ROW],
        "%hhu"
    );
    
    PASS();
}

TEST game_grid_column_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID COLUMN SIZE DOESN'T MATCH",
        GAME_GRID_COL_SIZE, b10x10s.game.size[COLUMN],
        "%hhu"
    );
    PASS();
}

TEST game_grid_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID CELL COLUMN DOESN'T MATCH",
        GAME_GRID_COUNT, b10x10s.game.count,
        "%hhu"
    );
    PASS();
}

TEST game_grid_empty_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID EMPTY CELL COLUMN DOESN'T MATCH",
        GAME_GRID_EMPTY_COUNT, b10x10s.game.empty_count,
        "%hhu"
    );
    PASS();
}

TEST game_grid(void) {
    lookup_t expected_grid[GRID_DIMENTIONS][GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        [ROW] = {
            { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
            { -1, -1, 15,  0,  0, 13,  0,  0, -1, -1, },
            { -1, -1, 42,  0,  0,  0,  0,  0,  0,  0, },
            { 14,  0,  0, 12,  0,  0, -1, 16,  0,  0, },
            { 19,  0,  0,  0,  4,  0,  0, -1, -1, -1, },
            { -1, -1, 27,  0,  0,  0,  0,  0, -1, -1, },
            { -1, -1, -1,  4,  0,  0, 22,  0,  0,  0, },
            { 16,  0,  0, -1, 15,  0,  0, 17,  0,  0, },
            { 30,  0,  0,  0,  0,  0,  0,  0, -1, -1, },
            { -1, -1,  6,  0,  0, 16,  0,  0, -1, -1, },
        },
        [COLUMN] = {
            { -1, -1, -1, 16, 23, -1, 10, 12, -1, -1, },
            { -1, -1, -1,  0,  0, 32,  0,  0, 17, 10, },
            { -1, 16,  8,  0,  0,  0,  0,  0,  0,  0, },
            { -1,  0,  0, 17,  0,  0,  7, -1,  0,  0, },
            { -1,  0,  0,  0,  5,  0,  0, 13, -1, -1, },
            { -1, -1, -1,  0,  0,  0,  0,  0, 14, 16, },
            { -1, 14, 15, -1,  0,  0, 14,  0,  0,  0, },
            { -1,  0,  0,  7,  6,  0,  0, 16,  0,  0, },
            { -1,  0,  0,  0,  0,  0,  0,  0, -1, -1, },
            { -1, -1, -1,  0,  0, -1,  0,  0, -1, -1, },
        }
    };

    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "GAME GRID ROW ELEMENTS DON'T MATCH",
                expected_grid[ROW][i][j], b10x10s.game.grids[ROW][i][j],
                "%hhd"
            );
            ASSERT_EQ_FMTm(
                "GAME GRID COLUMN ELEMENTS DON'T MATCH",
                expected_grid[COLUMN][i][j], b10x10s.game.grids[COLUMN][i][j],
                "%hhd"
            );
        }
    }
    PASS();
}

TEST grid_lookup(void) {
    lookup_t expected_grid[GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, },
        { -1, -1, -1,  0,  1, -1,  2,  3, -1, -1, },
        { -1, -1, -1,  4,  5,  6,  7,  8,  9, 10, },
        { -1, 11, 12, -1, 13, 14, -1, -1, 15, 16, },
        { -1, 17, 18, 19, -1, 20, 21, -1, -1, -1, },
        { -1, -1, -1, 22, 23, 24, 25, 26, -1, -1, },
        { -1, -1, -1, -1, 27, 28, -1, 29, 30, 31, },
        { -1, 32, 33, -1, -1, 34, 35, -1, 36, 37, },
        { -1, 38, 39, 40, 41, 42, 43, 44, -1, -1, },
        { -1, -1, -1, 45, 46, -1, 47, 48, -1, -1, },
    };

    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "BOARD LOOKUP GRID ELEMENTS DON'T MATCH",
                expected_grid[i][j], b10x10s.grid[i][j],
                "%hhd"
            );
        }
    }
    PASS();
}

TEST lookup_blocks(void) {
    ulookup_t expected_blocks[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, 2, 2, 3, 3, 3, 2, 2, 5, 5, 5, 5, 5, 2, 2, 3, 3, 3, 2, 2, 2, 2, 2, 2, 7, 7, 7, 7, 7, 7, 7, 2, 2, 2, 2, },
        [COLUMN] = { 2, 3, 2, 2, 2, 3, 7, 2, 2, 2, 2, 2, 2, 3, 7, 2, 2, 2, 2, 2, 7, 2, 2, 2, 7, 2, 2, 2, 7, 2, 2, 2, 2, 2, 7, 3, 2, 2, 2, 2, 2, 2, 7, 3, 2, 2, 2, 3, 2, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW BLOCKS DON'T MATCH",
            expected_blocks[ROW][i], b10x10s.blocks[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN BLOCKS DON'T MATCH",
            expected_blocks[COLUMN][i], b10x10s.blocks[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_sums(void) {
    ulookup_t expected_sums[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 15, 15, 13, 13, 42, 42, 42, 42, 42, 42, 42, 14, 14, 12, 12, 16, 16, 19, 19, 19,  4,  4, 27, 27, 27, 27, 27,  4,  4, 22, 22, 22, 16, 16, 15, 15, 17, 17, 30, 30, 30, 30, 30, 30, 30,  6,  6, 16, 16, },
        [COLUMN] = { 16, 23, 10, 12, 16, 23, 32, 10, 12, 17, 10, 16,  8, 23, 32, 17, 10, 16,  8, 17, 32,  7, 17,  5, 32,  7, 13,  5, 32, 13, 14, 16, 14, 15, 32, 14, 14, 16, 14, 15,  7,  6, 32, 14, 16,  7,  6, 14, 16, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW SUMS DON'T MATCH",
             expected_sums[ROW][i], b10x10s.sums[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN SUMS DON'T MATCH",
            expected_sums[COLUMN][i], b10x10s.sums[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_coordinates(void) {
    ulookup_t expected_coords[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, },
        [COLUMN] = { 3, 4, 6, 7, 3, 4, 5, 6, 7, 8, 9, 1, 2, 4, 5, 8, 9, 1, 2, 3, 5, 6, 3, 4, 5, 6, 7, 4, 5, 7, 8, 9, 1, 2, 5, 6, 8, 9, 1, 2, 3, 4, 5, 6, 7, 3, 4, 6, 7, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW COORDINATES DON'T MATCH",
            expected_coords[ROW][i], b10x10s.coords[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN COORDINATES DON'T MATCH",
            expected_coords[COLUMN][i], b10x10s.coords[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST is_wall_hit_compare_board(void) {
    bool expected_walls[GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        { true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  },
        { true,  true,  true,  false, false, true,  false, false, true,  true,  },
        { true,  true,  true,  false, false, false, false, false, false, false, },
        { true,  false, false, true,  false, false, true,  true,  false, false, },
        { true,  false, false, false, true,  false, false, true,  true,  true,  },
        { true,  true,  true,  false, false, false, false, false, true,  true,  },
        { true,  true,  true,  true,  false, false, true,  false, false, false, },
        { true,  false, false, true,  true,  false, false, true,  false, false, },
        { true,  false, false, false, false, false, false, false, true,  true,  },
        { true,  true,  true,  false, false, true,  false, false, true,  true,  },
    };

    for (ksize_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQm(
                "WALL HIT DON'T MATCH",
                expected_walls[i][j], is_wall_hit(b10x10s, i, j)
            );
        }
    }
    PASS();
}

SUITE (test_board_10x10s) {
    // TESTS IF KAKURO STRUCTURE WAS LOADED CORRECTLY
    RUN_TEST(game_grid_row_size);
    RUN_TEST(game_grid_column_size);
    RUN_TEST(game_grid_count);
    RUN_TEST(game_grid_empty_count);
    RUN_TEST(game_grid);
    RUN_TEST(grid_lookup);
    RUN_TEST(lookup_blocks);
    RUN_TEST(lookup_sums);
    RUN_TEST(lookup_coordinates);
    RUN_TEST(is_wall_hit_compare_board);
}
