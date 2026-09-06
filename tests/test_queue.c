/**
 * @file test_queue.c
 * @brief Automated unit test suite for Circular Queue library.
 * @author i7modes
 * @license MIT
 */

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

static int total_tests = 0;
static int passed_tests = 0;

#define TEST_ASSERT(expr, msg) do { \
    total_tests++; \
    if (expr) { \
        passed_tests++; \
        printf("  [PASS] %s\n", msg); \
    } else { \
        printf("  [FAIL] %s (Line %d)\n", msg, __LINE__); \
    } \
} while (0)

static void test_initialization_and_empty_safety(void)
{
    printf("\n--- Test: Initialization & Empty Safety ---\n");
    Queue *q = Queue_Create(5);
    TEST_ASSERT(q != NULL, "Queue_Create returns non-NULL");
    TEST_ASSERT(Queue_IsEmpty(q) == true, "New queue is empty");
    TEST_ASSERT(Queue_IsFull(q) == false, "New queue is not full");
    TEST_ASSERT(Queue_Size(q) == 0, "New queue size is 0");
    TEST_ASSERT(Queue_Capacity(q) == 5, "Queue capacity is 5");

    int val;
    TEST_ASSERT(Queue_Dequeue(q, &val) == false, "Dequeue on empty queue returns false safely");
    TEST_ASSERT(Queue_Peek(q, &val) == false, "Peek on empty queue returns false safely");

    Queue_Destroy(&q);
    TEST_ASSERT(q == NULL, "Queue_Destroy nullifies pointer");

    // Invalid creation
    TEST_ASSERT(Queue_Create(0) == NULL, "Queue_Create(0) returns NULL safely");
}

static void test_enqueue_dequeue_fifo_order(void)
{
    printf("\n--- Test: Enqueue, Dequeue & FIFO Ordering ---\n");
    Queue *q = Queue_Create(3);

    TEST_ASSERT(Queue_Enqueue(q, 10) == true, "Enqueue 10");
    TEST_ASSERT(Queue_Enqueue(q, 20) == true, "Enqueue 20");
    TEST_ASSERT(Queue_Enqueue(q, 30) == true, "Enqueue 30");

    TEST_ASSERT(Queue_Size(q) == 3, "Size is 3");
    TEST_ASSERT(Queue_IsFull(q) == true, "Queue is full");
    TEST_ASSERT(Queue_Enqueue(q, 40) == false, "Enqueue on full queue returns false safely");

    int peekVal;
    TEST_ASSERT(Queue_Peek(q, &peekVal) && peekVal == 10, "Peek returns 10 (front)");
    TEST_ASSERT(Queue_Size(q) == 3, "Peek did not alter size");

    int buffer[3];
    size_t copied = Queue_ToArray(q, buffer, 3);
    TEST_ASSERT(copied == 3, "Copied 3 items to array");
    TEST_ASSERT(buffer[0] == 10 && buffer[1] == 20 && buffer[2] == 30,
                "Array matches FIFO order (10, 20, 30)");

    int out;
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 10, "Dequeued 10");
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 20, "Dequeued 20");
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 30, "Dequeued 30");

    TEST_ASSERT(Queue_IsEmpty(q) == true, "Queue is empty after 3 dequeues");
    TEST_ASSERT(Queue_Size(q) == 0, "Queue size is 0");

    Queue_Destroy(&q);
}

static void test_circular_wraparound(void)
{
    printf("\n--- Test: Circular Ring Buffer Wraparound ---\n");
    Queue *q = Queue_Create(4);

    /* Enqueue 3 items */
    Queue_Enqueue(q, 1);
    Queue_Enqueue(q, 2);
    Queue_Enqueue(q, 3);

    /* Dequeue 2 items */
    int out;
    Queue_Dequeue(q, &out); /* 1 */
    Queue_Dequeue(q, &out); /* 2 */

    /* Now enqueue 3 more items: this will wrap around the end of the array */
    TEST_ASSERT(Queue_Enqueue(q, 4) == true, "Enqueue 4 (wrapped)");
    TEST_ASSERT(Queue_Enqueue(q, 5) == true, "Enqueue 5 (wrapped)");
    TEST_ASSERT(Queue_Enqueue(q, 6) == true, "Enqueue 6 (wrapped)");
    TEST_ASSERT(Queue_IsFull(q) == true, "Queue is full across boundary");

    /* Verify FIFO order across the wrap */
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 3, "Dequeued 3");
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 4, "Dequeued 4");
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 5, "Dequeued 5");
    TEST_ASSERT(Queue_Dequeue(q, &out) && out == 6, "Dequeued 6");
    TEST_ASSERT(Queue_IsEmpty(q) == true, "Queue is empty after wrapped reads");

    /* High-volume cycle test: 100 continuous iterations through capacity-3 queue */
    bool continuous_ok = true;
    for (int i = 0; i < 100; i++)
    {
        Queue_Enqueue(q, i);
        int deq;
        if (!Queue_Dequeue(q, &deq) || deq != i)
        {
            continuous_ok = false;
            break;
        }
    }
    TEST_ASSERT(continuous_ok == true, "100 continuous wrap-around cycles verified without corruption");

    Queue_Destroy(&q);
}

static void test_moving_average(void)
{
    printf("\n--- Test: Moving Average Algorithm ---\n");
    int stream[] = {1, 10, 3, 5};
    double results[4];

    /* Window size 3:
     * i=0: [1]          avg = 1.0
     * i=1: [1, 10]      avg = 5.5
     * i=2: [1, 10, 3]   avg = 14/3 = 4.666...
     * i=3: [10, 3, 5]   avg = 18/3 = 6.0
     */
    bool ok = Queue_MovingAverage(stream, 4, 3, results);
    TEST_ASSERT(ok == true, "Moving average executed successfully");
    TEST_ASSERT(fabs(results[0] - 1.0) < 0.001, "Step 0 avg is 1.0");
    TEST_ASSERT(fabs(results[1] - 5.5) < 0.001, "Step 1 avg is 5.5");
    TEST_ASSERT(fabs(results[2] - 4.6666) < 0.01, "Step 2 avg is ~4.67");
    TEST_ASSERT(fabs(results[3] - 6.0) < 0.001, "Step 3 avg is 6.0");
}

int main(void)
{
    printf("========================================\n");
    printf("  Circular Queue Automated Tests\n");
    printf("========================================\n");

    test_initialization_and_empty_safety();
    test_enqueue_dequeue_fifo_order();
    test_circular_wraparound();
    test_moving_average();

    printf("\n========================================\n");
    printf("  Results: %d/%d assertions passed\n", passed_tests, total_tests);
    printf("========================================\n");

    return (passed_tests == total_tests) ? 0 : 1;
}
