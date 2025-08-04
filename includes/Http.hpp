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

        //for chunk
        string tmpBodyChunk;
        size_t tmpChunkSize;
        bool chunkError;

        string header;
        string body;

        string method;
        string url;
        string filePath;
        string HttpVersion;

        bool canRespond;
        string respond;


        void readHeaders();
        void readBody();
        void readChunked();
        void readConfig();
        void readRouteConfig();
        void initConfig(int idx);

        //method
        string getContent(string);
        string getContentType(string);
        string getErrorCodePath(int code);
        string getResponse();
        void getServerBlock(const vector<cfgServer> &csVec);

        bool IsAllowMethod();
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

        void code301(string url); // Moved Permanently // browser will send a new http request and remember this new url
        void code303(); //see other // redirection to new url from server config
        void code400(); //bad request
        void code403(); //forbidden
        void code404(); //error not found
        void code405(); //method not found
        void code409(); //conflict
        void code413(); //post body too large
        void code500(); //server error
        void code504(); //time out

        Http();
        Http(const Http &);
        Http &operator=(const Http &);
    public:
        Http(const Config &_cf);
        ~Http();

        void parse(string input);
        void handleRequest();

        //getter
        bool getCanRespond() const;
        const string &getRespond();

};

