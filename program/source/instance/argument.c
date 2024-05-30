#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <regex.h>

#include <instance/argument.h>
#include <instance/settings.h>

#define FINITE_QUEUE
#define QUEUE_DATA_TYPE char *
#include <structures/abstract/queue.h>

#define CONSOLE_ARGUMENT_COUND 6
#define HASHTABLE_SIZE (1 << 3)

#define FILEPATH_FLAG_STRING_SHORT "-fp"
#define FILEPATH_FLAG_STRING_LONG  "--filepath"

#define FILEPATH_FLAG_HASH_SHORT 0x665d
#define FILEPATH_FLAG_HASH_LONG  0x2823

#define BACKTRACK_FLAG_STRING_SHORT "-bt"
#define BACKTRACK_FLAG_STRING_LONG  "--backtrack"

#define BACKTRACK_FLAG_HASH_SHORT 0x6259
#define BACKTRACK_FLAG_HASH_LONG  0x3652

#define FORWARD_CHECK_FLAG_STRING_SHORT "-fch"
#define FORWARD_CHECK_FLAG_STRING_LONG  "--forward-check"

#define FORWARD_CHECK_FLAG_HASH_SHORT 0x0e4e
#define FORWARD_CHECK_FLAG_HASH_LONG  0x6151

#define ARC_CONSISTENCY_FLAG_STRING_SHORT "-ac"
#define ARC_CONSISTENCY_FLAG_STRING_LONG  "--arc-consistency"

#define ARC_CONSISTENCY_FLAG_HASH_SHORT 0x614e
#define ARC_CONSISTENCY_FLAG_HASH_LONG  0x1b28

#define INFO_FLAG_STRING_SHORT "-i"
#define INFO_FLAG_STRING_LONG  "--information"

#define INFO_FLAG_HASH_SHORT 0x692d
#define INFO_FLAG_HASH_LONG  0x5a36

#define HELP_FLAG_STRING_SHORT "-h"
#define HELP_FLAG_STRING_LONG  "--help"

#define HELP_FLAG_HASH_SHORT 0x682d
#define HELP_FLAG_HASH_LONG  0x3829

#define LONGEST_FLAG_S  sizeof(ARC_CONSISTENCY_FLAG_STRING_LONG)

typedef union hash {
    uint16_t id;
    uint8_t  part[sizeof(uint16_t)];
} Hash;

typedef enum console_argument_flags {
    INVALID_F  = 0,

    FILEPATH_F        = 0b00010 | 0b00001,
    BACKTRACK_F       = 0b00100 | 0b00001,
    FORWARD_CHECK_F   = 0b01000 | 0b00001,
    ARC_CONSISTENCY_F = 0b10000 | 0b00001,

    INFORMATION_F = 0b010,
    HELP_F        = 0b100,
} Flag;

typedef struct argument {
    Flag   type;
    char * value;
} Argument;

#define FLAG_INDEX_COUND 2
typedef enum flag_index {
    SHORT = 0,
    LONG = 1,
} FIndex;

typedef struct flag_hashtable_element {
    uint32_t value[FLAG_INDEX_COUND];
    char     string[FLAG_INDEX_COUND][LONGEST_FLAG_S];
    Flag     type;
} FHElement;

typedef struct flag_hashtable {
    FHElement element[HASHTABLE_SIZE];
} FlagHashtable;

typedef struct settings_hashtable_element {
    void (*function)(char *);
    Flag type;
} SHElement;

typedef struct function_hashtable {
    SHElement element[HASHTABLE_SIZE];
} FunctionHashtable;

Hash   _get_hash(char * string);
Flag   _get_flag(char * flag_string);
bool   _is_valid_file(char * filename);

void   _setup_filepath(char * value);
void   _setup_backtrack(char * value);
void   _setup_forward_check(char * value);
void   _setup_arc_consistency(char * value);
void   _setup_information(char * value);
void   _setup_help(char * value);

