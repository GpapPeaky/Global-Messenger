#ifndef QUEUE_H_GA
#define QUEUE_H_GA

/**
 * @file queue.h  
 * @brief Dynamic queue implementation in C
 *
 * Provides all the basic operations of a queue , adding new elements (Enqueue)
 * removing elements (Dequeue) , printing the list and displaying certain elements
 * of the list.
 *
 * Author: Giorgos Apostolakis  
 * Date: 2025-10-21
 */

#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/*                              Error Code Macros                             */
/* -------------------------------------------------------------------------- */
#define QUEUE_EMPTY            -9999  /**< Queue is empty. */
#define QUEUE_NULL             -9998  /**< Null pointer passed as Queue. */
#define QUEUE_ALLOCATION_FAILED -9997 /**< Memory allocation failed. */

/**
 * @struct Node
 * @brief Represents a single Node of the Queue
 */
typedef struct Node Node;
/**
 * @struct Queue
 * @brief Represents the Queue
 */
typedef struct Queue Queue;

/**
 * @brief Initializes an empty queue
 * @param[out] queue Pointer to the Queue
 * @return 1 if successful, appropriate MACRO in case of failure
 * @note Caller must call Queue_Free() to avoid memory leaks
 */
int Queue_Init(Queue **queue);

/**
 * @brief Adds an element to the rear of the queue
 * @param[in,out] queue Pointer to the queue
 * @param[in] value Value to add to the queue
 * @return 1 if successful, appropriate MACRO in case of failure
 */
int Queue_Enqueue(Queue **queue, void* value);

/**
 * @brief Removes and returns the front element from the queue
 * @param[in,out] queue Pointer to the queue  
 * @return Front element value if successful, NULL in case of failure
 */
void* Queue_Dequeue(Queue **queue);

/**
 * @brief Returns the front element without removing it
 * @param[in] queue Pointer to the queue
 * @return Front elements value if successful, NULL in case of failure
 */
void* Queue_Peek(const Queue *queue);

/**
 * @brief Returns the number of elements in the queue
 * @param[in] queue Pointer to the queue
 * @return Number of elements if successful, appropriate MACRO in case of failure
 */
int Queue_GetSize(const Queue *queue);

/**
 * @brief Checks if the queue is empty
 * @param[in] queue Pointer to the queue
 * @return true if queue is empty or NULL, false otherwise
 */
bool Queue_IsEmpty(const Queue *queue);

/**
 * @brief Prints the queue contents for debugging
 * @param[in] queue Pointer to the queue
 * @note Format: [ front , ... , rear ]
 */
void Queue_Print(const Queue *queue);

/**
 * @brief Frees all the memory the queue has occupied
 * @param[in,out] queue Pointer to the queue , set to NULL after call
 */
void Queue_Free(Queue **queue);

/**
 * @brief Echoes a message to all clients in the online_clients queue
 * @param[in,out] online_clients Pointer to the queue of online clients
 * @param[in] buffer Message to echo
 */
void Queue_echo(Queue **online_clients, char* buffer);
void Queue_removeClient(Queue *online_clients, int connfd);

#endif