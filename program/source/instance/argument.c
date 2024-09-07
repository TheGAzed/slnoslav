#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <instance/argument.h>
#include <instance/settings.h>

#define FINITE_QUEUE
#define QUEUE_DATA_TYPE char *
#include <structures/abstract/queue.h>

#define CONSOLE_ARGUMENT_COUND 7
#define HASHTABLE_SIZE (1 << 3)

#define FILEPATH_FLAG_HASH_SHORT 0x3b
#define FILEPATH_FLAG_HASH_LONG  0x0b

#define BACKTRACK_FLAG_HASH_SHORT 0x3b
#define BACKTRACK_FLAG_HASH_LONG  0x64

#define FORWARD_CHECK_FLAG_HASH_SHORT 0x40
#define FORWARD_CHECK_FLAG_HASH_LONG  0x30

#define ARC_CONSISTENCY_FLAG_HASH_SHORT 0x2f
#define ARC_CONSISTENCY_FLAG_HASH_LONG  0x33

#define REDUCE_FLAG_HASH_SHORT 0x5f
#define REDUCE_FLAG_HASH_LONG  0x00

#define INFO_FLAG_HASH_SHORT 0x44
#define INFO_FLAG_HASH_LONG  0x6c

#define HELP_FLAG_HASH_SHORT 0x45
#define HELP_FLAG_HASH_LONG  0x11

typedef uint8_t full_hash_t;
typedef uint8_t part_hash_t;
typedef union hash {
    full_hash_t whole;
    part_hash_t slice[sizeof(full_hash_t)];
} Hash;

typedef enum console_argument_flags {
    FILEPATH_E        = (0 << 1) | 0b1,
    BACKTRACK_E       = (1 << 1),
    FORWARD_CHECK_E   = (2 << 1),
    ARC_CONSISTENCY_E = (3 << 1),
    REDUCE_E          = (4 << 1),

    INFORMATION_E = (5 << 1) | 0b0,
    HELP_E        = (6 << 1) | 0b0,

    INVALID_E     = -1,
} flag_e;

typedef struct argument {
    flag_e   type;
    char * value;
} argument_s;

#define FLAG_SIZE_TYPES 2
typedef enum flag_size {
    SHORT = 0,
    LONG  = 1,
} flas_size_e;

typedef struct flag_element {
    full_hash_t hash_key[FLAG_SIZE_TYPES];
    char        string_key[FLAG_SIZE_TYPES][LONGEST_FLAG_S];
    flag_e        value;
} flag_element_s;

typedef struct flag_hashtable {
    flag_element_s element[HASHTABLE_SIZE];
} flag_hashtable_s;

typedef void (*setup_fn)(char *);
typedef struct function_set {
    setup_fn element[HASHTABLE_SIZE];
} FunctionSet;

/**
 * @brief
 * Generates Hash union with hashed numerical value based on string input.
 * 
 * @note
 * Funtion uses a zeroed Hash union with one 16 bit integer and array of two 8 bit integers.
 * The string is iterated using a for loop where the for loop index 'i' is moduled 
 * by the array size (in this case 2) and the character value is XOR-ed into the Hash's 8 bit 
 * array at 'i' modulo (array size).
 * 
 * @param string to hash.
 * @return Hash union with numerical hash value.
 */
Hash   _get_hash(char * string);
/**
 * 
 * @return Initialized string-argument-to-flag hashtable
 */
flag_hashtable_s create_flag_table(void);
/**
 * @brief
 * Checks if string is a valid argument and returns enum value of argument.
 * @note
 * Function uses a hashtable to lookup the hashed string value and if matched compare it with the 
 * original string to make sure the string was found.
 * 
 * @param flag_string of argument.
 * @return Flag enum of valid program argument.
 */
flag_e   _get_flag(char * flag_string, flag_hashtable_s table);
/**
 * @brief Prints information about program.
 * 
 * @param value unused.
 */
