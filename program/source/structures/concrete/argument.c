#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <structures/concrete/argument.h>

#define CONSOLE_ARGUMENT_COUNT 1

#define FILEPATH_FLAG_STRING_SHORT "-fp"
#define FILEPATH_FLAG_STRING_LONG  "--filepath"

#define FILEPATH_FLAG_HASH_SHORT 0x0070662d
#define FILEPATH_FLAG_HASH_LONG  0x61706874

#define LONGEST_FLAG_S  sizeof(FILEPATH_FLAG_STRING_LONG)

typedef union hash {
    uint32_t id;
    uint8_t  part[sizeof(uint32_t)];
} Hash;

typedef enum console_argument_flags {
    INVALID_F  = 0,
    FILEPATH_F = 0b0010 | 0b0001,
} Flag;

typedef struct argument {
    Flag   type;
    char * value;
} Argument;

typedef enum flag_index {
    SHORT = 0,
    LONG = 1,
} FIndex;

typedef struct flag_hashtable_element {
    uint32_t value[2];
    char     string[2][LONGEST_FLAG_S];
    Flag     type;
} FHElement;

typedef struct flag_hashtable {
    FHElement element[CONSOLE_ARGUMENT_COUNT];
} FlagHashtable;

typedef struct settings_hashtable_element {
    void (*function)(Settings *, char *);
    Flag type;
} SHElement;

typedef struct function_hashtable {
    SHElement element[CONSOLE_ARGUMENT_COUNT];
} FunctionHashtable;

#define QUEUE_LIST_ARRAY_SIZE (1 << 8)

typedef struct queue_list_array {
    char **                   elements[QUEUE_LIST_ARRAY_SIZE];
    struct queue_list_array * next;
} QLArray;

typedef struct argument_queue {
    size_t  size;
    size_t  current;
    size_t  max;
    char ** elements;
} AQueue;

typedef enum argument_queue_index_position {
    AQI_POSITION_CURRENT = 1,
    AQI_POSITION_NEXT    = 0,
} AQIPosition;

#define FINITE_QUEUE
#define QUEUE_DATA_TYPE char *
#include <structures/abstract/queue.h>

Hash   _get_hash(char * string);
Flag   _get_flag(char * flag_string);
void   _setup_filepath(Settings * settings, char * value);
void   _setup_settings(Settings * settings, Argument argument);

Settings init_settings(int argc, char **argv) {
    Settings s = { .is_default = true, 0 };
    Queue q = { .elements = argv, .max = argc, .size = argc, .current = 0 };
    dequeue(&q);

    while (!is_empty_queue(q)) {
        Argument a = { 0 };
        char * flag_string = dequeue(&q);
        a.type = _get_flag(flag_string);
        assert(a.type != INVALID_F && "FLAG IS INVALID");
        s.is_default = false;

        if (a.type & 1) {
            assert(!is_empty_queue(q) && "EXPECTED VALUE AFTER FLAG");
            a.value = dequeue(&q);
        }

        _setup_settings(&s, a);
    }

    return s;
}

Hash _get_hash(char * string) {
    assert(string && "STRING PARAMETER IS NULL");
    Hash h = { 0 };
    for (size_t i = 0; string[i]; i++) h.part[i & 3] = string[i];

    return h;
}

Flag _get_flag(char * flag_string) {
    assert(flag_string && "FLAG STRING PARAMETER IS NULL");
    assert(flag_string[0] == '-' && "EXPECTED FLAG TO START WITH '-'");

    static FlagHashtable ht = {
        .element[0] = { 
            .value[SHORT]  = FILEPATH_FLAG_HASH_SHORT,   .value[LONG]   = FILEPATH_FLAG_HASH_LONG, 
            .string[SHORT] = FILEPATH_FLAG_STRING_SHORT, .string[LONG]  = FILEPATH_FLAG_STRING_LONG,
            .type = FILEPATH_F, 
        }
    };

    Hash h = _get_hash(flag_string);
    FIndex index = flag_string[1] == '-' ? LONG : SHORT;
    for (size_t i = 0; i < (CONSOLE_ARGUMENT_COUNT + 1) >> 1; i++) {
        size_t ai = (h.id + i) % CONSOLE_ARGUMENT_COUNT;
        size_t bi = ((h.id -i - 1) % CONSOLE_ARGUMENT_COUNT) + CONSOLE_ARGUMENT_COUNT;
        FHElement a = ht.element[ai];
        FHElement b = ht.element[bi];

        if (a.value[index] == h.id && !strncmp(flag_string, a.string[index], LONGEST_FLAG_S)) return a.type;
        if (b.value[index] == h.id && !strncmp(flag_string, b.string[index], LONGEST_FLAG_S)) return b.type;
    }

    return INVALID_F;
}

void _setup_filepath(Settings * settings, char * value) {
    assert(settings && "SETTINGS IS NULL");
    assert(value && "ARGUMENT VALUE IS NULL");

    settings->filepath = value;
}

void _setup_settings(Settings * settings, Argument argument) {
    static FunctionHashtable ht = {
        .element[0] = { .function = _setup_filepath, .type = FILEPATH_F, }
    };

    for (size_t i = 0; i < (CONSOLE_ARGUMENT_COUNT + 1) >> 1; i++) {
        size_t ai = (argument.type + i) % CONSOLE_ARGUMENT_COUNT;
        size_t bi = ((argument.type -i - 1) % CONSOLE_ARGUMENT_COUNT) + CONSOLE_ARGUMENT_COUNT;

        SHElement a = ht.element[ai];
        SHElement b = ht.element[bi];

        if (a.type == argument.type) { a.function(settings, argument.value); break; }
        if (b.type == argument.type) { b.function(settings, argument.value); break; }
    }
}
