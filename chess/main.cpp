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
	// move default:v1h0 upward
	// move down
	// move up 1
	// check default:== options: !=, <=, >=, >, <, in, !in
	// var_name |0| options: 0, 1, 2, 3, ...
	// equals (=)
	// value |1| options: 0, 1, 2, 3, ...
	// end condition |.|
	auto p = Piece().Move("d;m;c0,1;m;c0,1;");
	// do 2 times default:1
	// from (
	// to )
	auto p2 = Piece().Move("d;x2{m;c0,1;}");
	auto p = Piece().Move("d;m;c0,1;d;m;c0,1.m;");
	// do 2 times: delay 0
	auto p = Piece().Move("x2{d;m;c0,1;)");
	// same as x4{...}
	auto p = Piece().Move("x2{x2{m;=1,0;}}");
	auto p = Piece().Move("x2{x2{m;=1,0;}}");
	// set sight radius default:infinite
	// * s 1 = piece can see self
	// * s 2 = piece can see self and one cell away
	auto p = Piece().Move("m;s1");
	// set sight explicitly default: all pos
	// * pos = either relative to self or to board
	// * sight radius can be set to a pos
	// position relative to piece default: b = relative to board
	// pr1,2; = hr1vr2
	auto p = Piece().Move("m;s(hr1vr2;pr1,2;h5v3;}");
	// * s(pos1),(pos2); = s{pos1;pos2;}
	// * position must be not repeated
	auto p = Piece().Move("m;s{p5,3;p5,3;}"); // error
	// * position relative to board can't be negative
	auto p = Piece().Move("m;s{p3,-3;}"); // error
	auto p = Piece().Move("m;s{hr-3,v3;}"); // okay
	// {} summons a scanner from the piece
	// {{}} a summoned piece, summoning a scanner
	// () returns a string from function
	// {{c1,(r1)}} retrieve pos from in to out
	// {{c0,(rr1)}} retrieve reversely the pos, from out to in
	// * retrieve default:0 = retrieves pos of cur mover
	// * retrieve returns:h2v5, h4v8 = examples
	// condition: if A in ...
	auto p = Piece().Move("m;{m;{cin(rr1),(rr0),h1v2;}}");
	// * can't delay inside scanners
	auto p = Piece().Move("m;{d}"); //error
	// transform pos1 relative to pos2
	// (t(r),(rr))
	auto p = Piece().Move("m;{m;c(t(r),(rr)),h2v1;m;}");
	// scanner ending in condition, acts as condition
	auto p = Piece().Move("m;{m;c1,0};m;");
	// scanner ending in position, can be retrieved
	// * retrieve horizontally the pos (instead of vertically = traversing scopes) default:0 = retrieves previous
	auto p = Piece().Move("m;{m;rr;};m(rh)");
	// * retrieve pos horizontally reversely default:0
	// * retrieves position at "m;"
	auto p = Piece().Move("m;{m;rr;};m(rhr);");
	auto p = Piece().Move("m;{m;rr;};m(rhr1);");
	// * assigning bool to pos = error
	auto p = Piece().Move("m;{m;rr;};m(rhr1);");
	// * m returns void
	auto p = Piece().Move("m;");
	// orientation default:n = north
	// * changes orientation of mover
	auto p = Piece().Move("on,s;");
	// * relative to mover's north
	auto p = Piece().Move("orn,s;");
	// change north relative to board default:north
	auto p = Piece().Move("nn;");
	// change north relative to mover's north default:north
	auto p = Piece().Move("nrn;");
	// scanners can mark too
	auto p = Piece().Move("m;{m;=0,1;}");
	// bool of scanner is the last bool expr
	auto p = Piece().Move("m;{m;c0,1;m;}m;");
	// unset mark
	// * unsets var 0
	auto p = Piece().Move("m;=0;");
	// use pos as mark
	// h2v5 is a data while hr2v5 is a function
	auto p = Piece().Move("m;p=0,h2v5;");
	auto p = Piece().Move("m;p=0,(hr2v5);");
	
	
	

	
	
		
	
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
