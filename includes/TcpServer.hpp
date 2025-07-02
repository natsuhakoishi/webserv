#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

#include "Webserv.hpp"

class Http;

class Config;

class TcpServer
{
	public:
		TcpServer();
		TcpServer(std::vector<std::string> hostPorts, Config *_cf);
		~TcpServer();

		void	runServer();

	private:
		std::vector<std::string>	ips;
		std::vector<int>			ports;
		std::vector<int>			server_fds;
		std::vector<pollfd>			fds;

		void	initServer();
		void	handleClientConnection(size_t i);
		void	handleClientMessage(size_t i);

		TcpServer(const TcpServer &copy);
		TcpServer	&operator = (const TcpServer &copy);

		map<int, Http *> httpMap;
		Config *config;
};

#endif
