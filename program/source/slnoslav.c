#include <assert.h>
#include <stdio.h>

#include <slnoslav.h>

#include <instance/settings.h>
#include <algorithms/depth_first_search.h>

void run_program(void) {
    FILE * fp = fopen(get_settings_singleton()->filepath, "rb");
    
    assert(fp && "COULDN'T OPEN FILE");
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    if (solution.elements) print_state_array(solution);
    else printf("NO SOLUTION FOUND\n");

    destroy_state_array(&solution);
}
