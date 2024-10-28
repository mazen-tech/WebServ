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

# some tasks about CGI
* only *.py files should go to CGI
* html, css, jpg should be treated as static files and served directly from source file
* *.py should be all dynamic pages (with get parameters and post data)
* don't know about uploaded files - should it be handled by CGI or by server only?

# what I need from server to handle CGI right
* file name (eg index.py) it sould be passed as argument for CGI
* get args (string is ok, like "?name=jan&hobby=bike") best idea to put it in env_var
* post body redirected to STDIN

# what I will return
* I will print to std out rendered content
* If status is other than 200 i will print to std out something like error:500 or error:404, my content sould be finished with EOF (i can also print to std err if it helps)
* I don't think generating headers in  CGI is a good idea - I made some tests with content-length and it works best when content-length is calculated by server (see main_contetn_length.cpp rows from 159 to 166)
