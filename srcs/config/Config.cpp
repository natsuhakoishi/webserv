/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyan-bin <yyan-bin@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 07:46:00 by zgoh              #+#    #+#             */
/*   Updated: 2025/07/15 20:16:20 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int Config::_blockCount = 0;

//--------------[OCCF]--------------------------------------------------

Config::Config(string &filepath) {
	std::ifstream	infile;

	infile.open(filepath.c_str(), std::ios::in);
	if (!infile.fail())
	{
		if (infile.eof())
			throw ConfigError("Configuration file is empty!");
		scan_serverBody(infile);
		std::cout << "\033[38;5;11mRunning Config: \033[0m" << filepath << std::endl << std::endl;
		infile.close();
	}
	else
		throw ConfigError("Configuration file fail to open!");
}

Config::Config(const Config &other) 
: _Servers(other._Servers), _SocketTable(other._SocketTable) {
}

Config&	Config::operator=(const Config &other) {
	if (this != &other)
	{
		this->_blockCount = other._blockCount;
		this->_Servers = other._Servers;
		this->_SocketTable = other._SocketTable;
	}
	return (*this);
}

Config::~Config() {
}

//--------------[Getter]--------------------------------------------------

int	Config::get_blockCount() const {
	return (this->_blockCount);
}

vector<cfgServer>	Config::get_Servers() const {
	return (this->_Servers);
}

map<string,vector<int> >	Config::get_SocketTable() const {
	return (this->_SocketTable);
}

//--------------[Functions]--------------------------------------------------

void	Config::scan_serverBody(std::ifstream &infile) {
	string	server_body;
	string	line;
	bool	in_body = false;
	size_t	pos = 0;
	bool	first_Obrace = false;
	int		brace_count = 0;

	while (std::getline(infile, line))
	{
		line = Utils::trim_whitespaces(line);
		if (line.empty() || line.at(0) == '#' || Utils::is_blankLine(line))
			continue ;
		else if (!in_body)
		{
			pos = line.find_first_not_of(" \n\t\r\v\f");
			if (pos != std::string::npos && !line.compare(pos, 6, "server"))
				in_body = true;
			else
				throw ConfigError("Error: Couldn't find server directive / Brace not enclosed properly");
		}
		//found server directive
		//checking if it's in body; which mean exact follow by an open brace, ex: server {
		if (!first_Obrace && in_body)
		{
			size_t Obrace = line.find_first_not_of(" \n\t\r\v\f");
			size_t pos2 = line.find_first_not_of(" \n\t\r\v\f", pos+6);
			if (line == "server")
				continue ;
			else if (Obrace != std::string::npos && line[Obrace] == '{')
				first_Obrace = true; // brace on single line
			else if (pos2 != std::string::npos && (line.find("{", pos2) != std::string::npos || line.find(" {", pos2) != std::string::npos || line.find("	{", pos2)  != std::string::npos ))
				first_Obrace = true; // brace mixed inline w/ other content
			else
				throw ConfigError("Other character after keyword. (Expect: Open Brace)");
		}
		//continue parsing,current stage rely on brace counter
		//to validate all blocks enclosed properly / when a full server block is found
		size_t i = 0;
		while (i < line.size())
		{
			if (line[i] == '{')
				brace_count++;
			else if (line[i] == '}')
				brace_count--;
			++i;
		}
		if (brace_count)
		{
			server_body.append(line).append("\n");
			continue ;
		}
		else if (!brace_count && line.find("server", line.find_first_not_of(" \n\t\r\v\f")) != std::string::npos)
		{
			//urgh edge case, inline server block
			in_body = false;
			first_Obrace = false;
			this->_blockCount++;
			cfgServer a_server = cfgServer(this->_blockCount - 1);
			server_body.append(line).append("\n");
			a_server.parseServer(server_body);
			this->_Servers.push_back(a_server);
			server_body.clear();
		}
		else
		{
			in_body = false;
			first_Obrace = false;
			this->_blockCount++;
			cfgServer a_server = cfgServer(this->_blockCount - 1);
			a_server.parseServer(server_body);
			// a_server.display_parsedContent();
			this->_Servers.push_back(a_server);
			server_body.clear();
		}
	}
	if (brace_count)
		throw ConfigError("Brace: Not enclosed properly.");
	this->general_check();
	this->build_SocketTable();
	// this->print_ServerParsed();
	std::cout << "\033[0;32m-- Parsing Success! --\033[0m" << std::endl;
	// this->print_SocketTable();
}

void	Config::general_check() {
	vector<cfgServer>::iterator	it = this->_Servers.begin();
	while (it != this->_Servers.end())
	{
		cfgServer	&server = *it;
		vector<cfgRoute>	&temp_route = server.get_routes();
		vector<cfgRoute>::iterator	it2 = temp_route.begin();
		
		while (it2 != temp_route.end())
		{
			cfgRoute &current = *it2;
			if (current.get_rootPath().empty())
			{
				if (server.get_rootPath().empty())
					current.set_rootPath(".");
				else
					current.set_rootPath(server.get_rootPath());
			}
			if (current.get_indexPath().empty()) {
				current.set_indexPath(server.get_indexPath());}
			if (current.get_autoIndex_flag() == false)
				current.set_autoIndex(server.get_autoIndexS());
			if (current.get_clientBodySize() == 0)
			{
				//default value 2MB
				if (server.get_clientBodySize() == 0)
					current.set_clientSize(2097152);
				else
					current.set_clientSize(server.get_clientBodySize());
			}
			if (current.get_httpMethod().empty())
			{
				vector<string> temp;
				temp.push_back("GET");
				temp.push_back("POST");
				temp.push_back("DELETE");
				current.set_httpMethod(temp);
			}
			// else
			// {
			// 	const vector<string>& method = current.get_httpMethod();
			// 	if (std::find(method.begin(), method.end(), "POST") != method.end())
			// 	{
			// 		if (std::find(method.begin(), method.end(), "GET") == method.end())
			// 			Utils::print_warning(server.get_id(), current.get_path(), "allowed_methods", "GET is missing.");
			// 	}
			// }
			// if (!current.get_redirectionPath().empty())
			// 	Utils::print_warning(server.get_id(), current.get_path(), "return", "Others setup hidden by return.");
			++it2;
		}
		++it;
	}
}

void	Config::build_SocketTable()
{
	vector<cfgServer>::iterator	it = this->_Servers.begin();
	while (it != this->_Servers.end())
	{
		const vector<string> address = (*it).get_hostPort();
		
		vector<string>::const_iterator	it2 = address.begin();
		while (it2 != address.end())
		{
			this->_SocketTable[*it2].push_back((*it).get_id());
			++it2;
		}
		++it;
	}
	// vector<string>	used_port;
	// map<string, vector<int> >::iterator	table = this->_SocketTable.begin();
	// while (table != this->_SocketTable.end())
	// {
	// 	string port;
	// 	port = (*table).first.substr((*table).first.find(":") + 1);
	// 	vector<string>::iterator it3 = used_port.begin();
	// 	while (it3 != used_port.end())
	// 	{
	// 		if ((*it3).compare(port) == 0)
	// 			throw ConfigError("Detect bind to used port");
	// 		++it3;
	// 	}
	// 	used_port.push_back(port);
	// 	++table;
	// }
}

void	Config::print_ServerParsed() {
	vector<cfgServer>::iterator	it = this->_Servers.begin();
	while (it != this->_Servers.end())
	{
		(*it).display_parsedContent();
		vector<cfgRoute> temp_route = (*it).get_routes();
		vector<cfgRoute>::iterator	it2 = temp_route.begin();
		while (it2 != temp_route.end())
		{
			(*it2).displayContent();
			++it2;
		}
		std::cout << "\033[38;5;202m" << "/////////////////////////////////////////////////////////////////" << "\033[0m\n" << std::endl;
		++it;
	}
}

void	Config::print_SocketTable() {
	map<string, vector<int> >::iterator	test = this->_SocketTable.begin();
	std::cout << "-------Table--------\n";
	while (test != this->_SocketTable.end())
	{
		std::cout << test->first << "\033[0;96m -> \033[0m";
		vector<int>::iterator	test2 = test->second.begin();
		while (test2 != test->second.end())
		{
			std::cout << *test2 << " ";
			++test2;
		}
		std::cout << std::endl;
		++test;
	}
	std::cout << "--------------------\n";
}

//--------------[Exception]--------------------------------------------------

Config::ConfigError::ConfigError(string msg) throw() : _errMsg("Config: " + msg) {
}

const char*	Config::ConfigError::what() const throw() {
	return (this->_errMsg.c_str());
}

Config::ConfigError::~ConfigError() throw() {
}

// Config::CheckingError::CheckingError(int id, string path, string dir, string msg) throw() {
// 	std::ostringstream	oss;
// 	oss << "Server(" << id << ") Location <" << path << ">:  [" << dir << "]: argument invalid: " << msg;
// 	this->_errMsg = oss.str();
// }

// const char*	Config::CheckingError::what() const throw() {
// 	return (this->_errMsg.c_str());
// }

// Config::CheckingError::~CheckingError() throw() {
// }
