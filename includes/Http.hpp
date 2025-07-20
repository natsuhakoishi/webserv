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
        string rootPath;
        vector<cfgRoute> routes;
        bool autoindex;
        std::pair<string, string> cgiTypePath; //type:executePath
        string indexFile;
        size_t  bodySize;
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

        string response;

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
        void initConfig(int idx);

        //method
        string getContent(string);
        string getContentType(string);
        string getErrorCodePath(int code);
        string getResponse();
        // bool getServerFromServerName(const vector<cfgServer> &csVec);
        void getServerBlock(const vector<cfgServer> &csVec);
        void GET(pollfd, string);
        void POST(pollfd, string);
        void DELETE(pollfd, string);

        void handleCGI(string CGIpath);
        void CGIGet(vector<char *> &argv, string CGIpath);
        void CGIPost(vector<char *> &argv, string CGIpath);
        vector<string> getQueryParameters(string &url);

        vector<string> createEnv(map<string, string> &m);
        string handleAutoindex(string path);
        string createElement(string file);

        //utils
        bool isDirectory(const string &path);
        bool fileExistis(const string &path);
        bool IsCorrectPrefix(const string &url, const string &routePath) const;
        bool isExecutale(const string &path);

        void code403(int pfd); //forbidden
        void code404(int pfd); //error not found
        void code405(int pfd); //method not found
        void code301(int pfd, string url); //redirection
        void code409(int pfd); //conflict
        void code500(int pfd); //server error
        void code504(int pfd); //server error
        void code413(int pfd); //post body too large
        void code303(int pfd); //see other

        //getter
        bool getIsRespond() const;


};

