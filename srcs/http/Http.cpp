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
            code404(pfd.fd);
    }
    // else if
    // else if
}
