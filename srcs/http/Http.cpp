#include "../../includes/Http.hpp"

Http::Http() {}

Http::Http(string _buffer, pollfd _pfd): rev(_buffer), pfd(_pfd)
{
    cout << GREEN << "Client: " << rev << RESETEND;
    parse();
}

Http::~Http() {}

void Http::parse()
{
    //read headers
    readHeaders();
    // cout << YELLOW << this->body << RESETEND;

    this->filePath = "." + this->url;
}

void Http::readHeaders()
{
    size_t headersEnd = this->rev.find("\r\n\r\n");
    this->header = this->rev.substr(0, headersEnd);

    std::istringstream ss(this->header);
    std::string line;
    std::getline(ss, line);
    std::istringstream requestLine(line);

    requestLine >> this->method >> this->url >> this->HttpVersion;

    if (!this->method.compare("POST"))
        this->body = this->rev.substr(headersEnd + 4);

    cout << YELLOW << "method:" << method << ", " << "url:" << url << ", " << "httpVer:" << HttpVersion << RESETEND;
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

}

void Http::respond(pollfd pfd)
{
    if (!this->method.compare("GET"))
        GET(pfd, this->filePath);
    else if (!this->method.compare("POST"))
        POST(pfd, this->filePath);
    // else if
}
