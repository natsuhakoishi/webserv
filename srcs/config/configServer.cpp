/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 18:57:54 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/25 03:48:05 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "configServer.hpp"

//--------------[OCCF]--------------------------------------------------

cfgServer::cfgServer(int id) : _id(id), _clientBodySize(0) {
}

cfgServer::cfgServer(const cfgServer &other)
: _id(other._id), _serverName(other._serverName), _hostPort(other._hostPort),
  _errorCodes_map(other._errorCodes_map), _clientBodySize(other._clientBodySize),
  _root_path(other._root_path), _index_path(other._index_path), _Routes(other._Routes) {
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

//--------------[Exception]--------------------------------------------------

const char*	cfgServer::SemicolonMissing::what() const throw() {
	return ("Server: Semicolon is missing!");
}

const char*	cfgServer::DirectiveError::what() const throw() {
	return ("Server: Invalid directive detected!");
}

cfgServer::ArgError::ArgError(string msg) throw() : _errMsg("Server: argument invalid: " + msg) {
}

const char*	cfgServer::ArgError::what() const throw() {
	return (this->_errMsg.c_str());
}

//--------------[Functions]--------------------------------------------------

void	cfgServer::parseServer(string &content) {
	std::cout << "\n" << this->_id << "th" << std::endl;
	std::cout << content;
}
