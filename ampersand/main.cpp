#include <iostream>
#include <utility>

int main() {
	int a = 5;
	int& b = a;
	int* c = &a;
	std::cout << a << "==" << b << "==" << *c << std::endl;
	a = 3;
	std::cout << a << "==" << b << "==" << *c << std::endl;
	b = 4;
	std::cout << a << "==" << b << "==" << *c << std::endl;

	int d = std::move(a);
	std::cout << a << "==" << b << "==" << *c << "==" << d << std::endl;
	return 0;
}
