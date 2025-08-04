#include "../../includes/Http.hpp"

vector<string> Http::createEnv(map<string, string> &m)
{
    map<string, string>::iterator it = m.begin();
    vector<string> vec;

    for (; it != m.end(); ++it)
        vec.push_back(it->first + "=" + it->second);
    vec.push_back("REQUEST_METHOD=" + this->method);
    if (!this->headers["Transfer-Encoding"].compare("chunked"))
    {
        std::stringstream len;
        len << this->body.length();
        len >> this->headers["Content-Length"];
    }
    vec.push_back("CONTENT_LENGTH=" + this->headers["Content-Length"]);
    vec.push_back("CONTENT_TYPE=" + this->headers["Content-Type"]);
    vec.push_back("HTTP_USER_AGENT=" + this->headers["User-Agent"]);
    return vec;
}

vector<string> Http::getQueryParameters(string &url)
{
    std::istringstream ss(url);
    string tmp;
    vector<string> env;

    while (std::getline(ss, tmp, '&'))
    {
        if (tmp[tmp.find('=') + 1] == '&' || !tmp[tmp.find('=') + 1])
           tmp = "None";
        cout << RED << tmp << RESETEND;
        env.push_back(tmp);
    }
    return env;
}

void Http::CGIGet(vector<char *> &argv, string CGIpath)
{
    // for (size_t i = 0; i < argv.size(); i++)
    //     cout << GREEN << argv.size() << " ARGV:" << argv[i] << RESETEND;
    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid == 0)
    {
        // cout << RED << vecArgv[0] << RESETEND;
        // vector<string> qs;
        string qs("QUERY_STRING=");
        vector<string> qp;
        vector<string> vecTmp = createEnv(this->headers);
        vector<char *> vecEnv;

        for (size_t i = 0; i < vecTmp.size(); ++i)
            vecEnv.push_back(const_cast<char *>(vecTmp[i].c_str()));

        if (std::count(CGIpath.begin(), CGIpath.end(), '?') != 0) //if have Query Parameters, save all to env's vector
        {
            CGIpath = CGIpath.substr(CGIpath.find("?") + 1);

            qs.append(CGIpath);
            qp = getQueryParameters(CGIpath);
            for (size_t i = 0; i < qp.size(); ++i)
                vecEnv.push_back(const_cast<char *>(qp[i].c_str()));
        }
        vecEnv.push_back(const_cast<char *>(qs.c_str()));
        vecEnv.push_back(NULL);
        for (size_t i = 0; i < vecEnv.size(); ++i)
            cout << RED << "vecEnv: " << vecEnv[i] << RESETEND;
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);
        execve(argv[0], argv.data(), vecEnv.data());
        perror("execve");
        exit(1);
    }
    else
    {
        close(fd[1]);
        string CGIoutput;
        char buffer[1024];
        int readd = 0;
        time_t start = time(NULL);

        while (1)
        {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            // std::cerr << result << " aaaaa" << endl;
            if (result == -1)
            {
                perror("waitpid error");
                close(fd[0]);
                code500();
                return;
            }
            else if (!result)
            {
                if (time(NULL) - start > TIMEOUT)
                {
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0);
                    close(fd[0]);
                    code504();
                    return ;
                }
            }
            else if (result > 0)
            {
                while ((readd = read(fd[0], buffer, sizeof(buffer))) > 0)
                    CGIoutput.append(buffer, readd);
                close(fd[0]);
                if (CGIoutput.empty())
                {
                    code504();
                    return ;
                }
                cout << CGIoutput << endl;

                this->respond = CGIoutput;
                // send(this->pfd.fd, CGIoutput.c_str(), CGIoutput.length(), 0);
                // this->isRespond = true;
                // cout << BLUE << "CGI Respond ok" << RESETEND;
                break ;
            }
            // cout << "wait" << endl;
            usleep(10000);
        }

        // cout << GREEN << CGIoutput << RESETEND;
    }
}

