#ifndef CONFIG_HPP
# define CONFIG_HPP

//Containers
# include <string>
# include <vector>
# include <map>
# include <utility> //for std::pair
//Libraries & Headers
# include <exception>
# include <iostream>
# include <fstream>
# include "configServer.hpp"

using std::string;
using std::pair;

class cfgServer;

class Config {
	private:
		std::map<string,std::vector<cfgServer> >	bind_table; //how about std::vector<int>, store the index of cfgServer in the std::vector
		std::vector<cfgServer>						Servers;
	public:
		//OCCF
		Config(string filepath);
		Config(const Config &other);
		Config&	operator=(const Config &other);
		~Config();

		//getter
		std::map<string,std::vector<cfgServer> >	get_bindTable();
		std::vector<cfgServer>						get_Servers();
		//setter
		void	set_bindTable(const std::map<string,std::vector<cfgServer> > &inputS);
		void	set_Servers(const std::vector<cfgServer> &inputS);

		//exception
		class ConfigError : public std::exception {
			private:
				string	err_msg;
			public:
				ConfigError(const string &msg);
				const char*	what() const throw();
		};

		//member functions
		void	read_config(std::ifstream &infile);//loop and parse
};

#endif
