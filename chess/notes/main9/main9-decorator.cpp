#include "main9.hpp"
#include <algorithm>
#include <cassert>

ThatBlock operator|(ThatBlock b, BlockDecorator transform) {
	return transform(b);
};


BlockDecorator foldAtAnyStartWord(const Phrase& start_words) {
	return [&start_words](ThatBlock b) {
		std::vector<size_t> word_depths (start_words.size(), 0);
		std::vector<size_t> word_firsts (start_words.size(), 0);

		auto that_block = b->begin();
		for (; that_block != b->cend(); that_block++) {
			// Pass: a child block
			auto that_word = start_words.begin();
			auto that_depth = word_depths.begin();
			auto that_first = word_firsts.begin();
			for (; that_word != start_words.cend();
				that_word++, that_depth++, that_first++) {
				// Pass: a start word
				auto that_symbol = that_word->begin() + *that_depth;
				// Pass: a letter of the start word, according to depth it is in
/*				if (!b->At(i)->IsSymbol()) {
					// Pass: a word*/
				const Word this_word = (*that_block)->full_word();
				//auto this_symbol = w.begin();
				// Pass: this word and that word have different sizes
				// Pass: this word is unsimilar at one point with that word
				if (this_word.size() > std::distance(that_symbol, that_word->cend()) 
					|| !std::equal(this_word.begin(), this_word.end(), that_symbol)) {
					// Pass: word can not be included
					*that_depth = 0;
					continue;
				}
				// Pass: this word is not greater than that word
				// Pass: this word is inside that word
				// Pass: c and b->At(i) is equal

				// Pass: The first of the word is not recorded
				if (*that_depth == 0) {
					*that_first = std::distance(b->begin(), that_block);
				}
				// Pass: depth does not refer to distance reached
				*that_depth += std::distance(that_word->cbegin(), that_symbol + this_word.size());
				// Pass: depth now refers to next symbol of the word
				//continue;
					/*if (w.size() > std::distance(that_symbol, that_word->cend())) {
						// Pass: word can not be included
						*that_depth = 0;
						continue;
					}
					// Pass: this word is not greater than that word
					if (!std::equal(w.begin(), w.end(), that_symbol)) {
						*that_depth = 0;
						continue;
					}*/
					//for (; this_symbol != w.cend() && *this_symbol == *that_symbol; that_symbol++, this_symbol++) {}
					// Pass: this word is unsimilar at one point with that word
					/*if (this_symbol != w.cend()) {
						*that_depth = 0;
						continue;
					}*/
					// Pass: this word is inside that word
					/**that_depth++;
					continue;*/
/*				};
				// Pass: c and b at i are both symbols
				if (*that_symbol != b->At(i)->word()[0]) {
					*that_depth = 0;
					continue;
				}
				// Pass: c and b->At(i) is equal
				const size_t current_depth = *that_depth;*/
				//(*that_depth)++;
				if (*that_depth != that_word->size()) {
					continue;
				}
				// Pass: this is the end symbol of the word
				// Pass: the word depths are still being continued	
				for (size_t& depth : word_depths) {
					depth = 0;
				}
size_t then_n_blocks = b->Size();
				// Pass: start word is not grouped to one block
				auto that_first_block = b->begin() + *that_first;
				auto start_b = Block::StartWord(*that_word, that_first_block, that_block + 1);
size_t now_n_blocks = b->Size();
assert(now_n_blocks != then_n_blocks);
				b->Insert(*that_first, start_b);
size_t inserted_n_blocks = b->Size();
assert(inserted_n_blocks != now_n_blocks);
				that_block = that_first_block;
				break;
			}
		}
		return b;
	};
};


