/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyean-wa < yyean-wa@student.42kl.edu.my    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/23 18:42:33 by yyean-wa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

cfgServer::cfgServer()
{
	cfgRoute *temp = new cfgRoute();

	this->_Routes.push_back(*temp);
	this->_id = 0;
	this->_server_name = "localhost";
	this->_hostPort.push_back("0.0.0.0:4242");
	this->_hostPort.push_back("127.0.0.1:8080");
	this->_clientBodySize = 1024;
	this->_errorCodes_map[404] = "/error_page/404.html";
	delete temp;
}

cfgServer::cfgServer(string &content, int id) {
	//remove the body's close brace
	(void)content;
	// (void)id;
	this->_id = id;
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

cfgServer::~cfgServer()
{
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

std::map<int, string>	cfgServer::get_errorCodesMap() const {
	return (this->_errorCodes_map);
}

std::vector<cfgRoute>	cfgServer::get_routes() const {
	return (this->_Routes);
}
