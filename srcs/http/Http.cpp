#include "../../includes/Http.hpp"

Http::Http() {}

Http::Http(string buffer): rev(buffer)
{
    cout << "Client: " << rev << endl;
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

    if (this->url == "/")
        this->url = "/index.html"; //will update to config file default seting
    this->filePath = "." + this->url;

}

bool Http::fileExistis(string file)
{
    std::ifstream ifile(file.c_str());
    if (!ifile.good())
        return false;
    return true;
}

void Http::respond(pollfd pfd)
{
    if (this->method == "GET")
    {
        if (fileExistis(this->filePath))
            GET(pfd, this->filePath);
        else
        {
            cout << RED << "404!!!!!!!!!!" << RESETEND;
            string notFound("HTTP/1.1 404 Not Found\r\n\r\n");
            notFound += "<!doctype html><html lang=\"en\"><head><title>404 not found</title></head><body><main><h1>404 Not found</h1></main></body></html>";
            cout << BLUE << notFound << RESETEND;
            send(pfd.fd, notFound.c_str(), notFound.length(), 0);
            std::cout << "Client (fd: " << pfd.fd << ") Disconnected" << std::endl;
            close(pfd.fd);
            // 404 not found;
        }
    }
    // else if
    // else if
}
