#ifndef MAIN6_3_HPP
#define MAIN6_3_HPP

#include "main6-1.hpp"

class ToBePrinted {
private:
	typedef long long value_t;
	enum class Type : value_t { None, Int, Str, TypeBase, Type, Var, IntPtr, StrPtr, TypeBasePtr, TypePtr, VarPtr };
private:
	// non-pointer
	value_t int_ = 0;
	std::string str_ = "";
	Var::Type type_ = {};
	Var::TypeBase base_type_ = Var::TypeBase::Null;
	Var var_ = Var::Null();
private:
	// pointer
	value_t* int_ptr_ = nullptr;
	std::string* str_ptr_ = nullptr;
	Var::Type* type_ptr_ = nullptr;
	Var::TypeBase* base_type_ptr_ = nullptr;
	Var* var_ptr_ = nullptr;
private:
	Type print_type_ = Type::None;
public:
	ToBePrinted();
	// Non-Pointer
	ToBePrinted(value_t i);
	ToBePrinted(std::string s);
	ToBePrinted(Var::TypeBase bt);
	ToBePrinted(Var::Type t);
	ToBePrinted(Var v);
	// Pointer
	ToBePrinted(value_t* i_ptr);
	ToBePrinted(std::string* s_ptr);
	ToBePrinted(Var::TypeBase* bt_ptr);
	ToBePrinted(Var::Type* t_ptr);
	ToBePrinted(Var* v_ptr);
private:
	static std::string ToListString(const std::vector<std::string>& s_l);
	static std::string ToString(const enum Var::TypeBase& base_type);
	static std::string ToString(const Var::Type& type);
	static std::string ToString(const Var& v);
	std::string ToString() const;
public:
	class Printer;
}; // class ToBePrinted
	
class ToBePrinted::Printer {
private:
	std::vector<ToBePrinted> to_be_printed_l = {};
public:
	Printer(std::vector<ToBePrinted> tbp_l);
public:
	Printer operator+(const Printer& p) const;
private:
	std::string ToString() const;
public:
	void Print() const;
}; // class Printer

#endif // MAIN6_3_HPP
