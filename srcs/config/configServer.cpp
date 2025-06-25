/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/26 00:56:32 by zgoh             ###   ########.fr       */
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

cfgServer::ArgError::ArgError(string dir, string msg) throw()
: _errMsg("Server: [" + dir + "]: argument invalid: " + msg) {
}

const char*	cfgServer::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

//--------------[Functions]--------------------------------------------------

void	cfgServer::handle_serverIndex(vector<string> &line) {
		// std::cout << "server - index handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(line[0], "Too many arguments!");
	
	this->_index_path = line[1];
}

void	cfgServer::handle_serverRoot(vector<string> &line) {
		// std::cout << "server - root handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(line[0], "Too many arguments!");

	this->_root_path = line[1];
}

void	cfgServer::handle_clientBodySize(vector<string> &line) {
		// std::cout << "server - client max body size handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(line[0], "Too many arguments!");

	int		byteSize;
	string	suffix;
	size_t	pos = line[1].find_first_not_of("0123456789");

	if (pos != std::string::npos)
		suffix = line[1].substr(pos);
	else
	{
		this->_clientBodySize = std::atoi(line[1].c_str());
		return ;
	}
	for (size_t i = 0; i < suffix.size(); ++i)
		suffix[i] = std::tolower(suffix[i]);
	byteSize = std::atoi(line[1].c_str());
	if (suffix == "k" || suffix == "kb")
		byteSize = byteSize * 1024;
	else if (suffix == "m" || suffix == "mb")
		byteSize = byteSize * (1024 * 1024);
	else if (suffix == "g" || suffix == "gb")
		byteSize = byteSize * (1024 * 1024 * 1024);
	else
		throw cfgServer::ArgError(line[0], "Invalid file size.");
	this->_clientBodySize = byteSize;
}

void	cfgServer::handle_errorCodes(vector<string> &line) {
		// std::cout << "error pages handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(line[0], "No argument provided.");
	else if (line.size() > 3)
		throw cfgServer::ArgError(line[0], "Too many arguments! Format: <error_code> <HTML_filepath>");
	int	temp;
		
	temp = std::atoi(line[1].c_str());
	//todo the range need confirm again
	if (temp < 400 && temp > 600)
		throw cfgServer::ArgError(line[0], "Given error status code is out of range!");
	this->_errorCodes_map[temp] = line[2];
	//check the path provided? //memo atleast not now, later when validate stage
}

void	cfgServer::handle_hostPort(vector<string> &line) {
		// std::cout << "socket address / port handler called!" << std::endl;
		// std::cout << "\033[38;5;199m" << "NO!!!!!! IS TODO FAKKK MA LIFE <-- socket address, don't mind\033[0m" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(line[0], "Too many arguments! Please seperate to different line.");

	//check the format, accept socket address or port only
	//if is only port, then combine the port with ip 0.0.0.0, ex. 0.0.0.0:port
	//todo build up the table in cfgServer, ideally should be do here, so..
}

void	cfgServer::handle_serverName(vector<string> &line) {
		// std::cout << "server name handler called!" << std::endl;
	if (line.size() < 2) //memo server_name accept NULL
		return ;
	else if (line.size() > 2)
		throw cfgServer::ArgError(line[0], "Too many arguments!");

	//todo handle multiple server name
	this->_serverName = line[1];
}

void	cfgServer::parseServer(string &content) {
	std::istringstream	iss(content);
	string				line;
	map<string,void(cfgServer::*)(vector<string>&)>	list;
	vector<string>							tokens_holder;
	string	location_body;
	bool	in_body = false;

	list["server_name"] = &cfgServer::handle_serverName;
	list["listen"] = &cfgServer::handle_hostPort;
	list["client_max_body_size"] = &cfgServer::handle_clientBodySize;
	list["error_page"] = &cfgServer::handle_errorCodes;
	list["root"] = &cfgServer::handle_serverRoot;
	list["index"] = &cfgServer::handle_serverIndex;

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
			{
				std::cout << line << std::endl;
				throw SemicolonMissing();
			}
			//collect whole location block and throw to cfgRoute
			//same way of handling server block
			if (in_body)
			{
				//memo display location block captured
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
			tokens_holder = Utils::tokenizer(line);
			//memo visualize the generated tokens
				// vector<string>::iterator	token_it = tokens_holder.begin();
				// while (token_it != tokens_holder.end())
				// {
				// 	std::cout << *token_it << std::endl;
				// 	++token_it;
				// }
				// std::cout << std::endl;

			//loop through list to match
			map<string,void(cfgServer::*)(vector<string>&)>::iterator it = list.find(tokens_holder[0]);
			if (it != list.end())
				(this->*(it->second))(tokens_holder);
			else
			{
				std::cout << "\033[31mError -> \"" << tokens_holder[0] << "\"\033[0m" << std::endl;
				throw cfgServer::DirectiveError();
			}
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[31m" << e.what() << "\033[0m\n";
	}
}

void	cfgServer::display_parsedContent(void) {
	std::cout << "\033[38;5;69m-----" << this->_id + 1 << "th-----\033[0m" << std::endl;
	std::cout << "server_name: ";
	if (this->_serverName.empty())
		std::cout << "- \n";
	else
		std::cout << this->_serverName << std::endl;
	{
		vector<string>::iterator it = this->_hostPort.begin();
		while (it != this->_hostPort.end())
		{
			std::cout << "listen on: " << *it << std::endl;
			++it;
		}
	}
	{
		map<int,string>::iterator it = this->_errorCodes_map.begin();
		while (it != this->_errorCodes_map.end())
		{
			std::cout << "error_page: [" << it->first << "] " << it->second << std::endl;
			++it;
		}
	}
	std::cout << "client_max_body_size: " << this->_clientBodySize << std::endl;
	std::cout << "root: " << this->_root_path << std::endl;
	std::cout << "index: " << this->_index_path << std::endl << std::endl;
}
