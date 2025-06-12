#include "../../includes/Http.hpp"

string Http::GetContentType(string c)
{
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
    cout << BLUE << "GET: Client request: " << path << RESETEND;
    // struct stat s;
    // if (stat(path.c_str(), &s) == 0 && S_ISDIR(s.st_mode)) // && directory_listing == true
    // {
    //     code301(pfd.fd, path);
    // }
    string content(getContent(path));
    string type(GetContentType(path));

    std::ostringstream ss;

    ss << "HTTP/1.1 200 OK\r\n";
    ss << "Content-Type: " << type << "\r\n";
    ss << "Content-Length: " << content.length() << "\r\n";
    ss << "\r\n";
    ss << content;

    send(pfd.fd, ss.str().c_str(), ss.str().length(), 0);

    cout << BLUE << "GET: Respond succesful" << RESETEND;
}
