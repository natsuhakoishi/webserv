#include "../../includes/Http.hpp"

Http::Http() {}

Http::Http(string buffer): rev(buffer)
{
    cout << GREEN << "Client: " << rev << endl;
    parse();
}

Http::~Http() {}

void Http::parse()
{
    std::istringstream ss(this->rev);
    std::string line;

    std::getline(ss, line);
    std::istringstream requestLine(line);
    requestLine >> this->method >> this->url >> this->HttpVersion;
    // cout << method << "," << url << "," << HttpVersion << endl;

    //read headers

    this->filePath = "." + this->url;
}

void Http::respond(pollfd pfd)
{
    if (this->method == "GET")
        GET(pfd, this->filePath);
    // else if
    // else if
}
