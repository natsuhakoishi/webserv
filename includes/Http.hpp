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
        string rootPath;
        const Config &cf;
        cfgServer cs;
        vector<cfgRoute> routes;
        bool autoindex;
        string indexFile;
        int bodySize;
		map<int,string>		errorCodeMap;
        string redirectPath;
        string uplaodPath;

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
        void initConfig(int idx);
        bool IsCorrectPrefix(const string &url, const string &routePath) const;

        //method
        string getContent(string);
        string getContentType(string);
        string getErrorCodePath(int code);
        void GET(pollfd, string);
        void POST(pollfd, string);
        void DELETE(pollfd, string);

        string handleAutoindex(string path);
        string createElement(string file);

        //utils
        bool isDirectory(string path);
        bool fileExistis(string path);

        void code404(int pfd); //error not found
        void code403(int pfd); //forbidden
        void code301(int pfd, string url); //redirection
        void code409(int pfd); //conflict
        void code500(int pfd); //server error
        void code413(int pfd); //post body too large
        void code303(int pfd); //see other

        //getter
        bool getIsRespond() const;
};