void Http::CGIPost(vector<char *> &argv, string CGIpath)
{
    // for (size_t i = 0; i < argv.size(); i++)
    //     cout << GREEN << "POST ARGV:" << argv[i] << RESETEND;

    int cgiOut[2];
    int cgiIn[2];

    pipe(cgiOut);
    pipe(cgiIn);

    int pid = fork();
    if (pid == -1)
    {
        code500(); //server error
        return ;
    }
    else if (pid == 0)
    {
        string qs("QUERY_STRING=");
        vector<string> qp;
        vector<string> vecTmp = createEnv(this->headers);
        vector<char *> vecEnv;

        for (size_t i = 0; i < vecTmp.size(); ++i)
            vecEnv.push_back(const_cast<char *>(vecTmp[i].c_str()));

        // string boundary("Boundary=" + this->headers["Content-Type"].substr(this->headers["Content-Type"].find("----") + 4));
        // vecEnv.push_back(const_cast<char *>(boundary.c_str()));
        if (std::count(CGIpath.begin(), CGIpath.end(), '?') != 0) //if have Query Parameters, save all to env's vector
        {
            CGIpath = CGIpath.substr(CGIpath.find("?") + 1);

            qs.append(CGIpath);            
            qp = getQueryParameters(CGIpath);
            for (size_t i = 0; i < qp.size(); ++i)
                vecEnv.push_back(const_cast<char *>(qp[i].c_str()));
        }

        vecEnv.push_back(const_cast<char *>(qs.c_str()));
        vecEnv.push_back(NULL);
        for (size_t i = 0; i < vecEnv.size() - 1; ++i)
        cout << RED << "vecEnv: " << vecEnv[i] << RESETEND;

        for (size_t i = 0; i < argv.size() - 1; ++i)
            cout << RED << "argv: " << argv[i] << RESETEND;

        dup2(cgiIn[0], 0); 
        dup2(cgiOut[1], 1); 

        close(cgiIn[0]);
        close(cgiIn[1]);
        close(cgiOut[0]);
        close(cgiOut[1]);

        execve(argv[0], argv.data(), vecEnv.data());
        perror("execve");
        exit(1);
    }
    else
    {
        close(cgiIn[0]);
        close(cgiOut[1]);
        ssize_t total = 0;
        // sleep(1);

        // this->body = this->header + "\r\n\r\n" + this->body;

        // cout << this->body << endl;
        while (static_cast<size_t>(total) < this->body.size())
        {
            ssize_t n = write(cgiIn[1], this->body.c_str() + total, this->body.size() - total);
            if (n <= 0)
            {
                close(cgiIn[1]);
                close(cgiOut[0]);
                code500(); //server error
                return ;
            }
            total += n;
            usleep(10000);
        }
        close(cgiIn[1]);
        std::cerr << "total2: " << total << " Bodysize: " << this->body.size() << endl;

        string CGIoutput;
        char buffer[1024];
        int readd = 0;
        time_t start = time(NULL);

        while (1)
        {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            if (result == -1)
            {
                perror("waitpid error");
                close(cgiOut[0]);
                code500();
                return;
            }
            else if (!result)
            {
                if (time(NULL) - start > TIMEOUT)
                {
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0);
                    close(cgiOut[0]);
                    code504(); //time out
                    return ;
                }
            }
            else if (result > 0)
            {
                while ((readd = read(cgiOut[0], buffer, sizeof(buffer))) > 0)
                    CGIoutput.append(buffer, readd);
                close(cgiOut[0]);
                if (CGIoutput.empty())
                {
                    code504(); //time out
                    return ;
                }
                cout << BLUE << "Out:\n" << CGIoutput << RESETEND;

                this->respond = CGIoutput;
                if (!CGIoutput.compare("403\n"))
                    code403();
                else if (!CGIoutput.compare("ok\n") && !this->redirectPath.empty())
                    code303();
                else if (!CGIoutput.compare("ok\n"))
                {
                    string content = (getContent(this->rootPath + "/index/yeah.html").compare("") ? getContent(this->rootPath + "/index/yeah.html") : "<!doctype html><html lang=\"en\"><head><title>Upload Page [DefalutPage]</title></head><body><main><h1>Upload sucessful!</h1></main></body></html>");
                    std::ostringstream oss;

                    oss << "HTTP/1.1 200 OK\r\n";
                    oss << "Content-Type: " << "text/html" << "\r\n";
                    oss << "Content-Length: " << content.length() << "\r\n";
                    oss << "\r\n";
                    oss << content;

                    this->respond = oss.str();
                    // send(this->pfd.fd, oss.str().c_str(), oss.str().length(), 0);
                    // this->isRespond = true;
                    // cout << BLUE << "CGI Respond ok" << RESETEND;
                    // close(pfd.fd);
                }
                else
                    code500(); //server error

                return ;
            }
            // cout << "wait" << endl; //debug
            usleep(10000);
        }
    }

}

void Http::handleCGI(string CGIpath)
{
    size_t pos;
    string clearURL(this->rootPath + CGIpath); //remove ?...
    if (std::count(CGIpath.begin(), CGIpath.end(), '?') != 0)
        clearURL = this->rootPath + CGIpath.substr(0, CGIpath.find("?"));  //remove ?...
    else if ((pos = CGIpath.find(this->cgiTypePath.first)) != string::npos)
    {
        string check(CGIpath.substr(pos + this->cgiTypePath.first.length())); 
        // cout << "check: " << check << endl; //debug
        if (!check.empty())
        {
            if (check[0] != '/') //example cgi.pyaaa
                clearURL.clear();
            else
                clearURL = this->rootPath + CGIpath.substr(0, pos + this->cgiTypePath.first.length()); //remove /xxx/xxx
            this->headers["PATH_INFO"] = check;
            // cout << "ClearUrl: " << clearURL << endl; //debug
        }
    }
    if (!fileExistis(clearURL))
    {
        code404(); //not found
        return ;
    }
    if (isDirectory(clearURL)) //pass to GET method handle autoindex
    {
        GET(clearURL);
        return ;
    }
    if (!isExecutale(clearURL))
    {
        code403(); //forbidden
        return ;
    }        

    // for (size_t i = 0; i < vecEnv.size(); ++i)
        // cout << BLUE << vecEnv[i] << RESETEND; //debug
    vector<char *> vecArgv;

    if (this->cgiTypePath.first.compare("C?"))
        vecArgv.push_back(const_cast<char *>(this->cgiTypePath.second.c_str()));
    else
    {
        code400(); //bad request
        return ;
    }

    vecArgv.push_back(const_cast<char *>(clearURL.c_str()));
    vecArgv.push_back(NULL);

    if (!this->method.compare("GET"))
        CGIGet(vecArgv, CGIpath);
    else if (!this->method.compare("POST"))
        CGIPost(vecArgv, CGIpath);
    else
        code405(); //method not allow
}
