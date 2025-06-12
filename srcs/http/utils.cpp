#include "../../includes/Webserv.hpp"

string Http::getContent(string path)
{
    std::ifstream ifile(path.c_str(), std::ios::binary);

    if (!ifile.is_open())
        return "";

    std::ostringstream ss;

    ss << ifile.rdbuf();

    return ss.str();
}