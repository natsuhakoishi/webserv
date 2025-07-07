#include "../../includes/Webserv.hpp"

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		try
		{
			std::string	filepath = argv[1];
			Config  *config = new Config(filepath);
			vector<cfgServer> servers = config->get_Servers();

			vector<cfgServer>::iterator it = servers.begin();
			std::vector<std::string> hostPorts = it->get_hostPort();

			TcpServer *server = new TcpServer(hostPorts, config);
			server->runServer();

			delete config;
			delete server;
		}
		catch(const std::exception& e)
		{
			std::cerr << "\033[31m" << e.what() << "\033[0m\n";
		}
	}
	else
		std::cout << "Usage: ./webserv <configuration file>" << std::endl;
	return (0);
}

