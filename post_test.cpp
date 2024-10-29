#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        std::cerr << "Pipe failed" << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {  // Proces potomny
        // Zamknij zbędny koniec potoku
        close(pipe_fd[1]);
        
        // Przekieruj stdin na potok
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        
        // Ścieżka do skryptu Pythona
        char *args[] = {(char*)"/usr/bin/python3", (char*)"script.py", nullptr};
        
        // Uruchom skrypt Pythona
        execve(args[0], args, nullptr);
        
        std::cerr << "Execve failed" << std::endl;
        return 1;
    } else {  // Proces rodzica
        // Zamknij zbędny koniec potoku
        close(pipe_fd[0]);

        // Wyślij dane do skryptu Pythona
        std::string data = "Hello from C++";
        write(pipe_fd[1], data.c_str(), data.size());
        
        // Zamknij potok po zapisaniu danych
        close(pipe_fd[1]);
        
        // Poczekaj na zakończenie procesu potomnego
        wait(nullptr);
    }
    return 0;
}
