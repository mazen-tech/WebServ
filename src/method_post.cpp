#include "../header/read_conf.hpp"

std::string f_name ( std::string request )
{
    std::size_t pos = request.find("POST /");
    std::string filename;
    if (pos != std::string::npos)
    {
        pos += 6;
        std::size_t end_pos = request.find(' ', pos);
        if (end_pos != std::string::npos)
        {
            std::string filename = request.substr(pos, end_pos - pos);
        }
    }
    return (filename);
}

int met_post(char *buffer, int new_socket)
{
    int pipe_fd[2];
    int pipe_from_python[2];

    std::string filename = f_name(buffer);

    if (pipe(pipe_fd) == -1 || pipe(pipe_from_python) == -1)
    {
        std::cerr << "Pipe failed" << std::endl;
        return 1;
    }

    // Znalezienie Content-Length (długość danych POST)
    char *content_length_str = strstr(buffer, "Content-Length:");
    int content_length = 0;
    if (content_length_str != nullptr)
    {
        sscanf(content_length_str, "Content-Length: %d", &content_length);
    }
    // // Tworzenie bufora na dane POST
    char *post_data = new char[content_length + 1]();
    // read(new_socket, post_data, content_length);
    std::string line = strstr(buffer, "\r\n\r\n");
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
        const char *filename_cstr = filename.c_str();
        // const char *page = file_name.c_str();
        // PASS REQUESTED PAGE (eg. index.html) AS ARG
        const char *args[] = {python_path, script_path, filename_cstr, NULL};
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
    }
    else
    {
        close(pipe_fd[0]);
        close(pipe_from_python[1]);
        write(pipe_fd[1], line.c_str(), line.size());
        close(pipe_fd[1]);
        // Proces rodzica
        // Oczekiwanie na zakończenie procesu CGI
        waitpid(pid, nullptr, 0);

        char buffer[1024];
        ssize_t count = read(pipe_from_python[0], buffer, sizeof(buffer) - 1);
        if (count > 0)
        {
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
    memset(buffer, 0, strlen(buffer));
    return (0);
}
