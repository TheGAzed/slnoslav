#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <regex.h>

#include <slnoslav.h>

#include <alg/depth_first_search.h>

#ifndef EXPOSE_PRIVATE_FUNCTIONS

void _print_state(SArray s);
bool _is_valid_file(char * filepath);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

void run(Settings settings) {
    assert(settings.filepath && "NO FILEPATH FOUND");
    assert(_is_valid_file(settings.filepath));

    FILE * fp = fopen(settings.filepath, "rb");
    assert(fp && "COULDN'T OPEN FILE");

    SArray solution = depth_first_search(init_kakuro(fp));
    assert(solution.elements && "NO SOLUTION FOUND");
    _print_state(solution);

    free_state(&solution);
}

void _print_state(SArray s) {
    for (size_t i = 0; i < s.count; i++) {
        printf("%d ", get_one_value(s.elements[i]));
    }
}

bool _is_valid_file(char * filename) {
    regex_t rx;
    assert(regcomp( &rx, "(.*)\\.(kkr)", 0) == 0 && "REGEX PARSER FAILED");
    return regexec(&rx, filename, 0, NULL, 0) == 0;
}
