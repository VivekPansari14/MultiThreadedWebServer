# Multi-Threaded Web Server

## Overview

This project is a basic multi-threaded web server implemented in C. The server listens for incoming TCP connections on port 8080, handles client requests concurrently using threads, and responds with a simple "Hello World!" message. This implementation demonstrates the fundamentals of socket programming, multi-threading, and basic HTTP response handling in C.

## Features

- **TCP Socket:** Creates and binds a TCP socket.
- **Multi-Threading:** Handles each client connection in a separate thread.
- **HTTP Response:** Responds with a basic HTTP 200 OK response.
- **Concurrency:** Supports handling multiple client connections concurrently.

## Prerequisites

- **C Compiler:** GCC or any compatible C compiler.
- **POSIX Threads:** The pthread library is required for multi-threading.

## Installation

1. **Clone the Repository:**

   ```bash
   git clone <https://github.com/VivekPansari14/MultiThreadedWebServer.git>
   cd <MultiThreadedWebServer>
   ```

2. **Compile the Code:**

   ```bash
   gcc -o webserver webserver.c -lpthread
   ```

   This command compiles the source code `webserver.c` into an executable named `webserver`.

## Usage

1. **Start the Server:**

   ```bash
   ./webserver
   ```

   The server will start and listen on port 8080.

2. **Send a Request:**

   You can use a tool like [Postman](https://www.postman.com/) or `curl` to send a request to the server:

   ```bash
   curl http://localhost:8080
   ```

   Or, using Postman:
   - Open Postman.
   - Create a new GET request to `http://localhost:8080`.
   - Click on "Send."

   The server will respond with:

   ```
   HTTP/1.1 200 OK
   Content-Type: text/plain
   Content-Length: 12

   Hello World!
   ```

3. **Stop the Server:**

   To stop the server, press `Ctrl + C` in the terminal where the server is running. This will terminate the server process gracefully.

## Code Explanation

- **`handleRequest` Function:** Handles client connections, reads the request, and sends a simple HTTP response.
- **`main` Function:** Sets up the server socket, binds it to port 8080, listens for incoming connections, and creates a new thread for each connection.

## Contributing

Feel free to fork the repository and submit pull requests with improvements or new features. For any issues or feature requests, please open an issue on the repository's issue tracker.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.