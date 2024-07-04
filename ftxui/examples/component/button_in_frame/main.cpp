// Copyright 2022 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for to_string, operator+

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Button, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for operator|, text, Element, hbox, separator, size, vbox, border, frame, vscroll_indicator, HEIGHT, LESS_THAN
#include "ftxui/screen/color.hpp"  // for Color, Color::Default, Color::GrayDark, Color::White

using namespace ftxui;


int main() {
  int counter = 0;

	auto Style = [&](std::string&& active_label) -> ButtonOption {
	  auto option = ButtonOption::Animated(Color::Default, Color::GrayDark, Color::Default, Color::White);
	  option.transform = [&, active_label](const EntryState& s) {
		  std::string label = s.label;
		  if (s.focused) {
			label = label + ": " + active_label;
		  }
		  auto element = text(label);
		return element | center | borderEmpty;
	  };
	  return option;
	};

  auto container = Container::Vertical({});
  for (int i = 0; i < 30; ++i) {
	auto button = Button("Button " + std::to_string(i), [&, i]{ counter += i;}, Style("Adds " + std::to_string(i)));
    container->Add(button);
  }

  auto renderer = Renderer(container, [&] {
    return vbox({
               hbox({
                   text("Counter:"),
				   text(std::to_string(counter)),
               }) | center,
               separator(),
               container->Render() |  frame | vscroll_indicator |
                   size(HEIGHT, LESS_THAN, 20), 
           }) |
           border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(renderer);

  return 0;
}
