#include <assert.h>
#include <stdio.h>

#include <slnoslav.h>

#include <instance/settings.h>
#include <instance/statistics.h>
#include <algorithms/depth_first_search.h>

#define TEST

#ifdef TEST
#include <gui/graphics.h>
#endif

void run_program(void) {

#ifndef TEST
    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    
    assert(fp && "COULDN'T OPEN FILE");
    board_s board = create_board(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    destroy_board(&board);

    if (solution.elements) print_solution(solution);
    else printf("NO SOLUTION FOUND\n");

    print_statistics();

    destroy_state_array(&solution);
#else
    gui();
#endif

}
