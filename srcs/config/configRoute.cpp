/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/27 02:33:30 by zgoh             ###   ########.fr       */
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

void	cfgRoute::parseLocation(string &content) {
	std::istringstream	iss(content);
	string				line;
	bool				firstLine = false;
	
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
		//called tokenizer
		//loop list and called each handler
		std::cout << "\"path: " << this->_path << "\"" << std::endl;
		std::cout << line << std::endl;
	}
	std::cout << "-------------------" << std::endl;
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