#include "main10.hpp"

#include <iostream>

Block SetHello(Block __b) {
	__b.string += "hello";
	return __b;
}

Decorator Add(Block __b) {
	return [&](Block __b2) {
		__b2.blocks.push_back(__b);
		return __b2;
	};
}

Decorator Add(Blocks __bs) {
	return [&](Block __b) -> Block {
		__b.blocks.insert(__b.blocks.cend(), __bs.begin(), __bs.end());
		return __b;
	};
}

Decorator Insert(size_t __n, Block __b) {
	return [&](Block __b2) -> Block {
		__b2.blocks.insert(__b2.blocks.begin() + __n, __b);
		return __b2;
	};
}

Decorator Insert(size_t __n, Blocks __bs) {
	return [&](Block __b) {
		__b.blocks.insert(__b.blocks.begin() + __n, __bs.begin(), __bs.end());
		return __b;
	};
}


Block operator|(Block __b, Decorator __f) { return __f(__b); }

Decorator operator|(Decorator __f, Decorator __f2) {
	return [&](Block __b) -> Block {
		return __b | __f | __f2;
	};
}

Block::Block(const std::string& __s) : string(__s) {};

int main() {
	Block b = Block("Hello") | Add({Block(), Block(), Block()});
	auto c = b | SetHello | SetHello | Add(b) | Insert(0, Block("okay"));
	std::cout << c.blocks[0].string;
	return 0;
}
