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

using std::string;
using std::pair;

class cfgRoute;

class cfgServer {
	private:
		string								server_name; //equivalent to HTTP request header's Host
		std::vector<string>					hostPort; //IP:Port
		int									clientBodySize;
		std::vector<pair<int,string> >		errorCodes_map;
		// std::vector<pair<string,cfgRoute> >	Routes_map;
		std::vector<cfgRoute>				Routes;
	public:
		//OCCF
		cfgServer();
		cfgServer(const cfgServer &other);
		cfgServer&	operator=(const cfgServer &other);
		~cfgServer();

		//getter
		const string&							get_serverName() const;
		const std::vector<string>&				get_hostPort() const;
		int										get_clientBodySize() const;
		const std::vector<pair<int,string> >&	get_errorCodesMap() const;
		const std::vector<cfgRoute>&			get_routes() const;
		//setter
		void	set_serverName(const string &input);
		void	set_hostPort(const std::vector<string> &inputS);
		void	set_clientBodySize(int input);
		void	set_errorCodesMap(const std::vector<pair<int,string> > &inputS);
		void	set_routesMap(const std::vector<cfgRoute> &inputS);
};

#endif
