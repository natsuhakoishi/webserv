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
		const string&				get_requestPath() const;
		const string&				get_rootDirPath() const;
		const string&				get_indexFilePath() const;
		bool						get_autoIndexState() const;
		const std::vector<string>&	get_httpMethod() const;
		const string&				get_redirectionPath() const;
		const string&				get_uploadPath() const;
		const std::vector<pair<string,string> >	get_cgiInfo() const;
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
