#include "../../includes/Http.hpp"

// Http::Http()//: cf(Config())
// {
//     if (DEBUG)
//         cout << GREEN << "Default constructor called" << endl;
// }

Http::Http(pollfd _pfd, const Config &_cf)
: pfd(_pfd), cf(_cf), rootPath("."), autoindex(false), cgiTypePath(std::make_pair("Empty", "Empty")), isRespond(false)
{
    if (DEBUG)
        cout << GREEN << "Arg constructor called" << endl;
}

Http::Http(const Http &other): cf(other.cf)
{
    if (DEBUG)
        cout << GREEN << "Copy constructor called" << endl;
    *this = other;
}

Http::~Http() {}

Http &Http::operator=(const Http &q)
{
    if (this == &q)
        return *this;
    this->pfd = q.pfd;
    this->cs = q.cs;
    this->rootPath = q.rootPath;
    this->routes = q.routes;
    this->autoindex = q.autoindex;
    this->indexFile = q.indexFile;
    this->bodySize = q.bodySize;
    this->errorCodeMap = q.errorCodeMap;
    this->redirectPath = q.rootPath;
    this->uplaodPath = q.uplaodPath;

    this->headers = q.headers;
    this->allowMethod = q.allowMethod;

    this->rev = q.rev;
    this->buffer = q.buffer;

    this->header = q.header;
    this->body = q.body;

    this->method = q.method;
    this->url = q.url;
    this->filePath = q.filePath;
    this->headers = q.headers;
    this->HttpVersion = q.HttpVersion;

    this->isRespond = q.isRespond;

    return *this;
}

void Http::parse(string input)
{
    // cout << GREEN << "Client: " << input << RESETEND;
    this->buffer = input;
    this->rev.append(this->buffer);
    if (this->header.empty())
        readHeaders();
    if (!this->method.compare("POST"))
        readBody();
    else if (cgiTypePath.first.compare("Empty"))
        handleCGI(this->url);
    else if (!this->method.compare("GET"))
        GET(this->pfd, this->filePath);
    else if (!this->method.compare("DELETE"))
        DELETE(this->pfd, this->filePath);
    else //unknown method
        code405(this->pfd.fd);

    this->buffer.clear();
}

void Http::readHeaders()
{
    size_t headersEnd = this->buffer.find("\r\n\r\n");
    this->header = this->buffer.substr(0, headersEnd);

    std::istringstream ss(this->header);
    std::string line;
    std::getline(ss, line);
    std::istringstream requestLine(line);

    requestLine >> this->method >> this->url >> this->HttpVersion;

    this->headers["Method"] = this->method;
    this->headers["Url"] = this->url;
    this->headers["Http-Version"] = this->HttpVersion;

    cout << YELLOW << "method:" << method << ", " << "url:" << url << RESETEND;
    // cout << "headers: " << YELLOW << this->header << RESETEND;

    while (getline(ss, line))
    {
        size_t pos = line.find(":");
        string key = line.substr(0, pos);
        string value = line.substr(pos + 1);
        value.erase(0, value.find_first_not_of(" \t\r"));
        value.erase(value.find_last_not_of(" \t\r") + 1);
        this->headers[key] = value;
        // cout << YELLOW << "key:" << RESET << key << YELLOW << " value:" << RESET << value << "$" << RESETEND;
    }
    readConfig();
    readRouteConfig();

    // this->filePath = this->cs.get_rootPath() + this->url; // example ./index + /
    cout << YELLOW << "file path: " << this->filePath << RESETEND;
    // cout << YELLOW << this->cs.get_serverName() << RESETEND;

}


/*
check all server block's server name
    -macth -> bind
    -not macth -> 在所有server找host 并且绑定第一个对到的server host。
*/
void Http::readConfig()
{
    vector<cfgServer> csVec = this->cf.get_Servers();

    getServerBlock(csVec);
    cout << RED << "Server name: " << this->cs.get_serverName() << RESETEND;

    this->errorCodeMap = this->cs.get_errorCodesMap();
    // printMap(this->errorCodeMap);
    // printMap(this->headers);
}

void Http::getServerBlock(const vector<cfgServer> &csVec)
{
    map<string, cfgServer> csMap; //ipPort | cfgServer

    for (size_t i = 0; i < csVec.size(); ++i)
    {
        const vector<string> &hp = csVec[i].get_hostPort();
        for (size_t j = 0; j < hp.size(); ++j)
            if (!hp[j].compare(this->headers["Host"]))
                csMap[hp[j]] = csVec[i];
    }

    if (csMap.size() == 0)
    {
        string hostName = this->headers["Host"].substr(0, this->headers["Host"].find(':'));
        vector<cfgServer>::const_iterator csIter = csVec.begin();
        for (; csIter != csVec.end(); ++csIter)
        {
            if (!csIter->get_serverName().compare(hostName))
            {
                this->cs = *csIter;
                return ;
            }
        }
        csIter = csVec.begin();
        this->cs = *csIter;
    }
    else
    {
        map<string, cfgServer>::iterator csMapIter = csMap.begin();
        this->cs = csMapIter->second;
    }
}

