#include "../header/read_conf.hpp"

int main()
{
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
    address.sin_port = htons(8080);

    // Bindowanie socketu
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // Nasłuchiwanie
    listen(server_fd, 3);

    std::cout << "Serwer nasłuchuje na porcie 8080\n";

    while (true) {
        // Akceptowanie połączenia
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            std::cerr << "Błąd połączenia\n";
            return -1;
        }

        // Odbieranie danych od przeglądarki
        read(new_socket, buffer, 1024);
        // Sprawdzanie, czy to jest żądanie CGI (np. POST)

        // if (strstr(buffer, "POST /") != nullptr) {
        //     // Znalezienie Content-Length (długość danych POST)
        //     char *content_length_str = strstr(buffer, "Content-Length:");
        //     int content_length = 0;
        //     if (content_length_str != nullptr) {
        //         sscanf(content_length_str, "Content-Length: %d", &content_length);
        //     }

        //     // Tworzenie bufora na dane POST
        //     char *post_data = new char[content_length + 1]();
        //     read(new_socket, post_data, content_length);

        //     // Utworzenie procesu potomnego dla skryptu CGI
        //     pid_t pid = fork();
        //     if (pid == 0) {  // Proces potomny
        //         // Ustawienie zmiennych środowiskowych dla skryptu CGI
        //         setenv("REQUEST_METHOD", "POST", 1);
        //         setenv("CONTENT_LENGTH", std::to_string(content_length).c_str(), 1);

        //         // Przekazywanie danych POST przez standardowe wejście (stdin)
        //         dup2(new_socket, STDIN_FILENO);

        //         // Uruchomienie skryptu CGI
        //         const char *args[] = {"/path/to/cgi-bin/script.cgi", nullptr};  // Ścieżka do skryptu
        //         execvp(args[0], (char *const *)args);

        //         // Jeśli execvp się nie powiedzie
        //         exit(EXIT_FAILURE);
        //     } else {  // Proces rodzica
        //         // Oczekiwanie na zakończenie procesu CGI
        //         waitpid(pid, nullptr, 0);

        //         // Zwracanie odpowiedzi HTTP (np. wyniku skryptu CGI)
        //         const char *http_response =
        //             "HTTP/1.1 200 OK\r\n"
        //             "Content-Type: text/html\r\n"
        //             "Content-Length: 400\r\n"
        //             "\n"
        //             "<h1>Wynik skryptu CGI</h1>";
        //         send(new_socket, http_response, strlen(http_response), 0);
        //         std::cout << "Odpowiedź CGI została wysłana do klienta\n";
        //     }

        //     delete[] post_data;
        // }

        // Sprawdzanie żądania GET
        if (strstr(buffer, "GET /") != nullptr)
        {
            if (met_get(buffer, new_socket))
                return (1);
        }

        else {
            std::cout << "inny case\n"; 
                // Standardowa odpowiedź dla innych żądań
                const char *http_response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 40\r\n"
                    "\n"
                    "<h1>Hello World :) </h1>";
                send(new_socket, http_response, strlen(http_response), 0);
                std::cout << "Odpowiedź została wysłana do klienta\n";
            }

            // Zamknięcie połączenia
            close(new_socket);
    }

    close(server_fd);

    return 0;
}
