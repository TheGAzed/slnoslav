#include <greatest.h>

#include <instance/settings.h>
#include <algorithms/depth_first_search.h>

#define EASY_PUZZLE_PATH "./test/puzzles/easy/"

TEST easy_arc_consistency_backtrack_one(void) {
    ksize_t expected_solution[] = { 9, 7, 5, 7, 6, 8, 9 };

    get_settings_singleton()->filepath           = EASY_PUZZLE_PATH"1.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = false;

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

TEST easy_arc_consistency_backtrack_two(void) {
    ksize_t expected_solution[] = { 9, 6, 7, 3, 2, 1, 5 };

    get_settings_singleton()->filepath           = EASY_PUZZLE_PATH"2.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = false;

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

TEST easy_arc_consistency_backtrack_three(void) {
    ksize_t expected_solution[] = { 8, 5, 1, 4, 2, 3, 9 };

    get_settings_singleton()->filepath           = EASY_PUZZLE_PATH"3.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = false;

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

TEST easy_arc_consistency_backtrack_four(void) {
    ksize_t expected_solution[] = { 1, 4, 2, 3, 9, 1, 2 };

    get_settings_singleton()->filepath           = EASY_PUZZLE_PATH"4.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = false;

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

TEST easy_arc_consistency_backtrack_five(void) {
    ksize_t expected_solution[] = { 1, 8, 6, 9, 4, 5, 8 };

    get_settings_singleton()->filepath           = EASY_PUZZLE_PATH"5.kkr";
    get_settings_singleton()->is_arc_consistency = true;
    get_settings_singleton()->is_backtrack       = true;
    get_settings_singleton()->is_forward_check   = false;

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

SUITE (easy_arc_consistency_backtrack) {
    // 4x4
    RUN_TEST(easy_arc_consistency_backtrack_one);
    RUN_TEST(easy_arc_consistency_backtrack_two);
    RUN_TEST(easy_arc_consistency_backtrack_three);
    RUN_TEST(easy_arc_consistency_backtrack_four);
    RUN_TEST(easy_arc_consistency_backtrack_five);
}
