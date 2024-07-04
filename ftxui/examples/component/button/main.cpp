// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <cmath>
#include <ftxui/component/component_options.hpp>
#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border

using namespace ftxui;

// This is a helper function to create a button with a custom style.
// The style is defined by a lambda function that takes an EntryState and
// returns an Element.
// We are using `center` to center the text inside the button, then `border` to
// add a border around the button, and finally `flex` to make the button fill
// the available space.
ButtonOption Style() {
  auto option = ButtonOption::Animated();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) {
      element |= bold;
    }
    return element | center | borderEmpty | flex;
  };
  return option;
}

ButtonOption Style2() {
	auto option = ButtonOption::Animated();
	option.transform = [](const EntryState& s) {
		std::string label = s.label;
		if (s.focused) {
			label = "bold " + s.label;
		}
    	auto element = text(label);
 	   	if (!s.focused) {
    		element |= bold;
	    }
    	return element | center | borderEmpty | flex;
	};
	return option;
}

int main() {
  long long int value = 50;

  // clang-format off
  auto btn_dec_01 = Button("-1", [&] { value -= 1; }, Style());
  auto btn_inc_01 = Button("+1", [&] { value += 1; }, Style2());
  auto btn_dec_10 = Button("-v/2", [&] { value -= value/2; }, Style());
  auto btn_inc_10 = Button("+something I don't know", [&] { value += value*value; }, Style());
  // clang-format on

  // The tree of components. This defines how to navigate using the keyboard.
  // The selected `row` is shared to get a grid layout.
  int row = 0;
  auto buttons = Container::Vertical({
      Container::Horizontal({btn_dec_01, btn_inc_01}, &row) | flex,
      Container::Horizontal({btn_dec_10, btn_inc_10}, &row) | flex,
  });

  // Modify the way to render them on screen:
  auto component = Renderer(buttons, [&] {
    return vbox({
               text("value = " + std::to_string(value)),
               separator(),
			   separator(),
               buttons->Render() | flex,
           }) |
           flex | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
  return 0;
}
