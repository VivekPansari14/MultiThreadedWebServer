This code is a basic implementation of a multi-threaded web server in C. The server listens for incoming client connections on a specified port (in this case, port 8080), and for each connection, it creates a new thread to handle the client request. Below is a detailed explanation of the code:

### 1. **Header Files**

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
```

- **`stdio.h`:** Provides input and output functions like `printf` and `perror`.
- **`stdlib.h`:** Provides memory allocation, process control, and other utility functions like `malloc` and `exit`.
- **`string.h`:** Contains functions for manipulating strings and memory blocks, like `memset`.
- **`unistd.h`:** Provides access to POSIX operating system API, including functions like `read`, `write`, and `close`.
- **`arpa/inet.h`:** Provides definitions for internet operations, including functions to work with IP addresses and sockets.
- **`pthread.h`:** Provides functions to work with POSIX threads, allowing for multi-threaded programming.

### 2. **Function to Handle Client Requests**

```c
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
```

- **`handleRequest`:** This function handles each client connection.
- **`int socket = *((int *)client_socket);`:** The function receives a pointer to the client socket descriptor, which is dereferenced to get the actual socket value.
- **`free(client_socket);`:** Frees the memory allocated for the client socket descriptor.
- **`char buffer[1024];`:** Declares a buffer to hold the client's request data.
- **`memset(buffer, 0, sizeof(buffer));`:** Clears the buffer to ensure no residual data is present.
- **`read(socket, buffer, sizeof(buffer));`:** Reads the client's request data from the socket into the buffer.
- **`printf("Request: %s\n", buffer);`:** Prints the client's request to the console for logging/debugging purposes.
- **`char response[] = "...";`:** Defines a simple HTTP response with a "Hello World!" message.
- **`write(socket, response, sizeof(response) - 1);`:** Sends the HTTP response to the client.
- **`close(socket);`:** Closes the client socket after the response is sent.

### 3. **Main Function**

```c
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
```

- **`int main()`:** The main function where the server is set up and run.
- **`int server_socket, *client_socket;`:** Declare variables for the server socket and a pointer to the client socket.
- **`struct sockaddr_in server_addr, client_addr;`:** Declare structures for the server and client addresses.
- **`socklen_t addr_size;`:** Declare a variable for the size of the client address structure.

#### Creating the Server Socket

```c
server_socket = socket(AF_INET, SOCK_STREAM, 0);
if (server_socket < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
}
```
- **`socket(AF_INET, SOCK_STREAM, 0);`:** Creates a TCP socket.
- **`if (server_socket < 0) {...}`:** Checks if the socket creation failed and prints an error message if so.

#### Configuring the Server Address

```c
server_addr.sin_family = AF_INET;
server_addr.sin_port = htons(8080);  // Server port
server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
```
- **`server_addr.sin_family = AF_INET;`:** Specifies that the socket will use IPv4.
- **`server_addr.sin_port = htons(8080);`:** Sets the port number to 8080. `htons` converts the port number to network byte order.
- **`server_addr.sin_addr.s_addr = INADDR_ANY;`:** Allows the server to accept connections from any IP address.

#### Binding the Socket

```c
if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Bind failed");
    close(server_socket);
    exit(EXIT_FAILURE);
}
```
- **`bind(...)`:** Binds the server socket to the specified IP address and port.
- **`if (bind(...) < 0) {...}`:** Checks if binding failed and prints an error message if so.

#### Listening for Connections

```c
if (listen(server_socket, 10) < 0) {
    perror("Listen failed");
    close(server_socket);
    exit(EXIT_FAILURE);
}
```
- **`listen(server_socket, 10);`:** Puts the server socket into a passive listening state, waiting for incoming connections. The `10` specifies the backlog (the maximum number of connections that can be queued).

#### Main Loop to Accept and Handle Connections

```c
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
```
- **`while (1) {...}`:** The server runs indefinitely, accepting and handling connections in an infinite loop.
- **`client_socket = malloc(sizeof(int));`:** Allocates memory for a new client socket descriptor.
- **`*client_socket = accept(...);`:** Accepts a new client connection and assigns it to `client_socket`.
- **`pthread_create(&thread, NULL, handleRequest, client_socket);`:** Creates a new thread to handle the client request using the `handleRequest` function.
- **`pthread_detach(thread);`:** Detaches the thread so that its resources are automatically freed after it finishes.

#### Closing the Server Socket (Unreachable Code)

```c
close(server_socket);
```
- **`close(server_socket);`:** Closes the server socket. This line is technically unreachable in the current implementation because the server runs indefinitely, but it's good practice to include it for completeness.

### Summary

- The code sets up a simple multi-threaded web server that listens on port 8080.
- For each incoming client connection, a new thread is created to handle the request.
- The server responds to any client request with a basic "Hello World!" HTTP response.
- This setup allows the server to handle multiple clients concurrently, making it scalable for basic use cases.

This is a foundational project that can be extended to build more complex web servers with additional features like handling different HTTP methods, serving static files, or even building a custom routing system.
