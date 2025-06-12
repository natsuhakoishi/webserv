#ifndef CONFIGROUTE_HPP
# define CONFIGROUTE_HPP

//Containers
# include <string>
# include <vector>
# include <utility> //for std::pair
//Libraries & Headers
# include "Config.hpp"
# include "configServer.hpp"

using std::string;
using std::pair;

class cfgRoute {
	private:
		string									request_path;
		string									rootDir_path;
		string									indexFile_path;
		bool									autoIndex_state;
		std::vector<string>						http_method;
		// bool									has_redirec;
		string									redirection_path;
		// bool									has_upload;
		string									upload_path;
		// bool									has_cgi;
		std::vector<pair<string,string> >	cgi_info; //ex. format <file extension>,</file/to/script>
	public:
		//OCCF
		cfgRoute();
		cfgRoute(const cfgRoute &other);
		cfgRoute&	operator=(const cfgRoute &other);
		~cfgRoute();

		//getter
		string								get_requestPath();
		string								get_rootDirPath();
		string								get_indexFilePath();
		bool								get_autoIndexState();
		std::vector<string>					get_httpMethod();
		string								get_redirectionPath();
		string								get_uploadPath();
		std::vector<pair<string,string> >	get_cgiInfo();
		//setter
		void	set_requestPath(const string &input); 
		void	set_rootDirPath(const string &input);
		void	set_indexFilePath(const string &input);
		void	set_autoIndexState(bool input);
		void	set_httpMethod(const std::vector<string> &inputS);
		void	set_redirectionPath(const string &input);
		void	set_uploadPath(const string &input);
		void	set_cgiInfo(const std::vector<pair<string,string> > &inputS);
};

#endif
