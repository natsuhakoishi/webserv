#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv, char **env)
{
    char *nametmp = getenv("NAME");
    string name;
    if (!nametmp)
        name = "Who";
    else
        name = nametmp;
    
    char *saidtmp = getenv("SAY");
    string said;
    if (!saidtmp)
        said = "Hi";
    else
        said = saidtmp;    

    string content("<html lang=\"en\"><head><title>WhoSayHi</title></head><body><main><h1>" + name + " say " + said + "!</h1></main></body></html>");

    cout << "HTTP/1.1 200 ok\r\n";
    cout << "Content-Type: text/html\r\n";
    cout << "Content-Length: " << content.length() << "\r\n";
    cout << "\r\n";
    cout << content;

    return 0;
}