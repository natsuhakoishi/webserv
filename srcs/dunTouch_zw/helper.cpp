/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 01:43:14 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/14 19:56:11 by zgoh             ###   ########.fr       */
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

string	trim_line(string &line) {
	size_t front = line.find_first_not_of(" \n\t\r\v\f");
	size_t behind = line.find_last_not_of(" \n\t\r\v\f");

	if (front == std::string::npos || behind == std::string::npos)
		return ("");
	return (line.substr(front, behind - front + 1));
}

}
