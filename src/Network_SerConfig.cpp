/*#include "../header/Network_SerConfig.hpp"
#include <arpa/inet.h>


    Network_SerConfig();
    Network_SerConfig(const Network_SerConfig &copy);
    Network_SerConfig &operator=(const Network_SerConfig &cp);
    ~Network_SerConfig();


Network_SerConfig::Network_SerConfig()
{
    this->_port = 0;
    this->_host = 0;
    this->_listen_fd = 0;
}

Network_SerConfig::~Network_SerConfig() {}

Network_SerConfig::Network_SerConfig(const Network_SerConfig &copy)
{
    if (this != &copy)
    {
    this->_host = copy._host;
    this->_port = copy._port;
    this->_server_add = copy._server_add;
    this->_listen_fd = copy._listen_fd;
    }
    return ;
}

Network_SerConfig &Network_SerConfig::operator=(const Network_SerConfig &cp)
{
    if (this != &cp)
    {
    this->_host = cp._host;
    this->_port = cp._port;
    this->_server_add = cp._server_add;
    this->_listen_fd = cp._listen_fd;
    }
    return (*this);
}

void Network_SerConfig::setPort(std::string port)
{
    unsigned int PORT;

    PORT = 0;
    checkToken(port);
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!std::isdigit(port[i]))
            throw ErrorException("port syntaz error: setPort()/MainConfig");
        this->_port = (uint16_t) PORT;
    }
}

void Network_SerConfig::setHost(std::string host)
{
    checkToken(host);
    if (host == "localhost")
        host = "127.0.0.1";
    if (!isValidHost(host))
        throw ErrorException("host syntaz error: setHost()/MainConfig");
    this->_host = inet_addr(host.data());
}

void Network_SerConfig::setListen_FD(int fd)
{
    this->_listen_fd = fd;
}


    uint16_t const &getPort();
    in_addr_t const &getHost();


uint16_t const &Network_SerConfig::getPort()
{
    return(_port);
}

in_addr_t const &Network_SerConfig::getHost()
{
    return(_host);
}

int Network_SerConfig::getListen_FD()
{
    return (this->_listen_fd);
}


    bool checkLocations() const;
    void initErrorPages(void); 
    bool isValidHost(std::string host) const;
    bool isValideErrorPage();

void Network_SerConfig::initErrorPages(void)
{
    std::vector<int> errorCodes = {301, 302, 400, 401,
    402, 403, 404, 405, 406, 500, 501, 502, 503, 505};
    for (int code : errorCodes)
    {
        _error_pages[code] = "";
    }
}

bool Network_SerConfig::isValidHost(std::string host) const
{
    struct sockaddr_in sockaddr;
    return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);//here need check
}

bool Network_SerConfig::isValideErrorPage()
{
    std::map<short, std::string>::const_iterator it;
    for (it = this->_error_pages.begin(); it != this->_error_pages.end(); ++it)
    {
        if (it->first < 100 || it->first > 599)
            return (false);
        if (Read_conf::check_File_accessibility(getRoot() + it->second, 0) < 0
            || Read_conf::check_File_accessibility(getRoot() + it->second, 4) < 0)
            return (false);
    }
    return (true);
}

*/