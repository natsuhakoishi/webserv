#include "../../includes/Webserv.hpp"

int main() {
    std::vector<int> ports;
    ports.push_back(8080);
    ports.push_back(9090);

    TcpServer server(ports);
    server.runServer();

    return 0;
}