void   _setup_settings(Argument argument);

void init_program(int argc, char **argv) {
    Queue arg_queue = { .elements = argv, .max = argc, .size = argc, .current = 0 };
    dequeue(&arg_queue);

    while (!is_empty_queue(arg_queue)) {
        Argument argument = { 0 };
        char * flag_string = dequeue(&arg_queue);
        argument.type = _get_flag(flag_string);
        assert(argument.type != INVALID_F && "FLAG IS INVALID");

        if (argument.type & 1) {
            assert(!is_empty_queue(arg_queue) && "EXPECTED VALUE AFTER FLAG");
            argument.value = dequeue(&arg_queue);
        }

        _setup_settings(argument);
    }
}

Hash _get_hash(char * string) {
    assert(string && "STRING PARAMETER IS NULL");
    Hash hash = { 0 };
    for (size_t i = 0; string[i]; i++) hash.part[i & 1] ^= string[i];

    return hash;
}

Flag _get_flag(char * flag_string) {
    assert(flag_string && "FLAG STRING PARAMETER IS NULL");
    assert(flag_string[0] == '-' && "EXPECTED FLAG TO START WITH '-'");

    static FlagHashtable ht = {
        .element[1] = { 
            .value[SHORT]  = BACKTRACK_FLAG_HASH_SHORT,   .value[LONG]   = BACKTRACK_FLAG_HASH_LONG, 
            .string[SHORT] = BACKTRACK_FLAG_STRING_SHORT, .string[LONG]  = BACKTRACK_FLAG_STRING_LONG,
            .type          = BACKTRACK_F,
        },
        .element[3] = { 
            .value[SHORT]  = HELP_FLAG_HASH_SHORT,   .value[LONG]   = HELP_FLAG_HASH_LONG, 
            .string[SHORT] = HELP_FLAG_STRING_SHORT, .string[LONG]  = HELP_FLAG_STRING_LONG,
            .type          = HELP_F,
        },
        .element[4] = { 
            .value[SHORT]  = FILEPATH_FLAG_HASH_SHORT,   .value[LONG]   = FILEPATH_FLAG_HASH_LONG, 
            .string[SHORT] = FILEPATH_FLAG_STRING_SHORT, .string[LONG]  = FILEPATH_FLAG_STRING_LONG,
            .type          = FILEPATH_F,
        },
        .element[5] = { 
            .value[SHORT]  = INFO_FLAG_HASH_SHORT,   .value[LONG]   = INFO_FLAG_HASH_LONG, 
            .string[SHORT] = INFO_FLAG_STRING_SHORT, .string[LONG]  = INFO_FLAG_STRING_LONG,
            .type          = INFORMATION_F,
        },
        .element[6] = { 
            .value[SHORT]  = ARC_CONSISTENCY_FLAG_HASH_SHORT,   .value[LONG]   = ARC_CONSISTENCY_FLAG_HASH_LONG, 
            .string[SHORT] = ARC_CONSISTENCY_FLAG_STRING_SHORT, .string[LONG]  = ARC_CONSISTENCY_FLAG_STRING_LONG,
            .type          = ARC_CONSISTENCY_F,
        },
        .element[7] = { 
            .value[SHORT]  = FORWARD_CHECK_FLAG_HASH_SHORT,   .value[LONG]   = FORWARD_CHECK_FLAG_HASH_LONG, 
            .string[SHORT] = FORWARD_CHECK_FLAG_STRING_SHORT, .string[LONG]  = FORWARD_CHECK_FLAG_STRING_LONG,
            .type          = FORWARD_CHECK_F,
        },
    };

    Hash h = _get_hash(flag_string);
    FIndex index = flag_string[1] == '-' ? LONG : SHORT;
    for (size_t i = 0; i < (HASHTABLE_SIZE + 1) >> 1; i++) {
        size_t ai = (h.id + i)     & (HASHTABLE_SIZE - 1);
        size_t bi = (h.id - i - 1) & (HASHTABLE_SIZE - 1);

        FHElement a = ht.element[ai];
        FHElement b = ht.element[bi];

        if (a.value[index] == h.id && !strncmp(flag_string, ht.element[ai].string[index], LONGEST_FLAG_S)) return a.type;
        if (b.value[index] == h.id && !strncmp(flag_string, ht.element[bi].string[index], LONGEST_FLAG_S)) return b.type;
    }

    return INVALID_F;
}

