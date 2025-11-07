#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <semaphore.h>
#include "headers/queue.h"
#include <arpa/inet.h>
#include <errno.h>

#define MAX_MSG_LEN 256
#define MAX_MSG_NUM 100
#define PORT 8080

Queue *chat_history;
Queue *online_clients;

pthread_mutex_t history_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void remove_client(int connfd)
{
    pthread_mutex_lock(&clients_mutex);

    Queue_removeClient(online_clients, connfd);

    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *socket_desc)
{
    int connfd = *(int *)socket_desc;
    char buffer[MAX_MSG_LEN];
    int n;

    // Add client to online_clients
    pthread_mutex_lock(&clients_mutex);
    int *client_fd = malloc(sizeof(int));
    *client_fd = connfd;
    Queue_Enqueue(&online_clients, client_fd);
    pthread_mutex_unlock(&clients_mutex);

    // Send chat history to new client
    pthread_mutex_lock(&history_mutex);

    Queue_echo(&chat_history, buffer);

    pthread_mutex_unlock(&history_mutex);

    while (1)
    {
        bzero(buffer, MAX_MSG_LEN);
        n = read(connfd, buffer, sizeof(buffer) - 1); // -1 for null terminator

        if (n <= 0)
        {
            if (n == 0)
            {
                printf("Client disconnected (socket %d).\n", connfd);
            }
            else
            {
                perror("read error");
            }
            break;
        }

        buffer[n] = '\0'; // Ensure null termination

        // Store in history
        pthread_mutex_lock(&history_mutex);
        Queue_Enqueue(&chat_history, (void *)strdup(buffer));
        if (Queue_GetSize(chat_history) > MAX_MSG_NUM)
        {
            char *old_msg = (char *)Queue_Dequeue(&chat_history);
            free(old_msg);
        }
        pthread_mutex_unlock(&history_mutex);

        // Broadcast to all clients
        pthread_mutex_lock(&clients_mutex);
        Queue_echo(&online_clients, buffer);
        pthread_mutex_unlock(&clients_mutex);
    }

    // Remove client from online_clients
    remove_client(connfd);

    close(connfd);
    free(socket_desc);
    return NULL;
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    Queue_Init(&chat_history);
    Queue_Init(&online_clients);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        fprintf(stderr, "main: Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("main: Socket successfully created.\n");

    // Allow address reuse
    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt failed");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        fprintf(stderr, "main: Socket bind failed\n");
        exit(EXIT_FAILURE);
    }
    printf("main: Socket successfully binded.\n");

    if (listen(sockfd, 10) != 0)
    {
        fprintf(stderr, "main: Listen failed\n");
        exit(EXIT_FAILURE);
    }
    printf("main: Server listening on port %d...\n", PORT);

    while (1)
    {
        len = sizeof(cli);
        connfd = accept(sockfd, (struct sockaddr *)&cli, &len);
        if (connfd < 0)
        {
            perror("accept failed");
            continue;
        }
        printf("Client connected from %s:%d (socket %d)\n",
               inet_ntoa(cli.sin_addr), ntohs(cli.sin_port), connfd);

        int *new_sock = malloc(sizeof(int));
        *new_sock = connfd;

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, new_sock);
        pthread_detach(tid);
    }

    close(sockfd);
    return 0;
}