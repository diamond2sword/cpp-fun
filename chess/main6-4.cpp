#include "main6-3.hpp"
#include <iostream>

void ToBePrinted::Printer::Print() const {
	std::cout << ToString();
}

std::string ToBePrinted::Printer::ToString() const {
	std::string s = "";
	for (const ToBePrinted& tbp : to_be_printed_l) {
		s += tbp.ToString();
	}
	return s;
}

ToBePrinted::Printer ToBePrinted::Printer::operator+(const Printer& p) const {
	Printer result_p = *this;
	result_p.to_be_printed_l.reserve(p.to_be_printed_l.size());
	for (const ToBePrinted& tbp : p.to_be_printed_l) {
		result_p.to_be_printed_l.push_back(tbp);
	}
	return result_p;
};

ToBePrinted::Printer::Printer(std::vector<ToBePrinted> tbp_l) : to_be_printed_l(tbp_l) {};

std::string ToBePrinted::ToListString(const std::vector<std::string>& s_l) {
	std::string result_s = "";
	for (int i = 0; i < s_l.size(); i++) {
		result_s += (i == 0 ? "" : ", ") + s_l[i];
	}
	return "{" + result_s + "}";
};

std::string ToBePrinted::ToString(const enum Var::TypeBase& base_type) {
	switch (base_type) {
	case Var::TypeBase::Null:
		return "Null";
	case Var::TypeBase::Empty:
		return "Empty";
	case Var::TypeBase::Var:
		return "Var";
	case Var::TypeBase::Int:
		return "Int";
	case Var::TypeBase::List:
		return "List";
	case Var::TypeBase::Ptr:
		return "Ptr";
	default:
		return "TypeBaseNotImplemented";
	}
};
std::string ToBePrinted::ToString(const Var::Type& type) {
	std::vector<std::string> s_l = {};
	for (const Var::TypeBase& base_type : type) {
		s_l.push_back(ToString(base_type));
	}
	return ToListString(s_l);
};
std::string ToBePrinted::ToString(const Var& v) {
	switch (v.base_type_) {
	case Var::TypeBase::Null:
		return "Null";
	case Var::TypeBase::Empty:
		return "Empty";
	case Var::TypeBase::Var:
		return "Var";
	case Var::TypeBase::Int:
		return std::to_string(v.value_);
	case Var::TypeBase::List: {
		std::vector<std::string> s_l = {};
		for (const Var& v : v.list_) {
			s_l.push_back(ToString(v));
		}
		return ToListString(s_l);
	}
	case Var::TypeBase::Ptr:
		return ToString(*v);
	default:
		return "TypeBaseNotImplemented";
	}
};
std::string ToBePrinted::ToString() const {
	std::string s = "";
	switch (print_type_) {
	// Non-pointer
	case Type::None:
		return "";
	case Type::Int:
		return std::to_string(int_);
	case Type::Str:
		return str_;
	case Type::TypeBase:
		return ToString(base_type_);
	case Type::Type:
		return ToString(type_);
	case Type::Var:
		return ToString(var_);
	// Pointer
	case Type::IntPtr:
		return std::to_string(*int_ptr_);
	case Type::StrPtr:
		return *str_ptr_;
	case Type::TypeBasePtr:
		return ToString(*base_type_ptr_);
	case Type::TypePtr:
		return ToString(*type_ptr_);
	case Type::VarPtr:
		return ToString(*var_ptr_);
	default:
		return "ToBePrintedNotImplemented";
	}
};

ToBePrinted::ToBePrinted(Var* v_ptr) : var_ptr_(v_ptr), print_type_(Type::VarPtr) {};
ToBePrinted::ToBePrinted(Var::Type* t_ptr) : type_ptr_(t_ptr), print_type_(Type::TypePtr) {};
ToBePrinted::ToBePrinted(Var::TypeBase* bt_ptr) : base_type_ptr_(bt_ptr), print_type_(Type::TypeBasePtr) {};
ToBePrinted::ToBePrinted(std::string* s_ptr) : str_ptr_(s_ptr), print_type_(Type::StrPtr) {};
ToBePrinted::ToBePrinted(value_t* i_ptr) : int_ptr_(i_ptr), print_type_(Type::IntPtr) {};

ToBePrinted::ToBePrinted(Var v) : var_(v), print_type_(Type::Var) {};
ToBePrinted::ToBePrinted(Var::Type t) : type_(t), print_type_(Type::Type) {};
ToBePrinted::ToBePrinted(Var::TypeBase bt) : base_type_(bt), print_type_(Type::TypeBase) {};
ToBePrinted::ToBePrinted(std::string s) : str_(s), print_type_(Type::Str) {};
ToBePrinted::ToBePrinted(value_t i) : int_(i), print_type_(Type::Int) {};
ToBePrinted::ToBePrinted() : print_type_(Type::None) {};
