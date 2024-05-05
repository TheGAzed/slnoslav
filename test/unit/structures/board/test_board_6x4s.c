#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <greatest.h>

#include <structures/concrete/board.h>

#define FILEPATH "test/data/6x4s.kkr"
#define GAME_GRID_ROW_SIZE 6
#define GAME_GRID_COL_SIZE 4
#define GAME_GRID_COUNT (GAME_GRID_ROW_SIZE * GAME_GRID_COL_SIZE)
#define GAME_GRID_EMPTY_COUNT 11

Kakuro b6x4s;

void construct_6x4s() {
    FILE * fp =  fopen(FILEPATH, "rb");
    b6x4s = init_kakuro(fp);
    fclose(fp);
}

void destruct_6x4s() {
    free_kakuro(&b6x4s);
}

void __attribute__((constructor)) construct_6x4s();
void __attribute__((destructor)) destruct_6x4s();

TEST game_grid_row_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID ROW SIZE DOESN'T MATCH", 
        GAME_GRID_ROW_SIZE, b6x4s.game.size[ROW],
        "%hhu"
    );
    PASS();
}

TEST game_grid_column_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID COLUMN SIZE DOESN'T MATCH",
        GAME_GRID_COL_SIZE, b6x4s.game.size[COLUMN],
        "%hhu"
    );
    PASS();
}

TEST game_grid_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID CELL COLUMN DOESN'T MATCH",
        GAME_GRID_COUNT, b6x4s.game.count,
        "%hhu"
    );
    PASS();
}

TEST game_grid_empty_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID EMPTY CELL COLUMN DOESN'T MATCH",
        GAME_GRID_EMPTY_COUNT, b6x4s.game.empty_count,
        "%hhu"
    );
    PASS();
}

TEST game_grid(void) {
    lookup_t expected_grid[GRID_DIMENTIONS][GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        [ROW] = {
            { -1, -1, -1, -1, },
            { 10,  0,  0, -1, },
            {  4,  0,  0, -1, },
            { 18,  0,  0,  0, },
            { -1,  4,  0,  0, },
            { -1, 13,  0,  0, },
        },
        [COLUMN] = {
            { -1, 20, 22, -1, },
            { -1,  0,  0, -1, },
            { -1,  0,  0,  7, },
            { -1,  0,  0,  0, },
            { -1, -1,  0,  0, },
            { -1, -1,  0,  0, },
        }
    };

    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "GAME GRID ROW ELEMENTS DON'T MATCH",
                expected_grid[ROW][i][j], b6x4s.game.grids[ROW][i][j],
                "%hhd"
            );
            ASSERT_EQ_FMTm(
                "GAME GRID COLUMN ELEMENTS DON'T MATCH",
                expected_grid[COLUMN][i][j], b6x4s.game.grids[COLUMN][i][j],
                "%hhd"
            );
        }
    }
    PASS();
}

TEST grid_lookup(void) {
    lookup_t expected_grid[GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        { -1, -1, -1, -1, },
        { -1,  0,  1, -1, },
        { -1,  2,  3, -1, },
        { -1,  4,  5,  6, },
        { -1, -1,  7,  8, },
        { -1, -1,  9, 10, },
    };

    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "BOARD LOOKUP GRID ELEMENTS DON'T MATCH",
                expected_grid[i][j], b6x4s.grid[i][j],
                "%hhd"
            );
        }
    }
    PASS();
}

TEST lookup_blocks(void) {
    ulookup_t expected_blocks[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, },
        [COLUMN] = { 3, 5, 3, 5, 3, 5, 3, 5, 3, 5, 3, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW BLOCKS DON'T MATCH",
            expected_blocks[ROW][i], b6x4s.blocks[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN BLOCKS DON'T MATCH",
            expected_blocks[COLUMN][i], b6x4s.blocks[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_sums(void) {
    ulookup_t expected_sums[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 10, 10,  4,  4, 18, 18, 18, 4, 4, 13, 13, },
        [COLUMN] = { 20, 22, 20, 22, 20, 22,  7, 22, 7, 22, 7, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW SUMS DON'T MATCH",
             expected_sums[ROW][i], b6x4s.sums[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN SUMS DON'T MATCH",
            expected_sums[COLUMN][i], b6x4s.sums[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_coordinates(void) {
    ulookup_t expected_coords[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, },
        [COLUMN] = { 1, 2, 1, 2, 1, 2, 3, 2, 3, 2, 3, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW COORDINATES DON'T MATCH",
            expected_coords[ROW][i], b6x4s.coords[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN COORDINATES DON'T MATCH",
            expected_coords[COLUMN][i], b6x4s.coords[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}


TEST is_wall_hit_compare_board(void) {
    bool expected_walls[GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        { true, true,  true,  true,  },
        { true, false, false, true,  },
        { true, false, false, true,  },
        { true, false, false, false, },
        { true, true,  false, false, },
        { true, true,  false, false, },
    };

    for (ksize_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQm(
                "WALL HIT DON'T MATCH",
                expected_walls[i][j], is_wall_hit(b6x4s, i, j)
            );
        }
    }
    PASS();
}

SUITE (test_board_6x4s) {
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
