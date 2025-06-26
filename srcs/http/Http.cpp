#include "../../includes/Http.hpp"

// Http::Http()//: cf(Config())
// {
//     if (DEBUG)
//         cout << GREEN << "Default constructor called" << endl;
// }

Http::Http(pollfd _pfd, const Config &_cf): pfd(_pfd), cf(_cf), isRespond(false)
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

Http &Http::operator=(const Http &other)
{
    if (this == &other)
        return *this;
    this->rev = other.rev;
    this->buffer = other.buffer;
    this->pfd = other.pfd;
    this->header = other.header;
    this->body = other.body;
    this->method = other.method;
    this->filePath = other.filePath;
    this->HttpVersion = other.HttpVersion;
    this->headers = other.headers;
    this->isRespond = other.isRespond;

    return *this;
}

void Http::parse(string input)
{
    cout << GREEN << "Client: " << input << RESETEND;
    this->buffer = input;
    this->rev.append(this->buffer);
    if (this->header.empty())
        readHeaders();
    if (!this->method.compare("POST"))
        readBody();
    if (!this->method.compare("GET"))
        GET(this->pfd, this->filePath);
    if (!this->method.compare("DELETE"))
        DELETE(this->pfd, this->filePath);

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

    // this->filePath = "." + this->url; //todo: update to root

    // cout << YELLOW << "method:" << method << ", " << "url:" << url << ", " << "httpVer:" << HttpVersion << RESETEND;
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
}

void Http::readConfig()
{
    vector<cfgServer> csVec = this->cf.get_Servers();
    vector<cfgServer>::iterator iter = csVec.begin();
    string requestHost = this->headers["Host"].substr(this->headers["Host"].find(':') + 1);

    for (; iter != csVec.end(); ++iter) //search which server Iphost is macth
    {
        vector<string> hosts = iter->get_hostPort();
        vector<string>::iterator hostsIter = hosts.begin();
        for (; hostsIter != hosts.end(); ++hostsIter)
        {
            // cout << YELLOW << "debug:" << *hostsIter << " " << this->headers["Host"] << RESETEND; 
            if (hostsIter->substr(hostsIter->find(':') + 1) == requestHost)
            {
                this->cs = *iter;
                break ;
            }
        }
    }

    
    // cout << YELLOW << this->cs.get_serverName() << RESETEND;
}

void Http::readBody()
{
    size_t ContentLenght = static_cast<size_t>(std::atoi(this->headers["Content-Length"].c_str()));
    this->body = this->rev.substr(this->rev.find("\r\n\r\n") + 4);

    // size_t headersEnd = this->rev.find("\r\n\r\n");
    // tempBody.substr(headersEnd + 4);

    // cout << YELLOW << this->body << RESETEND;
    // cout << YELLOW << ContentLenght << " , " << this->body.length() << RESETEND;

    if (this->body.length() == ContentLenght)
        POST(this->pfd, this->filePath);
}

bool Http::getIsRespond() const
{
    return this->isRespond;
}
