#ifndef MAIN_9_HPP
#define MAIN_9_HPP

#include <map>
#include <stdexcept>
#include <string>
#include <vector>
	
class Block;
using Blocks = std::vector<Block>;
class Block {
public:
	Block(char symbol_, int first =  -1, int last = -1, Blocks blocks = {}) : first_(first), last_(last), blocks_(blocks) {};
private:
	char symbol_;
	int first_;
	int last_;
	Blocks blocks_;
public:
	static int main();
public:
	Blocks ToBlocks(const std::string& s) {
		static const std::map<char, int> delimiter_value_map = {
			{'{', 1},
			{'}', -1},
			{'(', 2},
			{')', -2},
			{'[', 3},
			{']', -3},
			{'<', 4},
			{'>', -4},
		};
		Blocks result_blocks = {};
		Blocks unresolved_blocks = {};
		for (int i = 0; i < s.size(); i++) {
			// Pass: a character
			const char symbol = s[i];
			if (!delimiter_value_map.contains(symbol)) {
				continue;
			}
			// Pass: the character is a delimiter
			const char delimiter_value = delimiter_value_map.at(symbol);
			if (delimiter_value < 0) {
				auto b = Block(symbol, i);
				unresolved_blocks.push_back(std::move(b));
			}
			// Pass: the character is a closing delimiter
			if (unresolved_blocks.empty()) {
				std::runtime_error("Blocks ToBlocks(const std::string& s): unresolved_blocks.empty()");
			}
			// Pass: there is an unresolved delimiter
			if (unresolved_blocks.back().first_ + delimiter_value != 0) {
				std::runtime_error("Blocks ToBlocks(const std::string& s): unresolved_blocks.back().first_ + delimiter_value != 0");
			}
			// Pass: the character is pair with recent delimiter
			if ()	
		}
	};
};

#endif

int main() {
	Block::main();
	return 0;
}

int Block::main() {
	Block b = Block();
	Blocks bs = {};
	return 0;
};
