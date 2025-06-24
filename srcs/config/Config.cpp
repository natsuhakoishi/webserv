/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 07:46:00 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/25 03:48:11 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int Config::_blockCount = 0;

//--------------[OCCF]--------------------------------------------------

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

//memo blocking basic test
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
		if (line.empty() || line.at(0) == '#' || Utils::is_blankLine(line)) //no matter in block or not, both this need to skip
			continue ;
		else if (!in_body) //searching for Server body
		{
			//keyword determine where the body start; so find the keyword first
			pos = line.find_first_not_of(" \n\t\r\v\f");
			if (pos != std::string::npos && line[pos] == 's')
			{
				string temp_buf;
				temp_buf = Utils::trim_line(line);
				//todo will die to evil one-whole-line format, because this if statement
				if (temp_buf == "server" || temp_buf == "server {" || temp_buf == "server	{")
				{
					in_body = true;
					if (temp_buf != "server")
						brace_count++;
					//memo server_body.append(temp_buf).append("\n");
				}
				continue ;
			}
			else //mostly mean detect content outside of block
				throw ConfigError("Undefined configuration.");
		}
		else if (in_body)
		{
			//found keyword then find for the open brace; if no one / not the first one then fail
			//to ensure body properly enclosed in paired braces
			if (!first_Obrace)
			{
				if (brace_count)//already found the open brace when searching & valid for keyword; skip for efficient
					first_Obrace = true;
				else
				{
					size_t Obrace = line.find_first_not_of(" \n\t\r\v\f");
					if (Obrace != std::string::npos)
					{
						//found the open brace as first character(other than blankspace); sequence correct
						first_Obrace = true;
						brace_count++;
						//memo server_body.append(line).append("\n");
					}
					else
						throw ConfigError("Other character after keyword. (Expect: Open Brace)");
				}
				server_body.append(line).append("\n");
			}
			//found the valid format, can get the body's content
			//what happen now? keep track of brace amount and appending line into server_body
			//if brace_count hit 0, means is end of one server block; stop and pass to next step
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
					this->_Servers.push_back(a_block);
					server_body.clear();
				}
			}
		}
	}
	if (brace_count)
		throw ConfigError("Braces' issue");
	if (!this->_blockCount)
		throw ConfigError("Couldn't find Server body.");
}

//--------------[Exception]--------------------------------------------------

Config::ConfigError::ConfigError(string msg) throw() : _errMsg("Config: " + msg) {
}

const char*	Config::ConfigError::what() const throw() {
	return (this->_errMsg.c_str());
}
