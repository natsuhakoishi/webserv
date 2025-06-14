#include "../../includes/Http.hpp"

Http::Http() {}

Http::Http(string _buffer, pollfd _pfd): rev(_buffer), pfd(_pfd)
{
    cout << GREEN << "Client: " << rev << endl;
    parse();
}

Http::~Http() {}

void Http::parse()
{

    //read headers
    readHeaders();

    this->filePath = "." + this->url;
}

void Http::readHeaders()
{
    size_t headersEnd = this->rev.find("\r\n\r\n");
    char buffer[1024] = {0};

    while ((headersEnd = this->rev.find("\r\n\r\n")) == string::npos)
        recv(this->pfd.fd, buffer, sizeof(buffer), 0);
    string header(this->rev.substr(0, headersEnd));
    // cout << "headers: " << YELLOW << header << endl;

    std::istringstream ss(header);
    std::string line;

    std::getline(ss, line);
    std::istringstream requestLine(line);
    requestLine >> this->method >> this->url >> this->HttpVersion;
    // cout << YELLOW << "method:" << method << ", " << "url:" << url << ", " << "httpVer:" << HttpVersion << RESETEND;

    while ()
    {
        
    }

}

void Http::respond(pollfd pfd)
{
    if (!this->method.compare("GET"))
        GET(pfd, this->filePath);
    // else if (!this->method.compare("POST"))
    //     POST(pfd, this->filePath);
    // else if
}
