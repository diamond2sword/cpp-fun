#include "main12.hpp"

int main() {
	auto a = ArrBoard<int, 8, 8>();
	auto a2 = ArrBoard<A, 8, 8>();
	auto a3 = ArrBoard<A, 1, 8>();
	auto v = VecBoard<int>(1, 1);
	auto v2 = VecBoard<A>(1, 1);
	//is_iterable<Arr<int, 1>>;

	return 0;
}
