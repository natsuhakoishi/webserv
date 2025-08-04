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

bool Http::isDirectory(const string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s))
        return false;
    if (S_ISDIR(s.st_mode))
        return true;
    return false;
}

bool Http::fileExistis(const string &file)
{
    std::ifstream ifile(file.c_str());
    if (!ifile.good())
        return false;
    return true;
}

bool Http::IsCorrectPrefix(const string &url, const string &routePath) const
{
    if (!url.compare(routePath))
        return true;
    if (url.find(routePath) == 0 && url[routePath.length()] == '/')
        return true;
    return false;
}

 //S_ISREG = is regular file
 //S_IEXEC = is user can execute
bool Http::isExecutale(const string &path)
{
    struct stat s;
    if (stat(path.c_str(), &s))
        return false;
    if (S_ISREG(s.st_mode) && (s.st_mode & S_IEXEC))
        return true;
    return false;
}

bool Http::IsAllowMethod()
{
    vector<string>::iterator iter = this->allowMethod.begin();
    for (; iter != this->allowMethod.end(); ++iter)
        if (this->method == *iter)
            return true;
    return false;
}