void   _print_information(void);
/**
 * @brief 
 * Prints helper information about what arguments can be use.
 * 
 * @param value unused.
 */
void   _print_help(void);

/**
 * @brief 
 * Main setup function that sets the setting based on specified argument.
 * 
 * @note
 * Function uses a hashtable to lookup the pointer to function that is needed 
 * for program settings.
 * 
 * @param argument structure containing argument flag enum and string value.
 */
void   _setup_settings(argument_s argument);

void setup_program(int argc, char **argv) {
    // argument queue to torn arguemnt value access like a queue (first-in-first-out).
    queue_s arg_queue = { .elements = argv, .max = argc, .size = argc, .current = 0 };
    dequeue(&arg_queue); // remove invocation name from queue

    flag_hashtable_s table = create_flag_table();

    while (!is_empty_queue(arg_queue)) {
        argument_s argument = { 0 };
        char * flag_string = dequeue(&arg_queue); // dequeue expected flag element
        argument.type = _get_flag(flag_string, table);
        assert(argument.type != INVALID_E && "FLAG IS INVALID"); // assert that flag is valid

        if (argument.type & 1) { // flag enums with followup values are odd (rightmost bit is always one)
            assert(!is_empty_queue(arg_queue) && "EXPECTED VALUE AFTER FLAG");
            argument.value = dequeue(&arg_queue); // sets the value for argument
        }

        // call to settings setup to set program settings
        _setup_settings(argument); 
    }
}

Hash _get_hash(char * string) {
    assert(string && "STRING PARAMETER IS NULL");
    Hash hash = { 0 };
    for (size_t i = 0; string[i]; i++) {
        // XOR of char at i position with hash byte at i mod (hash byte size).
        hash.slice[i & (sizeof(full_hash_t) - 1)] ^= string[i]; 
    }

    return hash;
}

flag_hashtable_s create_flag_table(void) {
    // hashtable to lookup flag using the hashed flag_string plus comparing 
    // it to its long and short form.
    return (flag_hashtable_s) {
        .element[0] = { 
            .hash_key[SHORT]   = FORWARD_CHECK_FLAG_HASH_SHORT,   .hash_key[LONG]    = FORWARD_CHECK_FLAG_HASH_LONG, 
            .string_key[SHORT] = FORWARD_CHECK_FLAG_STRING_SHORT, .string_key[LONG]  = FORWARD_CHECK_FLAG_STRING_LONG,
            .value             = FORWARD_CHECK_E,
        },
        .element[2] = { 
            .hash_key[SHORT]   = FILEPATH_FLAG_HASH_SHORT,   .hash_key[LONG]    = FILEPATH_FLAG_HASH_LONG, 
            .string_key[SHORT] = FILEPATH_FLAG_STRING_SHORT, .string_key[LONG]  = FILEPATH_FLAG_STRING_LONG,
            .value             = FILEPATH_E,
        },
        .element[3] = { 
            .hash_key[SHORT]   = BACKTRACK_FLAG_HASH_SHORT,   .hash_key[LONG]    = BACKTRACK_FLAG_HASH_LONG, 
            .string_key[SHORT] = BACKTRACK_FLAG_STRING_SHORT, .string_key[LONG]  = BACKTRACK_FLAG_STRING_LONG,
            .value             = BACKTRACK_E,
        },
       .element[4] = { 
            .hash_key[SHORT]   = INFO_FLAG_HASH_SHORT,   .hash_key[LONG]    = INFO_FLAG_HASH_LONG, 
            .string_key[SHORT] = INFO_FLAG_STRING_SHORT, .string_key[LONG]  = INFO_FLAG_STRING_LONG,
            .value             = INFORMATION_E,
        },
        .element[5] = { 
            .hash_key[SHORT]   = ARC_CONSISTENCY_FLAG_HASH_SHORT,   .hash_key[LONG]    = ARC_CONSISTENCY_FLAG_HASH_LONG, 
            .string_key[SHORT] = ARC_CONSISTENCY_FLAG_STRING_SHORT, .string_key[LONG]  = ARC_CONSISTENCY_FLAG_STRING_LONG,
            .value             = ARC_CONSISTENCY_E,
        },
        .element[6] = { 
            .hash_key[SHORT]   = HELP_FLAG_HASH_SHORT,   .hash_key[LONG]    = HELP_FLAG_HASH_LONG, 
            .string_key[SHORT] = HELP_FLAG_STRING_SHORT, .string_key[LONG]  = HELP_FLAG_STRING_LONG,
            .value             = HELP_E,
        },
       .element[7] = { 
            .hash_key[SHORT]   = REDUCE_FLAG_HASH_SHORT,   .hash_key[LONG]    = REDUCE_FLAG_HASH_LONG, 
            .string_key[SHORT] = REDUCE_FLAG_STRING_SHORT, .string_key[LONG]  = REDUCE_FLAG_STRING_LONG,
            .value             = REDUCE_E,
        },
    };
}

