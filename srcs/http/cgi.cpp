#include "../../includes/Http.hpp"

vector<string> Http::createEnv(map<string, string> &m)
{
    map<string, string>::iterator it = m.begin();
    vector<string> vec;

    for (; it != m.end(); ++it)
        vec.push_back(it->first + "=" + it->second);
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

void Http::handleCGI(string CGIpath)
{
    if (!cgiRoute) //config file did'nt set /cgi/
    {
        code403(this->pfd.fd);
        return ;
    }
    string clearURL(this->rootPath + CGIpath); //removed $...
    if (std::count(CGIpath.begin(), CGIpath.end(), '?') != 0)
        clearURL = this->rootPath + CGIpath.substr(0, CGIpath.find("?"));  //removed $...
    if (!fileExistis(clearURL))
    {
        code404(this->pfd.fd);
        return ;
    }
    if (isDirectory(clearURL)) //pass to GET method handle autoindex
    {
        GET(this->pfd, clearURL);
        return ;
    }
    if (!isExecutale(clearURL))
    {
        code403(this->pfd.fd);
        return ;
    }

    vector<string> vecTmp = createEnv(this->headers);
    vector<char *> vecEnv;

    for (size_t i = 0; i < vecTmp.size(); ++i)
        vecEnv.push_back(const_cast<char *>(vecTmp[i].c_str()));

    if (std::count(CGIpath.begin(), CGIpath.end(), '?') != 0) //if have Query Parameters, save all to env's vector
    {
        CGIpath = CGIpath.substr(CGIpath.find("?") + 1);

        vector<string> qp = getQueryParameters(CGIpath);
        for (size_t i = 0; i < qp.size(); ++i)
            vecEnv.push_back(const_cast<char *>(qp[i].c_str()));
    }

    vecEnv.push_back(NULL);
    // for (size_t i = 0; i < vecEnv.size(); ++i)
        // cout << BLUE << vecEnv[i] << RESETEND;
    vector<char *> vecArgv;

    if (clearURL.find('.', 1) == string::npos || !clearURL.substr(clearURL.find('.', 1), 4).compare(".out"))
        ;
    else if (!clearURL.substr(clearURL.find('.', 1), 4).compare(".sh"))
        vecArgv.push_back(const_cast<char *>("/usr/bin/bash"));
    else if (!clearURL.substr(clearURL.find('.', 1), 5).compare(".zsh"))
        vecArgv.push_back(const_cast<char *>("/usr/bin/zsh"));
    else if (!clearURL.substr(clearURL.find('.', 1), 4).compare(".py"))
        vecArgv.push_back(const_cast<char *>("/usr/bin/python3"));
    else
    {
        code500(this->pfd.fd);
        return ;
    }

    vecArgv.push_back(const_cast<char *>(clearURL.c_str()));
    vecArgv.push_back(NULL);

    int fd[2];
    pipe(fd);

    int pid = fork();
    if (pid == 0)
    {
        dup2(fd[1], 1);
        close(fd[1]);
        close(fd[0]);

        execve(vecArgv[0], vecArgv.data(), vecEnv.data());
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
            if (result == -1)
            {
                perror("waitpid error");
                close(fd[0]);
                code500(this->pfd.fd);
                return;
            }
            else if (!result)
            {
                if (time(NULL) - start > TIMEOUT)
                {
                    kill(pid, SIGKILL);
                    waitpid(pid, &status, 0);
                    close(fd[0]);
                    code504(this->pfd.fd);
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
                    code504(this->pfd.fd);
                    return ;
                }
                cout << CGIoutput << endl;
                send(this->pfd.fd, CGIoutput.c_str(), CGIoutput.length(), 0);
                this->isRespond = true;
                break ;
            }
            // cout << "wait" << endl;
            sleep(0.1);
        }

        // cout << GREEN << CGIoutput << RESETEND;
    }
}
