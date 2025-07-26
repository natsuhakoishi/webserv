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
	
		void	scan_serverBody(std::ifstream &infile);
		void	general_check();
		void	build_SocketTable();
		bool	checkRoute(cfgServer &block);

		class ConfigError : public std::exception {
			public:
				ConfigError(string msg) throw();
				virtual const char*	what() const throw();
				~ConfigError() throw();
			private:
				string	_errMsg;
		};

		class CheckingError : public std::exception {
			public:
				CheckingError(int id, string dir, string msg) throw();
				virtual const char*	what() const throw();
				~CheckingError() throw();
			private:
				string	_errMsg;
		};

	public:
		Config(string &filepath);
		Config(const Config &other);
		Config&	operator=(const Config &other);
		~Config();

		int							get_blockCount() const;
		vector<cfgServer>			get_Servers() const;
		map<string,vector<int> >	get_SocketTable() const;

		void	print_ServerParsed();
		void	print_SocketTable();
};

#endif
