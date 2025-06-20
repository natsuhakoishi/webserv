#include "../../includes/Http.hpp"

void Http::code404(int pfd)
{
    cout << RED << "404!" << RESETEND; //debug

    std::ostringstream ss;

    ss << "HTTP/1.1 404 Not Found\r\n\r\n";

    this->isRespond = true;
    string content = getContent("./error_page/404.html");
    if (!content.compare(""))
        ss << "<!doctype html><html lang=\"en\"><head><title>404 not found</title></head><body><main><h1>404 Not found</h1></main></body></html>";
    else
        ss << content;

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 404 successful" << endl; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    close(pfd);
}

void Http::code403(int pfd)
{
    cout << RED << "403!" << RESETEND; //debug

    std::ostringstream ss;

    this->isRespond = true;
    ss << "HTTP/1.1 403 Forbidden\r\n\r\n";

    string content = getContent("./error_page/403.html");
    if (!content.compare(""))
        ss << "<!doctype html><html lang=\"en\"><head><title>403 Forbidden</title></head><body><main><h1>403 Forbidden</h1></main></body></html>";
    else
        ss << content;

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 403 successful" << endl; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    close(pfd);
}

void Http::code301(int pfd, string url)
{
    cout << RED << "301!" << RESETEND; //debug
    std::ostringstream ss;

    int len = url.length() - 1;
    while (url[len] != '/')
        len--;
    string newPath(".");
    newPath.append(url.substr(len));

    ss << "HTTP/1.1 301 Moved Permanently\r\n";
    ss << "Location: " << newPath + "/\r\n";
    ss << "Content-Length: 0\r\n\r\n";

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 301 successful" << endl; //debug
    cout << BLUE << "Respond:" << RED << ss.str() << RESETEND; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    this->isRespond = true;
    close(pfd);
}

void Http::code409(int pfd)
{
    cout << RED << "409!" << RESETEND; //debug

    std::ostringstream ss;

    ss << "HTTP/1.1 409 Conflict\r\n\r\n";

    this->isRespond = true;
    string content = getContent("./error_page/409.html");
    if (!content.compare(""))
        ss << "<!doctype html><html lang=\"en\"><head><title>409 Conflict</title></head><body><main><h1>404 Conflict</h1></main></body></html>";
    else
        ss << content;

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 409 successful" << endl; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    close(pfd);
}

void Http::code500(int pfd)
{
    cout << RED << "500!" << RESETEND; //debug

    std::ostringstream ss;

    ss << "HTTP/1.1 500 Internal Server Error\r\n\r\n";

    this->isRespond = true;
    string content = getContent("./error_page/500.html");
    if (!content.compare(""))
        ss << "<!doctype html><html lang=\"en\"><head><title>500 Internal Server Error</title></head><body><main><h1>500 Internal Server Error</h1></main></body></html>";
    else
        ss << content;

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 500 successful" << endl; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    close(pfd);
}
