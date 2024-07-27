#include "main10.hpp"


Block SetHello(Block __b) {
	__b.string += "hello";
	return __b;
}

Decorator Add(Block __b) {
	return [=](Block __b2) {
		__b2.blocks.push_back(__b);
		return __b2;
	};
}

Decorator Add(Blocks __bs) {
	return [=](Block __b) -> Block {
		__b.blocks.insert(__b.blocks.cend(), __bs.begin(), __bs.end());
		return __b;
	};
}

Decorator Insert(size_t __pos, Block __b) {
	assert(__pos >= 0);
	return [=](Block __b2) -> Block {
		assert(__pos < __b2.blocks.size());
		__b2.blocks.insert(__b2.blocks.cbegin() + __pos, __b);
		return __b2;
	};
}

Decorator Insert(size_t __pos, Blocks __bs) {
	assert(__pos >= 0);
	return [=](Block __b) -> Block {
		assert(__pos < __b.blocks.size());

		__b.blocks.insert(
			__b.blocks.cbegin() + __pos, 
			__bs.begin(), __bs.end()
		);

		return __b;
	};
}

Decorator Group(size_t __pos, size_t __nblocks) {
	assert(
		__pos >= 0 &&
		__pos + __nblocks >= 0
	);
	return [=](Block __b) noexcept -> Block {
		assert(
			__pos < __b.blocks.size() &&
			__pos + __nblocks <= __b.blocks.size()
		);

		auto __b2 = Block();
		auto __first = __b.blocks.begin() + __pos;
		auto __last = __first + __nblocks;
			
		__b2.blocks.insert(__b2.blocks.cend(), __first, __last);
		__b.blocks.erase(__first, __last);
		__b.blocks.insert(__first, __b2);

		return __b;
	};
}

Decorator Ungroup(size_t __pos) {
	assert(__pos >= 0);
	return [=](Block __b) -> Block {
		assert(__pos < __b.blocks.size());
		assert(__b.blocks[__pos].blocks.size() > 0);
		
		__b.blocks.insert(
			__b.blocks.cbegin() + __pos + 1,
			__b.blocks[__pos].blocks.begin(),
			__b.blocks[__pos].blocks.end()
		);
		__b.blocks.erase(__b.blocks.cbegin() + __pos);

		return __b;
	};
}


Decorator AddIfEmpty(Block __b) {
	return If(__b.blocks.empty(), Add(Block()));
}

ConditionalDecorator AddIf(Predicate __pred, Block __such_b = Block()) {
	return [=](Block __b) -> Decorator {
		return If(__pred(__b), Add(__such_b));
	};
}

ConditionalDecorator AddSuchIfEmpty(Block __such_b) {
	return [=](Block __b) -> Decorator {
		return If(__b.blocks.empty(), Add(__such_b));
	};
}

ConditionalDecorator Switch(ConditionalDecorators __conditionals) {
	return [=](Block __b) -> Decorator {
		for (auto& __f : __conditionals)  {
			if ((__b | __f).has_transformed) {
				return __f(__b);
			}
		}
		return Nothing;
	};
}

Block Nothing(Block __b) {
	assert(__b.has_transformed);
	__b.has_transformed = false;
	return __b;
}

Decorator If(bool __e, Decorator __f) {	return __e ? __f : Nothing; };

Block operator|(Block __b, ConditionalDecorator __f) { return __b | __f(__b); }
Block& operator|=(Block& __b, ConditionalDecorator __f) { return __b = __b | __f; }

Block operator|(Block __b, Decorator __f) {
	__b.has_transformed = true;
	return __f(__b); 
}

Block& operator|=(Block& __b, Decorator __f) { return __b = __b | __f; }

Decorator operator|(Decorator __f, Decorator __f2) {
	return [=](Block __b) -> Block {
		return __b | __f | __f2;
	};
}

Block::Block(const std::string& __s) : string(__s) {};

Block::Block() : string(""), blocks({}), has_transformed(false) {};

int main() {
	std::cout << "Hello";
	Block b = Block("Hello") | Add({Block(), Block(), Block()});
	auto f = Add(Block("Ginuel")) | Group(0, 2);
	auto c = b | SetHello | SetHello | Add(b) | Insert(0, Block("okay")) | Group(0, 5);
	c |= f;
	c |= (Ungroup(0) | Ungroup(0)); c.blocks.clear();
	c |= AddIfEmpty; c.blocks.clear();
	c |= AddSuchIfEmpty(Block("Good Morning"));
	see(c.blocks.size());
	c |= Switch({
		AddSuchIfEmpty(Block("Good Morning")),
		AddIf([](Block __b) { return __b.blocks.size() == 1; }, Block("Good Evening")),
		AddIf([](Block __b) { return __b.blocks.size() == 2; }, Block("Good Afternoon"))
	});
	see(c.blocks.size());
	see(c.blocks[0].string);
	see(c.blocks[1].string);
	seeifnot(false, false);
	return 0;
}
