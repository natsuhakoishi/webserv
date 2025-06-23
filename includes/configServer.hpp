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
		vector<cfgRoute>	_Routes;
	public:
		cfgServer(string &content, int id);
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
};

#endif
