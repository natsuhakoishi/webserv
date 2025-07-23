/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:43:14 by zgoh              #+#    #+#             */
/*   Updated: 2025/07/24 03:42:50 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helper.hpp"

namespace Utils {

UtilsError::UtilsError(string msg) throw()
: _errMsg(msg) {
}

const char*	UtilsError::what() const throw() {
	return (this->_errMsg.c_str());
}

UtilsError::~UtilsError() throw() {
}

bool	is_blankLine(string &line) {
	string::iterator it = line.begin();
	
	while (it != line.end())
	{
		string c;
		c = *it;
		if (c.find_first_not_of(" \n\t\r\v\f") != string::npos)
		return (false);
		++it;
	}
	return (true);
}

string	trim_whitespaces(string &line) {
	size_t front = line.find_first_not_of(" \n\t\r\v\f");
	size_t behind = line.find_last_not_of(" \n\t\r\v\f");

	if (front == std::string::npos || behind == std::string::npos)
		return ("");
	return (line.substr(front, behind - front + 1));
}

string trim_inlineComment(string &line) {
	size_t hash = line.find('#');
	if (hash == std::string::npos)
		return (line);
	line.erase(hash);
	return (line);
}

vector<string>	tokenizer(string &line) {
	vector<string>	result;
	size_t	pos = 0;
	size_t	start;
	string	temp;

	if (line.find(";") != std::string::npos)
		line.erase(line.find(";"));
	while (pos < line.size())
	{
		pos = line.find_first_not_of(" \t", pos);
		if (pos == std::string::npos)
			return result;
		start = pos;
		while (pos < line.size() && line[pos] != ' ' && line[pos] != '\t')
			++pos;
		temp = line.substr(start, pos-start);
		if (temp.empty())
		{
			std::cout << "\033[31mError detect -> \"" << line << "\"\033[0m" << std::endl;
			throw UtilsError("Fail to create token");
		}
		result.push_back(temp);
	}
	return result;
}

void	print_warning(int server_id, string request_route, string directive, string msg) {
	std::cout << "\033[0;31mWarning: \033[0m";
	if (server_id >= 0)
		std::cout << "Server(" << server_id << ") ";
	if (!request_route.empty())
		std::cout << "Location <" << request_route << ">:  ";
	if (!directive.empty())
		std::cout << "[" << directive << "]: ";
	if (!msg.empty())
		std::cout << msg << std::endl;
}

vector<string>	splitInline(string line) {
	vector<string>	result;
	size_t	pos = 0;
	string	temp;

	line = Utils::trim_whitespaces(line);
	// std::cout << "Splitting -> " << line << std::endl;
	while (pos < line.size())
	{
		if (line[pos] == ';' || line[pos] == '{' || line[pos] == '}')
		{
			temp += line[pos];
			if (!temp.empty())
			{
				if (temp == ";")
				{
					std::cout << "\033[31mError detect -> \"" << line << "\"\033[0m" << std::endl;
					throw UtilsError("Unexpected ; found!");
				}
				result.push_back(temp);
				temp.clear();
			}
		}
		else
			temp += line[pos];
		++pos;
	}
	if (!temp.empty())
		result.push_back(temp);
	return (result);
}

}
