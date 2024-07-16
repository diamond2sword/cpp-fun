#ifndef MAIN6_1_HPP
#define MAIN6_1_HPP

#include <string>
#include <vector>

class Var {
private:
	using value_t = long long int;
	enum class TypeBase : value_t { Null, Empty, Var, Int, List, Ptr };
	using Type = std::vector<TypeBase>;
private:
	value_t value_ = NULL;
	std::vector<Var> list_ = {};
	Var* pointer_ = nullptr;
	enum TypeBase base_type_ = TypeBase::Null;
private:
	static std::vector<Var> List(const std::vector<value_t>& s); // list int to list Int
	static std::vector<Var> List(const std::string& s); // string to list Int
	static std::vector<Var> PtrList(const std::vector<Var*>& ptr_l); // list Ptr Var to list Ptr Var
private:
	Var(const TypeBase& t); // any from TypeBase
public:
	static Var Null(); // Null
	Var(); // Empty
	Var(const value_t& c); // Int from char
	Var(Var* p); // Ptr
	static Var Ptr(Var& v); // Ptr
	Var(const std::vector<value_t>& s); // List Int
	Var(const std::string& s); // List Int from string
	Var(const std::vector<Var>& l); // List Var
	Var(const std::vector<Var*>& l); // List Ptr
public:
	Var operator*() const;
	Var operator&();
private:
	Type DeduceType() const;
public:
	void DeduceType(Type* type_ptr, value_t* scope_ptr) const;
private:
	friend class ToBePrinted;
};

#endif // MAIN6_1_HPP
