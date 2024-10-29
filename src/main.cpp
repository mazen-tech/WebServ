#include "../header/read_conf.hpp"

int main() {
    Read_conf config("config.conf");

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Tworzenie gniazda
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Nie udało się utworzyć gniazda\n";
        return -1;
    }

    // Konfiguracja socketu
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Konfiguracja serwera
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(config.getPort());//htons(8080);
    // Bindowanie socketu
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Nasłuchiwanie
    //listen(server_fd, 3);
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listening failedn\n";
        return (-1);
    }

    std::cout << "Serwer nasłuchuje na porcie 8080\n";

    while (true)
    {
        // Akceptowanie połączenia
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket < 0)
        {
            std::cerr << "Błąd połączenia\n";
            return -1;
        }
        read(new_socket, buffer, 1024);

        if (strstr(buffer, "POST /") != NULL)
        {
            if (met_post(buffer, new_socket))
                return (1);
        }

        else if (strstr(buffer, "GET /") != NULL)
        {
            if (met_get(buffer, new_socket))
                return (1);
        }

        else
        {
            std::cout << "inny case\n"; 
                const char *http_response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 40\r\n"
                    "\n"
                    "<h1>Hello World :) </h1>";
                send(new_socket, http_response, strlen(http_response), 0);
                std::cout << "Odpowiedź została wysłana do klienta\n";
        }
            close(new_socket);
    }

    close(server_fd);

    return 0;
}
