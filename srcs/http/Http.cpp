#include "../../includes/Http.hpp"

// Http::Http(const Http &other): cf(other.cf)
// {
//     if (DEBUG)
//         cout << GREEN << "Copy constructor called" << endl;
//     *this = other;
// }


// Http &Http::operator=(const Http &q)
// {
//     if (this == &q)
//         return *this;
//     this->cs = q.cs;
//     this->rootPath = q.rootPath;
//     this->routes = q.routes;
//     this->autoindex = q.autoindex;
//     this->indexFile = q.indexFile;
//     this->bodySize = q.bodySize;
//     this->errorCodeMap = q.errorCodeMap;
//     this->redirectPath = q.rootPath;
//     this->uplaodPath = q.uplaodPath;

//     this->headers = q.headers;
//     this->allowMethod = q.allowMethod;

//     this->rev = q.rev;
//     this->buffer = q.buffer;

//     this->header = q.header;
//     this->body = q.body;

//     this->method = q.method;
//     this->url = q.url;
//     this->filePath = q.filePath;
//     this->headers = q.headers;
//     this->HttpVersion = q.HttpVersion;

//     this->canRespond = q.canRespond;

//     return *this;
// }


Http::Http(const Config &_cf): cf(_cf)
{
    this->rootPath = ".";
    this->autoindex = false;
    this->cgiTypePath = std::make_pair("Empty", "Empty");
    this->sizeTooLarge = false;
    this->canRespond = false;
    this->tmpChunkSize = 0;
    this->chunkError = false;

    if (DEBUG)
        cout << GREEN << "Arg constructor called" << endl;
}

Http::~Http() {}

void Http::parse(string input)
{
    // cout << GREEN << "Client: " << input << RESETEND;
    this->buffer = input;
    this->rev.append(this->buffer); //save input to buffer
    if (this->header.empty()) // first raed header
        readHeaders(); 
    if (!this->method.compare("POST"))
        readBody();
    else
        this->canRespond = true; //GET

    this->buffer.clear();
}

void Http::handleRequest()
{
    if (this->chunkError)
        code400(); //bad request
    else if (this->sizeTooLarge)
        code413(); //size too large

    if (std::find(this->allowMethod.begin(), this->allowMethod.end(), this->method) != this->allowMethod.end())
    {
        if (cgiTypePath.first.compare("Empty"))
            handleCGI(this->url);
        else if (!this->method.compare("POST"))
            POST(this->filePath);
        else if (!this->method.compare("GET"))
            GET(this->filePath);
        else if (!this->method.compare("DELETE"))
            DELETE(this->filePath);
    }
    else //unknown method or method not allow 
        code405();
}

void Http::readHeaders()
{
    size_t headersEnd = this->buffer.find("\r\n\r\n");
    this->header = this->buffer.substr(0, headersEnd);

    std::istringstream ss(this->header);
    std::string line;
    std::getline(ss, line);
    std::istringstream requestLine(line);

    requestLine >> this->method >> this->url >> this->HttpVersion; //save header's first line

    this->headers["Method"] = this->method;
    this->headers["Url"] = this->url;
    this->headers["Http-Version"] = this->HttpVersion;

    cout << YELLOW << "method:" << method << ", " << "url:" << url << RESETEND; //debug
    // cout << "headers: " << YELLOW << this->header << RESETEND; //debug

    while (getline(ss, line)) //save haeder's key & value to a map<>
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
    cout << YELLOW << "file path: " << this->filePath << RESETEND; //debug
    // cout << YELLOW << this->cs.get_serverName() << RESETEND;

}


/*
check all server block's server name
    -macth -> bind
    -not macth -> find host in all server, bind first macth server host
*/
void Http::readConfig()
{
    vector<cfgServer> csVec = this->cf.get_Servers(); //copy config of servers

    getServerBlock(csVec); //bind server's config
    cout << RED << "Server name: " << this->cs.get_serverName() << RESETEND; //debug

    this->errorCodeMap = this->cs.get_errorCodesMap(); //save error code map
    // printMap(this->errorCodeMap); //debug
    // printMap(this->headers); //debug
}

/**
 * bind server block
 * try to match server name
 * if not match
 * try to match ip+host
 * if not match
 * take the first server block 
 */
