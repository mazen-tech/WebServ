#include "../header/Main_SerConfig.hpp"

/*
    Main_SerConfig();
    Main_SerConfig(const Main_SerConfig &copy);
    Main_SerConfig &operator=(const Main_SerConfig &cp);
    ~Main_SerConfig();
*/

Main_SerConfig::Main_SerConfig()
{
    this->_server_name = "";
    this->_root = "";
    this->_index = "";
    this->_autoindex = false;
    this->_clint_max_body_size = 0; // need to define MAX CONTECT LENGTH
    //this->initErrorPages();
}

Main_SerConfig::Main_SerConfig(const Main_SerConfig &copy)
{
    if (this != &copy)
    {
        this->_server_name = copy._server_name;
        this->_root = copy._root;
        this->_index = copy._index;
        //this->_location = copy._location;
        this->_clint_max_body_size = copy._clint_max_body_size;
        this->_error_pages = copy._error_pages;
        this->_autoindex = copy._autoindex;
    }
    return ;
}


Main_SerConfig &Main_SerConfig::operator=(const Main_SerConfig &cp)
{
    if (this != &cp)
    {
        this->_server_name = cp._server_name;
        this->_root = cp._root;
        this->_index = cp._index;
        //this->_location = cp._location;
        this->_clint_max_body_size = cp._clint_max_body_size;
        this->_error_pages = cp._error_pages;
        this->_autoindex = cp._autoindex;
    }
    return (*this);
}

Main_SerConfig::~Main_SerConfig() 
{

}






/*
    void setServerName(std::string server_name);
    void setRoot(std::string root);
    void setIndex(std::string index);
    void setClintMaxBodySize(std::string str);
    void setAutoindex(std::string autoindex);
    void setErrorPage(std::vector<std::string> &str); //commented
    void serLocation(std::string nameLocation, std::vector<std::string> str);

*/

void Main_SerConfig::checkToken(std::string &tok)
{
    size_t pos = tok.rfind(';');
    if (pos != tok.size() - 1)
        throw ErrorException("Syntax error ';'");
    tok.erase(pos);
}

void Main_SerConfig::setServerName(std::string server_name)
{
    checkToken(server_name);
    this->_server_name = server_name;
}
void Main_SerConfig::setRoot(std::string root)
{
    char dir[1024];
    checkToken(root);
    if (Read_conf::get_path_type(root) == 2)
    {
        this->_root = root;
        return ;
    }
    getcwd(dir, 1024);
    std::string full_root = dir + root;
    if (Read_conf::get_path_type(full_root) != 2)
        throw ErrorException("Error syntax: setRoot: Main_config");
    this->_root = full_root;
}

void Main_SerConfig::setIndex(std::string index)
{
    checkToken(index);
    this->_index = index;
}

void Main_SerConfig::setClintMaxBodySize(std::string str)
{
    unsigned long body_size;

    body_size = 0;
    checkToken(str);
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
            throw ErrorException("Error syntax: setClint Max size: Main_config");
    }
    if (!ft_stoi(str))
        throw ErrorException("Error syntax: setClint Max size: Main_config");
    body_size = ft_stoi(str);
    this->_clint_max_body_size = body_size;
}
void Main_SerConfig::setAutoindex(std::string autoindex)
{
    checkToken(autoindex);
    if (autoindex != "on" && autoindex != "off")
        throw ErrorException("Error syntax: autoindex: Main_cnfig");
    if (autoindex == "on")
        this->_autoindex = true;
}

/*void Main_SerConfig::serLocation(std::string nameLocation, std::vector<std::string> str)
{

}*/

void Main_SerConfig::setErrorPage(std::vector<std::string> &str)
{
    if (str.empty())
        return;
    if (str.size() % 2 != 0)
        throw ErrorException("Error page: main_config");
    for (size_t i = 0; i < str.size() - 1; i++)
    {

        for(size_t j = 0; j < str[i].size(); j++)
        {
            if (!std::isdigit(str[i][j]))
                throw ErrorException("Error code must be a number: main_config");
        }

        if (str[i].size() != 3)
            throw ErrorException("Error code must be 3 numbers: mainconfig");

        short code_error = ft_stoi(str[i]);
        if (errorCodeStatus(code_error) == "Undefind" || code_error < 400)
            throw ErrorException("invalid error code: main_config " + str[i]);

        i++;
        std::string path = str[i];
        checkToken(path);
        if (Read_conf::get_path_type(path) !=  2)
        {
            if (Read_conf::get_path_type(this->_root + path) != 1)
                throw ErrorException("Invalid path for error page file: file does'n exict: Main_config" + this->_root + path);
            if (Read_conf::check_File_accessibility(this->_root + path, 0) == -1
                || Read_conf::check_File_accessibility(this->_root + path, 4) == -1)
                    throw ErrorException ("Error page file -> " + this->_root + path + " is not accessible");
        }


        std::map<short, std::string>::iterator it = this->_error_pages.find(code_error);
        if (it != _error_pages.end())
            this->_error_pages[code_error] = path;
        else 
            this->_error_pages.insert(std::make_pair(code_error, path));
    }
}



/*
    std::string const &getServerName();
    std::string const &getRoot();
    std::string const &getIndex();
    size_t const getClintMaxBodySize();
    bool const &getAutoindex();
    std::map<short, std::string> const &getErrorpage();
*/

std::string const &Main_SerConfig::getServerName()
{
    return (this->_server_name);
}

std::string const &Main_SerConfig::getRoot()
{
    return (this->_root);
}

std::string const &Main_SerConfig::getIndex()
{
    return (this->_index);
}

std::string const &Main_SerConfig::getPathErrorPage(short code)
{
    std::map<short, std::string>::iterator it = this->_error_pages.find(code);
    if (it == this->_error_pages.end())
        throw ErrorException("No such file for error_page");
    return (it->second);
}

size_t Main_SerConfig::getClintMaxBodySize()
{
    return (this->_clint_max_body_size);
}

bool const &Main_SerConfig::getAutoindex()
{
    return (this->_autoindex);
}

std::map<short, std::string> const &Main_SerConfig::getErrorpage()
{
    return (this->_error_pages);
}