void Http::readRouteConfig()
{
    this->routes = this->cs.get_routes();
    int idx = -1;

    for (int i = 0; i < static_cast<int>(this->routes.size()); ++i)
        if (IsCorrectPrefix(this->url, this->routes[i].get_path()))
            idx = i;
    if (idx == -1)
        for (int i = 0; i < static_cast<int>(this->routes.size()) && idx == -1; ++i)
            if (this->routes[i].get_path() == "/")
                idx = i;
    initConfig(idx);
}

void Http::initConfig(int idx)
{
    if (idx == -1)
    {
        cout << YELLOW << "Debug: Route not found, use server block data:" << endl;
        this->rootPath = this->cs.get_rootPath();
        this->filePath = this->rootPath + this->url;
        this->indexFile = this->cs.get_indexPath();
        this->autoindex = this->cs.get_autoIndexS();
        this->bodySize = this->cs.get_clientBodySize();
        this->allowMethod.push_back("GET");
        this->allowMethod.push_back("POST");
        this->allowMethod.push_back("DELETE");
        cout << "Root path: " << this->cs.get_rootPath() << endl;
        cout << "Index file: " << this->indexFile << endl;
        cout << "Auto index: " << (this->autoindex ? "on" : "off") << endl;
        cout << "Body size: " << this->bodySize << " " << this->cs.get_clientBodySize() << RESETEND;
    }
    else
    {
        cout << YELLOW << "Debug: Found route, using route block data: " << this->routes[idx].get_path() << endl;
        this->rootPath = this->routes[idx].get_rootPath();
        this->redirectPath = this->routes[idx].get_redirectionPath();
        this->filePath = this->rootPath + this->url;
        this->indexFile = this->routes[idx].get_indexPath();
        this->autoindex = this->routes[idx].get_autoIndex();
        this->bodySize = this->routes[idx].get_clientBodySize();
        this->allowMethod = this->routes[idx].get_httpMethod();
        this->uplaodPath = this->routes[idx].get_uploadPath();

        map<string, string> cgi = this->routes[idx].get_cgiInfo();
        cout << "cgi info: " << cgi.size() << endl;
        if (cgi.size() != 0)
        {
            map<string, string>::iterator it = cgi.begin();
            cout << it->first << " : " << it->second << endl;
            for (; it != cgi.end(); ++it)
            {
                if (url.find(it->first) != string::npos)
                {
                    this->cgiTypePath = *it;
                    break ;
                }
            }
            this->cgiTypePath.first = "C?";
        }

        cout << "Root path: " << this->routes[idx].get_rootPath() << endl;
        cout << "Index file: " << this->indexFile << endl;
        cout << "Auto index: " << (this->autoindex ? "on" : "off") << endl;
        cout << "Body size: " << this->bodySize << endl;
        cout << "Redirect: " << (this->redirectPath.empty() ? "no" : "yes ->" + this->redirectPath) << endl;
        cout << "Upload Path: " << this->uplaodPath << endl;
        for (int i = 0; i != static_cast<int>(this->allowMethod.size()); ++i)
            cout << "Allow method: " << this->allowMethod[i] << endl;
        cout << RESETEND;
    }

    this->headers["ROOT_PATH"] = this->rootPath;
    this->headers["UPLOAD_PATH"] = this->uplaodPath;
    this->headers["POST_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "POST") != this->allowMethod.end() ? "Y" : "N");
    this->headers["GET_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "GET") != this->allowMethod.end() ? "Y" : "N");
    this->headers["DEL_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "DELETE") != this->allowMethod.end() ? "Y" : "N");
    // cout << "GET: " << this->headers["GET_METHOD"] << " POST: " << this->headers["POST_METHOD"] << endl;
}

void Http::readBody()
{
    size_t ContentLenght = static_cast<size_t>(std::atoi(this->headers["Content-Length"].c_str()));
    if (ContentLenght > this->bodySize)
    {
        cout << RED << "POST: Size too large" << RESETEND;
        code413(this->pfd.fd);
        return ;
    }
    this->body = this->rev.substr(this->rev.find("\r\n\r\n") + 4);

    if (this->body.length() == ContentLenght)
    {
        if (this->cgiTypePath.first.compare("Empty"))
            handleCGI(this->url);
        else
            POST(this->pfd, this->filePath);
    }
}

bool Http::getIsRespond() const
{
    return this->isRespond;
}
