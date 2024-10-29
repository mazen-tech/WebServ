#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main() {
    int pipe_to_python[2];   // Potok do przesyłania danych do Pythona (stdin)
    int pipe_from_python[2]; // Potok do odbierania danych z Pythona (stdout)
    
    if (pipe(pipe_to_python) == -1 || pipe(pipe_from_python) == -1) {
        std::cerr << "Pipe failed" << std::endl;
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed" << std::endl;
        return 1;
    } else if (pid == 0) {  // Proces potomny
        // Zamknij zbędne końce potoków
        close(pipe_to_python[1]);
        close(pipe_from_python[0]);

        // Przekieruj stdin i stdout na odpowiednie końce potoków
        dup2(pipe_to_python[0], STDIN_FILENO);
        dup2(pipe_from_python[1], STDOUT_FILENO);

        // Zamknij duplikaty
        close(pipe_to_python[0]);
        close(pipe_from_python[1]);

        // Ścieżka do skryptu Pythona
        char *args[] = {(char*)"/usr/bin/python3", (char*)"script.py", nullptr};

        // Uruchom skrypt Pythona
        execve(args[0], args, nullptr);
        
        std::cerr << "Execve failed" << std::endl;
        return 1;
    } else {  // Proces rodzica
        // Zamknij zbędne końce potoków
        close(pipe_to_python[0]);
        close(pipe_from_python[1]);

        // Wyślij dane do skryptu Pythona
        std::string data = "Hello from C++";
        write(pipe_to_python[1], data.c_str(), data.size());
        
        // Zamknij potok po zapisaniu danych
        close(pipe_to_python[1]);

        // Odbierz wynik ze skryptu Pythona
        char buffer[256];
        ssize_t count = read(pipe_from_python[0], buffer, sizeof(buffer) - 1);
        if (count > 0) {
            buffer[count] = '\0';
            std::cout << "Received from Python: " << buffer << std::endl;
        }

        // Zamknij potok po odczytaniu danych
        close(pipe_from_python[0]);

        // Poczekaj na zakończenie procesu potomnego
        wait(nullptr);
    }
    return 0;
}
