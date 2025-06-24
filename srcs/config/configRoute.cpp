/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/25 03:50:34 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configRoute.hpp"

//--------------[OCCF]--------------------------------------------------

cfgRoute::cfgRoute() {
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

const char*	cfgRoute::SemicolonMissing::what() const throw() {
	return ("Route: Semicolon is missing!");
}

const char*	cfgRoute::DirectiveError::what() const throw() {
	return ("Route: Invalid directive detected!");
}

cfgRoute::ArgError::ArgError(string msg) throw() : _errMsg("Route: argument invalid: " + msg) {
}

const char*	cfgRoute::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

//--------------[Functions]--------------------------------------------------
