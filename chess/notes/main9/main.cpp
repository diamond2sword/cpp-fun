#include "main9.hpp"

#include <cassert>
#include <iostream>

int main() {
	Block::main();
	return 0;
}

int Block::main() {
	std::cout << "int Block::main()"; // BUG
	auto b = ToRawBlocks("Hello");
	Phrase l = {"Hello"};
	assert(b.begin() == b.end());
	std::cout << "int Block::main()"; // BUG
	auto container = Container(b.begin(), b.end());
	return 0;
	auto decorator = foldAtAnyStartWord(l); 
	auto now_b = container | decorator;
	assert(now_b->Size() == 1);
	return 0;
};
