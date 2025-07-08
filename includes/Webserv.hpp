#pragma once

#include <iostream>
#include <unistd.h>
#include <cstring> //for memset()
#include <sys/socket.h> //socket library
#include <netinet/in.h> //Internet family
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <sys/stat.h> //for stat & stat()
#include <dirent.h> //for opendir()

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

#include "Config.hpp"
#include "Http.hpp"
#include "TcpServer.hpp"

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define RESET  "\033[0m"
#define RESETEND  "\033[0m" << std::endl

template <typename m> 
void printMap(m _map)
{
    for (typename m::iterator it = _map.begin();
        it != _map.end(); it++)
        cout << "\033[0;31m" << it->first << "=" << it->second << "\033[0m" << std::endl;
}