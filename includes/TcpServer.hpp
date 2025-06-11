#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include <iostream>
# include <unistd.h>
# include <cstring> //for memset()
# include <sys/socket.h> //socket library
# include <netinet/in.h> //Internet family
# include <poll.h>
# include <fcntl.h>
# include <vector>
# include <cstdlib>
# include <algorithm>

class TcpServer
{
	public:
		TcpServer();
		TcpServer(std::vector<int> ports);
		~TcpServer();

		void	runServer();

	private:
		std::vector<int>	ports;
		std::vector<int>	server_fds;
		std::vector<pollfd>	fds;

		void	initServer();
		void	handleClientConnection(size_t i);
		void	handleClientMessage(size_t i);

		TcpServer(const TcpServer &copy);
		TcpServer	&operator = (const TcpServer &copy);
};

#endif
