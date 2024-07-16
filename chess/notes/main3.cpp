// < : I want to see ;-ended exprs until >
// there's no longer functions: 
// instead of "v<f<p>>0,f<l>[vti]{m(a0);};f0,1", 
// use "vl=0,{m(vir0);};vi=0,1;<p>(vlr0);"
	// Make list-variable named 0 equal to the list of expressions: retrieve the integer variable 0, and move as much as the retrieved number
	// Make integer-variable named 0 equal to 1
		// Parameter setting
	// Retrieved list-variable named 0, and put the attribute typed-p to the retrieved expression
		// Optional Type-checking: Does work when specified
// If all variables are string, then:
// use "v=0,{m(vr1);};v=1,1;<p>(vr0);"
	// the list and the variable parameter both now cannot have the same name
// use "v=0,{m/(vr1);};v=1,1;<p>(vr0);"
	// To avoid making the retriever being evaluated, use '/'
// use "v=0,'{m(vr1);}';v=1,1;<tp;s-1,0;>((vr0));"
	// Using global as scope, affects the outermost scope up to current scope
// "v=<ti;>0,<ts;>$(<tp;>{m(vr1);});v=1,1;((vr0));"
// <
// I trust the user: no more type checking
// "e=0,{m(vr1);};v=1,1;er0;"
// "e=:0,{m:(vr1);};v=:1,1;er:0;"
// m move
// p position
// o orientation
// e expression
// ; expr end
// : args start
// , args separator
// < attr start
// > attr end
// { list start
// } list end
// ( holder start
// ) holder end
// n north
// d delay
// s scope
#include <map>
#include <string>


class Expr {
public:
	enum class Type : char;
};

enum class Expr::Type : char {
	Move = 'm',
	Position = 'p',
	Orientation = 'o',
	Variable = 'v',
	Scope = 's',
	Delay = 'd',
	North = 'n',
	ArgsStart = ';',
	CommandEnd = ':',
	ArgsSeparator = ',',
	AttributeStart = '<',
	AttributeEnd = '>',
	ListStart = '{',
	ListEnd = '}',
	HolderStart = '(',
	HolderEnd = ')',
};

class Move : public Expr{
public:
	enum class Type : char;
};
class Position : public Expr {};
class Orientation : public Expr {};
class Variable : public Expr {};
class Scope : public Expr {};
class Delay : public Expr {};
class North : public Expr {};
class ArgsStart : public Expr {};
class CommandEnd : public Expr {};
class ArgsSeparator : public Expr {};
class AttributeStart : public Expr {};
class AttributeEnd : public Expr {};
class ListStart : public Expr {};
class ListEnd : public Expr {};
class HolderStart : public Expr {};
class HolderEnd : public Expr {};
	
static const std::map<Expr::Type, Expr> ExprTypeToExprMap {
	{(Expr::Type)'m', Move()},	
	{(Expr::Type)'p', Position()},
	{(Expr::Type)'o', Orientation()},
	{(Expr::Type)'v', Variable()},
	{(Expr::Type)'s', Scope()},
	{(Expr::Type)'d', Delay()},
	{(Expr::Type)'n', North()},
	{(Expr::Type)';', ArgsStart()},
	{(Expr::Type)':', CommandEnd()},
	{(Expr::Type)',', ArgsSeparator()},
	{(Expr::Type)'<', AttributeStart()},
	{(Expr::Type)'>', AttributeEnd()},
	{(Expr::Type)'{', ListStart()},
	{(Expr::Type)'}', ListEnd()},
	{(Expr::Type)'(', HolderStart()},
	{(Expr::Type)')', HolderEnd()},
};


enum class Move::Type : char {
	Set = '=',
	Retrieve = 'r',
	Delete = 'd',
};

class Set : public Move {};
class Retrieve : public Move {};
class Delete : public Move {};

static const std::map<Move::Type, Move> MoveTypeToMoveMap = {
	{(Move::Type)'=', Set()},
	{(Move::Type)'r', Retrieve()},
	{(Move::Type)'d', Delete()},
};


int main() {
	Expr::Type C = (Expr::Type)'(';
	ExprTypeToExprMap.at((Expr::Type)'m');
	Expr e = Set();
}
