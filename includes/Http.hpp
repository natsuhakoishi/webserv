#pragma once

#include "Webserv.hpp"

#ifndef DEBUG
# define DEBUG 0
#endif

class Http
{
    private:
        string rev;
        string buffer;
        pollfd pfd;
    
        string header;
        string body;

        string method;
        string url;
        string filePath;
        string HttpVersion;

        map<string, string> headers;

        bool isRespond;
        Http();

    public:
        Http(pollfd _pfd);
        Http(const Http &);
        ~Http();

        Http &operator=(const Http &);

        void parse(string input);
        void readHeaders();
        void readBody();

        void respond(pollfd);

        //GET method
        string getContent(string);
        string GetContentType(string);
        void GET(pollfd, string);
        void POST(pollfd, string);

        string autoindex(string path);
        string createElement(string file);

        //utils
        bool isDirectory(string path);
        bool fileExistis(string path);
    
        void code404(int pfd);
        void code403(int pfd);
        void code301(int pfd, string url);

        //getter
        bool getIsRespond() const;
};

