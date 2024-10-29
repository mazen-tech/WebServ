#include "../header/read_conf.hpp"

int main() {
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
        // std::cout << "Odebrano żądanie:\n" << buffer << std::endl;
        // Sprawdzanie, czy to jest żądanie CGI (np. POST)
        if (strstr(buffer, "POST /") != nullptr) {
            // Znalezienie Content-Length (długość danych POST)
            char *content_length_str = strstr(buffer, "Content-Length:");
            int content_length = 0;
            if (content_length_str != nullptr) {
                sscanf(content_length_str, "Content-Length: %d", &content_length);
            }

            // Tworzenie bufora na dane POST
            char *post_data = new char[content_length + 1]();
            read(new_socket, post_data, content_length);

            // Utworzenie procesu potomnego dla skryptu CGI
            pid_t pid = fork();
            if (pid == 0) {  // Proces potomny
                // Ustawienie zmiennych środowiskowych dla skryptu CGI
                setenv("REQUEST_METHOD", "POST", 1);
                setenv("CONTENT_LENGTH", std::to_string(content_length).c_str(), 1);

                // Przekazywanie danych POST przez standardowe wejście (stdin)
                dup2(new_socket, STDIN_FILENO);

                // Uruchomienie skryptu CGI
                const char *args[] = {"/path/to/cgi-bin/script.cgi", nullptr};  // Ścieżka do skryptu
                execvp(args[0], (char *const *)args);

                // Jeśli execvp się nie powiedzie
                exit(EXIT_FAILURE);
            } else {  // Proces rodzica
                // Oczekiwanie na zakończenie procesu CGI
                waitpid(pid, nullptr, 0);

                // Zwracanie odpowiedzi HTTP (np. wyniku skryptu CGI)
                const char *http_response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n"
                    "Content-Length: 400\r\n"
                    "\n"
                    "<h1>Wynik skryptu CGI</h1>";
                send(new_socket, http_response, strlen(http_response), 0);
                std::cout << "Odpowiedź CGI została wysłana do klienta\n";
            }

            delete[] post_data;
        } else if (strstr(buffer, "GET /") != nullptr) {
                // Przykład dla metody GET (dane przekazywane przez QUERY_STRING)
            char *query_string = strstr(buffer, "GET /");
            if (query_string != nullptr) {
                // Pobieranie wartości QUERY_STRING z URI
                
                int pipefd[2];
                if (pipe(pipefd) == -1) {
                    std::cerr << "Błąd przy tworzeniu pipe." << std::endl;
                    return 1;
                }

                char *end_of_uri = strchr(query_string, '\n');
                *end_of_uri = '\0';  // Odcinanie URI

                // Utworzenie procesu potomnego
                pid_t pid = fork();
                if (pid == 0) {
                    // Proces potomny
                    // Ustawienie QUERY_STRING w zmiennej środowiskowej

            // IMPORTANT!
            // DOES NOT WORK - LOOK AT LINE 123 - IT WORKS THAT WAY
                    setenv("QUERY_STRING", query_string , 1);  // 12 to długość "/cgi-bin/"
                    close(pipefd[0]); // Zamykamy czytanie w child

                    // Przekierowanie `stdout` na koniec zapisu potoku
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                    // Uruchomienie skryptu CGI
                    // const char *args[] = {"./cgi/mycgi.py", nullptr};  // Ścieżka do skryptu
                    // execvp(args[0], (char *const *)args);
                    const char *python_path = "/usr/bin/python3";
                    const char *script_path = "./src/cgi/mycgi.py";
                    const char *page = "index.html";
            // PASS REQUESTED PAGE (eg. index.html) AS ARG
                    const char *args[] = {python_path, script_path, page, NULL};
                    std::string qs = "QUERY_STRING=" + (std::string)query_string;
                    char *envp[] = {
                        (char *)"REQUEST_METHOD=GET",
                        (char *)"CONTENT_TYPE=text/html",
                        (char *)qs.c_str(),
                        NULL
                    };
                    execve(python_path, (char* const*)args, envp);
                    // hahaha
                    std::cout << "Nie tym razem frajerze! \n";
            // IMPORTANT END!

                    // Jeśli execvp się nie powiedzie
                    exit(EXIT_FAILURE);
                } else {  // Proces rodzica
                    // Oczekiwanie na zakończenie procesu CGI
                    waitpid(pid, nullptr, 0);
                    close(pipefd[1]); // Zamykamy zapis w parent

                    // Odbieranie danych z potoku
                    char buffer[100000];
                    ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
                    if (bytesRead > 0) {
                        buffer[bytesRead] = '\0'; // Dodanie null-terminatora
                        std::cout << "Parent otrzymał wiadomość:\n " << buffer << "\n";
                    close(pipefd[0]); // Zamykamy czytanie w parent

                    // Zwracanie odpowiedzi HTTP
                    // const char *http_response =
                    //     "HTTP/1.1 200 OK\r\n"
                    //     "Content-Type: text/html\r\n"
                    //     "Content-Length: 40\r\n"
                    //     "\n"
                    //     "<h1>Wynik skryptu CGI GET</h1>";
                    send(new_socket, buffer, strlen(buffer), 0);
                    std::cout << "Odpowiedź CGI GET została wysłana do klienta\n";
                    }
                }
            }
        } else {
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
