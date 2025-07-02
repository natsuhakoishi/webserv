/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyan-bin <yyan-bin@student.42kl.edu.my>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/07/03 01:11:21 by yyan-bin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configRoute.hpp"

//--------------[OCCF]--------------------------------------------------

cfgRoute::cfgRoute(int x): _autoIndex(false), _clientBodySize(0)
{
	if (x == 0)
	{
		this->_path = "/index";
		this->_root_path = "./servers/server1";
		this->_index_path = "upload.html";
		this->_autoIndex = false;
		this->_http_method.push_back("GET");
		this->_http_method.push_back("POST");
		this->_http_method.push_back("DELETE");
	}
	else if (x == 1)
	{
		this->_path = "/index2";
		this->_root_path = "./servers/server1";
		this->_index_path = "index.html";
		this->_autoIndex = false;
		this->_http_method.push_back("GET");
		this->_http_method.push_back("POST");
		this->_http_method.push_back("DELETE");
	}
	else if (x == 2)
	{
		this->_path = "/";
		this->_root_path = "./servers/server1";
		this->_index_path = "index.html";
		this->_clientBodySize = 10;
		this->_autoIndex = true;
		this->_http_method.push_back("GET");
		this->_http_method.push_back("POST");
	}
	else if (x == 3)
	{
		this->_path = "/clickHereToRedirectTo42";
		this->_root_path = "./servers/server1";
		this->_http_method.push_back("GET");
		this->_redirection_path = "/42/42.html";
	}
	else if (x == 4)
	{
		this->_path = "/upload";
		this->_root_path = "./servers/server1";
		this->_clientBodySize = 100000000;
		this->_http_method.push_back("GET");
		this->_http_method.push_back("POST");
		this->_upload_path = "/uploadTest";
		this->_redirection_path = "/index/yeah.html";
	}
}

cfgRoute::cfgRoute() : _autoIndex(false), _autoIndex_flag(false), _clientBodySize(0) {
	// std::cout << "\033[38;5;48m" << "Route construction trigger." << "\033[0m" << std::endl;
	this->_path = "/index";
	this->_root_path = ".";
	this->_index_path = "upload.html";
	this->_autoIndex = false;
	this->_http_method.push_back("GET");
	this->_http_method.push_back("POST");
	this->_http_method.push_back("DELETE");
	this->_redirection_path = "/error_page/42.html";
	this->_upload_path = "/upload";
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

int	cfgRoute::get_clientBodySize() const {
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
	if (line.size() < 2)
		throw cfgRoute::ArgError(this->_path, line[0], "No argument provided.");
	else if (line.size() > 3)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");

	//memo assume Python first
	size_t	pos = line[1].find(".");
	if (pos == std::string::npos)
	throw cfgRoute::ArgError(this->_path, line[0], "Please provide file extension.");
	string	temp = line[1].substr(pos, 3);
	if (temp != ".py")
		throw cfgRoute::ArgError(this->_path, line[0], "Server accept Python only.");

	this->_cgi_info[temp] = line[2];
}

void	cfgRoute::handle_client(vector<string> &line) {
	if (line.size() < 2)
		return ;
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
	if (line.size() == 1) //memo assume directive mentioned, but no arg given
		throw cfgRoute::ArgError(this->_path, line[0], "Argument missing!");
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	this->_upload_path = line[1];
}

void	cfgRoute::handle_redirect(vector<string> &line) {
	if (line.size() == 1) //memo assume directive mentioned, but no arg given
		throw cfgRoute::ArgError(this->_path, line[0], "Argument missing!");
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	this->_redirection_path = line[1];
}

void	cfgRoute::handle_methods(vector<string> &line) {
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

void	cfgRoute::handle_autoIndex(vector<string> &line) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");

	if (line[1] == "on")
		this->_autoIndex = true;
	else if (line[1] == "off")
		this->_autoIndex = false;
	else
		throw cfgRoute::ArgError(this->_path, line[0], "Auto index only accept on / off as argument!");
	this->_autoIndex_flag = true;
}

void	cfgRoute::handle_index(vector<string> &line) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	this->_index_path = line[1];
}

void	cfgRoute::handle_root(vector<string> &line) {
	if (line.size() < 2)
		return ;
	else if (line.size() > 2)
		throw cfgRoute::ArgError(this->_path, line[0], "Too many arguments!");
	this->_root_path = line[1];
}

void	cfgRoute::parseLocation(string &content) {
	//same, while loop + std::getline
	//throw the first line to splitRoute then move on
	//flag: indicate first line
	//ignore closed braces
	(void)content;
}

// string	cfgRoute::splitRoute(string &line) {
// 	//locate route after keyword
// 	//extract the route with start & index
// 	size_t	start;
// 	size_t pos = line.find("/", 7);
// 	if (pos == std::string::npos)
// 		throw cfgRoute::RouteError();
// 	start = pos;
// }