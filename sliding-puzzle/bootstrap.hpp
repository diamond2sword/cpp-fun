#include "sliding_puzzle.hpp"

#include <chrono>
#include <iomanip>
#include <ios>
#include <map>
#include <sstream>
#include <vector>

#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>
#include <ftxui/screen/screen.hpp>

ftxui::Element printBoard(const sp::Board &board, const std::string &duration) {
  using namespace ftxui;
  using namespace std;
  using namespace sp;

  auto make_tile = [](Board::value_type::value_type value) {
    string label = value ? to_string(value) : "";
    auto tile = text(label) | hcenter | vcenter;

    if (value) {
      tile |= border;
    }

    tile |= size(WIDTH, EQUAL, 9) | size(HEIGHT, EQUAL, 5) | color(Color::Blue);

    return tile;
  };

  auto make_row = [&](const Board::value_type &values) {
    vector<Element> tiles{};
    for (auto &&value : values) {
      tiles.push_back(make_tile(value));
    }
    return hbox(tiles);
  };

  auto make_board = [&](const Board &board) {
    vector<Element> rows{};
    for (auto &&row : board) {
      rows.push_back(make_row(row));
    }

    if (duration.size())
      rows.push_back(text(duration) | hcenter);

    return vbox(rows) | hcenter | vcenter | border;
  };

  return make_board(board);
}

void runPuzzle(size_t size) {
  using namespace std;
  using namespace chrono;
  using namespace ftxui;
  using namespace sp;

  SlidingPuzzle puzzle(size);
  time_point<high_resolution_clock> start{};
  time_point<high_resolution_clock> end{};

  auto timeToText{[&]() {
    auto time{end - start};
    stringstream stream;
    stream << "completion time = " << std::fixed << std::setprecision(3)
           << duration_cast<milliseconds>(time).count() / 1000.0 << " seconds";
    return stream.str();
  }};

  auto screen = ScreenInteractive::Fullscreen();

  auto renderer = Renderer([&] {
    string duration{};
    if (end != time_point<high_resolution_clock>{})
      duration = timeToText();

    return printBoard(puzzle.getBoard(), duration);
  });

  auto component = CatchEvent(renderer, [&](Event event) {
    if (start == time_point<high_resolution_clock>{}) {
      start = high_resolution_clock::now();
    }

    if (event == Event::Character('q')) {
      screen.ExitLoopClosure()();
      return true;
    }

    // movement of the numbered tiles is opposite of the free tile
    if (event == Event::ArrowUp) {
      puzzle.moveDown();
    }
    if (event == Event::ArrowDown) {
      puzzle.moveUp();
    }
    if (event == Event::ArrowLeft) {
      puzzle.moveRight();
    }
    if (event == Event::ArrowRight) {
      puzzle.moveLeft();
    }

    if (end == time_point<high_resolution_clock>{} && puzzle.isInOrder()) {
      end = high_resolution_clock::now();
    }

    return false;
  });

  screen.Loop(component);
}
