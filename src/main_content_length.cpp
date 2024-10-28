#include "../header/read_conf.hpp"

bool find_file(const std::string& dir, const std::string& target, std::string& found_path) {
    DIR* dp = opendir(dir.c_str());
    if (dp == nullptr) return false;

    struct dirent* entry;
    while ((entry = readdir(dp)) != nullptr) {
        std::string path = dir + "/" + entry->d_name;

        if (entry->d_type == DT_DIR) {
            // Pomijamy '.' i '..'
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                if (find_file(path, target, found_path)) {
                    closedir(dp);
                    return true;
                }
            }
        } else if (entry->d_type == DT_REG && target == entry->d_name) {
            found_path = path;
            closedir(dp);
            return true;
        }
    }
    closedir(dp);
    return false;
}

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
            // Wyszukiwanie URI w żądaniu
            char* query_string = strstr(buffer, "GET /") + 5;
            char* end_of_uri = strchr(query_string, ' ');
            *end_of_uri = '\0';

            std::string full_uri(query_string);
            std::string file_name;

            // Rozdzielenie nazwy pliku i query string
            size_t query_pos = full_uri.find('?');
            std::string query_params;
            if (query_pos != std::string::npos) {
                file_name = full_uri.substr(0, query_pos); // tylko nazwa pliku
                query_params = full_uri.substr(query_pos + 1); // część po "?"
            } else {
                file_name = full_uri; // brak query string
            }

            std::string file_path;
            std::cout << file_name << std::endl;
            std::cout << query_string << std::endl;
            if (find_file("./src", file_name, file_path))
            {
                if (file_name.find(".tpl") != std::string::npos) {
                    // Obsługa skryptu CGI
                    int pipefd[2];
                    if (pipe(pipefd) == -1) {
                        std::cerr << "Błąd przy tworzeniu pipe." << std::endl;
                        return 1;
                    }

                    pid_t pid = fork();
                    if (pid == 0) {
                        // Proces potomny (skrypt CGI)
                        close(pipefd[0]);  // Zamykamy odczytanie w dziecku
                        dup2(pipefd[1], STDOUT_FILENO);  // Przekierowanie `stdout` na zapis potoku
                        close(pipefd[1]);
                    std::cout << "j ";
                        // Ustawianie zmiennej QUERY_STRING dla skryptu CGI
                    const char *python_path = "/usr/bin/python3";
                    const char *script_path = "./src/cgi/mycgi.py";
                    const char *page = "index.html";
                    // PASS REQUESTED PAGE (eg. index.html) AS ARG
                    const char *args[] = {python_path, script_path, page, NULL};
                    std::string qs = "QUERY_STRING=" + (std::string)query_string;
                    std::cout << qs << std::endl;
                    char *envp[] = {
                        (char *)"REQUEST_METHOD=GET",
                        (char *)"CONTENT_TYPE=text/html",
                        (char *)qs.c_str(),
                        NULL
                    };
                    execve(python_path, (char* const*)args, envp);
                        exit(EXIT_FAILURE);
                    } else {
                        // Proces rodzica (odczytanie wyników z potoku)
                        close(pipefd[1]);  // Zamykamy zapis w rodzicu

                        char buffer[100000];
                        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
                        if (bytesRead > 0) {
                            buffer[bytesRead] = '\0';

                            // Wysyłanie odpowiedzi CGI do klienta
                            std::string http_response = "HTTP/1.1 200 OK\r\n"
                                                        "Content-Type: text/html\r\n"
                                                        "Content-Length: " + std::to_string(bytesRead) + "\r\n"
                                                        "Connection: close\r\n\r\n" + std::string(buffer);
                            send(new_socket, http_response.c_str(), http_response.size(), 0);
                        }
                        close(pipefd[0]);
                        waitpid(pid, nullptr, 0);
                    }
                }
                else {
                    // Obsługa pliku statycznego
                    std::ifstream file(file_path);
                    if (file) {
                        std::stringstream file_stream;
                        file_stream << file.rdbuf();
                        std::string file_content = file_stream.str();

                        // Ustalanie typu MIME
                        std::string content_type = "Content-Type: text/html\r\n";
                        if (file_name.find(".css") != std::string::npos)
                            content_type = "Content-Type: text/css\r\n";
                        else if (file_name.find(".jpg") != std::string::npos)
                            content_type = "Content-Type: image/jpeg\r\n";

                        // Wysyłanie odpowiedzi HTTP dla pliku statycznego
                        std::string http_response = "HTTP/1.1 200 OK\r\n" +
                                                    content_type +
                                                    "Content-Length: " + std::to_string(file_content.size()) + "\r\n"
                                                    "Connection: close\r\n\r\n" + file_content;
                        send(new_socket, http_response.c_str(), http_response.size(), 0);
                    }
                    else {
                        // Obsługa błędu 404
                        std::string error_response = "HTTP/1.1 404 Not Found\r\n"
                                                    "Content-Length: 0\r\n"
                                                    "Connection: close\r\n\r\n";
                        send(new_socket, error_response.c_str(), error_response.size(), 0);
                    }
                }
            }
            else {
                // Obsługa błędu 404
                std::string error_response = "HTTP/1.1 404 Not Found\r\n"
                                            "Content-Length: 0\r\n"
                                            "Connection: close\r\n\r\n";
                send(new_socket, error_response.c_str(), error_response.size(), 0);
            }

        }


    else {
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
