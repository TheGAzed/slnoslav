#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <instance/argument.h>
#include <instance/settings.h>

#define FINITE_QUEUE
#define QUEUE_DATA_TYPE char *
#include <structures/abstract/queue.h>

#define CONSOLE_ARGUMENT_COUND 6
#define HASHTABLE_SIZE (1 << 3)

#define FILEPATH_FLAG_HASH_SHORT 0x665d
#define FILEPATH_FLAG_HASH_LONG  0x2823

#define BACKTRACK_FLAG_HASH_SHORT 0x6259
#define BACKTRACK_FLAG_HASH_LONG  0x3652

#define FORWARD_CHECK_FLAG_HASH_SHORT 0x0e4e
#define FORWARD_CHECK_FLAG_HASH_LONG  0x6151

#define ARC_CONSISTENCY_FLAG_HASH_SHORT 0x614e
#define ARC_CONSISTENCY_FLAG_HASH_LONG  0x1b28

#define INFO_FLAG_HASH_SHORT 0x692d
#define INFO_FLAG_HASH_LONG  0x5a36

#define HELP_FLAG_HASH_SHORT 0x682d
#define HELP_FLAG_HASH_LONG  0x3829

typedef uint16_t full_hash_t;
typedef uint8_t  part_hash_t;
typedef union hash {
    full_hash_t whole;
    part_hash_t slice[sizeof(full_hash_t)];
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

#define FLAG_SIZE_TYPES 2
typedef enum flag_size_types {
    SHORT = 0,
    LONG  = 1,
} FSType;

typedef struct flag_hashtable_element {
    full_hash_t hash_key[FLAG_SIZE_TYPES];
    char        string_key[FLAG_SIZE_TYPES][LONGEST_FLAG_S];
    Flag        value;
} FHElement;

typedef struct flag_hashtable {
    FHElement element[HASHTABLE_SIZE];
} FlagHashtable;

typedef struct settings_hashtable_element {
    void (*value)(char *);
    Flag key;
} SHElement;

typedef struct function_hashtable {
    SHElement element[HASHTABLE_SIZE];
} FunctionHashtable;

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
 * @brief
 * Checks if string is a valid argument and returns enum value of argument.
 * @note
 * Function uses a hashtable to lookup the hashed string value and if matched compare it with the 
 * original string to make sure the string was found.
 * 
 * @param flag_string of argument.
 * @return Flag enum of valid program argument.
 */
Flag   _get_flag(char * flag_string);

/**
 * @brief 
 * Sets the filepath string for settings.
 * 
 * @param value filepath string.
 */
void   _setup_filepath(char * value);
/**
 * @brief 
 * Sets backtrackig to true/false for settings. True by default.
 * 
 * @param value boolean string - "true"/"false".
 */
void   _setup_backtrack(char * value);
/**
 * @brief 
 * Sets forward-checking to true/false for settings. False by default.
 * 
 * @param value boolean string - "true"/"false".
 */
void   _setup_forward_check(char * value);
/**
 * @brief 
 * Sets arc consistency to true/false for settings. True by default.
 * 
 * @param value boolean string - "true"/"false".
 */
void   _setup_arc_consistency(char * value);
/**
 * @brief Prints information about program.
 * 
 * @param value unused.
 */
void   _setup_information(char * value);
/**
 * @brief 
 * Prints helper information about what arguments can be use.
 * 
 * @param value unused.
 */
void   _setup_help(char * value);

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
void   _setup_settings(Argument argument);

void setup_program(int argc, char **argv) {
    // argument queue to torn arguemnt value access like a queue (first-in-first-out).
    Queue arg_queue = { .elements = argv, .max = argc, .size = argc, .current = 0 };
    dequeue(&arg_queue); // remove invocation name from queue 

    while (!is_empty_queue(arg_queue)) {
        Argument argument = { 0 };
        char * flag_string = dequeue(&arg_queue); // dequeue expected flag element
        argument.type = _get_flag(flag_string);
        assert(argument.type != INVALID_F && "FLAG IS INVALID"); // assert that flag is valid 

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
        hash.slice[i & 1] ^= string[i]; 
    }

    return hash;
}

Flag _get_flag(char * flag_string) {
    assert(flag_string && "FLAG STRING PARAMETER IS NULL");
    assert(flag_string[0] == '-' && "EXPECTED FLAG TO START WITH '-'");

    // hashtable to lookup flag using the hashed flag_string plus comparing 
    // it to its long and short form.
    static FlagHashtable ht = {
        .element[1] = { 
            .hash_key[SHORT]   = BACKTRACK_FLAG_HASH_SHORT,   .hash_key[LONG]    = BACKTRACK_FLAG_HASH_LONG, 
            .string_key[SHORT] = BACKTRACK_FLAG_STRING_SHORT, .string_key[LONG]  = BACKTRACK_FLAG_STRING_LONG,
            .value             = BACKTRACK_F,
        },
        .element[3] = { 
            .hash_key[SHORT]   = HELP_FLAG_HASH_SHORT,   .hash_key[LONG]    = HELP_FLAG_HASH_LONG, 
            .string_key[SHORT] = HELP_FLAG_STRING_SHORT, .string_key[LONG]  = HELP_FLAG_STRING_LONG,
            .value             = HELP_F,
        },
        .element[4] = { 
            .hash_key[SHORT]   = FILEPATH_FLAG_HASH_SHORT,   .hash_key[LONG]    = FILEPATH_FLAG_HASH_LONG, 
            .string_key[SHORT] = FILEPATH_FLAG_STRING_SHORT, .string_key[LONG]  = FILEPATH_FLAG_STRING_LONG,
            .value             = FILEPATH_F,
        },
        .element[5] = { 
            .hash_key[SHORT]   = INFO_FLAG_HASH_SHORT,   .hash_key[LONG]    = INFO_FLAG_HASH_LONG, 
            .string_key[SHORT] = INFO_FLAG_STRING_SHORT, .string_key[LONG]  = INFO_FLAG_STRING_LONG,
            .value             = INFORMATION_F,
        },
        .element[6] = { 
            .hash_key[SHORT]   = ARC_CONSISTENCY_FLAG_HASH_SHORT,   .hash_key[LONG]    = ARC_CONSISTENCY_FLAG_HASH_LONG, 
            .string_key[SHORT] = ARC_CONSISTENCY_FLAG_STRING_SHORT, .string_key[LONG]  = ARC_CONSISTENCY_FLAG_STRING_LONG,
            .value             = ARC_CONSISTENCY_F,
        },
        .element[7] = { 
            .hash_key[SHORT]   = FORWARD_CHECK_FLAG_HASH_SHORT,   .hash_key[LONG]    = FORWARD_CHECK_FLAG_HASH_LONG, 
            .string_key[SHORT] = FORWARD_CHECK_FLAG_STRING_SHORT, .string_key[LONG]  = FORWARD_CHECK_FLAG_STRING_LONG,
            .value             = FORWARD_CHECK_F,
        },
    };

    Hash h = _get_hash(flag_string);
    // check if flag string is in short or long form
    FSType length_type = flag_string[1] == '-' ? LONG : SHORT;
    // hashtable elements are accesed by distance from hash's modulo (hashtable element count).
    for (size_t i = 0; i < (HASHTABLE_SIZE + 1) >> 1; i++) {
        size_t ai = (h.whole + i)     & (HASHTABLE_SIZE - 1);
        size_t bi = (h.whole - i - 1) & (HASHTABLE_SIZE - 1);

        FHElement a = ht.element[ai];
        FHElement b = ht.element[bi];

        if (a.hash_key[length_type] == h.whole && !strncmp(flag_string, ht.element[ai].string_key[length_type], LONGEST_FLAG_S)) return a.value;
        if (b.hash_key[length_type] == h.whole && !strncmp(flag_string, ht.element[bi].string_key[length_type], LONGEST_FLAG_S)) return b.value;
    }

    return INVALID_F; // returned if flag wasnt found.
}

void _setup_filepath(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");
    get_settings_singleton()->filepath = value;
}

void _setup_backtrack(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool bool_true= !strncmp(value, "true",  sizeof("true"));
    // check if string value is boolean true or false 
    assert((bool_true|| !strncmp(value, "false", sizeof("false"))) && "BACKTRACK VALUE IS NEITHER TRUE NOR FALSE");

    // if 't' is true then value == "true", else value == "false"
    get_settings_singleton()->is_backtrack = bool_true;
}

void _setup_forward_check(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool bool_true = !strncmp(value, "true",  sizeof("true"));
    // check if string value is boolean true or false 
    assert((bool_true || !strncmp(value, "false", sizeof("false"))) && "FORWARD CHECK VALUE IS NEITHER TRUE NOR FALSE");

    // if 't' is true then value == "true", else value == "false"
    get_settings_singleton()->is_forward_check = bool_true;
}

void _setup_arc_consistency(char * value) {
    assert(value && "ARGUMENT VALUE IS NULL");

    bool bool_true = !strncmp(value, "true",  sizeof("true"));
    // check if string value is boolean true or false 
    assert((bool_true || !strncmp(value, "false", sizeof("false"))) && "ARC CONSISTENCY VALUE IS NEITHER TRUE NOR FALSE");

    // if 'bool_true' is true then value == "true", else value == "false"
    get_settings_singleton()->is_arc_consistency = bool_true;
}

void _setup_information(char * value) {
    printf("SLNOSLAV IS A CONSTRAIN SATISFACTION PROBLEM SOLVER FOR KAKURO PUZZLES\n");
    exit(EXIT_SUCCESS);
}

void _setup_help(char * value) {
    printf("Information helpful for the user:\n");
    printf("\t--help,-h                        show this message\n");
    printf("\t--information,-i                 show information about SLNOSLAV\n");

    printf("\nProgram settings:\n");
    printf("\t--filepath,-fp        <filepath> filepath to .kkr file           [\"program/0.kkr\"]\n");
    printf("\t--backtrack,-bt       true|false enable/disable backtracking     [true]\n");
    printf("\t--forward-check,-fch  true|false enable/disable forward checking [true]\n");
    printf("\t--arc-consistency,-ac true|false enable/disable arc-consistency  [true]\n");

    exit(EXIT_SUCCESS);
}

void _setup_settings(Argument argument) {
    // hashtable to lookup function pointer based on flag enum
    static FunctionHashtable ht = {
        .element[0] = { .value = _setup_arc_consistency, .key = ARC_CONSISTENCY_F, },
        .element[2] = { .value = _setup_information,     .key = INFORMATION_F,     },
        .element[1] = { .value = _setup_forward_check,   .key = FORWARD_CHECK_F,   },
        .element[3] = { .value = _setup_filepath,        .key = FILEPATH_F,        },
        .element[4] = { .value = _setup_help,            .key = HELP_F,            },
        .element[5] = { .value = _setup_backtrack,       .key = BACKTRACK_F,       },
    };

    // hashtable elements are accesed by distance from hash's modulo (hashtable element count).
    for (size_t i = 0; i < (HASHTABLE_SIZE + 1) >> 1; i++) {
        size_t first_index  = (argument.type + i)     & (HASHTABLE_SIZE - 1);
        size_t second_index = (argument.type - i - 1) & (HASHTABLE_SIZE - 1);

        SHElement first  = ht.element[first_index];
        SHElement second = ht.element[second_index];

        if (first.key  == argument.type) { first.value(argument.value);  break; }
        if (second.key == argument.type) { second.value(argument.value); break; }
    }
}
