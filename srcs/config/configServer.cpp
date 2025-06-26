/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/27 02:28:12 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

cfgServer::cfgServer()
{
	cfgRoute *temp = new cfgRoute();

	this->_Routes.push_back(*temp);
	this->_id = 0;
	this->_serverName = "localhost";
	this->_clientBodySize = 1024;
	this->_hostPort.push_back("0.0.0.0:4242");
	this->_hostPort.push_back("127.0.0.1:8080");
	this->_errorCodes_map[404] = "/error_page/404.html";
	delete temp;
}

cfgServer::cfgServer(int id) : _id(id), _clientBodySize(0), _autoIndexS(false) {
}

cfgServer::cfgServer(const cfgServer &other)
: _id(other._id), _serverName(other._serverName), _hostPort(other._hostPort),
  _errorCodes_map(other._errorCodes_map), _clientBodySize(other._clientBodySize),
  _root_path(other._root_path), _index_path(other._index_path), _autoIndexS(other._autoIndexS),
  _Routes(other._Routes) {
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
		this->_autoIndexS = other._autoIndexS;
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

map<int,string>	cfgServer::get_errorCodesMap() const {
	return (this->_errorCodes_map);
}

int	cfgServer::get_clientBodySize() const {
	return (this->_clientBodySize);
}

string	cfgServer::get_rootPath() const {
	return (this->_root_path);
}

string	cfgServer::get_indexPath() const {
	return (this->_index_path);
}

bool	cfgServer::get_autoIndexS() const {
	return (this->_autoIndexS);
}

vector<cfgRoute>	cfgServer::get_routes() const {
	return (this->_Routes);
}

//--------------[Exception]--------------------------------------------------

cfgServer::OtherError::OtherError(int id, int nl, string msg) throw() {
	std::ostringstream	oss;
	oss << "Server(" << id << "): " << msg << " <-- Ln " << nl;
	this->_errMsg = oss.str();
}

const char*	cfgServer::OtherError::what() const throw() {
	return (this->_errMsg.c_str());
}

cfgServer::OtherError::~OtherError() throw() {
}

const char*	cfgServer::DirectiveError::what() const throw() {
	return ("Server: Invalid directive detected!");
}

cfgServer::ArgError::ArgError(int id, string dir, string msg) throw() {
	std::ostringstream	oss;
	oss << "Server(" << id << "):  [" << dir << "]: argument invalid: " << msg;
	this->_errMsg = oss.str();
}

const char*	cfgServer::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

cfgServer::ArgError::~ArgError() throw() {
}

//--------------[Functions]--------------------------------------------------

void	cfgServer::handle_autoIndex(vector<string> &line) {
		//std::cout << "server - autoIndex handler called!" << std::endl;
	//memo if no value then autoindex off
	if (line.size() < 2)
	{
		std::cout << "\033[31mWarning: auto index argument not given. Set as default value (off).\033[0m\n";
		this->_autoIndexS = false;
		return ;
	}
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments!");
	
	if (line[1] == "on")
		this->_autoIndexS = true;
	else if (line[1] == "off")
		this->_autoIndexS = false;
	else
		throw cfgServer::ArgError(this->_id, line[0], "Auto index only accept on / off as argument!");
}

void	cfgServer::handle_serverIndex(vector<string> &line) {
		// std::cout << "server - index handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments!");
	
	this->_index_path = line[1];
}

void	cfgServer::handle_serverRoot(vector<string> &line) {
		// std::cout << "server - root handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments!");

	this->_root_path = line[1];
}

void	cfgServer::handle_clientBodySize(vector<string> &line) {
		// std::cout << "server - client max body size handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments!");

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
		throw cfgServer::ArgError(this->_id, line[0], "Invalid file size.");
	this->_clientBodySize = byteSize;
}

void	cfgServer::handle_errorCodes(vector<string> &line) {
		// std::cout << "error pages handler called!" << std::endl;
	if (line.size() < 3)
		throw cfgServer::ArgError(this->_id, line[0], "Not enough argument. Format: <error_code> <HTML_filepath>");
	else if (line.size() > 3)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments! Format: <error_code> <HTML_filepath>");

	if (line[1].find_first_not_of("0123456789") != std::string::npos)
		throw cfgServer::ArgError(this->_id, line[0], "Given error status code mixed with non-numeric value.");
	int	temp;
	temp = std::atoi(line[1].c_str());
	if (temp < 400 && temp > 600)
		throw cfgServer::ArgError(this->_id, line[0], "Given error status code is out of range!");
	this->_errorCodes_map[temp] = line[2];
}

void	cfgServer::handle_hostPort(vector<string> &line) {
		// std::cout << "socket address / port handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id, line[0], "Not enough argument.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments! Please seperate to different line.");

	if (line[1].find_first_not_of("0123456789:.") != std::string::npos)
		throw cfgServer::ArgError(this->_id, line[0], "Non-numeric value found?");
	size_t pos = line[1].find(":");
	if (pos != std::string::npos)
	{
		if (line[1].find_first_not_of(":") == std::string::npos)
			throw cfgServer::ArgError(this->_id, line[0], "Don't pass weird argument!");
		this->_hostPort.push_back(line[1]);
	}
	else
	{
		if (line[1].find(".") == std::string::npos)
		{
			string	temp = "0.0.0.0";
			temp.append(":").append(line[1]);
			this->_hostPort.push_back(temp);
		}
		else
			throw cfgServer::ArgError(this->_id, line[0], "No port given!");
	}
}

void	cfgServer::handle_serverName(vector<string> &line) {
		// std::cout << "server name handler called!" << std::endl;
	 //memo server_name accept to be NULL
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id, line[0], "Too many arguments!");

	this->_serverName = line[1];
}

