/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zgoh <zgoh@student.42kl.edu.my>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 07:50:04 by zgoh              #+#    #+#             */
/*   Updated: 2025/07/01 00:34:05 by zgoh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "configServer.hpp"
#include "configRoute.hpp"

int main(int argc, char** argv) {
	if (argc == 2)
	{
		try
		{
			std::string	filepath = argv[1];
			Config the_config(filepath);
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
