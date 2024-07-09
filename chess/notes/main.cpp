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
	auto p = Piece().Move("on;");
	// for each orientation in a,b,c,...
	auto p = Piece().Move("xon,s{m;}");
	// 2n = n,n
	auto p = Piece().Move("xo(2n),s{m;}");
	auto p = Piece().Move("xo2(2n),s{m;}"); //error
	auto p = Piece().Move("xo2,(2n),s{m;}"); //okay
	// * relative to mover's north
	auto p = Piece().Move("orn;");
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
	// retrieve pos for condition
	auto p = Piece().Move("m;p=0,(hr2v5);cin0,(hr2v5),h3v7;m;");
	// summon scanners with different orientation
	auto p = Piece().Move("m;on,s,w{m;=0,1}");
	// retrieve orientation horizontally default: 0
	// (ro) = vertical
	auto p = Piece().Move("m;o(roh){m;=0,1}");
	// transform relative to orientation of past self or of given orientation (to)
	// either vertical tov or horizontal toh
	auto p = Piece().Move("m;oe;o(toh(roh),on);");
	// mark orientation
	auto p = Piece().Move("m;oe;o=0,(ro);");
	// condition: OR gate
	auto p = Piece().Move("m;||(c0,1),(c1,2);m;");
	// condition: AND gate
	auto p = Piece().Move("m;&&(c0,1),(c1,2);m;");
	// condition: NOT gate
	auto p = Piece().Move("m;!(||(c0,1),(c1,2));m;");
	// condition: Lambda gate
	auto p = Piece().Move("m;g{!(||(&&a,b),b)}(c0,1),(c1,2);m;");
	// summon two or more scanners at once
	// combine orientation scanners and simple ones
	// * or{} = for each orientation relative to self's north
	auto p = Piece().Move("m;{{m;}orn{m;}}");
	// north is e, orn returns e, ore returns s
	// ors returns w
	auto p = Piece().Move("m;or(orn),(ore){m;}");
	// implement for diagonal and normal
	// then do for each orientation
	auto p = Piece().Move("xon,e,s,w{{m;=0,1}{mv1h1;=0,1}}");
	// reset mover's north
	auto p = Piece().Move("nr;");
	// summon a 'while' scanner
	// * will repeat itself until an expr returns false
	// must initially be a scanner so as to not count as a move
	auto p = Piece().Move("{x4{ore;w{m;c0,0;=1,1;}}}");
	// non-scanner while loop
	auto p = Piece().Move("xw{m;c0,1;}");
	// rotate orientation 1 clockwise default: 0
	auto p = Piece().Move("o1;");
	// rotate counterclockwise
	auto p = Piece().Move("oc1;");
	// move other piece
	// always successful
	// moving unto a piece overwrites the piece
	auto p = Piece().Move("m;mh2v1,h3v2;");
	// scanners can't move other pieces
	auto p = Piece().Move("{m;mh2v1,h3v2;}"); //error
	// this move is delayed 5 times
	// it is 6 moves long
	auto p = Piece().Move("d;d;d;d;d;m;");
	// it is similar to
	// x5{d;} = d;d;d;d;d;
	auto p = Piece().Move("x5{d;}m;");
	// mark scanner
	auto p = Piece().Move("m;{}=0,{...}");
	// retrieve a scanner and summon
	// (); will first be retrieved before being executed
	auto p = Piece().Move("m;({}r0);");
	// retrieve a loop from mark and summon as move's extension
	auto p = Piece().Move("m;(x{}r0);");
	// mark a loop
	auto p = Piece().Move("m;x{}=0,{...}");
	// retrieve delay versions default:0
	// can't be done by a scanner: dr always copies a delay; scanner can't have delay
	// r1 = retrieves future version
	// r0 = retrieves nothing
	// r-1 = retrieves past version
	auto p = Piece().Move("d;d;d;(dr1);"); //error
	// runs indefinitely
	auto p = Piece().Move("d;d;d;(dr-1);");
	// d;c1,0;(dr-1);
	// move 0 = c1,0;(dr-1);
	// if condition succeeds: move 1 = d;c1,0;(dr-1);
	// if condition succeeds: move 2 = d;c1,0;(dr-1);
	// until c1,0 = false
	auto p = Piece().Move("d;c1,0;(dr-1);");
	// until var 1 == 0, move
	auto p = Piece().Move("c1,0;m;(dr);");
	// m;(dr1);m;d;m;
	// m;m;m;d;m;
	auto p = Piece().Move("m;(dr1);m;d;m;");
	// if there is no command that can't be in scanner, dr can be used
	// runs indefinitely
	auto p = Piece().Move("{(dr);}"); 
	// stops on condition
	auto p = Piece().Move("{c0,1;(dr);}");
	auto p = Piece().Move("d;{c0,1;(dr);}"); //error
	auto p = Piece().Move("d;{c0,1;(dr);}"); //error
	// if elif else
	// condition: ct = always return true
	auto p = Piece().Move("c{0,1{...}!=1,0{...}in1,2,1{...}t{...}}");
	auto p = Piece().Move("c{0,1{...}!=1,0{...}t{...}in1,2,1{...}"); //skips last
	// switch case
	auto p = Piece().Move("...;c{(r),1{...}(r),0{...}t{...}");
	// better switch case
	auto p = Piece().Move("...;c(r),{1{...}0{...}t{...}");
	// switch case with cin
	auto p = Piece().Move("...;cin(r),{1,2,3{...}4,5,6{...}7,8,9{...}");
	//generate lists with range operator ..
	auto p = Piece().Move("...;cin(r),{(..1,3){...}(..5,6){...}(..7,9){...}");
	// () will be only evaluated when the scope enclosing it is accessed
	// 0r~ = from
	// you can have it statically evaluated
	auto p = Piece().Move("...;cin(r),{(~..1,3){...}(~..5,6){...}(~..7,9){...}");
	// you can choose the min scope level down can it be statically evaluated; default: 0 = until reached
	// from out to in: 0r~ = always
	auto p = Piece().Move("...;cin(r),{(0r~..1,3){...}(~..5,6){...}(~r1..7,9){...}");
	// can't do this: results to infinite callback
	auto p = Piece().Move("(0r~dr);"); // error
	// c!in = !(cin)
	// c!= = !(c==)
	auto p = Piece().Move("(0r~dr);"); // error
	// Add integer to variable
	// ++0 --1
	"++0;--0",
	// add arbitrary integer to var
	"+=0,2;-=2,4",
	"+=0,2;-=2,4",
	// multiply by, divide with, modulo 
	"*=0,3;/=2,3;%=4,2;",
	// mathematical expr
	"(+~1,2);",
	// retrieve var now 
	// retrieve var horizontally default:0
	"(+~(rv),(rvh1));",
	// retrieve var vertically (going out of scope)
	// retrieve var vertically reversed (going to)
	// retrieve var horizontally moving left to
	// all: default: 0
	"(rv);(rvr);(rvhr);",
	// retrieves with args
	// vertically 0
	// horizontally 1
	"(rv0,1);",
	// reverse or not
	"(rv0r,1r);",
	// retrieve pos now ==0 / vertically !=0
	// retrieve horizontally
	// reversed
	// arg: has to be unsigned
	"(r);(rh);(rr);(rhr);()",
	// set env var local
	"v=0,1",
	// set env var global
	"vg=0,1",
	// retrieve env var
	"vr0",
	// retrieve last modified env var
	"vrt",
	// retrieve unmodified var for longest time
	// same as retrieve reverse
	"vrt-1;vrtr1;",
	// retrieve and retrieve reverse
	"r;r-1; rr1;",
	// retrieve reverse and retrieve
	"rr;rr-1;r1",
	// unset local env variable
	"vu0;"
	// unset global env variable
	"vgu0;"
	// unset variable
	"u0;",
	// trash a piece
	"uh2v3",
	// trash multiple pieces
	"uh2v3,h3v4",
	// retrieve and place to
	// retrieve recently trashed default:0
	"ru,h2v3",
	// retrieve trashed for longest and put to pos1
	"rur,h3v5",
	// unset global env var
	"ug",
	// loop x named 0, will not execute
	"v0x{}",
	// exec loop and name it
	"x0v5{}", // will execute
	// make scanner named 5
	"v5{...}",
	// retrieve env loop and execute
	"(vx{}r0)",
	// retrieve env scanner and execute
	"(v{}r0)",
	// make global loop env var
	"vg0x{...};",
	// unset global loop
	"vg{}u0;",
	// make lesser-global variable
	"v1g0,1",
	// make lesser-local variable
	"v1l0,1",
	// retrieve lesser-global env var
	"(v1gr0);",
	// retrieve lesser-local env var
	"(v1gr0);",
	// scope info
	// retrieve mover's scope returns: int
	"{}r{}",
	// relative to being global
	"{{}r{};};", // returns 1
	// making loop env with arguments
	// var type
	// retrieve argument
	"v1l0x[vti;vtx;]{(a1);(a0);}",
	// retrieve loop env with arguments
	"(vx{}r0[0;(v{}r0);])",
	// default variable
	// var type
	// i = int
	// x = loop
	// x[i] = loop that takes i
	// {} = scanner
	// c = bool
	// w = while scanner
	// xw = while loop
	"v1l0x[vti,1;]{(a0);}",
	// unset loop env
	"v1l{}u0",
	// mark loop env
	// * when retrieved, maintains scope
	"vx{}=0,(vx{}r0);",
	// loops will return the returned by the last executed expr of it
	// returns pos
	"x{(r);};",
	// make a list
	"v0...{0;1;2;}",
	"v0...0,1,2;",
	// retrieve list
	// returns ...{0;1;2};
	"(v...r0)",
	// example: cin
	// cin0,1,2,3 = cin0,{1;2;3;};
	"cin0,(v...r0);",
	// append to string
	"v...0+1"
	// retrieve list element with index
	"v...0r1"
	// find first default: empty
	"v...0f1",
	// find last
	"v...0fl0;",
	// find nth presence of 5; default: 0 = first
	"v...0f1n5";
	"v...0fl1n5";
	// not empty
	"c!=(v...r0),{}"
	// will always return ...{}
	"(v...r0);"
	// will return (...{}) = a,b,c,d
	"((v...r0));"
	// returns ...{0;1;};
	"0...1;"
	// returns ...{0;1;}; up to 1,2;
	"((0...1;));"
	// typed loop	
	"v0...<i>{1;2;3;};"
	// typed loop with fixed size
	"v0...<i;3>{1;2;3;};"
	// vx default: empty, sets to nothing, returns value, otherwise return nothing
	"v0...:i{vx{1;2;3;};};"
	// loop with return type
	"v:x<i> =0,{...}"
	// scanner with return type
	":i{}"
	// typed var
	"v:i=0,1"	
	// typed scanner var
	"v: <i>{}=0,1"
	// typed function
	"v:x<i>[i;]=0,{...};"
	// lesser global function
	"vl1=0,<i>x[i,]{...};"
	// calling a function
	// returns <i>x[i;]{...};
	"<<i>x[i,]>vr0"
	// evaluate function
	"(<<i>x[i;]>vr0),args...;"
	// retrieves position
	"<p>r;" // error
	"<i>x{c0,1;m;};" // error, m returns nothing
};


	

	


	
	
	
	
	
	
	
	

	
	
		
	
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
