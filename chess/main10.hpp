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
	bool has_transformed;
public:
	Block();
	Block(const std::string&);
};

using Decorator = std::function<Block(Block __b)>;
using ConditionalDecorator = std::function<Decorator(Block)>;
using ConditionalDecorators = std::vector<ConditionalDecorator>;
using Predicate = std::function<bool(Block)>;
Decorator operator|(Decorator __f, Decorator __f2);
Block operator|(Block __b, Decorator __f);
Block& operator|=(Block& __b, Decorator __f);
Block operator|(Block __b, ConditionalDecorator __f);
Block& operator|=(Block& __b, ConditionalDecorator __f);
Block Nothing(Block __b);
Decorator If(bool __e, Decorator __f);

#endif // MAIN_10_HPP
