#include <iostream>
/*
 * Mic Test
 */
void println(std::string s) {
	std::cout << s << std::endl;
}

namespace Chess {

int main() {

	println("Hello world");
	// delay default:1
	// move default:u1 up 1
	// move down
	// move up 1
	// check
	// var_name |0|
	// equals (=)
	// value |1|
	// end condition |.|
	auto p = Piece().Move("dmc0=1.mc0=1.");
	// do 2 times default:1
	// from (
	// to )
	auto p2 = Piece().Move("dx2(mc0=1.)");
	auto p = Piece().Move("dmc0=1.dmc0=1.");
	// do 2 times: delay 0
	auto p = Piece().Move("x2(dmc0=1.)");
	// multiply move through iteration default:1
	auto p = Piece().Move("i(x2(dmc0=1.c1=2.))")
	
/*
	auto p = Piece()
		.AddMove(Move() | 
				MarkOverwrite(position({0,0}) | relative_to_target, {{"mark1", 1},{"mark2",2}}) | PlaceTry(position(1,2) | relative_to_color | relative_to_pos, position(2,3) | relative_to_color | relative_to_pos) |
				check({{0,1},{1,2}}));
*/
/*
	auto canMove = [](Board& b, Move& m) -> bool {
		if (m.source.InRow(4)) {
			return false;
		}
		if (Distance(m.source, m.target) != 1) {
			return false;
		}
		return true;
	}
	auto p = Piece()
		.AddMove(Move(canMove));
*/
/*
	auto b = Board()
		.SetShape(BoardShape::Default())
		.SetPieceSet(PieceSet::Default())
*/

	return 0;
}

} // namespace Chess
