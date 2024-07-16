#include <iostream>
#include <string>
#include <vector>

int main() {
	std::string s = "<>m;{m;};";
	int i = 0;
	std::vector<std::string> cmds = {};
	std::string type = "";
	std::string final_type = "";
	std::string list = "";
	std::string final_list = "";
	std::string expr = "";
	bool is_in_scope = true;
	char scope_sign = 0;
	while (i != s.size()) {
		char c = s[i++];
		std::cout << c << std::endl;
		if (!is_in_scope) {
			continue;
		}
		if ((!is_in_scope && scope_sign == '{') || c == '{') {
			is_in_scope = false;
			list = list + c;	
			if (c == '}') {
				is_in_scope = true;
			}
			continue;
		}
		if ((!is_in_scope && scope_sign == '<') || c == '<') {
			is_in_scope = false;
			type = type + c;
			if (c == '>') {
				is_in_scope = true;
			}
			continue;
		}
		if (c == 'm') {
			expr = 'm';
		}
		if (c == ';') {
			cmds.push_back(expr = type + expr);
			type = expr = "";
		}
	}
	return 0;
}
