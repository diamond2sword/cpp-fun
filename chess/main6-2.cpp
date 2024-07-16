#include "main6-3.hpp"
#include "main6-1.hpp"

using TBP = ToBePrinted;
using Printer = TBP::Printer;

int main () {
	Var base_v = Var();
	Var v = (std::vector<Var>){
		Var(""), Var(), (Var)&base_v
	};
	v.DeduceType();
	std::vector<TBP> tbp_l {
		TBP(v), TBP(" "), TBP()
	};
	Printer p = Printer(tbp_l);
	p.Print();
	return 0;
}