void Http::getServerBlock(const vector<cfgServer> &csVec)
{
    map<string, cfgServer> csMap; //ipPort | cfgServer

    for (size_t i = 0; i < csVec.size(); ++i) //get all match ports
    {
        const vector<string> &hp = csVec[i].get_hostPort();
        for (size_t j = 0; j < hp.size(); ++j)
            if (!hp[j].compare(this->headers["Host"])) //try to match host ex. 120.0.0.1
                csMap[hp[j]] = csVec[i];
    }

    if (csMap.size() == 0) //have'nt match port, try match serverName
    {
        string hostName = this->headers["Host"].substr(0, this->headers["Host"].find(':'));
        vector<cfgServer>::const_iterator csIter = csVec.begin();
        for (; csIter != csVec.end(); ++csIter) //loop all servers blocks
        {
            if (!csIter->get_serverName().compare(hostName)) //try to match serverName ex. 42KL.com
            {
                this->cs = *csIter; //bind server block
                return ;
            }
        }
        csIter = csVec.begin(); //have'nt match serverName, bind the first server block
        this->cs = *csIter;
    }
    else
    {
        map<string, cfgServer>::iterator csMapIter = csMap.begin(); //bind the first server block
        this->cs = csMapIter->second;
    }
}

/**
 * bind route block
 * init my config's variable
 */
void Http::readRouteConfig()
{
    this->routes = this->cs.get_routes();
    int idx = -1;

    for (int i = 0; i < static_cast<int>(this->routes.size()); ++i)
        if (IsCorrectPrefix(this->url, this->routes[i].get_path()))
            idx = i;
    if (idx == -1)
        for (int i = 0; i < static_cast<int>(this->routes.size()) && idx == -1; ++i)
            if (this->routes[i].get_path() == "/") //find '/' block
                idx = i;
    initConfig(idx);
}

/**
 * init config's variable
 * if have'nt bind route
 * take server's config (default)
 * else use the corrrect route
 */
