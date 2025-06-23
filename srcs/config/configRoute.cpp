/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configRoute.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyean-wa < yyean-wa@student.42kl.edu.my    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 19:03:32 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/23 18:16:34 by yyean-wa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configRoute.hpp"

//--------------[OCCF]--------------------------------------------------

cfgRoute::cfgRoute()
{
	this->_request_path = "/index";
	this->_rootDir_path = "/";
	this->_indexFile_path = "index.html";
	this->_autoIndex_state = true;
	this->_http_method.push_back("GET");
	this->_http_method.push_back("POST");
	this->_http_method.push_back("DELETE");
	this->_redirection_path = "/error_page/42.html";
	this->_upload_path = "/upload";
}

cfgRoute::cfgRoute(const cfgRoute &other)
: _request_path(other._request_path), _rootDir_path(other._rootDir_path), _indexFile_path(other._indexFile_path)
, _autoIndex_state(other._autoIndex_state), _http_method(other._http_method), _redirection_path(other._redirection_path)
, _upload_path(other._upload_path), _cgi_info(other._cgi_info) {
}

cfgRoute&	cfgRoute::operator=(const cfgRoute &other) {
	if (this != &other)
	{
		this->_request_path = other._request_path;
		this->_rootDir_path = other._rootDir_path;
		this->_indexFile_path = other._indexFile_path;
		this->_autoIndex_state = other._autoIndex_state;
		this->_http_method = other._http_method;
		this->_redirection_path = other._redirection_path;
		this->_upload_path = other._upload_path;
		this->_cgi_info = other._cgi_info;
	}
	return (*this);
}

cfgRoute::~cfgRoute() {
}

//--------------[Getter]--------------------------------------------------

string	cfgRoute::get_requestPath() const {
	return (this->_request_path);
}

string	cfgRoute::get_rootDirPath() const {
	return (this->_rootDir_path);
}

string	cfgRoute::get_indexFilePath() const {
	return (this->_indexFile_path);
}

bool	cfgRoute::get_autoIndexState() const {
	return (this->_autoIndex_state);
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

vector<pair<string,string> >	cfgRoute::get_cgiInfo() const {
	return (this->_cgi_info);
}
