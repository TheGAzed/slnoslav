#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <structures/argument.h>

Settings init_settings(int argc, char **argv) {
    Settings s = { .is_default = true, 0 };
    AQueue q = create_argument_queue(argc, argv);
    dequeue_argument(&q);

    while (!is_empty_argument_queue(q)) {
        Argument a = { 0 };
        char * flag_string = dequeue_argument(&q);
        a.type = _get_flag(flag_string);
        assert(a.type != INVALID_F && "FLAG IS INVALID");
        s.is_default = false;

        if (a.type & 1) {
            assert(!is_empty_argument_queue(q) && "EXPECTED VALUE AFTER FLAG");
            a.value = dequeue_argument(&q);
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

AQueue create_argument_queue(int argc, char **argv) {
    return (AQueue) { .current = 0, .elements = argv, .max = argc, .size = argc, };
}

void destroy_argument_queue (AQueue * queue) {
    return;
}

bool is_empty_argument_queue(AQueue queue) {
    return queue.size == 0;
}

bool is_full_argument_queue(AQueue queue) {
    return queue.size == queue.max;
}

void enqueue_argument(AQueue * queue, char * element) {
    assert(queue && "QUEUE POINTER IS NULL");
    assert(!is_full_argument_queue(*queue) && "CAN'T PUSH FULL QUEUE");

    size_t idx = _get_index_argument_queue(*queue, AQI_POSITION_NEXT);

    queue->elements[idx] = element;
    queue->size++;
}

char * dequeue_argument(AQueue * queue) {
    assert(queue && "QUEUE POINTER IS NULL");
    assert(!is_empty_argument_queue(*queue) && "CAN'T POP EMPTY QUEUE");

    char * e = peek_argument_queue(*queue);
    if (queue->size == 0) queue->elements = NULL;
    queue->current = (queue->current + 1) % queue->max;
    
    queue->size--;
    return e;
}

char * peek_argument_queue(AQueue queue) {
    assert(!is_empty_argument_queue(queue) && "CAN'T PEEK EMPTY QUEUE");
    size_t idx = _get_index_argument_queue(queue, AQI_POSITION_CURRENT);

    return queue.elements[idx];
}

size_t _get_index_argument_queue(AQueue queue, AQIPosition type) {
    assert((!is_empty_argument_queue(queue) || type != AQI_POSITION_CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY QUEUE");
    if (type == AQI_POSITION_CURRENT) return queue.current;
    return (queue.current + queue.size) % queue.max;
}
