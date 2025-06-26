/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/27 03:23:44 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configRoute.hpp"

//--------------[OCCF]--------------------------------------------------

cfgRoute::cfgRoute() {
		// this->_path = "/index";
		// this->_root_path = "/";
		// this->_index_path = "index.html";
		// this->_autoIndex = true;
		// this->_http_method.push_back("GET");
		// this->_http_method.push_back("POST");
		// this->_http_method.push_back("DELETE");
		// this->_redirection_path = "/error_page/42.html";
		// this->_upload_path = "/upload";
	// std::cout << "\033[38;5;48m" << "Route construction trigger." << "\033[0m" << std::endl;
}

cfgRoute::cfgRoute(const cfgRoute &other)
: _path(other._path), _root_path(other._root_path), _index_path(other._index_path),
  _autoIndex(other._autoIndex), _http_method(other._http_method), _redirection_path(other._redirection_path),
  _upload_path(other._upload_path), _clientBodySize(other._clientBodySize), _cgi_info(other._cgi_info) {
}

cfgRoute&	cfgRoute::operator=(const cfgRoute &other) {
	if (this != &other)
	{
		this->_path = other._path;
		this->_root_path = other._root_path;
		this->_index_path = other._index_path;
		this->_autoIndex = other._autoIndex;
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

string	cfgRoute::get_path() const { //update: rename
	return (this->_path);
}

string	cfgRoute::get_rootPath() const { //update: rename
	return (this->_root_path);
}

string	cfgRoute::get_indexPath() const { //update: rename
	return (this->_index_path);
}

bool	cfgRoute::get_autoIndex() const { //update: rename
	return (this->_autoIndex);
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

int	cfgRoute::get_clientBodySize() const {
	return (this->_clientBodySize);
}

map<string,string>	cfgRoute::get_cgiInfo() const { //update: return type changed
	return (this->_cgi_info);
}

//--------------[Exception]--------------------------------------------------

const char*	cfgRoute::RouteError::what() const throw() {
	return ("Route: No route specified in the location block!");
}

const char*	cfgRoute::SemicolonMissing::what() const throw() {
	return ("Route: Semicolon is missing!");
}

const char*	cfgRoute::DirectiveError::what() const throw() {
	return ("Route: Invalid directive detected!");
}

cfgRoute::ArgError::ArgError(string route, string dir, string msg) throw()
: _errMsg("Location " + route + ":  [" + dir + "]: argument invalid: " + msg) {
}

const char*	cfgRoute::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

cfgRoute::ArgError::~ArgError() throw() {
}

//--------------[Functions]--------------------------------------------------

void	cfgRoute::handle_cgi(vector<string> &line) {
		std::cout << "CGI handler called!" << std::endl;
	(void)line;
}

void	cfgRoute::handle_client(vector<string> &line) {
		// std::cout << "client max body size handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgRoute::ArgError(this->_path, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");

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
		throw cfgRoute::ArgError(this->_path, line[0], "Invalid file size.");
	this->_clientBodySize = byteSize;
}

void	cfgRoute::handle_upload(vector<string> &line) {
		std::cout << "upload handler called!" << std::endl;
	(void)line;
}

void	cfgRoute::handle_redirect(vector<string> &line) {
		std::cout << "return handler called!" << std::endl;
	(void)line;
}

void	cfgRoute::handle_methods(vector<string> &line) {
		std::cout << "allowed method handler called!" << std::endl;
	(void)line;
}

void	cfgRoute::handle_autoIndex(vector<string> &line) {
		// std::cout << "autoIndex handler called!" << std::endl;
	//memo if no value then autoindex off
	if (line.size() < 2)
	{
		std::cout << "\033[31mWarning: auto index argument not given. Set as default value (off).\033[0m\n";
		this->_autoIndex = false;
		return ;
	}
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	
	if (line[1] == "on")
		this->_autoIndex = true;
	else if (line[1] == "off")
		this->_autoIndex = false;
	else
		throw cfgRoute::ArgError(this->_path, line[0], "Auto index only accept on / off as argument!");
}

void	cfgRoute::handle_index(vector<string> &line) {
		// std::cout << "index handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgRoute::ArgError(this->_path, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	
	this->_index_path = line[1];
}

void	cfgRoute::handle_root(vector<string> &line) {
		// std::cout << "root handler called!" << std::endl;
	if (line.size() < 2)
		throw cfgRoute::ArgError(this->_path, line[0], "No argument provided.");
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");

	this->_root_path = line[1];
}

void	cfgRoute::parseLocation(string &content) {
	std::istringstream	iss(content);
	string				line;
	bool				firstLine = false;
	vector<string>		tokens_holder;
	map<string,void(cfgRoute::*)(vector<string>&)>	list;

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
		if (!firstLine)
		{
			firstLine = true;
			this->_path = splitRoute(line);
			if (this->_path.empty())
				throw cfgRoute::RouteError();
			continue ;
		}
		line = Utils::trim_inlineComment(line);
		line = Utils::trim_whitespaces(line);
		if (line == "}")
			break ;
		if (line[line.size()-1] != ';')
			throw SemicolonMissing();

		tokens_holder = Utils::tokenizer(line);
		map<string,void(cfgRoute::*)(vector<string>&)>::iterator it = list.find(tokens_holder[0]);
		if (it != list.end())
			(this->*(it->second))(tokens_holder);
		else
		{
			std::cout << "\033[31mError -> \"" << tokens_holder[0] << "\"\033[0m" << std::endl;
			throw cfgRoute::DirectiveError();
		}
	}
}

string	cfgRoute::splitRoute(string &line) {
	size_t	start;
	size_t	pos = line.find("/", 7);

	if (pos == std::string::npos)
		throw cfgRoute::RouteError();
	start = pos;
	while (pos < line.size() && line[pos] != ' ' && line[pos] != '\t')
		++pos;
	return (line.substr(start, pos-start));
}