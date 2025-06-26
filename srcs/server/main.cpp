#include "../../includes/Webserv.hpp"

int main()
{
	Config  *config = new Config();
	vector<cfgServer> servers = config->get_Servers();

	vector<cfgServer>::iterator it = servers.begin();
	std::vector<std::string> hostPorts = it->get_hostPort();

	TcpServer *server = new TcpServer(hostPorts);
	server->runServer();

	delete server;
	return 0;
}