void	cfgServer::parseServer(string &content) {
	std::istringstream	iss(content);
	string				line;
	int					nl = 1;
	bool				in_body = false;
	string				location_body;
	vector<string>		tokens_holder;
	map<string,void(cfgServer::*)(vector<string>&)>	list;

	list["server_name"] = &cfgServer::handle_serverName;
	list["listen"] = &cfgServer::handle_hostPort;
	list["client_max_body_size"] = &cfgServer::handle_clientBodySize;
	list["error_page"] = &cfgServer::handle_errorCodes;
	list["root"] = &cfgServer::handle_serverRoot;
	list["index"] = &cfgServer::handle_serverIndex;
	list["autoindex"] = &cfgServer::handle_autoIndex;

	while (std::getline(iss, line))
	{
		//memo have offset when server and first open brace not in same line
		++nl;
		line = Utils::trim_inlineComment(line);
		line = Utils::trim_whitespaces(line);

		if (!in_body)
		{
			if (line.find("location") != std::string::npos)
				in_body = true;
			else if (line[line.size()-1] != ';' && !in_body)
				throw OtherError(this->_id, nl, "Semicolon is missing!");
		}
		if (in_body)
		{
			location_body.append(line).append("\n");
			if (line.find_last_of("}") != std::string::npos)
			{
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

void	cfgServer::display_parsedContent(void) {
	std::cout << "\033[38;5;69m----- Server ID: " << this->_id << " -----\033[0m" << std::endl;
	std::cout << "\033[38;5;68mserver_name: \033[0m";
	if (this->_serverName.empty())
		std::cout << "- \n";
	else
		std::cout << this->_serverName << std::endl;
	{
		vector<string>::iterator it = this->_hostPort.begin();
		while (it != this->_hostPort.end())
		{
			std::cout << "\033[38;5;68mlisten on: \033[0m" << *it << std::endl;
			++it;
		}
	}
	{
		map<int,string>::iterator it = this->_errorCodes_map.begin();
		while (it != this->_errorCodes_map.end())
		{
			std::cout << "\033[38;5;68merror_page: \033[0m[" << it->first << "] " << it->second << std::endl;
			++it;
		}
	}
	std::cout << "\033[38;5;68mclient_max_body_size: \033[0m" << this->_clientBodySize << std::endl;
	std::cout << "\033[38;5;68mroot: \033[0m" << this->_root_path << std::endl;
	std::cout << "\033[38;5;68mindex: \033[0m" << this->_index_path << std::endl;
	std::cout << "\033[38;5;68mauto index: \033[0m" << (_autoIndexS==true ? "on" : "off") << std::endl << std::endl;
}
