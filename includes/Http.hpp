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
        int routesIndex;
        vector<cfgRoute> routes;
        bool autoindex;
        string indexFile;
        int bodySize;

        map<string, string> headers;
        vector<string> allowMethod;

        string rev;
        string buffer;

        string header;
        string body;

        string method;
        string url;
        string filePath;
        string HttpVersion;

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
        void readRouteConfig();
        void raedAllowMethod(vector<string> allow);
        void initConfig();
        bool IsCorrectPrefix(const string &url, const string &routePath) const;

        //method
        string getContent(string);
        string GetContentType(string);
        void GET(pollfd, string);
        void POST(pollfd, string);
        void DELETE(pollfd, string);

        string handleAutoindex(string path);
        string createElement(string file);

        //utils
        bool isDirectory(string path);
        bool fileExistis(string path);

        void code404(int pfd);
        void code403(int pfd);
        void code301(int pfd, string url);
        void code409(int pfd);
        void code413(int pfd);
        void code500(int pfd);

        //getter
        bool getIsRespond() const;
};

