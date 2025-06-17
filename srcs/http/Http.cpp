#include "../../includes/Http.hpp"

Http::Http()
{
    if (DEBUG)
        cout << GREEN << "Default constructor called" << endl;
}

Http::Http(pollfd _pfd): pfd(_pfd), isRespond(false)
{
    if (DEBUG)
        cout << GREEN << "Arg constructor called" << endl;
}

Http::Http(const Http &other)
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
    // cout << GREEN << "Client: " << input << RESETEND;
    this->buffer = input;
    this->rev.append(buffer);
    if (header.empty())
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

    this->filePath = "." + this->url;

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

    //parse headers
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

void Http::respond(pollfd pfd)
{
    if (!this->method.compare("GET"))
        GET(pfd, this->filePath);
    else if (!this->method.compare("POST"))
        POST(pfd, this->filePath);
    else if (!this->method.compare("DELETE"))
        DELETE(pfd, this->filePath);
}

bool Http::getIsRespond() const
{
    return this->isRespond;
}
