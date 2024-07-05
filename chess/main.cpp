// c++26
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>

int main() {
/*	using Cell = std::unordered_map<int, int>;
	Cell* board[64];
	for (int i = 0; i < 10; i++) {
		board[i] = new Cell;
	}
	for (int i = 0; i < 1000000; i++) {
		board[0]->insert({i, i});
		if (i == 999999) {
			std::cout << i+i;
		}
	}
	std::cout << board[0]->at(999999);

	std::filesystem::path filepath = "main.cpp";
	std::filebuf fb;
	if (!fb.open(filepath, std::ios::binary | std::ios::in)) {
		std::cout << "Open file " << filepath << " for read failed";
		return 1;
	}
*/

	char8_t h = 'a';
	std::cout << sizeof(h);
	return 0;
}
