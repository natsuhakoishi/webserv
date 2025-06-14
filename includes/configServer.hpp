#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

//Containers
# include <string>
# include <vector>
# include <map>
# include <utility> //for std::pair
//Libraries & Headers
# include "Config.hpp"
# include "configRoute.hpp"
# include "helper.hpp"

using std::string;
using std::pair;

class cfgRoute;

class cfgServer {
	private:
		int									_id;
		string								_server_name; //equivalent to HTTP request header's Host
		std::vector<string>					_hostPort; //IP:Port
		int									_clientBodySize;
		std::vector<pair<int,string> >		_errorCodes_map;
		// std::vector<pair<string,cfgRoute> >	Routes_map;
		std::vector<cfgRoute>				_Routes;
	public:
		//OCCF
		cfgServer(string &content, int id);
		cfgServer(const cfgServer &other);
		cfgServer&	operator=(const cfgServer &other);
		~cfgServer();

		//getter
		string							get_serverName();
		std::vector<string>				get_hostPort();
		int								get_clientBodySize();
		std::vector<pair<int,string> >	get_errorCodesMap();
		std::vector<cfgRoute>			get_routes();
		//setter
		void	set_serverName(const string &input);
		void	set_hostPort(const std::vector<string> &inputS);
		void	set_clientBodySize(int input);
		void	set_errorCodesMap(const std::vector<pair<int,string> > &inputS);
		void	set_routesMap(const std::vector<cfgRoute> &inputS);
};

#endif
