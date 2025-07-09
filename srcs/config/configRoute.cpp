/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/07/09 22:25:55 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configRoute.hpp"

//--------------[OCCF]--------------------------------------------------

cfgRoute::cfgRoute() : _autoIndex(false), _autoIndex_flag(false), _clientBodySize(0) {
}

cfgRoute::cfgRoute(const cfgRoute &other)
: _path(other._path), _root_path(other._root_path), _index_path(other._index_path),
  _autoIndex(other._autoIndex), _autoIndex_flag(other._autoIndex_flag), _http_method(other._http_method),
  _redirection_path(other._redirection_path), _upload_path(other._upload_path),
  _clientBodySize(other._clientBodySize), _cgi_info(other._cgi_info) {
}

cfgRoute&	cfgRoute::operator=(const cfgRoute &other) {
	if (this != &other)
	{
		this->_path = other._path;
		this->_root_path = other._root_path;
		this->_index_path = other._index_path;
		this->_autoIndex = other._autoIndex;
		this->_autoIndex_flag = other._autoIndex_flag;
		this->_http_method = other._http_method;
		this->_redirection_path = other._redirection_path;
		this->_upload_path = other._upload_path;
		this->_clientBodySize = other._clientBodySize;
		this->_cgi_info = other._cgi_info;
	}
	return (*this);
}

cfgRoute::~cfgRoute() {
}

//--------------[Getter]--------------------------------------------------

string	cfgRoute::get_path() const {
	return (this->_path);
}

string	cfgRoute::get_rootPath() const {
	return (this->_root_path);
}

string	cfgRoute::get_indexPath() const {
	return (this->_index_path);
}

bool	cfgRoute::get_autoIndex() const {
	return (this->_autoIndex);
}

bool	cfgRoute::get_autoIndex_flag() const {
	return (this->_autoIndex_flag);
}

vector<string>	cfgRoute::get_httpMethod() const {
	return (this->_http_method);
}

string	cfgRoute::get_redirectionPath() const {
	return (this->_redirection_path);
}

string	cfgRoute::get_uploadPath() const {
	return (this->_upload_path);
}

size_t	cfgRoute::get_clientBodySize() const {
	return (this->_clientBodySize);
}

map<string,string>	cfgRoute::get_cgiInfo() const {
	return (this->_cgi_info);
}

//--------------[Getter]--------------------------------------------------

void	cfgRoute::set_rootPath(const string &path) {
	this->_root_path = path;
}

void	cfgRoute::set_indexPath(const string &path) {
	this->_index_path = path;
}

void	cfgRoute::set_autoIndex(bool state) {
	this->_autoIndex = state;
}

void	cfgRoute::set_clientSize(int size) {
	this->_clientBodySize = size;
}

void	cfgRoute::set_httpMethod(const vector<string> &methods) {
	this->_http_method = methods;
}

//--------------[Exception]--------------------------------------------------

cfgRoute::ArgError::ArgError(int id, string route, string dir, string msg) throw() {
	std::ostringstream	oss;
	oss << "Server(" << id << "): ";
	if (!route.empty())
		oss << "Location " << route << ": ";
	if (!dir.empty())
		oss << " [" << dir << "]: ";
	if (!msg.empty())
		oss << msg;
	this->_errMsg = oss.str();
}

const char*	cfgRoute::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

cfgRoute::ArgError::~ArgError() throw() {
}

//--------------[Functions]--------------------------------------------------

void	cfgRoute::handle_cgi(vector<string> &line, int id) {
	if (line.size() != 3)
		throw ArgError(id,this->_path, line[0], "Invalid number of arguments");
	size_t	pos = line[1].find(".");
	if (pos == std::string::npos)
		throw ArgError(id,this->_path, line[0], "First argument should provide file extension.");
	string	temp = line[1].substr(pos);
	this->_cgi_info[temp] = line[2];
}

void	cfgRoute::handle_client(vector<string> &line, int id) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");

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
		throw ArgError(id, this->_path, line[0], "Invalid size suffix.");
	this->_clientBodySize = byteSize;
}

void	cfgRoute::handle_upload(vector<string> &line, int id) {
	if (line.size() == 1 || line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");
	this->_upload_path = line[1];
}

void	cfgRoute::handle_redirect(vector<string> &line, int id) {
	if (line.size() == 1 || line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");
	this->_redirection_path = line[1];
}

void	cfgRoute::handle_methods(vector<string> &line, int id) {
	(void)id;
	if (line.size() < 2)
		return ;
	vector<string>::iterator	it = line.begin();
	++it;
	while (it != line.end())
	{
		this->_http_method.push_back(*it);
		++it;
	}
}

void	cfgRoute::handle_autoIndex(vector<string> &line, int id) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");

	if (line[1] == "on")
		this->_autoIndex = true;
	else if (line[1] == "off")
		this->_autoIndex = false;
	else
		throw ArgError(id, this->_path, line[0], "Invalid argument, on / off only.");
	this->_autoIndex_flag = true;
}

void	cfgRoute::handle_index(vector<string> &line, int id) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");
	this->_index_path = line[1];
}

