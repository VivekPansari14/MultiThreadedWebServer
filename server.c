#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

// Function to handle each client connection
void *handleRequest(void *client_socket) {
    int socket = *((int *)client_socket);
    free(client_socket);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Read the client's request
    read(socket, buffer, sizeof(buffer));
    printf("Request: %s\n", buffer);

    // Simple HTTP response
    char response[] = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
    write(socket, response, sizeof(response) - 1);

    // Close the client socket
    close(socket);
    return NULL;
}

int main() {
    int server_socket, *client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    // Create a TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);  // Server port
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Start listening for connections
    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port 8080...\n");

    while (1) {
        addr_size = sizeof(client_addr);
        client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size)) < 0) {
            perror("Accept failed");
            free(client_socket);
            continue;
        }

        // Create a new thread to handle the client request
        pthread_t thread;
        if (pthread_create(&thread, NULL, handleRequest, client_socket) != 0) {
            perror("Thread creation failed");
            close(*client_socket);
            free(client_socket);
        }

        // Detach the thread so that resources are freed after it finishes
        pthread_detach(thread);
    }

    // Close the server socket (unreachable code, but good practice)
    close(server_socket);
    return 0;
}
