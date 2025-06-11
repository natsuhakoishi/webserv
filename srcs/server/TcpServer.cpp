#include "../../includes/TcpServer.hpp"

TcpServer::TcpServer()
{
	this->ips.push_back("0.0.0.0");
	this->ports.push_back(80);
	initServer();
}

TcpServer::TcpServer(std::vector<std::pair<std::string, int> > ipp)
{
	for (std::vector<std::pair<std::string, int> >::iterator it = ipp.begin(); it != ipp.end(); it++)
	{
		this->ips.push_back((*it).first);
		this->ports.push_back((*it).second);
	}
	initServer();
}

void	TcpServer::initServer()
{
	for(size_t i = 0; i < this->ports.size(); i++)
	{
		int	server_fd = socket(AF_INET, SOCK_STREAM, 0);

		if (server_fd < 0)
		{
			std::cerr << "Failure: Socket Creation" << std::endl;
			exit(1);
		}

		int	opt = 1;
		setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

		struct sockaddr_in address;
		std::memset(&address, 0, sizeof(address));
		address.sin_family = AF_INET;
		if (inet_pton(AF_INET, this->ips[i].c_str(), &(address.sin_addr)) <= 0)
		{
			std::cerr << "Error: Invalid Address: " << this->ips[i] << std::endl;
			exit(1);
		}
		address.sin_port = htons(ports[i]);

		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			std::cerr << "Failure: Bind on port " << ports[i] << std::endl;
			exit(1);
		}

		if (listen(server_fd, 3) < 0)
		{
			std::cerr << "Failure: Listen on port " << ports[i] << std::endl;
			exit(1);
		}

		int flags = fcntl(server_fd, F_GETFL, 0);
		fcntl(server_fd, F_SETFL, flags | O_NONBLOCK);

		struct pollfd pfd;
		pfd.fd = server_fd;
		pfd.events = POLLIN;
		pfd.revents = 0;

		this->fds.push_back(pfd);
		this->server_fds.push_back(server_fd);

		std::cout << "Listening on port " << ports[i] << "..." << std::endl;
	}
}

void	TcpServer::handleClientConnection(size_t i)
{
	struct sockaddr_in address;
	socklen_t addrlen = sizeof(address);

	int	new_socket = accept(this->fds[i].fd, (struct sockaddr *)&address, &addrlen);
	if (new_socket >= 0)
	{
		int nflags = fcntl(new_socket, F_GETFL, 0);
		fcntl(new_socket, F_SETFL, nflags | O_NONBLOCK);

		struct pollfd npoll;
		npoll.fd = new_socket;
		npoll.events = POLLIN;
		npoll.revents = 0;
		this->fds.push_back(npoll);

		std::cout << "Client (fd: "<< new_socket << ") Connected" << std::endl;

		// const char *msg = "Connected to the Server\n";
		// send(new_socket, msg, strlen(msg), 0);
	}
}

void	TcpServer::handleClientMessage(size_t i)
{
	char	buffer[1024] = { 0 };
	ssize_t bytes_read = recv(this->fds[i].fd, buffer, sizeof(buffer), 0);
	if (bytes_read > 0)
	{
		// std::cout << "Client (fd: " << this->fds[i].fd << "): " << buffer << std::endl;
		Http h((string(buffer)));
		h.respond(this->fds[i]);
	}
	else if (bytes_read == 0)
	{
		std::cout << "Client (fd: " << this->fds[i].fd << ") Disconnected" << std::endl;
		close(this->fds[i].fd);
		this->fds.erase(fds.begin() + i);
	}
}

void	TcpServer::runServer()
{
	while (1)
	{
		int poll_count = poll(this->fds.data(), this->fds.size(), -1);
		if (poll_count < 0)
		{
			std::cerr << "Failure: Poll" << std::endl;
			break ;
		}

		for(size_t i = 0; i < this->fds.size(); i++)
		{
			if (this->fds[i].revents & POLLIN)
			{
				if (std::find(this->server_fds.begin(), this->server_fds.end(), this->fds[i].fd) != server_fds.end())
					handleClientConnection(i);
				else
					handleClientMessage(i);
			}
		}
	}
}

TcpServer::~TcpServer()
{
	for(size_t i = 0; i < this->fds.size(); i++)
		close(fds[i].fd);
}
