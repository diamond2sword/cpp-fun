#include <iostream>
#include <string>

int main() {
	std::string __name, __address;
	while (true) {
		std::cout << "Name: ";
		std::cin >> __name;
		if (__name.size() == 1) {
			continue;
		}
		break;
	}
	std::cout << "Address: ";
	std::cin >> __address;
	if (__address.empty()) {
		
	}
	return 0;
}
