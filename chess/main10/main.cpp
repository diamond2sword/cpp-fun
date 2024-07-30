#include "main10.hpp"
#include "see.hpp"
#include <algorithm>
#include <stdexcept>

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
	assert(__pos >= 0);
	assert(__pos + __nblocks >= 0);
	return [=](Block __b) -> Block {
		assert(__pos < __b.blocks.size());
		assert(__pos + __nblocks <= __b.blocks.size());

		auto __b2 = Block();
		auto __first = __b.blocks.begin() + __pos;
		auto __last = __first + __nblocks;
			
		__b2.blocks.insert(__b2.blocks.cend(), __first, __last);
		__b.blocks.erase(__first, __last);
		__b.blocks.insert(__first, __b2);

		return __b;
	};
}

Decorator GroupReversed(size_t __pos, size_t __nblocks) {
	assert(__pos - __nblocks + 1 >= 0);
	assert(__pos + 1 >= 0);
	return [=](Block __b) -> Block {
		assert(__pos - __nblocks + 1 < __b.blocks.size());
		assert(__pos + 1 <= __b.blocks.size());
		return __b | Group(__pos - __nblocks + 1, __nblocks);
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

Block SubRange(size_t __pos, size_t __nblocks, Block __b) {
	assert(__pos >= 0);
	assert(__pos + __nblocks >= 0);
	assert(__pos < __b.blocks.size());
	assert(__pos + __nblocks <= __b.blocks.size());

	__b |= Group(__pos, __nblocks);
	return __b.blocks[__pos];
}

std::string FullString(BlockIter __first, BlockIter __last) {
	std::string s = "";

	for (; __first != __last; ++__first)
		s += __first->blocks.empty() ? __first->string
			: FullString(__first->blocks.begin(), __first->blocks.end());

	return s;
};

std::string GetBlockStrings(BlockIter __first, BlockIter __last, size_t __nchars) {
	std::vector<std::string> strings = {};
	for (; __first != __last; ++__first)
		strings.push_back(__first->blocks.empty() ? __first->string
			: FullString(__first->blocks.begin(), __first->blocks.end()));

	std::string result_s = "";
	for (std::string& s : strings) {
		__nchars -= s.size();
		if (__nchars < 0) break;
		result_s += s;	
	}

	return result_s;
}

BlockIter FindString(std::string __s, BlockIter __b_first, BlockIter __b_last) {
	size_t __range_size = __s.size();
	size_t __b_size = std::distance(__b_first, __b_last);

	assert(__range_size > 0);
	assert(__b_size >= __range_size);

	BlockIter __iter_end = __b_first + 1 + __b_size - __range_size;
	std::string __range_b_full_string = __s;
	return std::find_if(__b_first, __iter_end, [&](Block& b) -> bool {
		return GetBlockStrings(
			__b_first,
			__b_first + __range_size,
			__range_b_full_string.size()
		) == __range_b_full_string;
	});
}

BlockIter FindString(BlockIter __range_first, BlockIter __range_last, BlockIter __b_first, BlockIter __b_last) {
	size_t __range_size = std::distance(__range_first, __range_last);
	size_t __b_size = std::distance(__b_first, __b_last);

	assert(__range_size > 0);
	assert(__b_size >= __range_size);

	return FindString(FullString(__range_first, __range_last), __b_first, __b_last);
}


BlockRange::BlockRange() : nblocks(0), string("") {};
BlockRange::BlockRange(BlockIter begin, size_t nblocks) : begin(begin), nblocks(nblocks), string(FullString(begin, begin + nblocks)) {};

BlockRange FindFirstOf(std::vector<std::string> __strings, BlockIter __b_first, BlockIter __b_last) {
	assert(__strings.size() > 0);
	assert(std::distance(__b_first, __b_last) > 0);

	BlockRange __first_block_range = BlockRange(__b_last, 0);

	for (std::string& __s : __strings) {

		assert(__s.size() > 0);

		BlockIter it = FindString(__s, __b_first, __b_last);
		if (it < __first_block_range.begin) {
			__first_block_range = BlockRange(it, __s.size());
		}
	}

	return __first_block_range;
}

std::vector<BlockIter> FindAll(std::string __s, BlockIter __b_first, BlockIter __b_last) {	
	size_t __b_size = std::distance(__b_first, __b_last) > 0;
	size_t __s_size = __s.size();

	assert(__s_size > 0);
	assert(__b_size >= __s_size);

	std::vector<BlockIter> __founds = {};
	
	for (BlockIter __it = __b_first; __it != __b_last; ++__it) {
		BlockIter __found = FindString(__s, __it, __b_last);
		if (__found == __b_last) {
			break;
		}
		__founds.push_back(__found);
		__it += __b_size - 1;
	}

	return __founds;
}

std::vector<BlockRange> FindAllAnyOf(std::vector<std::string> __strings, BlockIter __b_first, BlockIter __b_last) {
	std::vector<BlockRange> __founds = {};

	for (BlockIter __it = __b_first; __it != __b_last; ++__it) {
		BlockRange __found = FindFirstOf(__strings, __it, __b_last);
		if (__found.begin == __b_last) {
			break;
		}
		__founds.push_back(__found);
		__it += __found.nblocks - 1;
	}

	return __founds;
}


FindFunction FindString(std::string __s) {
	return [=](BlockIter __b_first, BlockIter __b_last) -> BlockRange {
		return FindFirstOf({__s}, __b_first, __b_last);
	};
}

std::vector<BlockRange> FindAllPatterned(std::vector<FindFunction> __find_functions, BlockIter __b_first, BlockIter __b_last) {
	
	std::vector<BlockRange> __founds = {};

	size_t __n_f_changes = 0;
	for (BlockIter __it = __b_first; __it != __b_last; ++__it) {
		BlockRange __found = __find_functions[__n_f_changes](__it, __b_last);
		if (__found.begin == __b_last) {
			break;
		}
		__founds.push_back(__found);
		__it += __found.nblocks - 1;
		++__n_f_changes;
		__n_f_changes %= __find_functions.size();
	}

	return __founds;
}

BlockRangePair::BlockRangePair(BlockRange __first, BlockRange __last) : first(__first), last(__last) {};
BlockRangePair::BlockRangePair(BlockIter __b_last) : first(BlockRange(__b_last, 0)), last(BlockRange(__b_last, 0)) {};

BlockRangePair FindPaired(std::string __s, std::string __s2, BlockIter __b_first, BlockIter __b_last) {
	BlockRange __found = FindString(__s)(__b_first, __b_last);
	if (__found.begin == __b_last) return BlockRangePair(__b_last);
	BlockRange __found2 = FindString(__s2)(__found.begin, __b_last);
	if (__found2.begin == __b_last) return BlockRangePair(__b_last);
	return BlockRangePair(__found, __found2);	
}

using FindPairFunction = std::function<BlockRangePair(BlockIter __b_first, BlockIter __b_last)>;

FindPairFunction FindPaired(std::string __s, std::string __s2) {
	return [=](BlockIter __b_first, BlockIter __b_last) -> BlockRangePair {
		return FindPaired(__s, __s2, __b_first, __b_last);
	};
}

BlockRangePair FindFirstOfPaired(std::vector<FindPairFunction> __find_pair_functions, BlockIter __b_first, BlockIter __b_last) {

	BlockRangePair __first_pair_found = BlockRangePair(__b_last);

	size_t __n_f_p_changes = 0;
	for (BlockIter __it = __b_first; __it != __b_last; ++__it) {
		BlockRangePair __found_pair = __find_pair_functions[__n_f_p_changes](__it, __b_last);
		if (__found_pair.first.begin == __b_last) return BlockRangePair(__b_last);
		if (__found_pair.first.begin >= __first_pair_found.first.begin) continue;
		__first_pair_found = __found_pair;
		__it = __found_pair.last.begin + __found_pair.last.nblocks - 1;
		++__n_f_p_changes;
		__n_f_p_changes %= __find_pair_functions.size();
	}

	return __first_pair_found;
}

std::vector<BlockRangePair> FindAllAnyOfPaired(std::vector<FindPairFunction> __find_pair_functions, BlockIter __b_first, BlockIter __b_last) {
	std::vector<BlockRangePair> __pairs = {};

	for (BlockIter __it = __b_first; __it != __b_last; ++__it) {
		BlockRangePair __found = FindFirstOfPaired(__find_pair_functions, __it, __b_last);
		if (__found.first.begin > __b_last) break; 
		__pairs.push_back(__found);
		__it = __found.last.begin + __found.last.nblocks - 1;
	}

	return __pairs;
}

StringPair::StringPair(std::string __first, std::string __last) : first(__first), last(__last) {};

std::vector<BlockRangePair> FindAllAnyOfPaired(std::vector<StringPair> __s_pairs, BlockIter __b_first, BlockIter __b_last) {
	std::vector<FindPairFunction> __find_pair_functions = {};
	for (StringPair __s_pair : __s_pairs) {
		__find_pair_functions.push_back(FindPaired(__s_pair.first, __s_pair.last));
	}

	return FindAllAnyOfPaired(__find_pair_functions, __b_first, __b_last);
}
using BlockRangeSequence = std::vector<BlockRange>;
using FindSequenceFunction = std::function<BlockRangeSequence(BlockIter __b_first, BlockIter __b_last)>;

FindSequenceFunction FindSequenceAll(std::vector<std::string> __strings) {
	std::vector<FindFunction> __find_functions = {};
	for (std::string& __s : __strings) {
		__find_functions.push_back(FindString(__s));
	}
	return [=](BlockIter __b_first, BlockIter __b_last) -> BlockRangeSequence {
		return FindAllPatterned(__find_functions, __b_first, __b_last);
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
	c |= GroupReversed(1, 3);
	see(c.blocks.size());

	return 0;
}
