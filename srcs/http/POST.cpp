#include "../../includes/Http.hpp"

/*
------WebKitFormBoundaryFvZ8iJ3uAWsJFTNG
Content-Disposition: form-data; name="file"; filename="Spaghetti.conf"
Content-Type: application/octet-stream
*/

/*
------WebKitFormBoundaryjrbXO7AhTDnRYd2t
Content-Disposition: form-data; name="file"; filename="&#27927;&#34915;&#29483;&#29483;.mp4"
Content-Type: video/mp4
*/

void Http::POST(pollfd pfd, string path)
{
    std::istringstream ss(this->body);
    string boundary;
    string line;
    string thrLine;

    if (this->body.length() > this->bodySize)
    {
        code413(this->pfd.fd);
        return ;
    }

    std::getline(ss, boundary);
    std::getline(ss, line);
    std::getline(ss, thrLine);

    size_t pos = line.find("filename=\"");
    string filename = line.substr(pos + 10);
    filename.erase(filename.find_last_not_of("\"\r\n") + 1);
    // cout << YELLOW << "'" << filename << "'" << RESETEND;
    int start = boundary.length() + line.length() + thrLine.length() + 5;

    string pureBody = this->body.substr(start, this->body.rfind("\r\n------") - start);

    // cout << pureBody << endl;
    cout << BLUE << "POST: Client sent: " << GREEN << path + "/" + filename << RESETEND;

    mkdir(path.c_str(), 0777);
    std::ofstream outFile((path + "/" + filename).c_str());
    outFile << pureBody;
    outFile.close();

    //respond sucessful page
    string content = getContent("./index/yeah.html");
    std::ostringstream oss;

    oss << "HTTP/1.1 200 OK\r\n";
    oss << "Content-Type: " << "text/html" << "\r\n";
    oss << "Content-Length: " << content.length() << "\r\n";
    oss << "\r\n";
    oss << content;
    send(pfd.fd, oss.str().c_str(), oss.str().length(), 0);
    this->isRespond = true;

    cout << BLUE << "POST: Respond sucessful" << RESETEND;
    std::cout << "Client (fd: " << pfd.fd << ") Disconnected" << RESETEND; //debug
    close(pfd.fd);
}
