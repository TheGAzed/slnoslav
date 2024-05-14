#include <assert.h>
#include <stdio.h>
#include <regex.h>

#include <slnoslav.h>

#include <algorithms/depth_first_search.h>

bool _is_valid_file(char * filepath);

void run(Settings settings) {
    assert(settings.filepath && "NO FILEPATH FOUND");
    assert(_is_valid_file(settings.filepath));

    FILE * fp = fopen(settings.filepath, "rb");
    assert(fp && "COULDN'T OPEN FILE");
    Kakuro board = init_kakuro(fp);
    fclose(fp);
    
    SArray solution = depth_first_search(board);
    free_kakuro(&board);

    assert(solution.elements && "NO SOLUTION FOUND");
    print_state_array(solution);

    destroy_state_array(&solution);
}

bool _is_valid_file(char * filename) {
    regex_t rx;
    assert(regcomp( &rx, "(.*)\\.(kkr)", REG_EXTENDED) == 0 && "REGEX PARSER FAILED");
    bool is_valid = regexec(&rx, filename, 0, NULL, 0) == 0;
    regfree(&rx);

    return is_valid;
}
