#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

#include "Webserv.hpp"

class TcpServer
{
	public:
		TcpServer();
		TcpServer(std::vector<std::pair<std::string, int> > ipp);
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
};

#endif
