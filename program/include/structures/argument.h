#ifndef DATA_STRUCTURES_ARGUMENT_H
#define DATA_STRUCTURES_ARGUMENT_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define CONSOLE_ARGUMENT_COUNT 1

#define FILEPATH_FLAG_STRING_SHORT "-fp"
#define FILEPATH_FLAG_STRING_LONG  "--filepath"

#define FILEPATH_FLAG_HASH_SHORT 0x0070662d
#define FILEPATH_FLAG_HASH_LONG  0x61706874

#define LONGEST_FLAG_S  sizeof(FILEPATH_FLAG_STRING_LONG)

typedef struct settings {
    bool   is_default;
    char * filepath;
} Settings;

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

Settings init_settings(int argc, char **argv);

Hash   _get_hash(char * string);
Flag   _get_flag(char * flag_string);
void   _setup_filepath(Settings * settings, char * value);
void   _setup_settings(Settings * settings, Argument argument);

AQueue create_argument_queue    (int argc,        char **argv);
void   destroy_argument_queue   (AQueue * queue);
bool   is_empty_argument_queue  (AQueue   queue);
bool   is_full_argument_queue(AQueue queue);
void   enqueue_argument         (AQueue * queue,  char * element);
char * dequeue_argument         (AQueue * queue);
char * peek_argument_queue      (AQueue   queue);
size_t _get_index_argument_queue(AQueue queue,    AQIPosition type);

#endif /* DATA_STRUCTURES_ARGUMENT_H */
