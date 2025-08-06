**How to use?**
---
git clone, run make, set up your configuration file, run `./webserv <config file>`
<br><br>

**Introduction to configuration file:**
---
*Noted that only directives mentioned below are accepted else count as unknown
1. ***server_name*** -> server's host name
2. ***listen*** -> socket address or port that server listened on
3. ***root*** -> root filepath of the server
4. ***index*** -> default file (index file) to be sent when a directory is requested
5. ***autoindex*** -> whether turn on or off auto listing file directory feature
6. ***error_page*** -> map error status code with server's custom error page
7. ***allowed_methods*** -> set up which HTTP methods that route can be requested by client
8. ***upload*** -> where to save the files uploaded by client
9. ***redirection*** -> where to redirect
