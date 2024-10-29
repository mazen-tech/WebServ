/*#pragma once
#include "../header/read_conf.hpp"
#include "../header/Main_SerConfig.hpp"


class Network_SerConfig : public Main_SerConfig
{
private:
    uint16_t _port; //network related
    in_addr_t _host;//network related
    int _listen_fd;//network related
    struct sockaddr_in _server_add; //network related

public:
    Network_SerConfig();
    Network_SerConfig(const Network_SerConfig &copy);
    Network_SerConfig &operator=(const Network_SerConfig &cp);
    ~Network_SerConfig();

    void setPort(std::string port);
    void setHost(std::string host);
    void setListen_FD(int);

    uint16_t const &getPort();
    in_addr_t const &getHost();
    int getListen_FD();

    bool checkRoute() const; //check location still not done 
    void initErrorPages(void);
    bool isValidHost(std::string host) const;
    bool isValideErrorPage();
    //int isValidRoute(Location &location) const; //related to location
    public:
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "SERVER CONFIG ERROR: " + message;
				}
				virtual const char* what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};

    
};*/

