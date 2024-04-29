#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include <ds/argument.h>

#ifndef EXPOSE_PRIVATE_FUNCTIONS

typedef struct argument_queue {
    int count;
    int current;
    char   **values;
} AQueue;

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

Hash   _get_hash(char * string);
Flag   _get_flag(char * flag_string);
void   _setup_filepath(Settings * settings, char * value);
void   _setup_settings(Settings * settings, Argument argument);
AQueue _init_aqueue(size_t argc, char **argv);
bool   _is_empty_aqueue(AQueue queue);
char * _dequeue_aqueue(AQueue * queue);

#endif /* EXPOSE_PRIVATE_FUNCTIONS */

Settings init_settings(int argc, char **argv) {
    Settings s = { .is_default = true, 0 };
    AQueue q = _init_aqueue(argc, argv);

    while (!_is_empty_aqueue(q)) {
        Argument a = { 0 };
        char * flag_string = _dequeue_aqueue(&q);
        a.type = _get_flag(flag_string);
        assert(a.type != INVALID_F && "FLAG IS INVALID");
        s.is_default = false;

        if (a.type & 1) {
            assert(!_is_empty_aqueue(q) && "EXPECTED VALUE AFTER FLAG");
            a.value = _dequeue_aqueue(&q);
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

AQueue _init_aqueue(size_t argc, char **argv) {
    AQueue q = { .count = argc, .current = 0, .values = argv, };
    return q;
}

bool _is_empty_aqueue(AQueue queue) {
    return queue.current >= queue.count;
}

char * _dequeue_aqueue(AQueue * queue) {
    assert(_is_empty_aqueue(*queue) && "CAN'T DEQUEUE FROM EMPTY QUEUE");
    return queue->values[queue->count++];
}
