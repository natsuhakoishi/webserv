#include "../../includes/Http.hpp"

string Http::GetContentType(string c)
{
    if (!c.compare("./")) return "text/html";
    // cout << "eeeeeeeee:" << c.substr(c.length() - 5) << endl;
    if (!c.substr(c.length() - 5).compare(".html")) return "text/html";
    if (!c.substr(c.length() - 4).compare(".png")) return "image/png";
    if (!c.substr(c.length() - 4).compare(".gif")) return "image/gif";
    if (!c.substr(c.length() - 5).compare(".jpeg")) return "image/jpeg";
    if (!c.substr(c.length() - 4).compare(".csv")) return "text/csv";

    return "text/plain";
    // return "text/css";
}

/*
如果GET路径
檢查路徑 /index 是否是目錄：

    -是的話，看是否尾部缺 /：
        -缺 / → 301 Redirect 到 /index/

    -然後檢查 /index/index.html 是否存在：
        -存在就讀檔、送出 200

    -如果沒有 index 檔，看是否開啟「列目錄」：
        -有 → 回 HTML directory listing
        -沒有 → 回 403/404
*/
void Http::GET(pollfd pfd, string path)
{
    cout << BLUE << "GET: Client request: " << GREEN << path << RESETEND;
    bool Autoindex = false;

    if (std::find(this->allowMethod.begin(), this->allowMethod.end(), "GET") == this->allowMethod.end())
    {
        cout << RED << "GET: Method not allow" << RESETEND;
        code403(this->pfd.fd);
        return ;
    }

    if (!isDirectory(path) && !fileExistis(path)) //not a directory & file not exists
    {
        code404(pfd.fd);
        return ;
    }
    else if (isDirectory(this->filePath) && this->filePath[this->filePath.length() - 1] != '/')
    {
        code301(pfd.fd, path); //redirect ./example to ./example/
        return ;
    }
    else if (path[path.length() - 1] == '/')
    {
        if ((this->indexFile.empty() || !fileExistis(path + this->indexFile)) && this->autoindex) //if index.html not found, show directory (autoindex)
        {
            Autoindex = true;
            cout << RED << "Auto index" << RESETEND;
        }
        else if (fileExistis(path + this->indexFile))
        {
            cout << RED << "append" << RESETEND;
            path.append(this->indexFile);
        }
        else if (this->autoindex == false)
        {
            code403(pfd.fd);
            return ;
        }
    }

    // cout << RED << "debug " << path << RESETEND;

    string content;
    string type;

    if (Autoindex == true)
    {
        content = handleAutoindex(path);
        type = "text/html";
    }
    else
    {
        content = getContent(path);
        type = GetContentType(path);
    }
    // cout << RED << "debug" << content << RESETEND;

    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Type: " << type << "\r\n";
    ss << "Content-Length: " << content.length() << "\r\n";
    ss << "\r\n";
    ss << content;

    send(pfd.fd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond successful" << RESETEND;
    std::cout << "Client (fd: " << pfd.fd << ") Disconnected" << RESETEND; //debug
    close(pfd.fd);
    this->isRespond = true;
}