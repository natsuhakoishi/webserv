#ifndef CONFIGROUTE_HPP
# define CONFIGROUTE_HPP

//Containers
# include <string>
# include <vector>
# include <utility> //for std::pair
//Libraries & Headers
# include "Config.hpp"
# include "configServer.hpp"
# include "helper.hpp"

using std::string;
using std::vector;
using std::pair;

class cfgRoute {
	private:
		string							_request_path;
		string							_rootDir_path;
		string							_indexFile_path;
		bool							_autoIndex_state;
		vector<string>					_http_method;
		string							_redirection_path;
		string							_upload_path;
		vector<pair<string,string> >	_cgi_info;
	public:
		cfgRoute();
		cfgRoute(const cfgRoute &other);
		cfgRoute&	operator=(const cfgRoute &other);
		~cfgRoute();

		string							get_requestPath() const;
		string							get_rootDirPath() const;
		string							get_indexFilePath() const;
		bool							get_autoIndexState() const;
		vector<string>					get_httpMethod() const;
		string							get_redirectionPath() const;
		string							get_uploadPath() const;
		vector<pair<string,string> >	get_cgiInfo() const;
		//setter
			// void	set_requestPath(const string &input); 
			// void	set_rootDirPath(const string &input);
			// void	set_indexFilePath(const string &input);
			// void	set_autoIndexState(bool input);
			// void	set_httpMethod(const std::vector<string> &inputS);
			// void	set_redirectionPath(const string &input);
			// void	set_uploadPath(const string &input);
			// void	set_cgiInfo(const std::vector<pair<string,string> > &inputS);
};

#endif
