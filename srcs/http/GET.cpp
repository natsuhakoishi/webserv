#include "../../includes/Http.hpp"

string Http::GetContentType(string c)
{
    // cout << "eeeeeeeee:" << c.substr(c.length() - 5) << endl;
    if (!c.substr(c.length() - 5).compare(".html")) return "text/html";
    if (!c.substr(c.length() - 4).compare(".png")) return "image/png";
    if (!c.substr(c.length() - 4).compare(".gif")) return "image/gif";
    return "text/plain";
    // return "text/csv";
    // return "text/css";
}

string Http::getContent(string file)
{
    std::ifstream ifile(file.c_str(), std::ios::binary);

    if (!ifile.is_open())
        return "";
    
    std::ostringstream ss;

    ss << ifile.rdbuf();

    return ss.str();
}

void Http::GET(pollfd pfd, string path)
{
    string content(getContent(path));
    string type(GetContentType(path));

    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Type: " << type << "\r\n";
    ss << "Content-Length: " << content.length() << "\r\n";
    ss << "\r\n";
    ss << content;

    send(pfd.fd, ss.str().c_str(), ss.str().length(), 0);
}
