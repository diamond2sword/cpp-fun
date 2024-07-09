#include <algorithm>
#include <exception>
#include <stdexcept>
#include <string>
#include <vector>
#include <memory>
#include <array>
#include <iostream>

void PrintException(const std::exception& e) {
	std::cerr << e.what();
	try {
		std::rethrow_if_nested(e);
	}
	catch (const std::exception& nested_e) {
		PrintException(nested_e);
	}
}

void ThrowNestedException(const std::string&& what) {
	std::throw_with_nested(std::runtime_error(what));
}
using RawExprBase = std::string;
using RawExpr = std::shared_ptr<RawExprBase>;
using RawExprs = std::vector<RawExpr>;

class ExprBase;
using Expr = std::shared_ptr<ExprBase>;
using Exprs = std::vector<Expr>;

class ExprBase {
public:
	explicit ExprBase(RawExpr original_expr) {
		original_expr_ = original_expr;
	};
	// divide the expr to children_, then make children divide theirs
	virtual void Divide() {};
	virtual Exprs SubExprs() const {
		if (!children_.empty()) {
			return children_;
		}
			
		

		return {};
	};
protected:
	Exprs children_;
	RawExpr original_expr_;
};

class Loop : ExprBase {
	Exprs SubExprs() const override {
		
	};
};

class While : Loop {
	Exprs SubExprs() const override;
};

class Scanner : Loop {
	Exprs SubExprs() const override;
};


int main () {
	try {
		RawExpr s = std::make_shared<RawExprBase>("{)");
		Expr e = std::make_shared<ExprBase>(s);
	}
	catch (const std::exception& e) {
		PrintException(e);
	}
}

/*

struct ScopeBase;
using Scope = std::shared_ptr<ScopeBase>;
using Scopes = std::vector<Scope>;

using RawExprBase = std::string;
using RawExpr = std::shared_ptr<RawExprBase>;
using RawExprs = std::vector<RawExpr>;

class ExprBase;
using Expr = std::shared_ptr<ExprBase>;
using Exprs = std::vector<Expr>;

struct ScopeBase {
	bool vertical_direction;
	bool horizontal_direction;
	int horizontal_level;
	int vertical_level;
};

class ExprBase {
public:
	virtual Expr childAt(Scope s) const;
	virtual RawExpr returnType() const;
	virtual RawExpr exprType() const;
	virtual Scope scope() const;
private:
protected:
	RawExpr original_expr_;
	Exprs children_;
	Scope* scope_;
	
	friend class VarExpr;
};

class Var : ExprBase {};
*/


/*
namespace Chess {
	enum class Type {
		Null = 0
	};
	class Expression {
	public:
		Expression(std::string s) {};
		virtual Type Type(std::string s) = 0;
	};
	class NewExpression: public Expression {
	public:
		NewExpression(std::string s): Expression(s) {
				
		};
		Chess::Type Type(std::string s) {
			// TODO Implement
			return Type::Null;
		};
	private:
		Chess::Type type = Type::Null;
	};
	namespace Mover {
		namespace Loop {}
		namespace WhileLoop {
			namespace WhileScanner {}
		}
		namespace Scanner {}
		namespace ForLoop {}
		namespace OrientationForLoop {}
		namespace ForLoopScanner {}
		namespace WhileScanner {}
	}
	namespace Expr {}
	namespace Command {
		namespace Delay {}
		namespace Container {
			namespace Scope {}
			namespace List {
				namespace RawList {}
				namespace ContainedList {}
			}
			namespace Loop {
				namespace Scanner {}
				namespace WhileLoop {
					namespace WhileScanner {}
					namespace ForLoop {
						namespace OrientationForLoop {}
						namespace ForLoopScanner {
							namespace OrientationForLoopScanner {}
						}
					}
				}
			}
		}
	}
	namespace Container {
		namespace Loop {}
		namespace WhileLoop {
			namespace WhileScanner {}
		}
		namespace Scanner {}
		namespace ForLoop {}
		namespace OrientationForLoop {}
		namespace ForLoopScanner {}
		namespace WhileScanner {}
	}
	namespace Event {
		void OnPieceMoved() {};
		void OnColorChange() {};
		void OnPieceOrientationChange() {};
		void OnPieceDeletion() {};
		void OnCheckmate() {};
		void OnStart() {};
		void OnCell(int, int) {};
		void OnBlackMoved() {};
		void OnWhiteMoved() {};
		void OnDelay(std::vector<int>) {};
	}


	
} // namespace Chess


int main() {
}
*/

/*
namespace Mover {

class Program {
public:
	Program() = delete;
	static Program Make(std::string s) {
		return s |
		// to_expr: "x2{m;};"
		// child {expr1}
		// expr1: child {m1;}, type: {}, return_t: auto
		to_expr | 
		// becomes a command, that links itself horizontally and vertically to other commands with respect to scope and sentence structure 
		// parent calls children to render to him the command in them
		to_cmd |;
	};
};

int main () {
	auto prog = Program::Make("m;d;m;");
	prog();
	r
}



} // namespace Mover
*/
