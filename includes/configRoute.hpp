#ifndef CONFIGROUTE_HPP
# define CONFIGROUTE_HPP

//Containers
# include <string>
# include <vector>
# include <map>
//Libraries & Headers
# include "Config.hpp"
# include "configServer.hpp"
# include "helper.hpp"

using std::string;
using std::vector;
using std::map;

class cfgRoute {
	private:
		string				_path; //update: rename
		string				_root_path; //update: rename
		string				_index_path; //update: rename
		bool				_autoIndex; //update: rename
		vector<string>		_http_method;
		string				_redirection_path;
		string				_upload_path;
		int					_clientBodySize; //update: newly added
		map<string,string>	_cgi_info; //update: change container
	public:
		cfgRoute();
		cfgRoute(int); //for testing use
		cfgRoute(const cfgRoute &other);
		cfgRoute&	operator=(const cfgRoute &other);
		~cfgRoute();

		string				get_path() const; //update: rename
		string				get_rootPath() const; //update: rename
		string				get_indexPath() const; //update: rename
		bool				get_autoIndex() const; //update: rename
		vector<string>		get_httpMethod() const;
		string				get_redirectionPath() const;
		string				get_uploadPath() const;
		int					get_clientBodySize() const; //update: newly added
		map<string,string>	get_cgiInfo() const; //update: return type changed

		class RouteError : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class SemicolonMissing : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class DirectiveError : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class ArgError : public std::exception {
			public:
				ArgError(string route, string dir, string msg) throw();
				virtual const char*	what() const throw();
				~ArgError() throw();
			private:
				string	_errMsg;
		};

		void	parseLocation(string &content);
		string	splitRoute(string &line);
};

#endif
