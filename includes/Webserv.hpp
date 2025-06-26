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

#include "Http.hpp"
#include "TcpServer.hpp"
#include "Config.hpp"

#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[33m"
#define BLUE   "\033[34m"
#define RESET  "\033[0m"
#define RESETEND  "\033[0m" << std::endl
