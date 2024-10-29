#pragma once 

#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <cctype>
#include "Network_SerConfig.hpp"
#include "read_conf.hpp"
#include "utils.hpp"

//#include "Location.hpp"
//#include "ErrorPages.hpp"
//#include "SocketManager.hpp"

static std::string serverPara[] = {"Server_name", "listen", "root", 
    "index", "allow_methods", "client_body_buffer_size"};

//class Network_SerConfig;
class Main_SerConfig
{
protected:
    std::string _server_name; // config related
    std::string _root; //config related
    std::string _index; //config related
    unsigned long _clint_max_body_size; //config related
    bool _autoindex; //config related
    std::map<short, std::string> _error_pages; //config related
    //std::vector<location> _location; //location route 




public:
    Main_SerConfig(); //done
    Main_SerConfig(const Main_SerConfig &copy);  //done
    Main_SerConfig &operator=(const Main_SerConfig &cp);  //done
    ~Main_SerConfig();  //done

    void setServerName(std::string server_name);
    void setRoot(std::string root);
    void setIndex(std::string index);
    void setClintMaxBodySize(std::string str);
    void setAutoindex(std::string autoindex);
    void serLocation(std::string nameLocation, std::vector<std::string> str);
    void setErrorPage(std::vector<std::string> &str);
    void initErrorPages();

    std::string const &getServerName();//done
    std::string const &getRoot();//done
    std::string const &getIndex();//done
    std::string const &getPathErrorPage(short code);//done
    size_t getClintMaxBodySize();//done
    bool const &getAutoindex();//done
    std::map<short, std::string> const &getErrorpage();//done
    //std::vector<Location> const &getLocation(); //location route
    //std::vector<Location>::iterator getLocationKey(std::string key); //location rout

    //void	setupServer();
    static void checkToken(std::string &str);
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
};

