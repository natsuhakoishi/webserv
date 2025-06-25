/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/25 22:09:47 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

cfgServer::cfgServer(int id) : _id(id), _clientBodySize(0) {
}

cfgServer::cfgServer(const cfgServer &other)
: _id(other._id), _serverName(other._serverName), _hostPort(other._hostPort),
  _errorCodes_map(other._errorCodes_map), _clientBodySize(other._clientBodySize),
  _root_path(other._root_path), _index_path(other._index_path), _Routes(other._Routes) {
}

cfgServer&	cfgServer::operator=(const cfgServer &other) {
	if (this != &other)
	{
		this->_id = other._id;
		this->_serverName = other._serverName;
		this->_hostPort = other._hostPort;
		this->_errorCodes_map = other._errorCodes_map;
		this->_clientBodySize = other._clientBodySize;
		this->_root_path = other._root_path;
		this->_index_path = other._index_path;
		this->_Routes = other._Routes;
	}
	return (*this);
}

cfgServer::~cfgServer() {
}

//--------------[Getter]--------------------------------------------------

int	cfgServer::get_id() const {
	return (this->_id);
}

string	cfgServer::get_serverName() const {
	return (this->_serverName);
}

vector<string>	cfgServer::get_hostPort() const {
	return (this->_hostPort);
}

map<int,string>	cfgServer::get_errorCodesMap() const { //update: return type changed
	return (this->_errorCodes_map);
}

int	cfgServer::get_clientBodySize() const {
	return (this->_clientBodySize);
}

string	cfgServer::get_rootPath() const { //update: newly added
	return (this->_root_path);
}

string	cfgServer::get_indexPath() const { //update: newly added
	return (this->_index_path);
}

vector<cfgRoute>	cfgServer::get_routes() const {
	return (this->_Routes);
}

//--------------[Exception]--------------------------------------------------

const char*	cfgServer::SemicolonMissing::what() const throw() {
	return ("Server: Semicolon is missing!");
}

const char*	cfgServer::DirectiveError::what() const throw() {
	return ("Server: Invalid directive detected!");
}

cfgServer::ArgError::ArgError(string msg) throw() : _errMsg("Server: argument invalid: " + msg) {
}

const char*	cfgServer::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

//--------------[Functions]--------------------------------------------------

void	cfgServer::parseServer(string &content) {
	std::istringstream	iss(content);
	string				line;
	map<string,void(cfgServer::*)(string&)>	list;
	vector<string>							tokens_holder;
	string	location_body;
	bool	in_body = false;

	// list["server_name"] = &handle_serverName;
	// list["listen"] = &handle_hostPort;
	// list["client_max_body_size"] = &handle_clientBodySize;
	// list["error_page"] = &handle_errorCodes;
	// list["root"] = &handle_serverRoot;
	// list["index"] = &handle_serverIndex;

	try
	{
		//memo display conntent pass to here
			// std::cout << "\n" << this->_id << "th" << std::endl;
			// std::cout << content;
		while (std::getline(iss, line))
		{
			//pre-process the line
			line = Utils::trim_inlineComment(line);
			line = Utils::trim_whitespaces(line);

			//checking
			if (!in_body && line.find("location /") != std::string::npos) //found location block
				in_body = true;
			else if (!in_body && line[line.size()-1] != ';')
				throw SemicolonMissing();

			//collect whole location block and throw to cfgRoute
			//same way of handling server block
			if (in_body)
			{
					// std::cout << "\033[33m\t" << line << "\033[0m" << std::endl;
				location_body.append(line).append("\n");
				if (line.find_last_of("}") != std::string::npos)
				{
					location_body.append("}");
					in_body = false;
					cfgRoute a_block_found = cfgRoute();
					a_block_found.parseLocation(location_body);
					this->_Routes.push_back(a_block_found);
					location_body.clear();
				}
				continue ;
			}
			//todo multiple directive inline splitter
			//tokenize the line
			tokens_holder = Utils::tokenizer(line);
			//memo visualize the tokens
				// vector<string>::iterator	it = tokens_holder.begin();
				// while (it != tokens_holder.end())
				// {
				// 	std::cout << *it << std::endl;
				// 	++it;
				// }
				// std::cout << std::endl;
			//loop through list to match
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m\n";
	}
}
