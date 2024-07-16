#include "main6-3.hpp"
#include "main6-1.hpp"

using TBP = ToBePrinted;
using Printer = TBP::Printer;

int main () {
	Var v = (std::vector<Var>){
		Var(""), Var()
	};
	std::vector<TBP> tbp_l {TBP(v)};
	Printer p = Printer(tbp_l);
	p.Print();
	return 0;
}
