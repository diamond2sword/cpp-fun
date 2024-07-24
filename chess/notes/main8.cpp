#ifndef FOG_CHESS_HPP
#define FOG_CHESS_HPP

#include <array>
#include <cstring>
#include <functional>
#include <map>
#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <utility>

class Pos {
private:
	int x_ = 0;
	int y_ = 0;
};

class Cmd {
private:
	int scope_ = 0;
	std::vector<int> type_ = {};
	void operator|=(std::function<void(Cmd&)> editor_f);
	int x_ = -1;
	int y_ = -1;
	std::vector<std::vector<int>> data_ = {};
	int north_ = -1;
	int orientation_ = -1;
	int delay_ = -1;
	Cmd* head = nullptr;
	Cmd* tail = nullptr;
	bool is_reversed_ = false;
	bool is_relative_ = false;
};

class Block;
using Blocks = std::vector<Block>;
using BlockIter = std::__wrap_iter<Block*>;
class Block {
public:
	Block(int first) : first_(first) {};
	Block(int first, int last) : first_(first), last_(last) {};
	Block(int first, int last, Blocks blocks) : first_(first), last_(last), blocks_(std::move(blocks)) {};
	int first_ = -1;
	int last_ = -1;
	Blocks blocks_ = {};
};




Blocks from_range_at_if(Blocks& from, const std::function<bool(const Block&)> which_is) {
	Blocks to = {};
	const auto it = std::find_if(from.begin(), from.end(), which_is);
	for (auto that = it; that != from.cend(); that++) {
		to.push_back(*that);
	}
	while (from.cend() != it) {
		from.pop_back();
	}
	return to;
};

#endif // FOG_CHESS_HPP

int main() {
	const std::string s = "(((<>;<;>)))";
	const std::map<char, int> delimiter {
		{'{', 1},
		{'}', -1},
		{'<', 2},
		{'>', -2},
		{'(', 3},
		{')', -3},
		{'[', 4},
		{']', -4},
	};

	Blocks blocks = {};
	Blocks delimiters = {};

	for (int i = 0; i < s.size(); i++) {
		if (!delimiter.contains(s[i])) {
			if (delimiters.empty()) {
				auto b = Block(i, i);
				blocks.push_back(std::move(b));
				continue;
			}
			if (s[i] == ';') {
				const int delimiter_first = delimiters.empty() ? 0 : delimiters.back().first_;
				const auto it = std::find_if(blocks.begin(), blocks.end(), [delimiter_first, &s](const Block& b) { return b.first_ > delimiter_first && s[b.last_] != ';'; });
				const int first = it == blocks.cend() ? i : it->first_;
				auto b = Block(first, i);
				b.blocks_.insert(it, blocks.begin(), blocks.end());
				blocks.erase(it, blocks.cend());
				blocks.push_back(std::move(b));
				continue;
			}
			if (s[i] == ',') {
				const int delimiter_first = delimiters.empty() ? 0 : delimiters.back().first_;
				const auto it = std::find_if(blocks.begin(), blocks.end(), [delimiter_first, &s](const Block& b) { return b.first_ > delimiter_first && s[b.last_] != ';' && s[b.last_] != ','; });
				const int first = it == blocks.cend() ? i : it->first_;
				auto b = Block(first, i);
				b.blocks_.insert(it, blocks.begin(), blocks.end());
				blocks.erase(it, blocks.cend());
				blocks.push_back(std::move(b));
				continue;
			}
		}
		const int c_val = delimiter.at(s[i]);
		if (c_val > 0) {
			auto b = Block(i);
			delimiters.push_back(std::move(b));
			continue;
		}
		if (delimiter.at(s[delimiters.back().first_]) + c_val != 0) {
			std::cout << "ERROR: delimiter.at(s[delimiters.back().first_]) + c_val != 0";
			break;
		}
		const auto it = std::find_if(blocks.begin(), blocks.end(), [&delimiters](const Block& b) { return b.first_ > delimiters.back().first_; });
		delimiters.back().blocks_.insert(delimiters.back().blocks_.begin(), it, blocks.end());
		blocks.erase(it, blocks.cend());
		delimiters.back().last_ = i;
		blocks.push_back(delimiters.back());
		delimiters.pop_back();
	};
/*
	std::function<void(const Block& b)> treeify_loop;
	treeify_loop = [&treeify_loop, &s](const Block& b) -> void {
		static std::string indent = "";
		std::cout << indent << "scope: " << indent.size()
			<< ": first = '" << s[b.first_] << "' at " << b.first_
			<< "; last = '"  << s[b.last_]  << "' at " << b.last_
			<< std::endl;
		indent += " ";
		for (const Block& child_b : b.blocks_) {
			treeify_loop(child_b);
		}
		indent.erase(0, 1);
	};

	static const auto treeify = [&treeify_loop](const Block& b) -> void {
		treeify_loop(b);
	};

	treeify(Block(0, 0, blocks));

	std::cout << blocks.size() << " " << blocks.back().blocks_.size();
*/
	return 0;

}
