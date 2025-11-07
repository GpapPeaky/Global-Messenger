#include "../headers/queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <errno.h>

/**
 * @file queue.c
 * @brief Implementation of queue.h functions
 */
struct Queue
{
    int size;
    Node *front;
    Node *rear;
};
struct Node
{
    void* value;
    Node *next;
};

int Queue_Init(Queue **queue)
{
    if (!queue)
    {
        fprintf(stderr, "Queue_Init: Double pointer cannot be NULL\n");
        return QUEUE_NULL;
    }

    (*queue) = (Queue *)malloc(sizeof(Queue));
    if (!(*queue))
    {
        fprintf(stderr, "Queue_Init: Memory allocation failed\n");
        return QUEUE_ALLOCATION_FAILED;
    }

    (*queue)->front = NULL;
    (*queue)->rear = NULL;
    (*queue)->size = 0;

    return 1;
}

int Queue_Enqueue(Queue **queue, void* value)
{
    Node *newNode;
    if (!queue)
    {
        fprintf(stderr, "Queue_Enqueue: Double pointer cannot be NULL\n");
        return QUEUE_NULL;
    }
    if (!(*queue))
    {
        fprintf(stderr, "Queue_Enqueue: Queue cannot be NULL\n");
        return QUEUE_NULL;
    }

    newNode = (Node *)malloc(sizeof(Node));
    if (!newNode)
    {
        fprintf(stderr, "Queue_Enqueue: Memory allocation failed\n");
        return QUEUE_ALLOCATION_FAILED;
    }

    newNode->next = NULL;
    newNode->value = value;

    if ((*queue)->size == 0)
    {
        (*queue)->front = newNode;
        (*queue)->rear = newNode;
        (*queue)->size = 1;
        return 1;
    }
    (*queue)->rear->next = newNode;
    (*queue)->rear = newNode;
    (*queue)->size++;
    return 1;
}

void* Queue_Dequeue(Queue **queue)
{
    Node *toFree;
    void* returnValue;
    if (!queue)
    {
        fprintf(stderr, "Queue_Dequeue: Double pointer cannot be NULL\n");
        return NULL;
    }
    if (!(*queue))
    {
        fprintf(stderr, "Queue_Dequeue: Queue cannot be NULL\n");
        return NULL;
    }
    if ((*queue)->size == 0)
    {
        fprintf(stderr, "Queue_Dequeue: Queue cannot be empty\n");
        return NULL;
    }

    returnValue = (*queue)->front->value;
    toFree=(*queue)->front;
    (*queue)->front = (*queue)->front->next;
    free(toFree);
    (*queue)->size--;
    if((*queue)->size == 0){
        (*queue)->rear = NULL;
    }
    return returnValue;
}

void* Queue_Peek(const Queue *queue){
    if (!queue)
    {
        fprintf(stderr, "Queue_Peek: Queue cannot be NULL\n");
        return NULL;
    }
    if (queue->size == 0)
    {
        fprintf(stderr, "Queue_Peek: Queue cannot be empty\n");
        return NULL;
    }
    return queue->front->value;
}

int Queue_GetSize(const Queue *queue){
    if (!queue)
    {
        fprintf(stderr, "Queue_GetSize: Queue cannot be NULL\n");
        return QUEUE_NULL;
    }
    return queue->size;
}

bool Queue_IsEmpty(const Queue *queue){
    if (!queue)
    {
        fprintf(stderr, "Queue_IsEmpty: Queue cannot be NULL\n");
        return true;
    }
    return queue->size == 0;
}

/*void Queue_Print(const Queue *queue){
    Node* tmp;
    if (!queue)
    {
        fprintf(stderr, "Queue_Print: Queue cannot be NULL\n");
        return;
    }
    printf("Queue (size=%d) [", queue->size);
    tmp=queue->front;
    while(tmp){
        printf(" %d ",tmp->value);
        if(tmp!=queue->rear)
            printf(",");
        tmp=tmp->next;
    }
    printf("]\n");
}*/

void Queue_Free(Queue **queue){
    Node*tmp,*toFree;
    if(!queue)
        return;
    if(!*queue)
        return;
    tmp=(*queue)->front;
    while(tmp){
        toFree=tmp;
        tmp=tmp->next;
        free(toFree);
    }
    free(*queue);
    *queue=NULL;
}

void Queue_echo(Queue **online_clients, char* buffer)
{
    if (!online_clients || !(*online_clients))
    {
        fprintf(stderr, "Queue_echo: online_clients queue cannot be NULL\n");
        return;
    }

    Node *current = (*online_clients)->front;
    while (current)
    {
        int client_sock = *((int *)current->value);
        write(client_sock, buffer, strlen(buffer));
        current = current->next;
    }
}

void Queue_removeClient(Queue *online_clients, int connfd)
{
   
    Node *current = online_clients->front;
    Node *prev = NULL;
    
    while (current != NULL)
    {
        int *client_fd = (int *)current->value;
        if (*client_fd == connfd)
        {
            // Remove this node
            if (prev == NULL)
            {
                online_clients->front = current->next;
            }
            else
            {
                prev->next = current->next;
            }
            
            if (current == online_clients->rear)
            {
                online_clients->rear = prev;
            }
            
            free(client_fd);
            free(current);
            online_clients->size--;
            break;
        }
        prev = current;
        current = current->next;
    }
}
