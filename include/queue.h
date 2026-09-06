/**
 * @file queue.h
 * @brief Circular Array Queue (Ring Buffer) Implementation in C.
 * @author i7modes
 * @license MIT
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIN_QUEUE_CAPACITY 2

/**
 * @brief Circular Queue (Ring Buffer) structure.
 */
typedef struct Queue {
    int *data;              /**< Dynamically allocated array buffer */
    size_t capacity;        /**< Maximum number of elements queue can hold */
    size_t front;           /**< Index of the front element */
    size_t rear;            /**< Index where next element will be enqueued */
    size_t size;            /**< Current element count */
} Queue;

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

/**
 * @brief Allocates and initializes a circular queue with a specified capacity.
 * @param capacity Maximum number of items queue can store (must be >= 1).
 * @return Pointer to new Queue, or NULL on failure.
 */
Queue* Queue_Create(size_t capacity);

/**
 * @brief Clears all elements from the queue without freeing the internal buffer.
 * @param q Pointer to the queue.
 */
void Queue_Clear(Queue *q);

/**
 * @brief Deallocates the queue and its internal array, setting *q_ptr to NULL.
 * @param q_ptr Pointer to the Queue pointer variable.
 */
void Queue_Destroy(Queue **q_ptr);

/* -------------------------------------------------------------------------- */
/*                               Core Operations                              */
/* -------------------------------------------------------------------------- */

/**
 * @brief Checks if the queue contains zero elements.
 * @param q Pointer to the queue.
 * @return true if empty or q is NULL, false otherwise.
 */
bool Queue_IsEmpty(const Queue *q);

/**
 * @brief Checks if the queue has reached maximum capacity.
 * @param q Pointer to the queue.
 * @return true if full, false otherwise.
 */
bool Queue_IsFull(const Queue *q);

/**
 * @brief Returns the current number of elements in the queue.
 * @param q Pointer to the queue.
 * @return Number of elements.
 */
size_t Queue_Size(const Queue *q);

/**
 * @brief Returns the total capacity of the queue.
 * @param q Pointer to the queue.
 * @return Total capacity.
 */
size_t Queue_Capacity(const Queue *q);

/**
 * @brief Enqueues a new element at the rear of the ring buffer in O(1) time.
 * @param q Pointer to the queue.
 * @param value Element to insert.
 * @return true on success, false if queue is full or invalid.
 */
bool Queue_Enqueue(Queue *q, int value);

/**
 * @brief Dequeues and removes the front element in O(1) time.
 * @param q Pointer to the queue.
 * @param out_value Optional pointer to receive the removed value.
 * @return true on success, false if queue is empty or invalid.
 */
bool Queue_Dequeue(Queue *q, int *out_value);

/**
 * @brief Peeks at the front element without removing it in O(1) time.
 * @param q Pointer to the queue.
 * @param out_value Pointer to receive front value (must not be NULL).
 * @return true if found, false if queue is empty.
 */
bool Queue_Peek(const Queue *q, int *out_value);

/* -------------------------------------------------------------------------- */
/*                           Algorithms & Utilities                           */
/* -------------------------------------------------------------------------- */

/**
 * @brief Calculates the moving average of a data stream using a sliding window queue.
 * @param stream Input array of integers.
 * @param n Length of stream.
 * @param window_size Size of the sliding window.
 * @param results Output buffer of size n to receive moving average at each step.
 * @return true on success, false on invalid arguments.
 */
bool Queue_MovingAverage(const int *stream, size_t n, size_t window_size, double *results);

/**
 * @brief Copies elements in FIFO order into a provided buffer.
 * @return Number of elements copied.
 */
size_t Queue_ToArray(const Queue *q, int *buffer, size_t max_len);

/**
 * @brief Prints the circular queue visually in terminal:
 *        [Front: 10] -> [20] -> [30] -> [Rear: 40] (Size: 4/8)
 * @param q Pointer to the queue.
 */
void Queue_Print(const Queue *q);

/* -------------------------------------------------------------------------- */
/*                       Legacy API Compatibility Layer                       */
/* -------------------------------------------------------------------------- */

#define MinQueueSize 5

typedef struct QueueRecord {
    int Capacity;
    int Front;
    int Rear;
    int Size;
    int *Array;
} *LegacyQueue;

static inline int isEmpty(LegacyQueue Q) {
    return (Q == NULL || Q->Size == 0) ? 1 : 0;
}

static inline int isFull(LegacyQueue Q) {
    return (Q != NULL && Q->Size == Q->Capacity) ? 1 : 0;
}

static inline int Succ(int Value, LegacyQueue Q) {
    if (++Value == Q->Capacity) Value = 0;
    return Value;
}

static inline void MakeEmpty(LegacyQueue Q) {
    if (Q == NULL) return;
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

static inline LegacyQueue CreateQueue(int Capacity) {
    if (Capacity < MinQueueSize) {
        printf("Queue size is too small\n");
        return NULL;
    }
    LegacyQueue Q = (LegacyQueue)malloc(sizeof(struct QueueRecord));
    if (Q == NULL) return NULL;
    Q->Array = (int *)malloc(sizeof(int) * Capacity);
    if (Q->Array == NULL) {
        free(Q);
        return NULL;
    }
    Q->Capacity = Capacity;
    MakeEmpty(Q);
    return Q;
}

static inline void DisposeQueue(LegacyQueue Q) {
    if (Q != NULL) {
        free(Q->Array);
        free(Q);
    }
}

static inline void Enqueue(int X, LegacyQueue Q) {
    if (Q == NULL || isFull(Q)) {
        printf("Full Queue!\n");
        return;
    }
    Q->Size++;
    Q->Rear = Succ(Q->Rear, Q);
    Q->Array[Q->Rear] = X;
}

static inline int Front(LegacyQueue Q) {
    if (Q == NULL || isEmpty(Q)) {
        printf("Empty Queue!\n");
        return 0;
    }
    return Q->Array[Q->Front];
}

static inline void Dequeue(LegacyQueue Q) {
    if (Q == NULL || isEmpty(Q)) {
        printf("Empty Queue!\n");
        return;
    }
    Q->Size--;
    Q->Front = Succ(Q->Front, Q);
}

static inline int FrontAndDequeue(LegacyQueue Q) {
    if (Q == NULL || isEmpty(Q)) {
        printf("Empty Queue!\n");
        return 0;
    }
    Q->Size--;
    int X = Q->Array[Q->Front];
    Q->Front = Succ(Q->Front, Q);
    return X;
}

#ifdef __cplusplus
}
#endif

#endif /* QUEUE_H */
