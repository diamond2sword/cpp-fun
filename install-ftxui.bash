#!/bin/bash

main () {
	cd "$HOME"
	ftxui_path="$HOME/ftxui"
	git clone https://github.com/ArthurSonzogni/ftxui 
	mkdir "$ftxui_path/build" && cd "$ftxui_path/build"
	LDFLAGS=" -llog -lpython3" cmake .. -DCMAKE_INSTALL_PREFIX="$PREFIX"
	make install
	rm -rf "$ftxui_path"
}


main "$@"
