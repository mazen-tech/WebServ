#include "../header/read_conf.hpp"

Read_conf::Read_conf() : path("/default/path"), port(8080), size(0) {}

// Parameterized constructor with config file path
Read_conf::Read_conf(const std::string& configFile) : path("/default/path"), port(8080), size(0) {
    std::ifstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open configuration file at " << configFile << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            if (key == "port") port = std::stoi(value);
            else if (key == "document_root") path = value;
        }
    }
}


Read_conf::~Read_conf()
{

}

//#METHODS#
int Read_conf::get_path_type(std::string const path)
{
    struct stat buf;
    int res;

    res = stat(path.c_str(), &buf);
    if (res == 0)
    {
        if (buf.st_mode & S_IFREG) //file
            return (REGULAR_FILE);
        else if (buf.st_mode & S_IFDIR) //directory
            return (DIRECTORY);
        else if (buf.st_mode & S_IFLNK) //links
            return (SYM_LINK);
        else if (buf.st_mode & S_IFCHR) //character device 
            return (CHAR_DEV);
        else 
            return (OTHER); //sth else 
    }
    else 
    {
        std::cerr << "Error retrieving path info : " << strerror(errno) << std::endl;
        return (-1);
    }
}

/*
R_OK (4): Check for read permission.
W_OK (2): Check for write permission.
X_OK (1): Check for execute permission.
F_OK (0): Check for existence of the file.

+ access function to check the calling process's permission for the file
*/

std::string Read_conf::getDocumentRoot() {
    return path;
}

std::string Read_conf::getPath()
{
    return (this->path);
}

int Read_conf::getSize()
{
    return (this->size);
}

int Read_conf::getPort()
{
    return port;
}

int Read_conf::check_File_accessibility(std::string const path, int permission)
{
    return (access(path.c_str(), permission));
}

int Read_conf::check_File_Readability(std::string const path, std::string const fileName)
{
    if (get_path_type(fileName) == REGULAR_FILE
        && check_File_accessibility(fileName, 4) == 0)
        return (0);
    if (get_path_type(path + fileName) == REGULAR_FILE
        && check_File_accessibility(path + fileName, 4) == 0)
        return (0);
    return (-1);
}


std::string Read_conf::read_File(std::string file_path)
{
    if (file_path.empty() || file_path.length() == 0)
        return (NULL);

    std::ifstream configuration_file(path.c_str());
    if (!configuration_file.is_open() || configuration_file.fail() 
    || !configuration_file)
        return (NULL);

    std::stringstream conf_file_stream; //create file stream
    conf_file_stream << configuration_file.rdbuf(); //read content into stringstream
    return (conf_file_stream.str());//returning content as a string
}
