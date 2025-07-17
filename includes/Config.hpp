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
# include "helper.hpp"

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

		class ConfigError : public std::exception {
			private:
				string	_errMsg;
			public:
				ConfigError(string msg) throw();
				virtual const char*	what() const throw();
				~ConfigError() throw();
		};

		void	scan_serverBody(std::ifstream &infile);
		void	print_ServerParsed();
		void	print_SocketTable();
		void	general_check();
		void	build_SocketTable();
};

#endif
