#include "../header/read_conf.hpp"

void met_get()
{
        // Przykład dla metody GET (dane przekazywane przez QUERY_STRING)
    char *query_string = strstr(buffer, "GET /");
    if (query_string != nullptr) {
        // Pobieranie wartości QUERY_STRING z URI
        
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            std::cerr << "Błąd przy tworzeniu pipe." << std::endl;
            return 1;
        }

        char *end_of_uri = strchr(query_string, ' ');
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
            char *envp[] = {
                (char *)"REQUEST_METHOD=bambino",
                NULL};
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
                std::cout << "Parent otrzymał wiadomość:\n " << buffer;
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
}
