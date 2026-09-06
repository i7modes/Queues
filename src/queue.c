/**
 * @file queue.c
 * @brief Implementation of Circular Array Queue (Ring Buffer).
 * @author i7modes
 * @license MIT
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*                         Lifecycle & Memory Management                      */
/* -------------------------------------------------------------------------- */

Queue* Queue_Create(size_t capacity)
{
    if (capacity == 0)
    {
        return NULL;
    }

    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (q == NULL)
    {
        fprintf(stderr, "Error: Out of memory in Queue_Create (handle).\n");
        return NULL;
    }

    q->data = (int *)malloc(sizeof(int) * capacity);
    if (q->data == NULL)
    {
        fprintf(stderr, "Error: Out of memory in Queue_Create (buffer).\n");
        free(q);
        return NULL;
    }

    q->capacity = capacity;
    q->front = 0;
    q->rear = 0;
    q->size = 0;

    return q;
}

void Queue_Clear(Queue *q)
{
    if (q == NULL)
    {
        return;
    }

    q->front = 0;
    q->rear = 0;
    q->size = 0;
}

void Queue_Destroy(Queue **q_ptr)
{
    if (q_ptr == NULL || *q_ptr == NULL)
    {
        return;
    }

    Queue *q = *q_ptr;
    if (q->data != NULL)
    {
        free(q->data);
    }

    free(q);
    *q_ptr = NULL;
}

/* -------------------------------------------------------------------------- */
/*                               Core Operations                              */
/* -------------------------------------------------------------------------- */

bool Queue_IsEmpty(const Queue *q)
{
    return (q == NULL || q->size == 0);
}

bool Queue_IsFull(const Queue *q)
{
    return (q != NULL && q->size == q->capacity);
}

size_t Queue_Size(const Queue *q)
{
    return (q != NULL) ? q->size : 0;
}

size_t Queue_Capacity(const Queue *q)
{
    return (q != NULL) ? q->capacity : 0;
}

bool Queue_Enqueue(Queue *q, int value)
{
    if (q == NULL || Queue_IsFull(q))
    {
        return false;
    }

    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % q->capacity;
    q->size++;
    return true;
}

bool Queue_Dequeue(Queue *q, int *out_value)
{
    if (q == NULL || Queue_IsEmpty(q))
    {
        return false;
    }

    if (out_value != NULL)
    {
        *out_value = q->data[q->front];
    }

    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return true;
}

bool Queue_Peek(const Queue *q, int *out_value)
{
    if (q == NULL || Queue_IsEmpty(q) || out_value == NULL)
    {
        return false;
    }

    *out_value = q->data[q->front];
    return true;
}

/* -------------------------------------------------------------------------- */
/*                           Algorithms & Utilities                           */
/* -------------------------------------------------------------------------- */

bool Queue_MovingAverage(const int *stream, size_t n, size_t window_size, double *results)
{
    if (stream == NULL || results == NULL || n == 0 || window_size == 0)
    {
        return false;
    }

    Queue *q = Queue_Create(window_size);
    if (q == NULL)
    {
        return false;
    }

    double running_sum = 0.0;

    for (size_t i = 0; i < n; i++)
    {
        if (Queue_IsFull(q))
        {
            int evicted;
            Queue_Dequeue(q, &evicted);
            running_sum -= (double)evicted;
        }

        Queue_Enqueue(q, stream[i]);
        running_sum += (double)stream[i];

        results[i] = running_sum / (double)Queue_Size(q);
    }

    Queue_Destroy(&q);
    return true;
}

size_t Queue_ToArray(const Queue *q, int *buffer, size_t max_len)
{
    if (q == NULL || buffer == NULL || max_len == 0)
    {
        return 0;
    }

    size_t count = (q->size < max_len) ? q->size : max_len;

    for (size_t i = 0; i < count; i++)
    {
        size_t idx = (q->front + i) % q->capacity;
        buffer[i] = q->data[idx];
    }

    return count;
}

void Queue_Print(const Queue *q)
{
    if (q == NULL)
    {
        printf("(null)\n");
        return;
    }

    if (Queue_IsEmpty(q))
    {
        printf("[Empty Queue] (0/%zu capacity)\n", q->capacity);
        return;
    }

    for (size_t i = 0; i < q->size; i++)
    {
        size_t idx = (q->front + i) % q->capacity;
        if (i == 0)
        {
            printf("[Front: %d]", q->data[idx]);
        }
        else if (i == q->size - 1)
        {
            printf(" -> [Rear: %d]", q->data[idx]);
        }
        else
        {
            printf(" -> [%d]", q->data[idx]);
        }
    }

    printf(" (Size: %zu/%zu)\n", q->size, q->capacity);
}
