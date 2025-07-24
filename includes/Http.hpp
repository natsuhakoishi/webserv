#pragma once

#include "Webserv.hpp"

#ifndef DEBUG
# define DEBUG 0
#endif

class Config;

class Http
{
    private:
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
        bool sizeTooLarge;

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

        bool canRespond;
        string respond;
        Http();

    public:
        Http(const Config &_cf);
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
        void handleRequest();
        void GET(string);
        void POST(string);
        void DELETE(string);

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

        void code403(); //forbidden
        void code404(); //error not found
        void code405(); //method not found
        void code301(string url); //redirection
        void code409(); //conflict
        void code500(); //server error
        void code504(); //server error
        void code413(); //post body too large
        void code303(); //see other

        //getter
        bool getCanRespond() const;
        const string &getRespond();

};

