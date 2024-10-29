#include "../header/server.hpp"
#include "../header/read_conf.hpp"

Server::Server(int port) {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Failed to create socket\n";
        exit(EXIT_FAILURE);
    }

    configureSocket();

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind Failed\n";
        exit(EXIT_FAILURE);
    }
}

Server::~Server() {
    close(server_fd);
}

void Server::configureSocket() {
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
}

void Server::start() {
    listenForConnections();
}

void Server::listenForConnections() {
    if (listen(server_fd, 3) < 0) {
        std::cerr << "Listening failed\n";
        exit(EXIT_FAILURE);
    }

    std::cout << GREEN << "Server is running...\n" << RESET;
    std::cout << GREEN << "Server is listening on port.... " << RESET << std::endl; 

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Connection error\n";
            continue;
        }

        handleConnection(new_socket);
    }
}

void Server::handleConnection(int new_socket) {
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);

    if (strstr(buffer, "GET /") != nullptr) {
        if (met_get(buffer, new_socket)) return;
    } else {
        const char *http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 40\r\n"
            "\n"
            "<h1>Hello World :)</h1>";
        send(new_socket, http_response, strlen(http_response), 0);
        std::cout << YELLOW << "Response sent to client" << RESET << "[GET]" << std::endl;
    }

    close(new_socket);
}
