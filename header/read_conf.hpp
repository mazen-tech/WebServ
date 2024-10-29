#ifndef READ_CONF_HPP
#define READ_CONF_HPP

#define GREEN "\033[32m"
#define RED "\033[31m"
#define RESET "\033[0m"

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <netinet/in.h>
#include <dirent.h>

int met_get(char *buffer, int new_socket);
bool find_file(const std::string& dir, const std::string& target, std::string& found_path);


class Read_conf
{
private:
    std::string path; //document root path
    int port;
    size_t size;

public:
    Read_conf();
    Read_conf(const std::string &configFile = "/mnt/c/Users/miche/OneDrive/Desktop/cpp/WebServ/configurations/config.conf");
    ~Read_conf();

    static int get_path_type(std::string const path);
    static int check_File_accessibility(std::string const path, int mode);
    static int check_File_Readability(std::string const path, std::string const index);
    std::string read_File(std::string file_path);

    std::string getPath();
    int getSize();
    int getPort();

    //new method
    std::string getDocumentRoot();
    enum
    {
    REGULAR_FILE = 1,
    DIRECTORY = 2,
    SYM_LINK = 3,
    CHAR_DEV = 4,
    OTHER = 5
    };
};

#endif
