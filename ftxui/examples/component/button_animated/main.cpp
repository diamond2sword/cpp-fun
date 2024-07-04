// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <algorithm>
#include <iostream>
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for gauge, separator, text, vbox, operator|, Element, border
#include "ftxui/screen/color.hpp"  // for Color, Color::Blue, Color::Green, Color::Red

using namespace ftxui;


int main() {
	int min_value = -50;
	int max_value = 50;
	if (min_value > max_value) {
		std::cout << "The min value must not be greater than max value";
		return 1;
	}
  int default_value = (min_value + max_value) / 2; //default
	int value = default_value;
	

  // The tree of components. This defines how to navigate using the keyboard.
  auto buttons = Container::Horizontal({
      Button(
          "Decrease", [&] { value = std::max(value - 1, min_value); }, ButtonOption::Animated(Color::Red)),
      Button(
          "Reset", [&] { value = default_value; }, ButtonOption::Animated(Color::Green)),
      Button(
          "Increase", [&] { value = std::min(value + 1, max_value); }, ButtonOption::Animated(Color::Blue)),
  });

  // Modify the way to render them on screen:
  auto component = Renderer(buttons, [&] {
    return vbox({
        vbox({
            text("value = " + std::to_string(value)),
            separator(),
            gauge((value - min_value) / (float)(max_value - min_value)),
        }) | border,
        buttons->Render(),
    });
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
  return 0;
}
