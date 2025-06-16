#pragma once

#include "Webserv.hpp"

class Http
{
    private:
        string rev;
        pollfd pfd;
    
        string header;
        string body;

        string method;
        string url;
        string filePath;
        string HttpVersion;
    
        map<string, string> headers;
        Http();

    public:
        Http(string _buffer, pollfd _pfd);
        ~Http();
        void parse();
        void readHeaders();

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
};

