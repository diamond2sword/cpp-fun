#pragma once

#include <algorithm>
#include <exception>
#include <functional>
#include <numeric>
#include <random>
#include <utility>
#include <vector>

namespace sp {

using Board = std::vector<std::vector<size_t>>;

class SlidingPuzzle {
public:
  SlidingPuzzle(size_t size) {
    if (size == 0)
      throw std::out_of_range{"invalid size"};

    for (size_t i{0}; i < size; i++) {
      Board::value_type row(size, 0);
      for (size_t j{0}; j < size; j++) {
        row[j] = i * size + j + 1;
      }
      m_board.push_back(row);
    }

    m_board[size - 1][size - 1] = 0;
    m_freeTileX = m_freeTileY = size - 1;

    shuffle();
  }

  void moveUp() {
    if (m_freeTileX > 0) {
      std::swap(m_board[m_freeTileX][m_freeTileY],
                m_board[m_freeTileX - 1][m_freeTileY]);
      m_freeTileX -= 1;
    }
  }

  void moveDown() {
    if (m_freeTileX < (m_board.size() - 1)) {
      std::swap(m_board[m_freeTileX][m_freeTileY],
                m_board[m_freeTileX + 1][m_freeTileY]);
      m_freeTileX += 1;
    }
  }

  void moveLeft() {
    if (m_freeTileY > 0) {
      std::swap(m_board[m_freeTileX][m_freeTileY],
                m_board[m_freeTileX][m_freeTileY - 1]);
      m_freeTileY -= 1;
    }
  }

  void moveRight() {
    if (m_freeTileY < (m_board.size() - 1)) {
      std::swap(m_board[m_freeTileX][m_freeTileY],
                m_board[m_freeTileX][m_freeTileY + 1]);
      m_freeTileY += 1;
    }
  }

  const Board &getBoard() { return m_board; }

  void shuffle() {
    std::mt19937 engine(std::random_device{}());
    std::discrete_distribution<size_t> dist{1, 1, 1, 1};

    size_t size{m_board.size()};
    for (size_t i = 0; i < size * size * size + 1000; i++) {
      auto move{dist(engine)};
      switch (move) {
      case 0:
        moveUp();
        break;
      case 1:
        moveDown();
        break;
      case 2:
        moveLeft();
        break;
      case 3:
        moveRight();
        break;
      default:
        break;
      }
    }
  }

  bool isInOrder() {
    size_t lastValue{};
    for (auto &&row : m_board) {
      for (auto &&value : row) {
        if (value) {
          if (lastValue >= value)
            return false;
          lastValue = value;
        }
      }
    }
    return true;
  }

private:
  Board m_board{};
  size_t m_freeTileX{};
  size_t m_freeTileY{};
};
} // namespace sp
