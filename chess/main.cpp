#include <iostream>
#include <vector>
#include <ncurses.h>

struct Walk {
public:
	void (*onEmpty)();
	void (*onNotEmpty)();
	void (*onEnemy)();
	void (*onAlly)();
	void (*onWalk)();
	void (*onFailure)();
	void (*onSuccess)() = [](){
		std::cout << "Hello World!" << std::endl;
	};
};

#include <vector>
template<class A, class B, class C>
std::vector<std::vector<std::vector<int(*)(A***)>(*)(A**)>(*)(A*)> a = {[](A*){B a = {[](A**){C a = {[](A***){return 0;}}; return a;}}; return a;}};

template<class A> 
auto a<A, std::vector<std::vector<int(*)(A***)>(*)(A**)>, std::vector<int(*)(A***)>> = {[](A*){auto a = {[](A**){auto a = {[](A***){return 0;}}; return a;}}; return a;}};

template<>
auto a<char, std::vector<std::vector<int(*)(char***)>(*)(char**)>, std::vector<int(*)(char***)>> = {[](char*){auto a = {[](char**){auto a = {[](char***){return 0;}}; return a;}}; return a;}};

/*
auto walk = [](int step, void (*onEmpty)()) { 
	// void (*onNotEmpty)()=0, void (*onEnemy)()=0, void (*onAlly)()=0, void (*onWalk)()=0, void (*onFailure)()=0, void (*onSuccess)()=0) {
	onEmpty();
	return 0;
};
*/

int main(int argc, char** argv) {
/*
	std::cout << "Hello World!" << std::endl;
	Chess c;
	c.fen = "rnbkqbnr/pppppppp/8/8/8/8/pppppppp/RNBKQBNR w KQkq - 0 1";
	c.load_fen();
	for (int i = 0; i < c.board.size(); i++) {
		std::cout << i + 1 << "p: " << c.board[i].name << " " << std::endl;
	}
*/
	auto hello = [](int argc, char** argv) {
		auto hello = [](int argc, char** argv) {
			auto hello = [](int argc, char** argv) {
				return main(argc, argv);
			};
			return hello(argc, argv);
		};
		return hello(argc, argv);
	};
	std::vector<int (*)(int, char**)> fatv = {main, main, hello, hello, [](int argc, char** argv){return main(argc, argv);}};
	/* Move generation through functions
	 * Function Pointers - helps to make the process of generating piece abilities possible
	 * Move conditions, iterative moves, iterative move conditions, 
	 */
	fatv.push_back(main);
	std::cout << "Hello World!" << std::endl;
	static int rc;
	if (rc == 1) {
		return 0;
	}
	rc++;
	for (int (*fun)(int, char**): fatv) {
		fun(argc, argv);
	}
	
	return 0;
}
