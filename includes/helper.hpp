#ifndef HELPER_HPP
# define HELPER_HPP

# include <string>
# include <vector>
# include <exception>
# include <iostream>
# include <cctype>

using std::string;
using std::vector;

namespace Utils {

class UtilsError : public std::exception {
	public:
		UtilsError(string msg) throw();
		virtual const char*	what() const throw();
		~UtilsError() throw();
	private:
		string _errMsg;
};

bool	is_blankLine(string &line);
string	trim_whitespaces(string &line);
string	trim_inlineComment(string &line);
vector<string>	tokenizer(string &line);
void	print_warning(int server_id, string request_route, string directive, string msg);

}

#endif
