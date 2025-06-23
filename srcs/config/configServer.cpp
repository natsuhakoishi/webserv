/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyean-wa < yyean-wa@student.42kl.edu.my    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/23 16:15:12 by yyean-wa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

cfgServer::cfgServer(string &content, int id) {
	//remove the body's close brace
	(void)content;
	// (void)id;
	this->_id = id;
	this->_hostPort.push_back("0.0.0.0:8080");
	this->_hostPort.push_back("127.0.0.1:9090");
	this->_hostPort.push_back("127.0.0.1:4242");
	this->_hostPort.push_back("172.17.0.1:4242");
}

cfgServer::cfgServer(const cfgServer &other)
: _id(other._id), _server_name(other._server_name), _hostPort(other._hostPort),
  _clientBodySize(other._clientBodySize), _errorCodes_map(other._errorCodes_map), _Routes(other._Routes) {
}

cfgServer&	cfgServer::operator=(const cfgServer &other) {
	if (this != &other)
	{
		this->_id = other._id;
		this->_server_name = other._server_name;
		this->_hostPort = other._hostPort;
		this->_clientBodySize = other._clientBodySize;
		this->_errorCodes_map = other._errorCodes_map;
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
	return (this->_server_name);
}

std::vector<string>	cfgServer::get_hostPort() const {
	return (this->_hostPort);
}

int	cfgServer::get_clientBodySize() const {
	return (this->_clientBodySize);
}

std::vector<pair<int,string> >	cfgServer::get_errorCodesMap() const {
	return (this->_errorCodes_map);
}

std::vector<cfgRoute>	cfgServer::get_routes() const {
	return (this->_Routes);
}
