#include <stdio.h>
#include <stdlib.h>
#define MinQueueSize 5

typedef struct QueueRecord *Queue;

struct QueueRecord
{

    int Capacity;
    int Front;
    int Rear;
    int Size;
    int *Array;
};

int isEmpty(Queue);
int isFull(Queue);
Queue CreateQueue(int);
void MakeEmpty(Queue);
void DisposeQueue(Queue);
int Succ(int, Queue);
void Enqueue(int, Queue);
int Front(Queue);
void Dequeue(Queue);
int FrontAndDequeue(Queue);

int main()
{

    Queue myQueue = CreateQueue(12);

    printf("is Empty: %d\n", isEmpty(myQueue));
    Enqueue(1, myQueue);
    printf("%d\n", FrontAndDequeue(myQueue));
    Enqueue(2, myQueue);
    printf("%d\n", Front(myQueue));
    Dequeue(myQueue);
    Enqueue(3, myQueue);
    printf("%d\n", Front(myQueue));
    printf("is Full: %d\n", isFull(myQueue));
    MakeEmpty(myQueue);
    printf("is Empty: %d\n", isEmpty(myQueue));

    DisposeQueue(myQueue);

    printf("\nDone!\n");
    return 0;
}

int isEmpty(Queue Q)
{
    return Q->Size == 0;
}

int isFull(Queue Q)
{
    return Q->Size == Q->Capacity;
}

Queue CreateQueue(int Capacity)
{
    Queue Q;

    if (Capacity < MinQueueSize)
    {
        printf("Queue size is too small\n");
        exit(1);
    }
    else
    {

        Q = (Queue)malloc(sizeof(struct QueueRecord));

        if (Q == NULL)
        {
            printf("Out of memory!\n");
            exit(1);
        }

        Q->Array = (int *)malloc(sizeof(int) * Capacity);

        if (Q->Array == NULL)
        {
            printf("Out of memory!\n");
            exit(1);
        }

        Q->Capacity = Capacity;
        MakeEmpty(Q);

        return Q;
    }
}

void MakeEmpty(Queue Q)
{
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

void DisposeQueue(Queue Q)
{

    if (Q != NULL)
    {
        free(Q->Array);
        free(Q);
    }
}

int Succ(int Value, Queue Q)
{

    if (++Value == Q->Capacity)
        Value = 0;

    return Value;
}

void Enqueue(int X, Queue Q)
{

    if (isFull(Q))
    {
        printf("Full Queue!\n");
    }
    else
    {
        Q->Size++;
        Q->Rear = Succ(Q->Rear, Q);
        Q->Array[Q->Rear] = X;
    }
}

int Front(Queue Q)
{

    if (isEmpty(Q))
    {
        printf("Empty Queue!\n");
    }
    else
    {
        return Q->Array[Q->Front];
    }
}

void Dequeue(Queue Q)
{

    if (isEmpty(Q))
    {
        printf("Empty Queue!\n");
    }
    else
    {
        Q->Size--;
        Q->Front = Succ(Q->Front, Q);
    }
}

int FrontAndDequeue(Queue Q)
{
    int X = 0;
    if (isEmpty(Q))
    {
        printf("Empty Queue!\n");
    }
    else
    {
        Q->Size--;
        X = Q->Array[Q->Front];
        Q->Front = Succ(Q->Front, Q);
    }

    return X;
}
