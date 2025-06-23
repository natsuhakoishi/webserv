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
using std::vector;
using std::pair;
using std::map;

class cfgRoute;

class cfgServer {
	private:
		int							_id;
		string						_server_name;
		vector<string>				_hostPort;
		int							_clientBodySize;
		map<int, string>			_errorCodes_map;
		vector<cfgRoute>			_Routes;
	public:
		cfgServer();
		cfgServer(string &content, int id);
		cfgServer(const cfgServer &other);
		cfgServer&	operator=(const cfgServer &other);
		~cfgServer();

		int								get_id() const;
		string							get_serverName() const;
		std::vector<string>				get_hostPort() const;
		int								get_clientBodySize() const;
		map<int, string>				get_errorCodesMap() const;
		std::vector<cfgRoute>			get_routes() const;
		//setter
			// void	set_serverName(const string &input);
			// void	set_hostPort(const std::vector<string> &inputS);
			// void	set_clientBodySize(int input);
			// void	set_errorCodesMap(const std::vector<pair<int,string> > &inputS);
			// void	set_routesMap(const std::vector<cfgRoute> &inputS);
};

#endif
