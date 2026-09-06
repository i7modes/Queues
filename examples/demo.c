/**
 * @file demo.c
 * @brief Demonstration program for Circular Queue library.
 * @author i7modes
 * @license MIT
 */

#include "queue.h"
#include <stdio.h>

static void print_separator(const char *title)
{
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

int main(void)
{
    print_separator("1. Creating Circular Queue (Capacity 5)");
    Queue *q = Queue_Create(5);
    printf("Queue created. Is empty? %s\n", Queue_IsEmpty(q) ? "Yes" : "No");
    Queue_Print(q);

    print_separator("2. Enqueueing Elements");
    printf("Enqueueing 10, 20, 30...\n");
    Queue_Enqueue(q, 10);
    Queue_Enqueue(q, 20);
    Queue_Enqueue(q, 30);
    Queue_Print(q);

    int frontVal;
    if (Queue_Peek(q, &frontVal))
    {
        printf("Front Element: %d\n", frontVal);
    }

    print_separator("3. Circular Wraparound Demonstration");
    printf("Dequeueing 2 elements (10, 20)...\n");
    int dequeued;
    Queue_Dequeue(q, &dequeued);
    printf("Dequeued: %d\n", dequeued);
    Queue_Dequeue(q, &dequeued);
    printf("Dequeued: %d\n", dequeued);
    Queue_Print(q);

    printf("Enqueueing 40, 50, 60, 70 (wrapping around ring buffer)...\n");
    Queue_Enqueue(q, 40);
    Queue_Enqueue(q, 50);
    Queue_Enqueue(q, 60);
    Queue_Enqueue(q, 70);
    Queue_Print(q);
    printf("Is Full? %s (Size: %zu/%zu)\n",
           Queue_IsFull(q) ? "Yes" : "No", Queue_Size(q), Queue_Capacity(q));

    printf("Attempting to enqueue 80 into full queue: %s\n",
           Queue_Enqueue(q, 80) ? "Success" : "Rejected safely!");

    print_separator("4. Sliding Window Moving Average Algorithm");
    int stream[] = {1, 10, 3, 5, 2, 8, 4};
    size_t stream_len = sizeof(stream) / sizeof(stream[0]);
    size_t window = 3;
    double averages[7];

    if (Queue_MovingAverage(stream, stream_len, window, averages))
    {
        printf("Stream:          ");
        for (size_t i = 0; i < stream_len; i++) printf("%4d ", stream[i]);
        printf("\nMoving Avg (w=%zu):", window);
        for (size_t i = 0; i < stream_len; i++) printf("%4.1f ", averages[i]);
        printf("\n");
    }

    print_separator("5. Legacy API Compatibility Check");
    LegacyQueue legacy = CreateQueue(6);
    Enqueue(100, legacy);
    Enqueue(200, legacy);
    printf("Legacy Front(): %d\n", Front(legacy));
    printf("Legacy FrontAndDequeue(): %d\n", FrontAndDequeue(legacy));
    printf("Legacy Front() after Dequeue: %d\n", Front(legacy));
    DisposeQueue(legacy);
    printf("Legacy queue disposed cleanly.\n");

    print_separator("6. Complete Teardown");
    Queue_Destroy(&q);
    printf("Queue pointer: %p (0 memory leaks)\n", (void *)q);

    printf("\nAll Queue demonstrations completed successfully!\n");
    return 0;
}
