#ifndef MAIN_10_HPP
#define MAIN_10_HPP

#include <vector>
#include <string>
#include <__functional/function.h>

class Block;
using Blocks = std::vector<Block>;

class Block {
public:
	Blocks blocks;
	std::string string;
public:
	Block() = default;
	Block(const std::string&);
};

using Decorator = std::function<Block(Block __b)>;
Block operator|(Block __b, Decorator __f);
Decorator operator|(Decorator __f, Decorator __f2);




#endif // MAIN_10_HPP
