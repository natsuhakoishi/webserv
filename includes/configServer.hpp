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
# include <sstream>
# include <cstdlib>
# include <cctype>

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
		bool				_autoIndex; //update: newly added
		vector<cfgRoute>	_Routes;
	public:
		cfgServer();
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
		bool				get_autoIndex() const; //update: remove std namespace

		class OtherError : public std::exception {
			public:
				OtherError(string msg) throw();
				virtual const char*	what() const throw();
				~OtherError() throw();
			private:
				string	_errMsg;
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
				ArgError(int id, string dir, string msg) throw();
				virtual const char*	what() const throw();
				~ArgError() throw();
			private:
				string	_errMsg;
		};

		void	parseServer(string &content);
		void	handle_serverName(vector<string> &line);
		void	handle_hostPort(vector<string> &line);
		void	handle_errorCodes(vector<string> &line);
		void	handle_clientBodySize(vector<string> &line);
		void	handle_serverRoot(vector<string> &line);
		void	handle_serverIndex(vector<string> &line);
		void	display_parsedContent(void);
};

#endif
