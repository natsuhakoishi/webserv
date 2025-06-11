#ifndef CONFIG_SERVER_HPP
# define CONFIG_SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>

using std::string;

class Config {
	std::pair<string,std::vector<cfgServer*> >	table_hostPort;
	std::vector<cfgServer>						servers;
};

class cfgServer {
		string										server_name;
		std::vector<std::pair<string,int> >			hostPort; //host = IP
		int											client_body_size; //or size_t?
		std::vector<std::pair<int,string> >			error_pages;
		std::vector<std::pair<string,cfgRoute> >	route_map; //access type is matter
};

class cfgRoute {
		string									location_path;
		string									rootDir_path; //file directory
		string									indexFile_path; //the one gonna send back to client when request is a dir
		bool									autoIndex_state; //determine whether send error pages or generate directory listing automatically
		std::vector<string>						httpMethods; //using vector<string> temporarily
		string									redirection_path; //redirect webpage to where
		bool									has_upload; //checking flag
		string									upload_path; //where the stuff upload by clients going to store
		bool									has_cgi; //checking flag
		std::vector<std::pair<string,string> >	cgi_info; //ex. format <file extension>,<correspond path>
};

#endif
