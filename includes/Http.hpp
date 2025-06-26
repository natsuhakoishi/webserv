#pragma once

#include "Webserv.hpp"

#ifndef DEBUG
# define DEBUG 0
#endif

class Config;

class Http
{
    private:
        pollfd pfd;
        const Config &cf;
        cfgServer cs;

        string rev;
        string buffer;

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
        Http(pollfd _pfd, const Config &_cf);
        Http(const Http &);
        ~Http();

        Http &operator=(const Http &);

        void parse(string input);
        void readHeaders();
        void readBody();
        void readConfig();

        //method
        string getContent(string);
        string GetContentType(string);
        void GET(pollfd, string);
        void POST(pollfd, string);
        void DELETE(pollfd, string);

        string autoindex(string path);
        string createElement(string file);

        //utils
        bool isDirectory(string path);
        bool fileExistis(string path);

        void code404(int pfd);
        void code403(int pfd);
        void code301(int pfd, string url);
        void code409(int pfd);
        void code500(int pfd);

        //getter
        bool getIsRespond() const;
};

