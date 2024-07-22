#ifndef VAR_HPP
#define VAR_HPP

#include <__type_traits/is_char_like_type.h>
#include <charconv>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iterator>
#include <map>
#include <memory>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>
#include <string>
#include <algorithm>
#include <ranges>
#include <functional>
#include <iterator>
#include <array>
#include <stdexcept>
#include <iostream>

class Var {
public:
	class Base;
private:
	using value_t = long long int;
	using ptr_t = Base*;
	using list_t = std::vector<Base>;
	enum class type_t {
		Null,
		Free,
		Int,
		Ptr,
		List,
		Map,
		ID,
	};
private:
	static size_t counter_;
	static list_t list_;
public:
	class Null;
	class Free;
	class Int;
	class Ptr;
	class List;
	class Map;
	class ID;
public:
	static int main();
public:
	static Base New(Base v);
};

class Var::Base {
protected:
	value_t value_;
	ptr_t pointer_;
	list_t list_;
	enum type_t type_;
	size_t id_;
public:
	Base(value_t n, ptr_t p, const list_t& l, type_t t) 
		: value_(n), pointer_(p), list_(std::move(l)), type_(t), id_(counter_++) {
	};
public:
	Base& operator*() const {
		return *pointer_;
	};
	size_t ID() const {
		return id_;
	};
};

class Var::ID : public Base {
private:
	ID(size_t id) : Base(id, nullptr, {}, type_t::ID) {};
public:
	ID(const Base& v) : ID(v.ID()) {};
	~ID() {
		auto it = std::find_if(
			Var::list_.begin(), 
			Var::list_.end(), 
			[this](const Base& v) {
				return v.ID() == id_;
			}
		);
		Var::list_.erase(it);
	};
};

size_t Var::counter_ = 0;
Var::list_t Var::list_ = {};

Var::Base Var::New(Base v) {
	list_.push_back(v);
	return ID(v);
};

class Var::Null : public Base {
public:
	Null() : Base(0, nullptr, {}, type_t::Null) {};
};

class Var::Free : public Base {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Free(value_t n, ptr_t p, const list_t& l) : Base(n, p, l, type_t::Free) {};
	
	Free() : Free(0, nullptr, {}) {};
};

class Var::Int : public Base {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Int(value_t n) : Base(n, nullptr, {}, type_t::Int) {};
	
	Int() : Int(0) {};
};

class Var::Ptr : public Base {
public:
	Ptr(ptr_t p) : Base(0, p, {}, type_t::Ptr) {};
	Ptr(Base& v) : Ptr(&v) {};
	//Ptr(Var& v) : Ptr(&v) {};
	
	template <class var_t> 
	requires std::is_convertible_v<var_t, Base> && (!std::is_reference_v<var_t>)
	Ptr(var_t v) : Base(0, nullptr, {v}, type_t::Ptr) { pointer_ = &list_[0]; };
	//Ptr() : Ptr(Null()) {};
	Ptr() : Ptr(Null()) {};
public:
	static int main();
};

class Var::List : public Base {
private:
	template <
		class value_t,
		bool isIntVar = std::is_arithmetic_v<value_t>,
		bool isPtrVar = std::is_same_v<value_t, ptr_t>,
		class var_t = std::conditional_t<isIntVar, Int, Ptr>>
	requires isIntVar || isPtrVar
	using enable_if_var_value_t = var_t;

	template <
		class range_t,
		bool isArray = std::is_array_v<range_t>,
		bool isPointer = std::is_pointer_v<range_t>,
		class no_extent_value_t = std::remove_all_extents_t<range_t>,
		class no_ptr_value_t = std::remove_pointer_t<range_t>,
		class value_t = std::conditional_t<isArray, no_extent_value_t, no_ptr_value_t>>
	requires isArray || isPointer
	using enable_if_extent_or_ptr_t = value_t;

	template <
		template <class...> class range_t, 
		class value_t,
		class var_t = enable_if_var_value_t<value_t>>
	static list_t FromRange(const range_t<value_t>& l) {
		list_t result_l = {};
		for (value_t value : l) result_l.push_back(var_t(value)); 
		return result_l;
	};

	template <
		class range_t, 
		size_t extent = std::extent_v<range_t>,
		class value_t = enable_if_extent_or_ptr_t<range_t>,
		class var_t = enable_if_var_value_t<value_t>>
	static list_t FromRange(const range_t& l, size_t size = extent) {
		list_t result_l = {};
		for (int i = 0; i < size; i++) result_l.push_back(var_t(l[i])); 
		return result_l;
	}; 
public:
	List(const list_t& l = {}) : Base(0, nullptr, l, type_t::List) {};

	template <class range_t>
	List(const range_t& l) : List(FromRange(l)) {};

	template <class value_t, size_t N>
	List(std::array<value_t, N> l) : List(FromRange(l.data(), N)) {};

	template <class value_t>
	List(const std::initializer_list<value_t>& il) : List(FromRange(il)) {};
	
};

#endif // VAR_HPP

int main() {
	Var::Base v1 = Var::Int();
	std::string s = "Hello World!";
	std::vector<float> l = {0.f, 2.3, 3.6};
	Var::Base v = Var::List("");
	Var::Base v2 = Var::List(l);
	Var::Base v3 = Var::List();
	//Var p4 = Var(0, nullptr, {}, Var::type_t::Null);
	Var::Ptr::main();
	return 0;
}

int Var::main() {
	Var::Base f1 = Var::Ptr(nullptr);
	Var::Base p1 = Var::Ptr(f1);	
	f1.pointer_ = &p1;
	p1.value_ = 1;
	std::cout << p1 << std::endl;
	return 0;
};
