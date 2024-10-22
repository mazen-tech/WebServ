# WebServer Setup
## Resources 

[How to build a simple HTTP server](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[Simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)

[C++ Web Programming](https://www.tutorialspoint.com/cplusplus/cpp_web_programming.htm)

## Tasks to start with 
* Building a default configration 
</p>server configuration listens on port e.g8080, with custom error pages, supports multiple HTTP methods(READ / GIT / POST etc...), enables CGI for Python/bash scripts, and redirects.</p>

* Configration.conf  (configuration file that the web server reads and uses to determine its behavior)
* Configration file.cpp (Handles reading the raw configuration file)
* Configration file.hpp
* Parsing the configration file .cpp / .hpp (Parses and processes the raw configuration data)

# step 2 (after bulding the configration system)
* Server Configration.cpp (Represents the parsed server configurations as objects)
* Server Configration.hpp
