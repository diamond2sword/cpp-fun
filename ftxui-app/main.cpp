#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include "hello-world.hpp"

int main(){
  const std::string hello {"Hello"},
        world {"World"},
        myftxui {"FTXUI"};

  ftxui::Element doc = ftxui::hbox(
    ftxui::text( hello ),
    ftxui::text( world ) | ftxui::border,
    ftxui::text( myftxui ) | ftxui::border
  );
  ftxui::Screen screen = ftxui::Screen::Create(
    ftxui::Dimension::Full(),
    ftxui::Dimension::Fit(doc)
  );

  ftxui::Render(screen, doc);
  screen.Print();

  std::cout << '\n';
	hello_world::execute();
  return 0;
}
