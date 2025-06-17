#include "../../includes/Http.hpp"

void	Http::DELETE(pollfd pfd, string path)
{
	struct stat	s;

	if (stat(path.c_str(), &s) != 0) //0 means it have files or folder, 1 means no
	{
		code404(pfd.fd);
		return ;
	}
	if (access(path.c_str(), W_OK) != 0) //check is it writeable, cuz want delete must can write access
	{
		code403(pfd.fd);
		return ;
	}

	int	flag = -1; //check status of delete result
	if (S_ISREG(s.st_mode)) //check the file is regular file or not
		flag = remove(path.c_str());
	else if (S_ISDIR(s.st_mode)) //check the file is directory or not
	{
		DIR* dir = opendir(path.c_str());
		if (dir == NULL)
		{
			code404(pfd.fd); //500 later //reminder
			return ;
		}
		struct dirent *enterdir;
		bool isEmpty = true;
		while ((enterdir = readdir(dir)) != NULL)
		{
			if (std::string(enterdir->d_name) != "." && std::string(enterdir->d_name) != "..") //check is the folder got stuff
			{
				isEmpty = false;
				break ;
			}
		}
		closedir(dir);
		if (isEmpty == false) //if folder got stuff cant delete, send 409 conflict
		{
			code403(pfd.fd); //409 later //reminder
			return ;
		}
		flag = rmdir(path.c_str()); //success 0, else 1
	}
	if (flag != 0)
	{
		code404(pfd.fd);//500 later //reminder
		return ;
	}

	std::ostringstream	ss;
	ss << "HTTP/1.1 204 No Content\r\n\r\n";
	send(pfd.fd, ss.str().c_str(), ss.str().length(), 0);
	this->isRespond = true;
	close(pfd.fd);
}
