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
    char buffer[1024] = {0};
    ssize_t bytes_read = -1;
    while (1)
    {
        bytes_read = recv(this->pfd.fd, buffer, sizeof(buffer), 0);
        cout << bytes_read << endl;
        if (bytes_read == 0)
            break ;
        this->rev += buffer;
    }
    //read headers
    readHeaders();
    cout << YELLOW << this->body << RESETEND;

    this->filePath = "." + this->url;
}

void Http::readHeaders()
{
    size_t headersEnd = this->rev.find("\r\n\r\n");
    this->header = this->rev.substr(0, headersEnd);
    // this->body = this->rev.substr(headersEnd + 4);
    // cout << "headers: " << YELLOW << header << endl;

    std::istringstream ss(header);
    std::string line;

    std::getline(ss, line);
    std::istringstream requestLine(line);
    requestLine >> this->method >> this->url >> this->HttpVersion;
    // cout << YELLOW << "method:" << method << ", " << "url:" << url << ", " << "httpVer:" << HttpVersion << RESETEND;

    // while (getline(ss, line))
    // {
    //     size_t pos = line.find(":");
    //     string key = line.substr(0, pos);
    //     string value = line.substr(pos + 1);
    //     value.erase(0, value.find_first_not_of(" \t\r"));
    //     value.erase(value.find_last_not_of(" \t\r") + 1);
    //     this->headers[key] = value;
    //     cout << YELLOW << "key:" << RESET << key << YELLOW << " value:" << RESET << value << "$" << RESETEND;
    // }

}

void Http::respond(pollfd pfd)
{
    if (!this->method.compare("GET"))
        GET(pfd, this->filePath);
    // else if (!this->method.compare("POST"))
    //     POST(pfd, this->filePath);
    // else if
}
