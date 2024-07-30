#ifndef MAIN_10_HPP
#define MAIN_10_HPP

#include <vector>
#include <string>
#include <__functional/function.h>

class Block;
using Blocks = std::vector<Block>;
using BlockIter = std::__wrap_iter<Block*>;

class Block {
public:
	Blocks blocks;
	std::string string;
	bool has_transformed;
public:
	Block();
	Block(const std::string&);
};

class BlockRange {
public:
	BlockRange();
	BlockRange(BlockIter begin, size_t nblocks);
	BlockIter begin;
	std::string string;
	size_t nblocks;
};

class BlockRangePair {
public:
	BlockRangePair(BlockRange __first, BlockRange __last);
	BlockRangePair(BlockIter __b_last);
	BlockRange first;
	BlockRange last;
};

class StringPair {
public:
	StringPair(std::string __first, std::string __last);
	std::string first;
	std::string last;
};

using FindFunction = std::function<BlockRange(BlockIter __b_first, BlockIter __b_last)>;

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
