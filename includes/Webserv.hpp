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
#include "fstream"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::map;

#include "Http.hpp"
#include "TcpServer.hpp"
