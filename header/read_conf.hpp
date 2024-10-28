#ifndef READ_CONF_HPP
#define READ_CONF_HPP

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

void met_get();

class Read_conf
{
private:
    std::string path;
    size_t size;

public:
    Read_conf();
    Read_conf(const std::string path);
    ~Read_conf();

    static int get_path_type(std::string const path);
    static int check_File_accessibility(std::string const path, int mode);
    static int check_File_Readability(std::string const path, std::string const index);
    std::string read_File(std::string file_path);

    std::string getPath();
    int getSize();

    enum
    {
    REGULAR_FILE = 1,
    DIRECTORY = 2,
    SYM_LINK = 3,
    CHAR_DEV = 4,
    OTHER = 5
    };
};

class server
{
    public:
        server();
        
} ;

#endif
