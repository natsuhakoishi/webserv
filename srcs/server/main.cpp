#include "../../includes/Webserv.hpp"

int main() {
    std::vector<std::pair<std::string, int> > ipp;
    std::pair<std::string, int> temp;

    temp = std::make_pair("127.0.0.1", 8080);
    ipp.push_back(temp);

    temp = std::make_pair("0.0.0.0", 9090);
    ipp.push_back(temp);

    TcpServer server(ipp);
    server.runServer();

    return 0;
}

// int main()
// {
//     Http h("/");

//     h.autoindex("./srcs");
// }
