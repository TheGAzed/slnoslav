#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include <greatest.h>

#include <ds/board.h>

#define FILEPATH "data/test/4x4.kkr"
#define GAME_GRID_ROW_SIZE 4
#define GAME_GRID_COL_SIZE 4
#define GAME_GRID_COUNT (GAME_GRID_ROW_SIZE * GAME_GRID_COL_SIZE)
#define GAME_GRID_EMPTY_COUNT 7

Kakuro b4x4;

void construct_4x4() {
    FILE * fp =  fopen(FILEPATH, "rb");
    b4x4 = init_kakuro(fp);
    fclose(fp);
}

void destruct_4x4() {
    free_kakuro(&b4x4);
}

void __attribute__((constructor)) construct_4x4();
void __attribute__((destructor)) destruct_4x4();

TEST game_grid_row_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID ROW SIZE DOESN'T MATCH", 
        GAME_GRID_ROW_SIZE, b4x4.game.size[ROW],
        "%hhu"
    );
    
    PASS();
}

TEST game_grid_column_size(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID COLUMN SIZE DOESN'T MATCH",
        GAME_GRID_COL_SIZE, b4x4.game.size[COLUMN],
        "%hhu"
    );
    PASS();
}

TEST game_grid_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID CELL COLUMN DOESN'T MATCH",
        GAME_GRID_COUNT, b4x4.game.count,
        "%hhu"
    );
    PASS();
}

TEST game_grid_empty_count(void) {
    ASSERT_EQ_FMTm(
        "GAME GRID EMPTY CELL COLUMN DOESN'T MATCH",
        GAME_GRID_EMPTY_COUNT, b4x4.game.empty_count,
        "%hhu"
    );
    PASS();
}

TEST game_grid(void) {
    lookup_t expected_grid[GRID_DIMENTIONS][GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        [ROW] = {
            { -1, -1, -1, -1, },
            { 13,  0,  0, -1, },
            { 12,  0,  0,  0, },
            { -1,  3,  0,  0, },
        },
        [COLUMN] = {
            { -1,  5, 19, -1, },
            { -1,  0,  0,  4, },
            { -1,  0,  0,  0, },
            { -1, -1,  0,  0, },
        }
    };

    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "GAME GRID ROW ELEMENTS DON'T MATCH",
                expected_grid[ROW][i][j], b4x4.game.grids[ROW][i][j],
                "%hhd"
            );
            ASSERT_EQ_FMTm(
                "GAME GRID COLUMN ELEMENTS DON'T MATCH",
                expected_grid[COLUMN][i][j], b4x4.game.grids[COLUMN][i][j],
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
        { -1,  2,  3,  4, },
        { -1, -1,  5,  6, },
    };


    for (size_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQ_FMTm(
                "BOARD LOOKUP GRID ELEMENTS DON'T MATCH",
                expected_grid[i][j], b4x4.grid[i][j],
                "%hhd"
            );
        }
    }
    PASS();
}

TEST lookup_blocks(void) {
    ulookup_t expected_blocks[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 2, 2, 3, 3, 3, 2, 2, },
        [COLUMN] = { 2, 3, 2, 3, 2, 3, 2, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW BLOCKS DON'T MATCH",
            expected_blocks[ROW][i], b4x4.blocks[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN BLOCKS DON'T MATCH",
            expected_blocks[COLUMN][i], b4x4.blocks[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_sums(void) {
    ulookup_t expected_sums[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 13, 13, 12, 12, 12,  3, 3, },
        [COLUMN] = {  5, 19,  5, 19,  4, 19, 4, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW SUMS DON'T MATCH",
             expected_sums[ROW][i], b4x4.sums[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN SUMS DON'T MATCH",
            expected_sums[COLUMN][i], b4x4.sums[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST lookup_coordinates(void) {
    ulookup_t expected_coords[GRID_DIMENTIONS][GAME_GRID_EMPTY_COUNT] = {
        [ROW]    = { 1, 1, 2, 2, 2, 3, 3, },
        [COLUMN] = { 1, 2, 1, 2, 3, 2, 3, },
    };

    for (size_t i = 0; i < GAME_GRID_EMPTY_COUNT; i++) {
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP ROW COORDINATES DON'T MATCH",
            expected_coords[ROW][i], b4x4.coords[ROW][i],
            "%hhd"
        );
        ASSERT_EQ_FMTm(
            "BOARD LOOKUP COLUMN COORDINATES DON'T MATCH",
            expected_coords[COLUMN][i], b4x4.coords[COLUMN][i],
            "%hhd"
        );
    }
    PASS();
}

TEST is_wall_hit_at_row_sub_zero_col_sub_zero_true(void) {
    ksize_t row = 0, col = 0;
    ASSERTm(
        "EXPECTED NO WALL HIT",
        is_wall_hit(b4x4, --row, --col)
    );
    PASS();
}

TEST is_wall_hit_at_row_sub_zero_true(void) {
    ksize_t row = 0;
    ASSERTm(
        "EXPECTED NO WALL HIT",
        is_wall_hit(b4x4, --row, 0)
    );
    PASS();
}

TEST is_wall_hit_at_col_sub_zero_true(void) {
    ksize_t col = 0;
    ASSERTm(
        "EXPECTED NO WALL HIT",
        is_wall_hit(b4x4, 0, --col)
    );
    PASS();
}

TEST is_wall_hit_at_row_outer_edge_col_outer_edge_true(void) {
    ksize_t row = GAME_GRID_ROW_SIZE, col = GAME_GRID_COL_SIZE;
    ASSERTm(
        "EXPECTED WALL HIT",
        is_wall_hit(b4x4, row, col)
    );
    PASS();
}

TEST is_wall_hit_at_row_outer_edge_true(void) {
    ksize_t row = GAME_GRID_ROW_SIZE;
    ASSERTm(
        "EXPECTED WALL HIT",
        is_wall_hit(b4x4, row, 0)
    );
    PASS();
}

TEST is_wall_hit_at_col_outer_edge_true(void) {
    ksize_t col = GAME_GRID_COL_SIZE;
    ASSERTm(
        "EXPECTED WALL HIT",
        is_wall_hit(b4x4, 0, col)
    );
    PASS();
}
 
TEST is_wall_hit_compare_board(void) {
    bool expected_walls[GAME_GRID_ROW_SIZE][GAME_GRID_COL_SIZE] = {
        { true, true,  true,  true,  },
        { true, false, false, true,  },
        { true, false, false, false, },
        { true, true,  false, false, },
    };

    for (ksize_t i = 0; i < GAME_GRID_ROW_SIZE; i++) {
        for (size_t j = 0; j < GAME_GRID_COL_SIZE; j++) {
            ASSERT_EQm(
                "WALL HIT DON'T MATCH",
                expected_walls[i][j], is_wall_hit(b4x4, i, j)
            );
        }
    }
    PASS();
}

SUITE (test_board_4x4) {
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

    // TESTS IF FUNCTION is_wall_hit() WORKS CORRECTLY
    RUN_TEST(is_wall_hit_at_row_sub_zero_col_sub_zero_true);
    RUN_TEST(is_wall_hit_at_row_sub_zero_true);
    RUN_TEST(is_wall_hit_at_col_sub_zero_true);
    RUN_TEST(is_wall_hit_at_row_outer_edge_col_outer_edge_true);
    RUN_TEST(is_wall_hit_at_row_outer_edge_true);
    RUN_TEST(is_wall_hit_at_col_outer_edge_true);
    RUN_TEST(is_wall_hit_compare_board);
}
