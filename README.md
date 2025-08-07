**Usage:**
---
`./webserv <path to config file>`
<br><br>

**Configuration Guide**
---
##### !! Only directives listed below are supported, others are ignored;
1. ``server_name <name>``

&emsp;&emsp;Set server's host name

2. ``listen <port>`` &nbsp;***OR***&nbsp; ``listen <socket_address>``

&emsp;&emsp;Set socket address or port the server will listen on (e.g., 127.0.0.1:4242 or 4242)

3. ``root <filepath>``

&emsp;&emsp;Set root directory of the server

4. ``index <filepath>``

&emsp;&emsp;Set default file to serve when a directory is requested by the client

5. ``autoindex <value>``

&emsp;&emsp;Enable or disable automatic directory listing feature

6. ``error_page <status_code> <filepath>``

&emsp;&emsp;Map a specific status code to a custom error page

7. ``allowed_methods <method> <method> ...``

&emsp;&emsp;Set which HTTP methods are allowed for the route

8. ``upload <filepath>``

&emsp;&emsp;Set the location where file uploaded by the client will be saved

9. ``client_max_body_size <size>``

&emsp;&emsp;Set the maximum upload size (read as bytes by default, but units like GB or kb is accepted)

10. ``redirection <URL>``

&emsp;&emsp;Set the URL to redirect request to

11. ``cgi <file_extension> <path_to_executor>``

&emsp;&emsp;Define CGI handler for files with the given extension (e.g. ***cgi*** ***.py*** ***/usr/bin/python3***)
