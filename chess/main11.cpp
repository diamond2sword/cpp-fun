#include "main11.hpp"
#include "see.hpp"

int main() {
	__board<8, 8, __piece> b;
	auto __bp = __board_ptr<8, 8>(&b);
	for (int i = 0; i < 8; i++) {
		see(__bp.pos.row);
		for (int j = 0; j < 8; j++) {
			seeifneq(__bp.piece().name, __piece().name);
			seeifneq(__bp.piece().attrs.size(), 0);
			__bp.move(__pos(0, 1));

			int __then_col = __bp.pos.col;
			__pos __new_pos = __bp.move(__pos(0, 1));
			if (__new_pos.col != __then_col + 1)
				__bp.pos.col = 0;
		}
		int __then_row = __bp.pos.row;
		__pos __now_pos = __bp.move(__pos(1, 0));
		if (__now_pos.row != __then_row + 1)
			__bp.pos.row = 0;
	}
	return 0;
}