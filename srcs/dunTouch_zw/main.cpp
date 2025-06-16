/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 07:50:04 by zgoh              #+#    #+#             */
/*   Updated: 2025/06/16 17:14:34 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
// #include "configServer.hpp"
// #include "configRoute.hpp"

int main(int argc, char** argv) {
	if (argc == 2)
	{
		try
		{
			std::string	filepath = argv[1];
			Config the_config(filepath);
			std::cout << the_config.get_blockCount() << std::endl;
			vector<cfgServer> temp = the_config.get_Servers();
			vector<cfgServer>::iterator it = temp.begin();
			while (it != temp.end())
			{
				cfgServer temp_buf = *it;
				std::cout << temp_buf.get_id() << std::endl;
				++it;
			}
		}
		catch (...)
		{
			std::cout << "Unknown error!" << std::endl;
		}
	}
	else
		std::cout << "Usage: ./webserve <configuration file>" << std::endl;
	return (0);
}
