/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyan-bin <yyan-bin@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/28 13:15:10 by yyan-bin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

// cfgServer::cfgServer()
// {
// 	this->_Routes.push_back(cfgRoute());
// 	this->_id = 0;
// 	this->_serverName = "localhost";
// 	this->_clientBodySize = 1024;
// 	this->_hostPort.push_back("0.0.0.0:4242");
// 	this->_hostPort.push_back("127.0.0.1:8080");
// 	this->_errorCodes_map[404] = "/error_page/404.html";
// 	this->_errorCodes_map[403] = "/error_page/403.html";
// 	this->_errorCodes_map[409] = "/error_page/409.html";
// 	this->_errorCodes_map[500] = "/error_page/500.html";
// 	this->_errorCodes_map[413] = "/error_page/413.html";
// 	this->_root_path = ".";
// 	this->_autoIndex = true;
// 	this->_index_path = "upload.html";
// }

cfgServer::cfgServer()
{
	this->_Routes.push_back(cfgRoute(0));
	this->_Routes.push_back(cfgRoute(1));
	this->_Routes.push_back(cfgRoute(2));
	this->_id = 0;
	this->_serverName = "localhost";
	this->_clientBodySize = 1024;
	this->_hostPort.push_back("0.0.0.0:4242");
	this->_hostPort.push_back("127.0.0.1:8080");
	this->_errorCodes_map[404] = "/error_page/404.html";
	this->_errorCodes_map[403] = "/error_page/403.html";
	this->_errorCodes_map[409] = "/error_page/409.html";
	this->_errorCodes_map[500] = "/error_page/500.html";
	this->_errorCodes_map[413] = "/error_page/413.html";
	this->_root_path = "./servers/server1";
	this->_autoIndex = true;
	this->_index_path = "index.html";
}

cfgServer::cfgServer(int id) : _id(id), _clientBodySize(0) {
}

cfgServer::cfgServer(const cfgServer &other)
: _id(other._id), _serverName(other._serverName), _hostPort(other._hostPort),
  _errorCodes_map(other._errorCodes_map), _clientBodySize(other._clientBodySize),
  _root_path(other._root_path), _index_path(other._index_path), _autoIndex(other._autoIndex),
  _Routes(other._Routes)
{
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
		this->_autoIndex = other._autoIndex;
		this->_Routes = other._Routes;
		this->_autoIndex = other._autoIndex;
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

bool	cfgServer::get_autoIndex() const {
	return (this->_autoIndex);
}

//--------------[Exception]--------------------------------------------------

cfgServer::OtherError::OtherError(string msg) throw() : _errMsg("Server: " + msg) {
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

void	cfgServer::handle_serverIndex(vector<string> &line) {
		// std::cout << "server - index handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id,line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments!");
	
	this->_index_path = line[1];
}

void	cfgServer::handle_serverRoot(vector<string> &line) {
		// std::cout << "server - root handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id,line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments!");

	this->_root_path = line[1];
}

void	cfgServer::handle_clientBodySize(vector<string> &line) {
		// std::cout << "server - client max body size handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id,line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments!");

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
		throw cfgServer::ArgError(this->_id,line[0], "Invalid file size.");
	this->_clientBodySize = byteSize;
}

void	cfgServer::handle_errorCodes(vector<string> &line) {
		// std::cout << "error pages handler called!" << std::endl;
	if (line.size() < 3)
		throw cfgServer::ArgError(this->_id,line[0], "Not enough argument. Format: <error_code> <HTML_filepath>");
	else if (line.size() > 3)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments! Format: <error_code> <HTML_filepath>");

	if (line[1].find_first_not_of("0123456789") != std::string::npos)
		throw cfgServer::ArgError(this->_id,line[0], "Given error status code mixed with non-numeric value.");
	int	temp;
	temp = std::atoi(line[1].c_str());
	if (temp < 400 && temp > 600)
		throw cfgServer::ArgError(this->_id,line[0], "Given error status code is out of range!");
	this->_errorCodes_map[temp] = line[2];
}

void	cfgServer::handle_hostPort(vector<string> &line) {
		// std::cout << "socket address / port handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgServer::ArgError(this->_id,line[0], "Not enough argument.");
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments! Please seperate to different line.");

	if (line[1].find_first_not_of("0123456789:.") != std::string::npos)
		throw cfgServer::ArgError(this->_id,line[0], "Non-numeric value found?");
	size_t pos = line[1].find(":");
	if (pos != std::string::npos)
	{
		//check port without ip but have ":"?
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
			throw cfgServer::ArgError(this->_id,line[0], "No port given!");
	}
}

void	cfgServer::handle_serverName(vector<string> &line) {
		// std::cout << "server name handler called!" << std::endl;
	 //memo server_name accept to be NULL
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw cfgServer::ArgError(this->_id,line[0], "Too many arguments!");

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

		//memo display conntent pass to here
			// std::cout << "\n" << this->_id << "th" << std::endl;
			// std::cout << content;
		while (std::getline(iss, line))
		{
			//1 - pre-process the line
			line = Utils::trim_inlineComment(line);
			line = Utils::trim_whitespaces(line);

			//2 - basic checking
			if (!in_body && line.find("location") != std::string::npos) //found location block
				in_body = true;
			else if (!in_body && line[line.size()-1] != ';')
			{
				std::cout << line << std::endl;
				throw OtherError("Semicolon is missing!");
			}
			//todo what if i entered invalid scope again...
			//2.1 - handle location block
			if (in_body)
			{
				//memo display captured location block
					// std::cout << "\033[33m\t" << line << "\033[0m" << std::endl;
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
			//3 - tokenize line
			tokens_holder = Utils::tokenizer(line);
			if (tokens_holder.empty())
				throw OtherError("Tokenizing fail.");
			//memo visualize the generated tokens
				// vector<string>::iterator	token_it = tokens_holder.begin();
				// while (token_it != tokens_holder.end())
				// {
				// 	std::cout << *token_it << std::endl;
				// 	++token_it;
				// }
				// std::cout << std::endl;

			//4 - match directive & called the matched handler
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
	std::cout << "\033[38;5;68mindex: \033[0m" << this->_index_path << std::endl << std::endl;
}
