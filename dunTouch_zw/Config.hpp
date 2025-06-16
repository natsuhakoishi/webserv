#ifndef CONFIG_HPP
# define CONFIG_HPP

//Containers
# include <string>
# include <vector>
# include <map>
//Libraries & Headers
# include <iostream>
# include <exception>
# include <fstream>
# include "configServer.hpp"
# include "utils.hpp"

using std::string;
using std::vector;
using std::map;

class cfgServer;

class Config {
	private:
		static int					_blockCount;
		vector<cfgServer>			_Servers;
		map<string,vector<int> >	_SocketTable;
	public:
		Config(string &filepath);
		Config(const Config &other);
		Config&	operator=(const Config &other);
		~Config();

		int							get_blockCount() const;
		vector<cfgServer>			get_Servers() const;
		map<string,vector<int> >	get_SocketTable() const;
		
		void	set_SocketTable(string newAddress, int id);

		class ConfigError : public std::exception {
			private:
				const char	*_err_msg;
			public:
				ConfigError(const char *msg) throw();
				virtual const char*	what() const throw();
		};

		void	get_serverBody(std::ifstream &infile);
};

#endif
