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

    while (true) {
        // Akceptowanie połączenia
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            std::cerr << "Błąd połączenia\n";
            return -1;
        }

        // Odbieranie danych od przeglądarki
        read(new_socket, buffer, 1024);
        // std::cout << buffer << std::endl;
        // char *line = strstr(buffer, "\r\n\r\n");
        // std::cout << buffer << std::endl;
        // Sprawdzanie, czy to jest żądanie CGI (np. POST)
        // std::cout << strstr(buffer, "POST /") << std::endl;
        if (strstr(buffer, "POST /") != NULL)
        {
            int pipe_fd[2];
            int pipe_from_python[2];
            if (pipe(pipe_fd) == -1 || pipe(pipe_from_python) == -1) {
                std::cerr << "Pipe failed" << std::endl;
                return 1;
            }


            // Znalezienie Content-Length (długość danych POST)
            char *content_length_str = strstr(buffer, "Content-Length:");
            int content_length = 0;
            if (content_length_str != nullptr) {
                sscanf(content_length_str, "Content-Length: %d", &content_length);
            }
            // // Tworzenie bufora na dane POST
            char *post_data = new char[content_length + 1]();
            // read(new_socket, post_data, content_length);
            std::string line = strstr(buffer, "\r\n\r\n");
            std::cout << line << std::endl;
            // // Utworzenie procesu potomnego dla skryptu CGI
            pid_t pid = fork();
            
            if (pid == 0)
            {  // Proces potomny
                close(pipe_fd[1]);
                close(pipe_from_python[0]);

                dup2(pipe_fd[0], STDIN_FILENO);
                dup2(pipe_from_python[1], STDOUT_FILENO);
                close(pipe_fd[0]);
                close(pipe_from_python[1]);
                // Przekazywanie danych POST przez standardowe wejście (stdin)
                // dup2(STDOUT_FILENO, STDIN_FILENO);
                // std::cout << line << EOF << std::endl;

                const char *python_path = "/usr/bin/python3";
                const char *script_path = "./src/cgi/mycgi.py";
                // const char *page = file_name.c_str();
                // PASS REQUESTED PAGE (eg. index.html) AS ARG
                const char *args[] = {python_path, script_path, NULL};
                // std::string qs = "QUERY_STRING=" + (std::string)query_string;
                // std::cout << qs << std::endl;
                char *envp[] = {
                    (char *)"REQUEST_METHOD=GET",
                    (char *)"CONTENT_TYPE=text/html",
                    // (char *)qs.c_str(),
                    NULL
                };
                execve(python_path, (char* const*)args, envp);
                exit(EXIT_FAILURE);
            } else {
                close(pipe_fd[0]);
                close(pipe_from_python[1]);
                write(pipe_fd[1], line.c_str(), line.size());
                close(pipe_fd[1]);
                  // Proces rodzica
                // Oczekiwanie na zakończenie procesu CGI
                waitpid(pid, nullptr, 0);

                char buffer[1024];
                ssize_t count = read(pipe_from_python[0], buffer, sizeof(buffer) - 1);
                if (count > 0) {
                    buffer[count] = '\0';
                }

                // Zamknij potok po odczytaniu danych
                close(pipe_from_python[0]);

                std::string http_response = "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(count) + "\r\n"
                "Connection: close\r\n\r\n" + std::string(buffer);
                send(new_socket, http_response.c_str(), http_response.size(), 0);

                // Zwracanie odpowiedzi HTTP (np. wyniku skryptu CGI)
                // const char *http_response =
                //     "HTTP/1.1 200 OK\r\n"
                //     "Content-Type: text/html\r\n"
                //     "Content-Length: 400\r\n"
                //     "\n"
                //     "<h1>Wynik skryptu CGI</h1>";
                // send(new_socket, http_response, strlen(http_response), 0);
                std::cout << "Odpowiedź CGI została wysłana do klienta\n";
            }

            delete[] post_data;
        }

        // Sprawdzanie żądania GET
        else if (strstr(buffer, "GET /") != nullptr)
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
