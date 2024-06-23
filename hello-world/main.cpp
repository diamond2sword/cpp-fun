#include "hello.hpp"

int main (int argc, char** argv) {
	int (*main)(int, char**) = [](int argc, char** argv) -> int {
		helloWorld();
		return 0;
	};
	return main(argc, argv);
}
