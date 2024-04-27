#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <regex.h>

#include <slnoslav.h>

#include <ds/board.h>
#include <alg/depth_first_search.h>

#define PUZZLE_PATH_LEN 16
#define PUZZLE_PATH "../data/puzzles/"

void _print_state(SArray s, size_t of_length);
bool _is_valid_file(char * filepath);
bool _is_file(char pathname[PUZZLE_PATH_LEN + FILENAME_MAX]);

void run() {
    DIR * puzzles = opendir(PUZZLE_PATH);
    assert(puzzles && "COULDN'T OPEN DIRECTORY");
    struct dirent * d = NULL;

    while ((d = readdir(puzzles))) {
        char filepath[PUZZLE_PATH_LEN + FILENAME_MAX] = PUZZLE_PATH;
        strncat(filepath, d->d_name, FILENAME_MAX);

        if (!_is_file(filepath) && !_is_valid_file(d->d_name)) continue;

        FILE * fp = fopen(filepath, "rb");
        assert(fp && "COULDN'T OPEN FILE");

        Kakuro board = init_kakuro(fp);
        SArray solution = depth_first_search(board);
        assert(solution.elements && "NO SOLUTION FOUND");

        _print_state(solution, board.game.empty_count);

        fclose(fp);
    }
    
    closedir(puzzles);
}

void _print_state(SArray s, size_t of_length) {
    for (size_t i = 0; i < of_length; i++) {
        printf("%d ", get_one_value(s.elements[i]));
    }
}

bool _is_valid_file(char * filename) {
    regex_t rx;
    assert(regcomp( &rx, "(.*)\\.(kkr)", 0) == 0 && "REGEX PARSER FAILED");
    return regexec(&rx, filename, 0, NULL, 0) == 0;
}

bool _is_file(char pathname[PUZZLE_PATH_LEN + FILENAME_MAX]) {
    struct stat buf;
    assert(stat(pathname, &buf) != -1 && "STAT FAILED");
    return S_ISREG(buf.st_mode);
}