void	cfgRoute::handle_root(vector<string> &line, int id) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw ArgError(id, this->_path, line[0], "Invalid number of arguments");
	this->_root_path = line[1];
}

void	cfgRoute::parseLocation(string &content, int server_id) {
	std::istringstream	iss(content);
	string				line;
	bool				firstLine = false;
	vector<string>		tokens_holder;
	map<string,void(cfgRoute::*)(vector<string>&,int)>	list;
	
	list["root"] = &cfgRoute::handle_root;
	list["index"] = &cfgRoute::handle_index;
	list["autoindex"] = &cfgRoute::handle_autoIndex;
	list["allowed_methods"] = &cfgRoute::handle_methods;
	list["return"] = &cfgRoute::handle_redirect;
	list["upload"] = &cfgRoute::handle_upload;
	list["client_max_body_size"] = &cfgRoute::handle_client;
	list["cgi"] = &cfgRoute::handle_cgi;

	while (std::getline(iss, line))
	{
		vector<string>	inline_directives = Utils::splitInline(line);
		vector<string>::iterator it2 = inline_directives.begin();
		while (it2 != inline_directives.end())
		{
			string	inlines = *it2;
			inlines = Utils::trim_whitespaces(inlines);
			inlines = Utils::trim_inlineComment(inlines);
			if (inlines.empty() || inlines == "{")
			{
				++it2;
				continue ;
			}
			else if (inlines == "}")
				break ;
			if (!firstLine)
			{
				firstLine = true;
				this->_path = splitRoute(inlines, server_id);
				if (this->_path.empty())
					throw ArgError(server_id, "", "", "Error! Can't split out route.");
				++it2;
				continue ;
			}
			if (inlines[inlines.size() - 1] != ';')
			{
				std::cout << red << "Error -> \"" << inlines << "\"" << reset << std::endl;
				throw ArgError(server_id, this->_path, "", "not terminated by \";\"");
			}
			tokens_holder = Utils::tokenizer(inlines);
			map<string,void(cfgRoute::*)(vector<string>&,int)>::iterator it = list.find(tokens_holder[0]);
			if (it != list.end())
				(this->*(it->second))(tokens_holder,server_id);
			else
			{
				std::cout << red << "Error -> \"" << tokens_holder[0] << "\"" << reset << std::endl;
				throw ArgError(server_id, this->_path, "", "unknown directive");
			}
			++it2;
		}
	}
}

string	cfgRoute::splitRoute(string &line, int id) {
	size_t	start;
	size_t	pos = line.find("/", 7);

	if (pos == std::string::npos)
		throw ArgError(id, "", "Location", "No route given!");
	start = pos;
	while (pos < line.size() && line[pos] != ' ' && line[pos] != '\t' && line[pos] != '{')
		++pos;
	return (line.substr(start, pos - start));
}

void	cfgRoute::displayContent(void) {
	std::cout << "\033[38;5;69m----- Route: \033[0m" << this->_path << " -----" << std::endl;
	std::cout << "\033[38;5;68mroot: \033[0m" << this->_root_path << std::endl;
	std::cout << "\033[38;5;68mindex: \033[0m" << this->_index_path << std::endl;
	std::cout << "\033[38;5;68mauto index: \033[0m" << (this->_autoIndex==true ? "on" : "off") << std::endl;
	{
		vector<string>::iterator	it = this->_http_method.begin();
		std::cout << "\033[38;5;68mallowed method: \033[0m";
		while (it != this->_http_method.end())
		{
			std::cout << *it << " ";
			++it;
		}
		std::cout << std::endl;
	}
	std::cout << "\033[38;5;68mredirect: \033[0m" << this->_redirection_path << std::endl;
	std::cout << "\033[38;5;68mupload: \033[0m" << this->_upload_path << std::endl;
	std::cout << "\033[38;5;68mclient_max_body_size: \033[0m" << this->_clientBodySize << std::endl;
	{
		
		std::cout << "\033[38;5;68mCGI: \033[0m";
		if (this->_cgi_info.empty())
			std::cout << "-" << std::endl;
		else
		{
			map<string,string>::iterator	it = (this->_cgi_info.begin());
			std::cout << it->first << " -> " << it->second << std::endl;
		}
	}
	std::cout << std::endl;
}
