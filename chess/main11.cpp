#include "main11.hpp"
#include "see.hpp"

int main() {
	__board<8, 8, __piece> b;
	auto __bp = __board_ptr<8, 8>(&b);
	seeifneq(__bp.piece().name, __piece().name);
	return 0;
}
