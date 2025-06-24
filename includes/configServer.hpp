#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

//Containers
# include <string>
# include <vector>
# include <map>
//Libraries & Headers
# include "Config.hpp"
# include "configRoute.hpp"
# include "helper.hpp"

using std::string;
using std::vector;
using std::map;

class cfgRoute;

class cfgServer {
	private:
		int					_id;
		string				_serverName; //update: rename
		vector<string>		_hostPort;
		map<int,string>		_errorCodes_map; //update: change container
		int					_clientBodySize;
		string				_root_path; //update: newly added
		string				_index_path; //update: newly added
		vector<cfgRoute>	_Routes;
	public:
		cfgServer(int id); //update: split logic out from constructor
		cfgServer(const cfgServer &other);
		cfgServer&	operator=(const cfgServer &other);
		~cfgServer();

		int					get_id() const;
		string				get_serverName() const;
		vector<string>		get_hostPort() const; //update: remove std namespace
		map<int,string>		get_errorCodesMap() const; //update: return type
		int					get_clientBodySize() const;
		string				get_rootPath() const; //update: newly added
		string				get_indexPath() const; //update: newly added
		vector<cfgRoute>	get_routes() const; //update: remove std namespace

		class SemicolonMissing : public std::exception {
			public:
				virtual const char*	what() const throw();
		};

		//situations will throw this exception are:
		//1. directive not matched @ invalid directive @ directive not handle in this project
		//2. directive found in wrong scope
		class DirectiveError : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		//1. when no argument value come along with the directive
		//2. the argument value is invalid
		class ArgError : public std::exception {
			public:
				ArgError(string msg) throw();
				virtual const char*	what() const throw();
			private:
				string	_errMsg;
		};

		void	parseServer(string &content);
};

#endif