bool _is_valid_file(char * filename) {
    regex_t rx;
    assert(regcomp( &rx, "(.*)\\.(kkr)", REG_EXTENDED) == 0 && "REGEX PARSER FAILED");
    bool is_valid = regexec(&rx, filename, 0, NULL, 0) == 0;
    regfree(&rx);

    return is_valid;
}

void _setup_filepath(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");
    assert(_is_valid_file(value));
    get_settings_singleton()->filepath = value;
}

void _setup_backtrack(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool t = !strncmp(value, "true", 4);
    bool f = !strncmp(value, "false", 5);
    assert((t || f) && "VALUE IS NEITHER TRUE NOR FALSE");

    get_settings_singleton()->is_backtrack = t;
}

void _setup_forward_check(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool t = !strncmp(value, "true", 4);
    bool f = !strncmp(value, "false", 5);
    assert((t || f) && "VALUE IS NEITHER TRUE NOR FALSE");

    get_settings_singleton()->is_forward_check = t;
}

void _setup_arc_consistency(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool t = !strncmp(value, "true", 4);
    bool f = !strncmp(value, "false", 5);
    assert((t || f) && "VALUE IS NEITHER TRUE NOR FALSE");

    get_settings_singleton()->is_arc_consistency = t;
}

void _setup_information(char * value) {
    printf("SLNOSLAV IS A CONSTRAIN SATISFACTION PROBLEM SOLVER FOR KAKURO PUZZLES\n");
    exit(EXIT_SUCCESS);
}

void _setup_help(char * value) {
    printf("Information helpful for the user:\n");
    printf("\t-h --help                                    show this message\n");
    printf("\t-i --information                             show information about SLNOSLAV\n");

    printf("\nProgram settings:\n");
    printf("\t-fp  <filepath> --filepath        <filepath> filepath to .kkr file           [\"test/data/11x11s.kkr\"]\n");
    printf("\t-bt  true|false --backtrack       true|false enable/disable backtracking     [true]\n");
    printf("\t-fch true|false --forward-check   true|false enable/disable forward checking [false]\n");
    printf("\t-ac  true|false --arc-consistency true|false enable/disable arc-consistency  [true]\n");

    exit(EXIT_SUCCESS);
}

void _setup_settings(Argument argument) {
    static FunctionHashtable ht = {
        .element[0] = { .function = _setup_arc_consistency, .type = ARC_CONSISTENCY_F, },
        .element[2] = { .function = _setup_information,     .type = INFORMATION_F,     },
        .element[1] = { .function = _setup_forward_check,   .type = FORWARD_CHECK_F,   },
        .element[3] = { .function = _setup_filepath,        .type = FILEPATH_F,        },
        .element[4] = { .function = _setup_help,            .type = HELP_F,            },
        .element[5] = { .function = _setup_backtrack,       .type = BACKTRACK_F,       },
    };

    for (size_t i = 0; i < (HASHTABLE_SIZE + 1) >> 1; i++) {
        size_t first_index  = (argument.type + i)     & (HASHTABLE_SIZE - 1);
        size_t second_index = (argument.type - i - 1) & (HASHTABLE_SIZE - 1);

        SHElement first = ht.element[first_index];
        SHElement second = ht.element[second_index];

        if (first.type == argument.type)  { first.function(argument.value); break; }
        if (second.type == argument.type) { second.function(argument.value); break; }
    }
}
