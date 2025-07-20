#include "../../includes/TcpServer.hpp"

TcpServer::TcpServer(): config(NULL)
{
}

void	TcpServer::handleIpp(vector<std::pair<string, int> > ipp)
{
	vector<std::pair<string, int> > checked_ipp;

	for (size_t i = 0; i < ipp.size(); ++i)
	{
		string ip = ipp[i].first;
		int port = ipp[i].second;
		bool conflict = false;

		for (size_t j = 0; j < checked_ipp.size(); ++j)
		{
			string exist_ip = checked_ipp[j].first;
			int exist_port = checked_ipp[j].second;

			if (exist_port == port && (exist_ip == "0.0.0.0" || ip == "0.0.0.0" || exist_ip == ip))
			{
				conflict = true;
				break;
			}
		}
		if (!conflict)
		{
			this->ips.push_back(ip);
			this->ports.push_back(port);
			checked_ipp.push_back(ipp[i]);
		}
	}
}

TcpServer::TcpServer(Config *_cf): config(_cf)
{
	vector<cfgServer> server = _cf->get_Servers();
	vector<std::pair<string, int> > ipp;
	for (vector<cfgServer>::iterator it = server.begin(); it != server.end(); ++it)
	{
		vector<string> hostPorts = it->get_hostPort();
		vector<int> used_ports;

		for (size_t i = 0; i < hostPorts.size(); ++i)
		{
			size_t	separate_pos = hostPorts[i].find(":");
			if (separate_pos == string::npos)
			{
				cout << "Error: Invalid HostPort" << endl;
				exit(1);
			}
			string host = hostPorts[i].substr(0, separate_pos);
			string port_str = hostPorts[i].substr(separate_pos + 1);

			if (port_str.empty())
			{
				cout << "Invalid Port" << endl;
				exit(1);
			}

			int port = atoi(port_str.c_str());

			if (std::find(used_ports.begin(), used_ports.end(), port) != used_ports.end())
			{
				std::cerr << RED << "Failure: Port " << port << " is in used." << RESETEND << std::endl;
			}
			else
			{
				cout << GREEN << "Host " << host << " Port " << port << RESETEND << endl;
				ipp.push_back(std::make_pair(host, port));
				used_ports.push_back(port);
			}
		}
	}
	handleIpp(ipp);
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
		address.sin_port = htons(this->ports[i]);

		if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
		{
			std::cerr << "Failure: Bind on port " << this->ports[i] << std::endl;
			exit(1);
		}

		if (listen(server_fd, 3) < 0)
		{
			std::cerr << "Failure: Listen on port " << this->ports[i] << std::endl;
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

		std::cout << "Listening on port " << this->ports[i] << "..." << " | IP: " << this->ips[i] << std::endl;
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
		map<int, Http *>::iterator isExisit = this->httpMap.find(this->fds[i].fd);
		if (isExisit == this->httpMap.end())
		{
			// cout << YELLOW << "new http object created" << RESETEND;
			this->httpMap[this->fds[i].fd] = new Http(this->fds[i], *this->config);
		}
		Http *h = this->httpMap[this->fds[i].fd];
		h->parse((string(buffer, bytes_read)));

		/*
		if (h->getCanRespond() == true)
		{
			h->handleRequest();
			string respond = h->getRespond();
			send(this->fds[i].fd, respond.c_str(), respond.length(), 0);
			// cout << RED << "http object deleted" << RESETEND;
			delete h;
			this->httpMap.erase(this->fds[i].fd);
			close(this->fds[i].fd);
		*/          //comment below 6 row
		if (h->getIsRespond() == true)
		{
			// cout << RED << "http object deleted" << RESETEND;
			// delete h;
			// this->httpMap.erase(this->fds[i].fd);
			this->fds[i].revents = POLLOUT;
		}
	}
	else if (bytes_read == 0)
	{
		std::cout << "Client (fd: " << this->fds[i].fd << ") Disconnected" << std::endl;
		close(this->fds[i].fd);
		this->fds.erase(fds.begin() + i);
	}
}

void	TcpServer::handleClientSend(size_t i)
{
	Http *h = this->httpMap[this->fds[i].fd];

	string response = h->getResponse();

	cout << response << endl;
	ssize_t bytes_send = send(this->fds[i].fd, response.c_str(), response.length(), 0);

	if (bytes_send < 0)
	{
		std::cerr << "Error: Send: fd " << this->fds[i].fd << std::endl;
	}
	close(this->fds[i].fd);
	this->fds.erase(this->fds.begin() + i);

	delete h;
	this->httpMap.erase(this->fds[i].fd);
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
			else if (this->fds[i].revents & POLLOUT)
				handleClientSend(i);
		}
	}
}

TcpServer::~TcpServer()
{
	for(size_t i = 0; i < this->fds.size(); i++)
		close(fds[i].fd);
}
