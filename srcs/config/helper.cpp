/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:43:14 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/25 22:10:46 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "helper.hpp"

namespace Utils {

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
	size_t	pos;
	size_t	start;

	//memo
	//method using: indexing
	//split by spaces / tabs
	//should handle consecutive version of both characters
	//use substr to generate token

	line.erase(line.find(";"));
	while (pos < line.size())
	{
		//memo
		//record the start of the directive/argument
		//loop throught until hit delimeter " " or "\t"
		//provide the start and the end to substr
		//push the substr into vector
		pos = line.find_first_not_of(" \t", pos);
		start = pos;
		if (pos == std::string::npos)
		{
			std::cout << "AAAAAAAAAAAAAAAAAA" << std::endl;
			break ;
		}
		while (pos < line.size() && line[pos] != ' ' && line[pos] != '\t')
			++pos;
		if (pos == line.size() - 1)
		{
			std::cout << "AAAAAAAAAAAAAAAAAA" << std::endl;
			break ;
		}
		result.push_back(line.substr(start,pos-start));
	}
	return result;
}

}
