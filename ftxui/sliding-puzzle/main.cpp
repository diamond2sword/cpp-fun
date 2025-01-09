#include <iostream>
#include "bootstrap.hpp"

int main() {
  std::cout << "q: Exit, Arrow Keys: Moving Tiles. Enter "
               "size of the board (3 for a 3x3 board): ";

  size_t size{};
  std::cin >> size;

  if (size == 0) {
    std::cout << "Invalid size." << std::endl;
  }

  runPuzzle(size);
}
