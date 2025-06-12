#include "../../includes/Http.hpp"

void Http::code404(int pfd)
{
    cout << RED << "404!" << RESETEND; //debug

    std::ostringstream ss;

    ss << "HTTP/1.1 404 Not Found\r\n\r\n";

    string content = getContent("./error_page/404.html");
    if (!content.compare(""))
        ss << "<!doctype html><html lang=\"en\"><head><title>404 not found</title></head><body><main><h1>404 Not found</h1></main></body></html>";
    else
        ss << content;

    send(pfd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond 404 succesful" << endl; //debug
    std::cout << "Client (fd: " << pfd << ") Disconnected" << RESETEND; //debug
    close(pfd);
}
