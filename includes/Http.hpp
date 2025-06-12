#pragma once

#include "Webserv.hpp"

class Http
{
    private:
        string rev;
        string method;
        string url;
        string filePath;
        string HttpVersion;
        map<string, string> headers;
        Http();

    public:
        Http(string buffer);
        ~Http();
        void parse();
        void respond(pollfd);

        //helper function
        bool fileExistis(string);

        //GET method
        string getContent(string);
        string GetContentType(string);
        void GET(pollfd, string);

        void code404(int pfd);
        void code301(int pfd, string url);
};

