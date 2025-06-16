#ifndef HELPER_HPP
# define HELPER_HPP

# include <string>
# include <iostream>
# include <cctype>

using std::string;

namespace Utils {

bool	is_blankLine(string &line);
string	trim_line(string &line);
string	trim_inlineComment(string &line);
int		find_socketAddress(string &line);

}

#endif
