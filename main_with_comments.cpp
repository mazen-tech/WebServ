#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

// run program and type http://localhost:8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // creating socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    //there should be -1 insted of 0, bcs the -1 means that it fails
    if (server_fd == -1) {
        std::cerr << "Nie udało się utworzyć gniazda\n";
        return -1;
    }

    // socket configuration
    // jponieck:: tried to comment this line and program still works. The function below
    // is changing the default sokcet config , most important are SO_REUSEADDR and SO_REUSEPORT
    // because it allows server to handle multiple processes on the same address
    // Also the chat bot said that we should not use the |, better is to call that func two times
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // server configuration
    address.sin_family = AF_INET; // meand address type IPv4
    address.sin_addr.s_addr = INADDR_ANY; // assigns address (ex localhost) don't know how to change it
    address.sin_port = htons(8080);  //port number - hotons converts 8080 to apropriate format

    // bind socket to address
    bind(server_fd, (struct sockaddr *)&address, sizeof(address)); // casting sockaddr_it to sockaddr .. what for? should we care?

    // listen
    listen(server_fd, 3); // 3 ist the number of connections in queue , we can change this number
    // accept will execute the firs one id we want more connections to be handled we have to use accept several times
    // I asked gpt if theese 3 request will be executed one by one and i got interesting answer (see below)*

    std::cout << "Server listening on port 8080\n";

    // accept connection
    // possibly here is a good place to fork child process 
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    if (new_socket < 0) {
        std::cerr << "Błąd połączenia\n";
        return -1;
    }

    // read data from browser
    read(new_socket, buffer, 1024);
    std::cout << "Odebrano żądanie:\n" << buffer << std::endl;

    // strange string - but it is legal in C to write string like this "something""something else"  important tu use \r\n between STATUS LINE, HEADER LINE, EMPTY LINE, BODY
    const char *http_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 40\r\n"
    "\r\n"
    "<h1>Hello World :) </h1>";

    send(new_socket, http_response, strlen(http_response), 0);
    std::cout << http_response << "\n";
    std::cout << "Odpowiedź została wysłana do klienta\n";

    // close connection
    close(new_socket);
    close(server_fd);

    return 0;
}

// *When using the accept() function, connections are handled sequentially, not in parallel. This means that accept() will handle one connection at a time.

// How does accept() work?
// Each call to accept() retrieves one connection from the queue of pending connections (determined by the backlog value set in listen()).
// The server must call accept() for every incoming connection. When a client tries to connect, if there's space in the backlog queue, the connection is placed in the queue.
// accept() waits for a connection to be available in the queue, then accepts it. If no connection is pending, accept() blocks until a new connection request is made.
// Sequential handling:
// If the backlog is set to 3 (as in listen(server_fd, 3);), the server can have up to 3 pending connections, but accept() must be called individually for each connection.
// If there are three connections in the queue, accept() must be called three times—once for each connection.
// How to handle multiple connections simultaneously?
// To handle multiple connections in parallel, you need additional mechanisms:

// Threads: The server can create a new thread for each connection after calling accept(), allowing concurrent handling of multiple clients.

// fork(): You can use fork() to create a new process to handle each connection.

// Non-blocking I/O: Using techniques like select(), poll(), or epoll(), the server can handle multiple connections in one process/thread without blocking.

// This way, you can accept and process multiple connections concurrently.
