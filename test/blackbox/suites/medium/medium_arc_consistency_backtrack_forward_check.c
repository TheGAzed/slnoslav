#include <greatest.h>

#include <instance/settings.h>
#include <algorithms/depth_first_search.h>

#define MEDIUM_PUZZLE_PATH "./test/puzzles/medium/"

TEST medium_arc_consistency_backtrack_forward_check_one(void) {
    ksize_t expected_solution[] = {
        7, 9,    3, 1, 8,          9, 8, 6,
        2, 5,    1, 2, 4,    3, 8, 7, 9, 5,
        3, 7, 9,    3, 7, 8, 1, 9,    5, 1,
        1, 8, 4, 3, 5,    9, 7,    8, 7, 3,
              5, 1,             1, 2,
        4, 8, 7,    3, 1,    4, 7, 9, 8, 2,
        7, 9,    4, 9, 7, 6, 8,    1, 6, 3,
        1, 6, 8, 2, 7,    7, 9, 8,    3, 1,
        2, 1, 4,          1, 2, 4,    9, 7,
    };

    get_settings_singleton()->filepath           = MEDIUM_PUZZLE_PATH"1.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = true;

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    ASSERTm("COULDN'T OPEN FILE", fp);
    
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    ASSERTm("NO SOLUTION FOUND", solution.size);
    ASSERTm("LENGTHS DON'T ADD UP", solution.size == (sizeof(expected_solution) / sizeof(ksize_t)));

    for (ksize_t i = 0; i < solution.size; i++) {
        ASSERTm("INVALID SOLUTION", get_one_value(solution.elements[i]) == expected_solution[i]);
    }

    destroy_state_array(&solution);

    PASS();
}

TEST medium_arc_consistency_backtrack_forward_check_two(void) {
    ksize_t expected_solution[] = {
           9, 8, 4, 3,          6, 8, 3, 9,
           7, 4, 2, 1, 9,       2, 3, 1, 5,
              9, 7,    7, 2, 8, 4, 1,
        2, 8, 3, 1, 9,    8, 9, 1,    9, 3,
        8, 9, 7,    8, 2, 6, 3,    4, 1, 2, 
        1, 2,    4, 7, 9,    5, 2, 6, 3, 1,
              2, 3, 5, 1, 8,    4, 9,
        8, 9, 4, 6,       5, 9, 3, 1, 8,
        4, 7, 1, 2,          7, 1, 2, 4,
    };

    get_settings_singleton()->filepath           = MEDIUM_PUZZLE_PATH"2.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = true;

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    ASSERTm("COULDN'T OPEN FILE", fp);
    
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    ASSERTm("NO SOLUTION FOUND", solution.size);
    ASSERTm("LENGTHS DON'T ADD UP", solution.size == (sizeof(expected_solution) / sizeof(ksize_t)));

    for (ksize_t i = 0; i < solution.size; i++) {
        ASSERTm("INVALID SOLUTION", get_one_value(solution.elements[i]) == expected_solution[i]);
    }

    destroy_state_array(&solution);

    PASS();
}

TEST medium_arc_consistency_backtrack_forward_check_three(void) {
    ksize_t expected_solution[] = {
              2, 4, 1,          2, 4, 1, 7,
        8, 6, 5, 7, 3,       7, 6, 8, 3, 9,
        5, 1, 3, 2,          3, 1, 6,
           9, 7,          1, 2, 4,    9, 7,
        5, 7, 1, 2,    9, 3,    3, 6, 1, 2,
        1, 3,    3, 9, 7,          9, 7,
              3, 1, 5,          3, 1, 2, 5,
        4, 9, 7, 6, 8,       9, 6, 7, 4, 8,
        1, 7, 2, 4,          3, 1, 2, 
    };

    get_settings_singleton()->filepath           = MEDIUM_PUZZLE_PATH"3.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = true;

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    ASSERTm("COULDN'T OPEN FILE", fp);
    
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    ASSERTm("NO SOLUTION FOUND", solution.size);
    ASSERTm("LENGTHS DON'T ADD UP", solution.size == (sizeof(expected_solution) / sizeof(ksize_t)));

    for (ksize_t i = 0; i < solution.size; i++) {
        ASSERTm("INVALID SOLUTION", get_one_value(solution.elements[i]) == expected_solution[i]);
    }

    destroy_state_array(&solution);

    PASS();
}

TEST medium_arc_consistency_backtrack_forward_check_four(void) {
    ksize_t expected_solution[] = {
        5, 9,    7, 8, 9,       4, 2, 3, 1,
        1, 3,    1, 2, 8,    8, 7, 9, 6, 2,
        3, 5, 1, 2, 4,       9, 8, 6,
        2, 7, 3,          1, 2, 6,
           8, 6, 9,    9, 3,    9, 3, 8,
                 6, 1, 5,          1, 5, 2,
              9, 8, 6,       3, 6, 2, 7, 4,
        8, 9, 3, 4, 2,    9, 6, 8,    3, 1,
        4, 7, 1, 2,       2, 1, 4,    9, 7,
    };

    get_settings_singleton()->filepath           = MEDIUM_PUZZLE_PATH"4.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = true;

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    ASSERTm("COULDN'T OPEN FILE", fp);
    
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    ASSERTm("NO SOLUTION FOUND", solution.size);
    ASSERTm("LENGTHS DON'T ADD UP", solution.size == (sizeof(expected_solution) / sizeof(ksize_t)));

    for (ksize_t i = 0; i < solution.size; i++) {
        ASSERTm("INVALID SOLUTION", get_one_value(solution.elements[i]) == expected_solution[i]);
    }

    destroy_state_array(&solution);

    PASS();
}

TEST medium_arc_consistency_backtrack_forward_check_five(void) {
    ksize_t expected_solution[] = {
        6, 8, 9,       2, 1, 4,    9, 4, 7,
        2, 4, 3,    7, 8, 5, 9,    8, 6, 9,
           1, 2, 9, 3, 5,       4, 1, 2,
        7, 9,    7, 2,    2, 3, 8, 4,
        2, 3, 4,    9, 2, 1, 7,    3, 1, 7,
              8, 1, 6, 4,    8, 2,    8, 9,
           7, 9, 3,       9, 6, 4, 8, 7,
        9, 2, 6,    9, 8, 7, 5,    2, 4, 1,
        7, 1, 3,    1, 4, 2,       6, 9, 3,
    };

    get_settings_singleton()->filepath           = MEDIUM_PUZZLE_PATH"5.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = true;

    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    ASSERTm("COULDN'T OPEN FILE", fp);
    
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    ASSERTm("NO SOLUTION FOUND", solution.size);
    ASSERTm("LENGTHS DON'T ADD UP", solution.size == (sizeof(expected_solution) / sizeof(ksize_t)));

    for (ksize_t i = 0; i < solution.size; i++) {
        ASSERTm("INVALID SOLUTION", get_one_value(solution.elements[i]) == expected_solution[i]);
    }

    destroy_state_array(&solution);

    PASS();
}

SUITE (medium_arc_consistency_backtrack_forward_check) {
    // 10 x 13
    RUN_TEST(medium_arc_consistency_backtrack_forward_check_one);
    RUN_TEST(medium_arc_consistency_backtrack_forward_check_two);
    RUN_TEST(medium_arc_consistency_backtrack_forward_check_three);
    RUN_TEST(medium_arc_consistency_backtrack_forward_check_four);
    RUN_TEST(medium_arc_consistency_backtrack_forward_check_five);
}
