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
    if (server_fd == 0) {
        std::cerr << "Nie udało się utworzyć gniazda\n";
        return -1;
    }

    // socket configuration
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // server configuration
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);  // Port 80 dla HTTP

    // bind socket to address
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // listen
    listen(server_fd, 3);

    std::cout << "Server listening on port 8080\n";

    // accept connection
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    
    if (new_socket < 0) {
        std::cerr << "Błąd połączenia\n";
        return -1;
    }

    // read data from browser
    read(new_socket, buffer, 1024);
    std::cout << "Odebrano żądanie:\n" << buffer << std::endl;

    const char *http_response =
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html\r\n"
    "Content-Length: 40\r\n"
    "\n"
    "<h1>Hello World :) </h1>";

    send(new_socket, http_response, strlen(http_response), 0);
    std::cout << http_response << "\n";
    std::cout << "Odpowiedź została wysłana do klienta\n";

    // close connection
    close(new_socket);
    close(server_fd);

    return 0;
}