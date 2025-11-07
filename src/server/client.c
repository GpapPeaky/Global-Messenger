#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <semaphore.h>
#include "headers/queue.h"
#include <arpa/inet.h>
#include <errno.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int sockfd;

void *receive_messages(void *arg)
{
    char buffer[MAX];
    int n;
    while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[n] = '\0';
        printf("\n%s", buffer);
        fflush(stdout);
    }
    printf("\nDisconnected from server.\n");
    exit(0);
    return NULL;
}

int main()
{
    struct sockaddr_in servaddr;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        fprintf(stderr, "client: Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("client: Socket successfully created.\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Connect to localhost

    // Connect to server
    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
    {
        fprintf(stderr, "client: Connection with server failed\n");
        exit(EXIT_FAILURE);
    }
    printf("client: Connected to server.\n");

    // Create thread to receive messages
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_detach(recv_thread);

    // Send messages to server
    char buffer[MAX];
    while (1)
    {
        printf("");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        if (strncmp(buffer, "/quit", 5) == 0)
            break;

        write(sockfd, buffer, strlen(buffer));
    }

    close(sockfd);
    printf("client: Disconnected.\n");
    return 0;
}