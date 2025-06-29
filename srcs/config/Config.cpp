/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 07:46:00 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/30 05:26:21 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int Config::_blockCount = 0;

//--------------[OCCF]--------------------------------------------------

Config::Config()
{
	this->_blockCount = 1;
	this->_Servers.push_back(cfgServer());
}

Config::Config(string &filepath) {
	std::ifstream	infile;

	try
	{
		infile.open(filepath.c_str(), std::ios::in);
		if (!infile.fail())
		{
			if (infile.eof())
				throw ConfigError("Configuration file is empty!");
			scan_serverBody(infile);
			infile.close();
		}
		else
			throw ConfigError("Configuration file fail to open!");
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m\n";
	}
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

//--------------[Setter]--------------------------------------------------

void	Config::set_SocketTable(string newAddress, int id) {
	this->_SocketTable[newAddress].push_back(id);
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
			if (pos != std::string::npos && line[pos] == 's')
			{
				string temp_buf;
				temp_buf = Utils::trim_whitespaces(line);
				//todo will die to evil one-whole-line format, because this if statement
				if (temp_buf == "server" || temp_buf == "server {" || temp_buf == "server	{")
				{
					in_body = true;
					if (temp_buf != "server")
						brace_count++;
				}
				continue ;
			}
			else //memo mostly mean detect content outside of block, but buggy
				throw ConfigError("Undefined configuration.");
		}
		else if (in_body)
		{
			if (!first_Obrace)
			{
				if (brace_count)
					first_Obrace = true;
				else
				{
					size_t Obrace = line.find_first_not_of(" \n\t\r\v\f");
					if (Obrace != std::string::npos && line[Obrace] == '{')
					{
						first_Obrace = true;
						brace_count++;
						continue ;
					}
					else
						throw ConfigError("Other character after keyword. (Expect: Open Brace)");
				}
				server_body.append(line).append("\n");
			}
			else if (first_Obrace)
			{
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
				else
				{
					in_body = false;
					first_Obrace = false;
					this->_blockCount++;
					cfgServer a_block = cfgServer(this->_blockCount - 1);
					a_block.parseServer(server_body);
					// a_block.display_parsedContent();
					this->_Servers.push_back(a_block);
					server_body.clear();
				}
			}
		}
	}
	if (brace_count)
		throw ConfigError("Braces' issue");
	else if (!this->_blockCount)
		throw ConfigError("Couldn't find Server body.");
	this->print_parse(*this);
}

void	Config::print_parse(Config &the_parsed) {
	vector<cfgServer>::iterator	it = the_parsed._Servers.begin();
	while (it != the_parsed._Servers.end())
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

//--------------[Exception]--------------------------------------------------

Config::ConfigError::ConfigError(string msg) throw() : _errMsg("Config: " + msg) {
}

const char*	Config::ConfigError::what() const throw() {
	return (this->_errMsg.c_str());
}

Config::ConfigError::~ConfigError() throw() {
}
