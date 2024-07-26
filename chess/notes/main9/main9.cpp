#include "main9.hpp"

#include <cassert>
#include <map>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <iostream>

Block::Block(char symbol, char last_symbol, int first, int last, ThoseBlocks blocks) :
	symbol_(symbol), last_symbol_(last_symbol),
	first_(first), last_(last), blocks_(std::move(blocks)) {};

Block::Block(int first, int last, ThoseBlocks blocks) : first_(first), last_(last) {
	AddBlocks(blocks);
};

Block::Block(Word word, Word last_word, BlockIter first, BlockIter last) :
	word_(word),
	last_word_(last_word) {
	
	const size_t distance = std::distance(first, last);
	assert(distance != 0);
	first_ = (*first)->first_;
	last_ = (*last)->last_;
	AddBlocks(first, last);
};
	
Block::Block(BlockIter first, BlockIter last) {
	std::cout << "Block::Block(BlockIter first, BlockIter last)"; // BUG
	AddBlocks(first, last);
};

ThatBlock Block::Pair(char symbol, char last_symbol, int first, int last, ThoseBlocks blocks) {
	return std::make_shared<Block>(symbol, last_symbol, first, last, std::move(blocks));
};

ThatBlock Block::Single(char symbol, int position) {
	return std::make_shared<Block>(symbol, symbol, position, position);
};

ThatBlock Block::SingleWord(int first, int last, ThoseBlocks blocks) {
	return std::make_shared<Block>(first, last, blocks);
};

ThatBlock Block::StartWord(Word word, BlockIter first, BlockIter last) {
	assert(std::distance(first, last) != 0);
	return std::make_shared<Block>(word, (*last)->last_word_, first, last);
};

ThatBlock Block::EndWord(Word last_word, BlockIter first, BlockIter last) {
	assert(std::distance(first, last) != 0);
	return std::make_shared<Block>((*first)->word_, last_word, first, last);
};

ThatBlock Block::PairWord(Word word, Word last_word, BlockIter first, BlockIter last) {
	return std::make_shared<Block>(word, last_word, first, last);
};

ThatBlock Block::Container(BlockIter first, BlockIter last) {
	std::cout << "ThatBlock Block::Container(BlockIter first, BlockIter last)"; // BUG
	return std::make_shared<Block>(first, last);
}
	
Word Block::full_word() const {
	Word result_word = "";
	for (const ThatBlock& b : blocks_) {
		result_word += b->IsSymbol() ? b->word_ : b->full_word();
	}
	return result_word;
};

Word& Block::word() { return word_; };
Word& Block::last_word() { return last_word_; };
int& Block::first() { return first_; };
int& Block::last() { return last_; };

BlockIter Block::begin() { return blocks_.begin(); };
ConstBlockIter Block::cbegin() const { return blocks_.cbegin(); };
BlockIter Block::end() { return blocks_.end(); };
ConstBlockIter Block::cend() const { return blocks_.cend(); };

Block::~Block() {
	// copied from FTXUI
	DetachAllBlocks();
}

void Block::DetachAllBlocks() {
	// copied from FTXUI
	while (!blocks_.empty()) {
		blocks_[0]->Detach();
	}
}

ThatBlock& Block::At(size_t n) {
	assert(n < Size());
	return blocks_[n];
};

size_t Block::Size() const {
	return blocks_.size();
};
	
void Block::AddBlocks(BlockIter first, BlockIter last) {
	std::cout << "void Block::AddBlocks(BlockIter first, size_t npos)"; // BUG
	last--;
	first--;
	for (; last != first; first--) {
		Add(*(last));
	}
};

void Block::AddBlocks(ThoseBlocks& blocks) {
	for (ThatBlock& b : blocks) {
		Add(b);
	}
};

void Block::Insert(size_t position, ThatBlock& b) {
	// copied from FTXUI
	b->parent_ = this;
	b->Detach();
	blocks_.insert(blocks_.begin() + position, std::move(b));
};

void Block::Add(ThatBlock& b) {
	// copied from FTXUI
	b->parent_ = this;
	b->Detach();
	blocks_.push_back(std::move(b));
};

void Block::Detach() {
	// copied from FTXUI
	if (parent_ == nullptr) {
		return;
	}
	auto it = std::find_if(
		parent_->blocks_.begin(),
		parent_->blocks_.end(),
		[this](const ThatBlock& b) {  
			return this == b.get();
		});
	Block* parent = parent_;
	parent_ = nullptr;
	parent->blocks_.erase(it);
}

Block* Block::Parent() const {
	return parent_;
};

ThoseBlocks Block::ToBlocks(const std::string& s) {
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
	ThoseBlocks result_blocks = {};
	ThoseBlocks unresolved_blocks = {};
	for (int i = 0; i < s.size(); i++) {
		// Pass: a character
		const char symbol = s[i];
		if (!delimiter_value_map.contains(symbol)) {
			auto b = Block::Single(symbol, i);
			result_blocks.push_back(std::move(b));
			continue;
		}
		// Pass: the character is a delimiter
		const int delimiter_value = delimiter_value_map.at(symbol);
		if (delimiter_value > 0) {
			auto b = Block::Single(symbol, i);
			unresolved_blocks.push_back(std::move(b));
			continue;
		}
	 	// Pass: the character is a closing delimiter
		assert(!unresolved_blocks.empty());
		// Pass: there is an unresolved delimiter
		assert(delimiter_value_map.at(unresolved_blocks.back()->symbol_) + delimiter_value == 0);
		// Pass: the character is pair with recent delimiter
		auto resolved_b = Block::Pair(unresolved_blocks.back()->symbol_, s[i], unresolved_blocks.back()->first_, i);
		unresolved_blocks.pop_back();
		// Pass: resolved pair is taken out of unresolved blocks
		const auto it = std::find_if(
			result_blocks.begin(),
			result_blocks.end(),
			[&resolved_b](const ThatBlock& b) -> bool { 
				return b->first_ > resolved_b->first_;
			}
		);
		resolved_b->blocks_.insert(
			resolved_b->blocks_.cend(),
			it, result_blocks.end()
		);
		result_blocks.erase(it, result_blocks.cend());
		// Pass: the enclosed in result blocks are moved to the resolved pair
		result_blocks.push_back(std::move(resolved_b));
		// Pass: the resolved pair is added to result blocks
	}
	// Pass: there are pair blocks that are resolved and are not resolved
	assert(unresolved_blocks.empty());
	// Pass: every pair blocks are resolved
	return result_blocks;
};

ThatBlock Block::ToBlock(const std::string& s) {
	// Pass: a string
	ThoseBlocks blocks = ToBlocks('{' + s + '}');
	// Pass: there is only one block in the those_blocks
	return blocks.at(0);
};

ThoseBlocks Block::ToRawBlocks(const std::string& s) {
	ThoseBlocks blocks = {};
	for (int i = 0; i < s.size(); i++) {
		auto b = Block::Single(s[i], i);
		blocks.push_back(std::move(b));
	};
	return blocks;
};

bool Block::IsSymbol() {
	return word_.size() == 1;
};
