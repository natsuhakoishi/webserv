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

bool Http::isDirectory(string path)
{
    struct stat s;
    stat(path.c_str(), &s);
    if (S_ISDIR(s.st_mode))
        return true;
    return false;
}

bool Http::fileExistis(string file)
{
    std::ifstream ifile(file.c_str());
    if (!ifile.good())
        return false;
    return true;
}
