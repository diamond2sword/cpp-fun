// Copyright 2021 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/screen/terminal.hpp>
#include <memory>  // for allocator, shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string
#include <vector>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, separator, Element, operator|, vbox, border

using namespace ftxui;

// An example of how to compose multiple components into one and maintain their
// interactiveness.

auto get_separator = [] -> Component {
	return Renderer([] { return separator(); });
};

int main() {

	std::string pane_names[9] = {"top left", "top", "top right", "left", "center", "right", "bottom left", "bottom", "bottom right"};
	int pane_counters[9];
	Components buttons_containers;
	Components pane_containers;
	Components v_compositions;

	Component h_composition = Container::Horizontal({});
	for (int i = 0; i < 3; i++) {
		Component v_composition = Container::Vertical({});
		for (int j = 0; j < 3; j++) {
			int index = i * 3 + j;
			int reverse_index = j * 3 + i;

			pane_counters[index] = 0;

			buttons_containers.push_back(
				Container::Horizontal({
		    		Button("Decrease", [&, index] { pane_counters[index]--; }) | center | flex,
			    	Button("Increase", [&, index] { pane_counters[index]++; }) | center | flex,
			  	})
			);

		  	pane_containers.push_back(
				Renderer(buttons_containers[index], [&, index] {
					return vbox({
						text("This is the " + pane_names[index] + " control") | center,
						separator() | size(WIDTH, EQUAL, 100),
						text(pane_names[index] + " button count: " + std::to_string(pane_counters[index])) | center,
						buttons_containers[index]->Render(),
					});
				})
		  	);

			
			if (j != 0) {
				v_composition->Add(get_separator());
			}

			v_composition->Add(pane_containers[index] | flex);
		}

		v_compositions.push_back(v_composition);

		if (i != 0) {
			h_composition->Add(get_separator());
		}

		h_composition->Add(v_compositions[i]);
		
	}
/*
  auto left_count = 0;
  auto mid_count = 0;
  auto right_count = 0;

  auto left_buttons = Container::Horizontal({
      Button("Decrease", [&] { left_count--; }),
      Button("Increase", [&] { left_count++; }),
  });

  auto mid_buttons = Container::Horizontal({
      Button("Decrease", [&] { mid_count--; }),
      Button("Increase", [&] { mid_count++; }),
  });

  auto right_buttons = Container::Horizontal({
      Button("Decrease", [&] { right_count--; }),
      Button("Increase", [&] { right_count++; }),
  });

  // Renderer decorates its child with a new rendering function. The way the
  // children reacts to events is maintained.
  auto leftpane = Renderer(left_buttons, [&] {
    return vbox({
               text("This is the left control"),
               separator(),
               text("Left button count: " + std::to_string(left_count)),
               left_buttons->Render(),
           }) |
           border;
  });
  
  auto midpane = Renderer(mid_buttons, [&] {
    return vbox({
               text("This is the mid control"),
               separator(),
               text("Mid button count: " + std::to_string(mid_count)),
               mid_buttons->Render(),
           }) |
           border;
  });

  auto rightpane = Renderer(right_buttons, [&] {
    return vbox({
               text("This is the right control"),
               separator(),
               text("Right button count: " + std::to_string(right_count)),
               right_buttons->Render(),
           }) |
           border;
  });

  // Container groups components togethers. To render a Container::Horizontal,
  // it render its children side by side. It maintains their interactiveness and
  // provide the logic to navigate from one to the other using the arrow keys.
  auto composition = Container::Horizontal({leftpane, midpane, rightpane});
*/
  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(h_composition | border);
  return 0;
}

// Thanks to Chris Morgan for this example!
