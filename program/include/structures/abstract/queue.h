#ifndef STRUCTURES_ABSTRACT_QUEUE_H
#define STRUCTURES_ABSTRACT_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

//#define FINITE_QUEUE
//#undef FINITE_QUEUE

#ifndef QUEUE_DATA_TYPE 

#define QUEUE_DATA_TYPE void *

#endif /* QUEUE_DATA_TYPE */

#ifndef FINITE_QUEUE

#define QUEUE_LIST_ARRAY_SIZE (1 << 8)

typedef struct queue_list_array {
    QUEUE_DATA_TYPE           elements[QUEUE_LIST_ARRAY_SIZE];
    struct queue_list_array * next;
} QLArray;

#endif /* FINITE_QUEUE */

typedef struct queue {
    size_t size, current;

#ifdef FINITE_QUEUE
    size_t max;
    QUEUE_DATA_TYPE * elements;
#else
    QLArray * head, tail;
#endif /* FINITE_QUEUE */

} Queue;

typedef enum queue_index_position {
    QI_POSITION_CURRENT = 1,
    QI_POSITION_NEXT    = 0,
} QIPosition;

#ifdef FINITE_QUEUE

static Queue create_queue(size_t max);
static bool  is_full_queue(Queue queue);

#else

static Queue create_queue(void);

#endif /* FINITE_QUEUE */

static bool            is_empty_queue(Queue queue);
static void            enqueue(Queue * queue, QUEUE_DATA_TYPE element);
static QUEUE_DATA_TYPE dequeue(Queue * queue);
static QUEUE_DATA_TYPE peek_queue(Queue queue);
static Queue           copy_queue(Queue queue, QUEUE_DATA_TYPE (*copy_element)(QUEUE_DATA_TYPE));
static void            destroy_queue(Queue * queue, void (*free_element)(QUEUE_DATA_TYPE *));
static size_t          _get_index_queue(Queue queue, QIPosition type);

#ifdef FINITE_QUEUE

static inline Queue create_queue(size_t max) {
    Queue queue = { .elements = malloc(sizeof(QUEUE_DATA_TYPE) * max), .max = max, .size = 0, .current = 0 };
    assert(queue.elements && "MEMORY ALLOCATION FAILED");
    return queue;
}

static inline bool is_full_queue(Queue queue) {
    return queue.size == queue.max;
}

#else

static inline Queue create_queue(void)  {
    return (Queue) { 0 };
}

#endif /* FINITE_QUEUE */

static inline bool is_empty_queue(Queue queue) {
    return !queue.size;
}

static inline void enqueue(Queue * queue, QUEUE_DATA_TYPE element) {
    assert(queue && "QUEUE POINTER IS NULL");

#ifdef FINITE_QUEUE
    assert(!is_full_queue(*queue) && "CAN'T PUSH FULL QUEUE");
#endif /* FINITE_QUEUE */

    size_t idx = _get_index_queue(*queue, QI_POSITION_NEXT);

#ifdef FINITE_QUEUE
    queue->elements[idx] = element;
#else
    if (!idx) {
        QLArray * temp = malloc(sizeof(QLArray));
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->next = NULL;

        if (!queue->size) queue->head = queue->tail = temp;
        else queue->tail = queue->tail->next = temp;
    }

    queue->tail->elements[idx] = element;
#endif /* FINITE_QUEUE */
    queue->size++;
}

static inline QUEUE_DATA_TYPE dequeue(Queue * queue) {
    assert(queue && "QUEUE POINTER IS NULL");
    assert(!is_empty_queue(*queue) && "CAN'T POP EMPTY QUEUE");

    QUEUE_DATA_TYPE e = peek_queue(*queue);

#ifdef FINITE_QUEUE
    queue->current = (queue->current + 1) % queue->max;
#else
    size_t idx = _get_index_queue(*queue, QI_POSITION_NEXT);

    if (idx == (QUEUE_LIST_ARRAY_SIZE - 1)) {
        QLArray * temp = queue->head;

        queue->head = queue->head->next;
        free(temp);
    }

    queue->current = (queue->current + 1) % QUEUE_LIST_ARRAY_SIZE;
#endif /* FINITE_QUEUE */

    queue->size--;

#ifdef FINITE_QUEUE
    if (!queue->size) queue->elements = NULL;
#else
    if (!queue->size) queue->tail = NULL;
#endif /* FINITE_QUEUE */

    return e;
}

static inline QUEUE_DATA_TYPE peek_queue(Queue queue) {
    assert(!is_empty_queue(queue) && "CAN'T PEEK EMPTY QUEUE");
    size_t idx = _get_index_queue(queue, QI_POSITION_CURRENT);

#ifdef FINITE_QUEUE
    return queue.elements[idx];
#else
    return queue.head->elements[idx];
#endif /* FINITE_QUEUE */

}

static inline Queue copy_queue(Queue queue, QUEUE_DATA_TYPE (*copy_element)(QUEUE_DATA_TYPE)) {
#ifdef FINITE_QUEUE
    Queue copy = queue;
    assert((copy.elements = malloc(sizeof(QUEUE_DATA_TYPE) * copy.max)) && "MEMORY ALLOCATION FAILED");

    size_t current_index = _get_index_queue(queue, QI_POSITION_CURRENT);
    for (size_t i = 0; i < copy.size; i++) {
        size_t index = (current_index + i) % copy.max;
        copy.elements[index] = copy_element ? copy_element(queue.elements[index]) : queue.elements[index];
    }
#else
    Queue copy = create_queue();
    copy.current = queue.current;

    QLArray * current_array = queue.head;
    for (size_t i = _get_index_queue(queue, QI_POSITION_CURRENT); i < queue.size && current_array;) {
        QLArray * temp = malloc(sizeof(QLArray));
        assert(temp && "MEMORY ALLOCATION FAILED");
        temp->next = NULL;

        if (!copy.size) copy.head = copy.tail = temp;
        else copy.tail = copy.tail->next = temp;

        for (size_t j = i % QUEUE_LIST_ARRAY_SIZE; j < QUEUE_LIST_ARRAY_SIZE; i++, j++) {
            copy.tail->elements[j] = copy_element ? copy_element(current_array->elements[j]) : current_array->elements[j];
            copy.size++;
        }
        current_array = current_array->next;
    }

#endif /* FINITE_QUEUE */

    return copy;
}

static inline void destroy_queue(Queue * queue, void (*free_element)(QUEUE_DATA_TYPE *)) {
    while (!is_empty_queue(*queue)) {
        QUEUE_DATA_TYPE e = dequeue(queue);
        if (free_element) free_element(&e);
    }

#ifdef FINITE_QUEUE
    free(queue->elements);
    queue->max = 0;
#endif /* FINITE_QUEUE */

    queue->current = 0;
}

static inline size_t _get_index_queue(Queue queue, QIPosition type) {
    assert((!is_empty_queue(queue) || type != QI_POSITION_CURRENT) && "CAN'T GET CURRENT INDEX IF EMPTY QUEUE");
    if (type == QI_POSITION_CURRENT) return queue.current;

#ifdef FINITE_QUEUE
    return (queue.current + queue.size) % queue.max;
#else
    return (queue.current + queue.size) % QUEUE_LIST_ARRAY_SIZE;
#endif /* FINITE_QUEUE */

}

#endif /* STRUCTURES_ABSTRACT_QUEUE_H */
