#include "../../includes/Webserv.hpp"

int main(int argc, char **argv)
{
	std::vector<std::pair<std::string, int> > ipp;
	std::pair<std::string, int> temp;

	if (argc == 2)
	{
		Config  *config = new Config(string(argv[1]));
		vector<cfgServer> servers = config->get_Servers();

		vector<cfgServer>::iterator it = servers.begin();
		std::vector<std::string> hostPorts = it->get_hostPort();
		for (size_t i = 0; i < hostPorts.size(); ++i)
		{
			size_t	separate_pos = hostPorts[i].find(":");
			if (separate_pos == string::npos)
			{
				cout << "Invalid HostPort" << endl;
				return (1);
			}
			string host = hostPorts[i].substr(0, separate_pos);
			string port_str = hostPorts[i].substr(separate_pos + 1);

			if (port_str.empty())
			{
				cout << "Invalid Port" << endl;
				return (1);
			}

			int port = atoi(port_str.c_str());

			cout << GREEN << "Host " << host << " Port " << port << RESETEND << endl;

			temp = std::make_pair(host, port);
			ipp.push_back(temp);
		}
		TcpServer *server = new TcpServer(ipp);
		server->runServer();

		delete server;
	}
	else
	{
		temp = std::make_pair("127.0.0.1", 8080);
		ipp.push_back(temp);

		temp = std::make_pair("0.0.0.0", 9090);
		ipp.push_back(temp);

		TcpServer *server = new TcpServer(ipp);
		server->runServer();

		delete server;
	}
	return 0;
}

// int main()
// {
//     Http h("/");

//     h.autoindex("./srcs");
// }
