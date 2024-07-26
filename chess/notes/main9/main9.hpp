#ifndef MAIN_9_HPP
#define MAIN_9_HPP

#include <__memory/shared_ptr.h>
#include <__functional/function.h>
#include <__iterator/wrap_iter.h>
#include <string>
#include <vector>
#include <map>

	
class Block;
using ThatBlock = std::shared_ptr<Block>;
using ThoseBlocks = std::vector<ThatBlock>;
using BlockIter = std::__wrap_iter<ThatBlock*>;
using ConstBlockIter = std::__wrap_iter<const ThatBlock*>;

using Symbol = char;
using Word = std::basic_string<Symbol>;
using Phrase = std::vector<Word>;
using WordValueMap = std::map<Word, int>;

class Block {
public:
	Block(char symbol, char last_symbol, int first, int last, ThoseBlocks blocks = {});
	Block(int first, int last, ThoseBlocks blocks = {});
	Block(Word word, Word last_word, BlockIter first, BlockIter last);
	Block(BlockIter first, BlockIter last);
	~Block();
public:
	static ThatBlock Single(char symbol, int position);
	static ThatBlock Pair(char symbol, char last_symbol, int first, int last, ThoseBlocks blocks = {});
	static ThatBlock SingleWord(int first, int last, ThoseBlocks blocks = {});
	static ThatBlock StartWord(Word word, BlockIter first, BlockIter last);
	static ThatBlock EndWord(Word last_word, BlockIter first, BlockIter last);
	static ThatBlock PairWord(Word word, Word last_word, BlockIter first, BlockIter last);
	static ThatBlock Container(BlockIter first, BlockIter last);
private:
	char symbol_;
	char last_symbol_;
	Word word_ = "";
	Word last_word_ = "";
	int first_ = -1;
	int last_ = -1;
	ThoseBlocks blocks_ = {};
	Block* parent_ = nullptr;
public:
	Word& word();
	Word& last_word();
	int& first();
	int& last();
	Word full_word() const;
public:
	bool IsSymbol();
public:
	BlockIter begin();
	ConstBlockIter cbegin() const;
	BlockIter end();
	ConstBlockIter cend() const;
public:
	size_t Size() const;
	ThatBlock& At(size_t position);
	void Add(ThatBlock&);
	void Insert(size_t position, ThatBlock&);
	void AddBlocks(ThoseBlocks&);
	void AddBlocks(BlockIter first, BlockIter last);
	void Detach();
	void DetachAllBlocks();
	Block* Parent() const;
public:
	static int main();
public:
	static ThoseBlocks ToRawBlocks(const std::string& s);
	static ThoseBlocks ToBlocks(const std::string& s);
	static ThatBlock ToBlock(const std::string& s);
	static std::string Treeify(ThatBlock b);

};

using BlockDecorator = std::function<ThatBlock(ThatBlock)>;
using BlockCondition = std::function<BlockDecorator(ThatBlock, BlockDecorator)>;
using BlockConditions = std::vector<BlockCondition>;

ThatBlock operator|(ThatBlock, BlockDecorator);
ThatBlock operator|(ThatBlock, BlockConditions);
// DECORATOR
BlockDecorator foldAtAnyStartWord(const Phrase&);
BlockDecorator foldAtAnyEndWord(const Phrase&);
BlockDecorator foldAtAnyWordPair(const WordValueMap&);
BlockDecorator decorateBlockAt(size_t N, BlockDecorator);
BlockDecorator decorateAllBlocks(BlockDecorator);

// CONDITIONAL DECORATOR
BlockDecorator isEmpty(ThatBlock, BlockDecorator);
BlockCondition isBlockCount(size_t N, BlockDecorator);
BlockCondition containsAnyWord(const Phrase&, BlockDecorator);
BlockCondition isAnyWordPair(const WordValueMap&, BlockDecorator);
BlockCondition isAnyBlock(BlockCondition, BlockDecorator);
BlockCondition areAllBlocks(BlockCondition, BlockDecorator);
BlockCondition isBlockAt(size_t N, BlockCondition, BlockDecorator);
BlockCondition isParentAt(size_t N, BlockCondition, BlockDecorator);

/*ThatBlock& operator|=(ThatBlock&, BlockDecorator);
ThoseBlocks operator|(ThoseBlocks, BlockDecorator);*/
//BlockDecorator operator|(BlockDecorator, BlockDecorator);


//ThatBlock operator|(ThatBlock, BlockCondition);


/*BlockDecorator foldPair(Symbol start, Symbol end);
BlockDecorator foldAtAnyPair(const std::map<char, int>& delimiter_value_map);*/

/*BlockDecorator foldAtEnd(Symbol);
BlockDecorator foldAtStart(Symbol);
BlockDecorator foldWord(Word);
BlockDecorator foldAtEndWord(Word);
BlockDecorator foldAtStartWord(Word);*/

//ThatBlock reversed(ThatBlock);

/*BlockDecorator foldAtAnyEnd(Word);
BlockDecorator foldAtAnyStart(Word);
BlockDecorator foldAtAnyWord(Phrase);*/

/*BlockDecorator isSymbol(ThatBlock, BlockDecorator);
BlockCondition contains(Symbol, BlockDecorator);
BlockCondition containsWord(Word, BlockDecorator);
BlockCondition containsPhrase(Phrase, BlockDecorator);
BlockCondition containsAnySymbol(Word, BlockDecorator);*/

/*BlockDecorator copyChildAt(size_t N);
BlockDecorator copyParentAt(size_t N);
BlockDecorator copySelfToChildAt(size_t N);*/

/*BlockCondition isPair(Symbol start, Symbol end, BlockDecorator);
BlockCondition isWordPair(Word start, Word end, BlockDecorator);*/


//ThatBlock detachAllBlocks(ThatBlock);
//BlockDecorator detachBlockAt(size_t N);





#endif // MAIN_9_HPP