void Http::initConfig(int idx)
{
    if (idx == -1) // no bind any route even '/'
    {
        cout << YELLOW << "Debug: Route not found, use server block data:" << endl; //debug
        this->rootPath = this->cs.get_rootPath();
        this->filePath = this->rootPath + this->url;
        this->indexFile = this->cs.get_indexPath();
        this->autoindex = this->cs.get_autoIndexS();
        this->bodySize = this->cs.get_clientBodySize();
        this->allowMethod.push_back("GET");
        this->allowMethod.push_back("POST");
        this->allowMethod.push_back("DELETE");
        {//debug
            cout << "Root path: " << this->cs.get_rootPath() << endl; //debug
            cout << "Index file: " << this->indexFile << endl; //debug
            cout << "Auto index: " << (this->autoindex ? "on" : "off") << endl; //debug
            cout << "Body size: " << this->bodySize << " " << this->cs.get_clientBodySize() << RESETEND; //debug
        }
    }
    else
    {
        cout << YELLOW << "Debug: Found route, using route block data: " << this->routes[idx].get_path() << endl; //debug
        this->rootPath = this->routes[idx].get_rootPath();
        this->redirectPath = this->routes[idx].get_redirectionPath();
        this->filePath = this->rootPath + this->url;
        this->indexFile = this->routes[idx].get_indexPath();
        this->autoindex = this->routes[idx].get_autoIndex();
        this->bodySize = this->routes[idx].get_clientBodySize();
        this->allowMethod = this->routes[idx].get_httpMethod();
        this->uplaodPath = this->routes[idx].get_uploadPath();

        map<string, string> cgi = this->routes[idx].get_cgiInfo();
        // cout << "cgi info: " << cgi.size() << endl;
        if (cgi.size() != 0)
        {
            map<string, string>::iterator it = cgi.begin();
            this->cgiTypePath.first = "C?";
            for (; it != cgi.end(); ++it)
            {
                cout << it->first << " : " << it->second << endl;
                if (url.find(it->first) != string::npos)
                {
                    this->cgiTypePath = *it;
                    break ;
                }
            }
        }
        {//debug
            cout << "CgiType: " << this->cgiTypePath.first << " : " << this->cgiTypePath.second << endl; //debug
            cout << "Root path: " << this->routes[idx].get_rootPath() << endl; //debug
            cout << "Index file: " << this->indexFile << endl; //debug
            cout << "Auto index: " << (this->autoindex ? "on" : "off") << endl; //debug
            cout << "Body size: " << this->bodySize << endl; //debug
            cout << "Redirect: " << (this->redirectPath.empty() ? "no" : "yes ->" + this->redirectPath) << endl; //debug
            cout << "Upload Path: " << this->uplaodPath << endl; //debug
            for (int i = 0; i != static_cast<int>(this->allowMethod.size()); ++i) //debug
                cout << "Allow method: " << this->allowMethod[i] << endl; //debug
            cout << RESETEND; //debug
        }
    }

    //for cgi env
    this->headers["ROOT_PATH"] = this->rootPath;
    this->headers["UPLOAD_PATH"] = this->uplaodPath;
    this->headers["POST_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "POST") != this->allowMethod.end() ? "Y" : "N");
    this->headers["GET_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "GET") != this->allowMethod.end() ? "Y" : "N");
    this->headers["DEL_METHOD"] = (std::find(this->allowMethod.begin(), this->allowMethod.end(), "DELETE") != this->allowMethod.end() ? "Y" : "N");
    // for (map<string, string>::iterator it = this->headers.begin(); it != this->headers.end(); ++it)
    //     cout << "HEADER: " << it->first << " : " << it->second << endl;
    // cout << "GET: " << this->headers["GET_METHOD"] << " POST: " << this->headers["POST_METHOD"] << endl;
}

/**
 * read by chunked body and normal body
 * if find "/r/n/r/n" or "0" means body is end, canRespond = true
 * else http will keep receive body data until body is end
 */
void Http::readBody()
{
    if (!this->headers["Transfer-Encoding"].compare("chunked"))
        readChunked();
    else //read normal body
    {
        size_t ContentLenght = static_cast<size_t>(std::atoi(this->headers["Content-Length"].c_str()));
        if (ContentLenght > this->bodySize)
        {
            cout << RED << "POST: Size too large" << RESETEND;
            this->canRespond = true;
            this->sizeTooLarge = true;
            return ;
        }
        this->body = this->rev.substr(this->rev.find("\r\n\r\n") + 4);

        if (this->body.length() == ContentLenght)
            this->canRespond = true;
    }
    // cout << "body:\n" << body << endl;
}

void Http::readChunked()
{
    string buf;
    if (this->body.empty())
        buf = this->rev.substr(this->rev.find("\r\n\r\n") + 4);
    else
        buf = this->buffer;

    if (this->chunkError && buf.find("0\r\n\r\n") != string::npos)
    {
        this->canRespond = true;
        return ;
    }

    size_t pos = 0;
    while (1)
    {
        size_t end = 0;
        size_t chunkSize = 0;
        std::stringstream ss_size;

        if (this->tmpChunkSize != 0)
        {
            this->tmpBodyChunk.append(buf.substr(0, this->tmpChunkSize));
            pos += this->tmpChunkSize + 2;

            this->tmpChunkSize = 0;
        }

        end = buf.find("\r\n", pos);
        if (end == string::npos) //error
        {
            cout << RED << "posssssssssssssssss" << RESETEND; //debug
            if (buf.find("0\r\n\r\n") != string::npos)
                this->canRespond = true;
            this->chunkError = true;
            break ;
        }

        string sizeString = buf.substr(pos, end - pos);
        ss_size << std::hex << sizeString;
        ss_size >> chunkSize;

        cout << "size: " << chunkSize << endl; //debug
        if (chunkSize == 0) //end of the body
        {
            cout << RED << "0000000000000" << RESETEND; //debug
            this->canRespond = true;
            break ;
        }
        pos = end + 2;

        if (pos + chunkSize > buf.length()) //body of the middle, wait next body data
        {
            cout << pos << "+" << chunkSize << '>' << buf.length() << endl;
            this->tmpChunkSize = pos + chunkSize - buf.length();
            break ;
        }
        this->tmpBodyChunk.append(buf.substr(pos, chunkSize));
        cout << "pos: " << pos << endl; //debug
        cout << "buf: " << buf.substr(pos, chunkSize) << endl; //debug
        pos += chunkSize;

        if (buf.substr(pos, 2).compare("\r\n")) //error
        {
            cout << RED << "breakeddd" << RESETEND; //debug
            if (buf.find("0\r\n\r\n") != string::npos)
                this->canRespond = true;
            this->chunkError = true;
            break ;
        }
        pos += 2;
    }
    this->body.append(this->tmpBodyChunk);
}

bool Http::getCanRespond() const
{
    return this->canRespond;
}

const string &Http::getRespond()
{
    if (this->respond.empty()) //it won't happen, but just to be safe
        code500(); //server error
    // cout << GREEN << "respond:\n" << this->respond << RESETEND; //debug
    return this->respond;
}
