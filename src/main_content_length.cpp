#include "../header/read_conf.hpp"
#include "../header/Main_SerConfig.hpp"
#include "../header/server.hpp"

int main() {
    // Load the configuration from the config file
    Read_conf config("/mnt/c/Users/miche/OneDrive/Desktop/cpp/webtest/WebServ/configurations/config.conf");
    if (config.getPort() != 8080) { // error need fixed 
        std::cerr << "Configuration not loaded properly. Exiting...\n";
        return -1;
    }

    // Create the server instance and start it
    Server server(config.getPort());
    server.start();

    return 0;
}




/*int main() {

    Read_conf config("/mnt/c/Users/miche/OneDrive/Desktop/cpp/webtest/WebServ/configurations/config.conf");
    if (config.getPort() != 8080) { // default port
        std::cerr << "Configuration not loaded properly. Exiting...\n";
        return -1;
    }


    //Main_SerConfig serverConfig;
    //serverConfig.setServerName("MyServer"); // You might want to retrieve this from your config
    //serverConfig.setRoot("/var/www/html"); // Adjust accordingly
    //serverConfig.setIndex("index.html"); // Adjust accordingly
    //serverConfig.setClintMaxBodySize("1024"); // Adjust accordingly
    //serverConfig.setAutoindex("off"); // Adjust accordingly 

    int server_port = config.getPort();
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // Tworzenie gniazda
    if (server_fd == 0) {
        std::cerr << "Nie udało się utworzyć gniazda\n";
        return -1;
    }

    // Konfiguracja socketu
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    // Konfiguracja serwera
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server_port);//8080

    // Bindowanie socketu
    //bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    if (bind (server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "Bind Failed\n";
        return -1;
    }

    // Nasłuchiwanie
    //listen(server_fd, 3);
    if (listen(server_fd, 3) < 0)
    {
        std::cerr << "Listening failedn\n";
        return (-1);
    }

    std::cout << GREEN << "Serwer nasłuchuje na porcie ... " << RESET << "[" << server_port << "]" <<std::endl;

    while (true) {
        // Akceptowanie połączenia
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket < 0) {
            std::cerr << "Błąd połączenia\n";
            return -1;
            //continue; // continue to listen the new connection
        }

        // Odbieranie danych od przeglądarki
        read(new_socket, buffer, 1024);

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
*/