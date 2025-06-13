#include "../../includes/Http.hpp"

string Http::createElement(string file, struct stat isdir)
{
    if (S_ISDIR(isdir.st_mode))
        return "<a href=\"" + file + "/\">" + file + "/</a>"; //directory
    return "<a href=\"" + file + "\">" + file + "</a>"; //file
}

string Http::autoindex(string path)
{
    std::ostringstream ss;
    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        //error page;
        return "";
    }
    struct dirent *entry;

    ss << "<!DOCTYPE html>";
    ss << "<html lang=\"en\">";
    ss << "<body>";
    ss << "<h1>" << "index of " << path << "</h1>";
    while ((entry = readdir(dir)) != NULL)
    {
        string filename = entry->d_name;
        if (filename == "." || filename == "..")
            continue ;
        string fullPath = path + filename;
        struct stat s;
        stat(fullPath.c_str(), &s);
        // cout << GREEN << fullPath << " " << S_ISDIR(s.st_mode) << RESETEND;
        ss << "<div>";
        ss << createElement(fullPath, s);
        ss << "</div>";
    }
    ss << "</body>";
    ss << "</html>";
    closedir(dir);

    return ss.str();
}