flag_e _get_flag(char * flag_string, flag_hashtable_s table) {
    assert(flag_string && "FLAG STRING PARAMETER IS NULL");
    assert(flag_string[0] == '-' && "EXPECTED FLAG TO START WITH '-'");

    Hash h = _get_hash(flag_string);
    // check if flag string is in short or long form
    flas_size_e length_type = flag_string[1] == '-' ? LONG : SHORT;
    // hashtable elements are accesed by distance from hash's modulo (hashtable element count).
    for (size_t i = 0; i < (HASHTABLE_SIZE + 1) >> 1; i++) {
        size_t ai = (h.whole + i)     & (HASHTABLE_SIZE - 1);
        size_t bi = (h.whole - i - 1) & (HASHTABLE_SIZE - 1);

        flag_element_s a = table.element[ai];
        flag_element_s b = table.element[bi];

        if (a.hash_key[length_type] == h.whole && !strncmp(flag_string, a.string_key[length_type], LONGEST_FLAG_S)) {
            return a.value;
        }
        if (b.hash_key[length_type] == h.whole && !strncmp(flag_string, b.string_key[length_type], LONGEST_FLAG_S)) {
            return b.value;
        }
    }

    return INVALID_E; // returned if flag wasnt found.
}

void _print_information(void) {
    printf("SLNOSLAV IS A CONSTRAIN SATISFACTION PROBLEM SOLVER FOR KAKURO PUZZLES\n");
    exit(EXIT_SUCCESS);
}

void _print_help(void) {
    puts("Information helpful for the user:");
    puts("\t--help,-h             show this message");
    puts("\t--information,-i      show information about SLNOSLAV");

    puts("\nProgram settings:");
    puts("\t--filepath,-fp        <filepath> filepath to .kkr file [\"puzzles/0.kkr\"]");
    puts("\t--backtrack,-bt       enable backtracking              [disabled]");
    puts("\t--forward-check,-fch  enable forward checking          [disabled]");
    puts("\t--arc-consistency,-ac enable arc-consistency           [disabled]");
    puts("\t--reduce,-r           enable reduced states            [disabled]");

    exit(EXIT_SUCCESS);
}

void _setup_settings(argument_s argument) {
    switch (argument.type) {
        case FILEPATH_E        : { get_settings_singleton()->filepath = argument.value; break; }
        case BACKTRACK_E       : { get_settings_singleton()->is_backtrack = true;       break; }
        case FORWARD_CHECK_E   : { get_settings_singleton()->is_forward_check = true;   break; }
        case ARC_CONSISTENCY_E : { get_settings_singleton()->is_arc_consistency = true; break; }
        case REDUCE_E          : { get_settings_singleton()->is_reduce = true;          break; }
        case INFORMATION_E     : { _print_information();                                break; }
        case HELP_E            : { _print_help();                                       break; }
        default                : { assert(false && "Unexpected argument type");         break; }
    }
}
