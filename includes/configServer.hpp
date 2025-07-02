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
# include <algorithm>

using std::string;
using std::vector;
using std::map;

class cfgRoute;

class cfgServer {
	private:
		int					_id;
		string				_serverName;
		vector<string>		_hostPort;
		map<int,string>		_errorCodes_map;
		int					_clientBodySize;
		string				_root_path;
		string				_index_path;
		bool				_autoIndexS;
		vector<cfgRoute>	_Routes;

	public:
		cfgServer(int id); //update: split logic out from constructor
		cfgServer(const cfgServer &other);
		cfgServer&	operator=(const cfgServer &other);
		~cfgServer();
		
		int					get_id() const;
		string				get_serverName() const;
		vector<string>		get_hostPort() const;
		map<int,string>		get_errorCodesMap() const;
		int					get_clientBodySize() const;
		string				get_rootPath() const;
		string				get_indexPath() const;
		bool				get_autoIndexS() const;
		vector<cfgRoute>	get_routes() const;
		vector<cfgRoute>&	get_routes();

		class OtherError : public std::exception {
			public:
				OtherError(int id, int nl, string msg) throw();
				virtual const char*	what() const throw();
				~OtherError() throw();
			private:
				string	_errMsg;
		};

		class ArgError : public std::exception {
			public:
				ArgError(int id, string dir, string msg) throw();
				virtual const char*	what() const throw();
				~ArgError() throw();
			private:
				string	_errMsg;
		};
		
		class CheckingError : public std::exception {
			public:
				CheckingError(int id, string path, string dir, string msg) throw();
				virtual const char*	what() const throw();
				~CheckingError() throw();
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
		void	handle_autoIndexS(vector<string> &line);
		void	display_parsedContent(void);
		void	general_check(cfgServer &block);
};

#endif
