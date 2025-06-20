#include "../../includes/Http.hpp"

string Http::createElement(string path)
{
    if (isDirectory(path))
        return "<a href=\"/" + path + "/\">" + path + "/</a>"; //directory
    return "<a href=\"/" + path + "\">" + path + "</a>"; //file
}

string Http::autoindex(string path)
{
    std::ostringstream ss;
    DIR *dir = opendir(path.c_str());
    if (!dir)
    {
        //error page;
        cout << RED << "autoindex: huh" << RESETEND;
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
        if (filename == ".") // || filename == ".."
            continue ;
        string fullPath = path + filename;
        // cout << GREEN << fullPath << RESETEND;
        ss << "<div>";
        ss << createElement(fullPath);
        ss << "</div>";
    }
    ss << "</body>";
    ss << "</html>";
    closedir(dir);

    return ss.str();
}
