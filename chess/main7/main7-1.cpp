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
private:
	using value_t = long long int;
	using ptr_t = Var*;
	using list_t = std::vector<Var>;
	enum class type_t {
		Null,
		Free,
		Int,
		Ptr,
		List,
		Map,
	};
private:
	value_t value_ = 0;
	ptr_t pointer_ = nullptr;
	list_t list_ = {};
	enum type_t type_ = type_t::Free;
private:
	Var(value_t n, ptr_t p, const list_t& l, type_t t)
		: value_(n), pointer_(p), list_(std::move(l)), type_(t) {};
public:
	Var& operator[](size_t n) {
		if (n > list_.size()) {
			std::string error_msg = 
				"n > list_.size(), n = " + std::to_string(n) + 
				", list_.size() = " + std::to_string(list_.size());
			std::__throw_out_of_range(error_msg.c_str());
		}
		return list_[n];
	};
/*
	const Var& operator[](size_t n) const {
		if (n > list_.size()) {
			std::string error_msg = 
				"n > list_.size(), n = " + std::to_string(n) + 
				", list_.size() = " + std::to_string(list_.size());
			std::__throw_out_of_range(error_msg.c_str());
		}
		return list_[n];
	};
*/
	Var& operator*() const {
		if (pointer_ == nullptr) {
			std::__throw_runtime_error("in getting pointed Var: pointer_ == nullptr");
		}
		if ((*pointer_).type_ == type_t::Null) {
			std::__throw_runtime_error("in getting pointed Var: (*pointer_).type_ == type_t::Null");
		}
		return *pointer_;
	};
public:
	class Null;
	class Free;
	class Int;
	class Ptr;
	class List;
	class Map;
	//friend class List;
};


class Var::Null : public Var {
public:
	Null() : Var(0, nullptr, {}, type_t::Null) {};
};

class Var::Free : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Free(value_t n, ptr_t p, const list_t& l) : Var(n, p, l, type_t::Free) {};
	Free() : Free(0, nullptr, {}) {};
};

class Var::Int : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Int(value_t n) : Var(n, nullptr, {}, type_t::Int) {};
	Int() : Int(0) {};
};

class Var::Ptr : public Var {
public:
	Ptr(ptr_t p) : Var(0, p, {}, type_t::Ptr) {};
	Ptr(Var& v) : Ptr(&v) {};
	template <class var_t>
	requires std::is_convertible_v<var_t, Var> || (!std::is_reference_v<var_t>)
	Ptr(var_t v) : Var(0, nullptr, {v}, type_t::Ptr) { pointer_ = &list_[0]; };
	//template <class var_t>
	//requires (std::is_lvalue_reference_v<var_t>)
	Ptr(nullptr_t p = nullptr) : Ptr(Null()) {};
public:
	static int main() {
		Var f = Free();
		Var& f_ref = f;
		auto p = Ptr(Int(8));
		auto p2 = Ptr(Free());
		auto p3 = Ptr(f_ref);
		return 0;
	};
};

class Var::List : public Var {
private:
	//template <template <class...> class in_iter_t, class value_t>
	//static list_t FromRange(in_iter_t<value_t*>, const in_iter_t<value_t*>); 
	//template <class value_t>
	//static list_t FromRange(value_t*, const value_t*); 
	
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
/*
	template <
		class impl_value_t, class arg_value_t,
		bool isRangeImpl = !std::is_null_pointer_v<impl_value_t>,
		bool isRange = !std::is_null_pointer_v<arg_value_t>,
		class value_t = std::conditional_t<isRangeImpl, impl_value_t, arg_value_t>>
	
	using enable_if_impl_or_arg_value_t = value_t;
*/

	template <
		template <class...> class range_t, 
		class value_t,
		class var_t = enable_if_var_value_t<value_t>>
	static list_t FromRange(const range_t<value_t>& l) {
		list_t result_l = {};
		//result_l.reserve(l.size());
		for (value_t value : l) result_l.push_back(var_t(value)); 
		return result_l;
	};
	template <
		class range_t, 
		size_t extent = std::extent_v<range_t>,/*
		bool isArray = std::is_array_v<range_t>,
		bool isPointer = std::is_pointer_v<range_t>,
		class no_extent_value_t = std::remove_all_extents_t<range_t>,
		class no_ptr_value_t = std::remove_pointer_t<range_t>,
		//bool isStdArray = std::is_same_v<range_t, >
		//class value_t = std::conditional_t<isArray, no_extent_value_t, no_ptr_value_t>,
		bool isIntVar = std::is_arithmetic_v<value_t>,
		bool isPtrVar = std::is_same_v<value_t, ptr_t>,
		class var_t = std::conditional_t<isIntVar, Int, Ptr>>*/
		//int N = std::extent_v<range_t>,
		class value_t = enable_if_extent_or_ptr_t<range_t>,
		class var_t = enable_if_var_value_t<value_t>>
	//requires isArray || isPointer
	static list_t FromRange(const range_t& l, size_t size = extent) {
		list_t result_l = {};
		//result_l.reserve(size);
		for (int i = 0; i < size; i++) result_l.push_back(var_t(l[i])); 
		return result_l;
	}; 
	/*
	template <
		class range_impl_t = nullptr_t,
			size_t range_extent = std::extent_v<range_impl_t>,
			bool isImplArray = std::is_array_v<range_impl_t>,
			bool isImplPointer = std::is_pointer_v<range_impl_t>,
			class no_extent_impl_value_t = std::remove_all_extents_t<range_impl_t>,
			class no_ptr_impl_value_t = std::remove_pointer_t<range_impl_t>,
			class impl_value_t = std::conditional_t<isImplArray, no_extent_impl_value_t, no_ptr_impl_value_t>,
		//class impl_value_t = enable_if_extent_or_ptr_t<range_impl_t>,
		template <class...> class range_t = std::vector, class arg_value_t = nullptr_t,

		bool isRangeImpl = !std::is_null_pointer_v<impl_value_t>,
		bool isRange = !std::is_null_pointer_v<arg_value_t>,
		class value_t = std::conditional_t<isRangeImpl, impl_value_t, arg_value_t>,
		//class value_t = enable_if_impl_or_arg_value_t<impl_value_t, arg_value_t>,
		bool isIntVar = std::is_arithmetic_v<value_t>,
		bool isPtrVar = std::is_same_v<value_t, ptr_t>,
		//class var_t = enable_if_var_value_t<value_t>
		class var_t = std::conditional_t<isRangeImpl, impl_value_t, value_t>
	>
	//requires (isPtrVar || isIntVar) && (isRange || (isRangeImpl && (isImplArray || isImplPointer)))
	static list_t FromRange(const range_impl_t& l = nullptr, size_t size = range_extent, const range_t<arg_value_t>& l2 = {})  {
		list_t result_l = {};
		result_l.reserve(l.size());
		//for (value_t value : l) result_l.push_back(var_t(value)); 
		return FromRange(l, l.size());
	};
	*/
		/*int N = std::extent_v<range_t>,
		bool isArray = std::is_array_v<range_t>,
		bool isPointer = std::is_pointer_v<range_t>,
		class no_extent_value_t = std::remove_all_extents_t<range_t>,
		class no_ptr_value_t = std::remove_pointer_t<range_t>,*/
		//bool isStdArray = std::is_same_v<range_t, >
		//class value_t = std::conditional_t<isArray, no_extent_value_t, no_ptr_value_t>,
/*		bool isIntVar = std::is_arithmetic_v<value_t>,
		bool isPtrVar = std::is_same_v<value_t, ptr_t>,
		class var_t = std::conditional_t<isIntVar, Int, Ptr>
		//int N = std::extent_v<range_t>,
		class value_t = enable_if_extent_or_ptr_t<range_t>,
		class var_t = enable_if_var_value_t<value_t>>
	//requires isArray || isPointer*/
/*	template <class in_iter_t>
	static list_t FromRange(in_iter_t l, size_t size) {

	};*/
	//template <class value_t>
	//requires (std::is_arithmetic_v<value_t>)
	//static list_t FromRange(const std::basic_string<value_t>&); 
	//template <class value_t>
	//requires (std::is_arithmetic_v<value_t>)
	//static list_t FromRange(const std::initializer_list<value_t> &); 
public:
	List(const list_t& l) : Var(0, nullptr, l, type_t::List) {}; // Default
	List() : List((list_t){}) {}; // Empty
	template <class range_t>
	List(const range_t& l) : List(FromRange(l)) {};
	template <class value_t, size_t N>
	List(std::array<value_t, N> l) : List(FromRange(l.data(), N)) {};
	template <class value_t>
	List(const std::initializer_list<value_t>& il) : List(FromRange(il)) {};
	//List(std::array<value_t, 100> l) : List(FromRange(l.data(), 100)) {};
	//List(std::array<value_t, 10> l) : List(FromRange(l.begin(), 10)) {};
	//List(const std::array<value_t, N>& l) : List(FromRange(l)) {};
	//List(const std::initializer_list<int>& il) : List(FromRange(il)) {};
	//List(const std::initializer_list<float>& il) : List(FromRange(il, true)) {};
	//template <template <class...> class range_t, class value_t>
	//List(const range_t<value_t>& l) : List(FromRange(l)) {};
	//template <class range_t, class value_t = std::remove_all_extents_t<range_t>, int N = std::extent_v<range_t>> requires std::is_convertible_v<range_t, int[N]> List(const range_t& s) : List(s) {};
	//template <class range_t, size_t N = std::extent_v<range_t>>
	//List(const range_t& s) : List(FromRange(s, N)) {};
	//template <class char_t>
	//List(std::basic_string<char_t> s);
	//List(const std::vector<int>& l) : List(FromRange(l)) {};
	//List(const std::initializer_list<int>& il) : List(static_cast<std::vector<int>>(il)) {};

	//template <class value_t>
	//List(const value_t* l) : List(FromRange(l,l))  {};
//	List(int* first, int* last) : List(FromRange(first, last))  {};
//	List(char* first, char* last) : List(FromRange(first, last))  {};
//	List(std::vector<int>& l) : List(FromRange(l.begin(), l.end()))  {};
//	List(std::vector<int>& l, bool i) : List(FromRange(l.begin(), l.end()))  {};

//	List(std::initializer_list<char> il) : List(FromRange(il))  {};
//	List(const std::initializer_list<float>& l) : List(FromRange(l))  {};
//	List(const std::initializer_list<float>& l, bool i) : List(FromRange(l))  {};
//	List(const std::vector<char>& l) : List(FromRange(l)) {};
//	List(std::string& l) : List(FromRange(l))  {};
	//List(const std::string& l) : List(FromRange(l))  {};
	//template <class range_impl_t> List(const range_impl_t& l) : List(l) {};
	template <class value_t>
	using L = std::initializer_list<value_t>;
	static int main() {
		unsigned long long a[10];
		const int a3[0] = {};
		const std::array<value_t, 12> a2 = {19193999292, 2833, 1118};
		Var l0 = List(a2);
		Var l = List(a);
		auto i = 1.f;
		std::string s = "";
		Var l2 = List(U"nwndux8wj2+(+3369κδκεν∪⊖δℕ∈≠⊙∈5χρ≠ρσ5ξζξ⊂⊖⊃⊙ΩΚ∫Ν⌋⊙Φ5Χ≠Ρ6∀≠⇒⊖≤δ∮≠δ");
		Var l3 = List((list_t){List()});
		Var l4 = List({&l, &l2, &l3});
		Var f1 = Free(1000, &l4, {});
		Var l5 = List({f1, Ptr(&f1), f1});
		std::cout << char(char8_t((*(*l5[2])[1])[0].value_));
		Var l_1 = List("1010101001010101011");
		L<L<int>> l6 = {{1, 2, 3}, {1,2}};
		std::cout << *((l6.begin() + 1)->begin() + 1);
		Var l_2 = List("");
		return 0;
	};

	//template <class range_t> List(range_t l) : List(FromRange(l)) {};
	//template <class value_t> List(std::initializer_list<value_t> il) : List(static_cast<std::vector<value_t>>(il)) {};
	//List(std::string s) : List(FromRange(s.begin(), s.end())) {};
/*
	template <template <class...> class range_t>
//	List(const std::string& s) : List(Fromange(s, [](value_t n) { return Int(n); })) {}; // List Int
//	List(const std::vector<value_t>& l) : List(FromRange(l, [](value_t n) { return Int(n); })) {}; // List Int
	List(const std::vector<ptr_t>& l) : List(FromRange(l, [](ptr_t p) -> Var { return Ptr(p); } )) {}; // List Ptr Var

//	template<class value_t>
//	List(value_t* first, value_t* last) : List(FromRange(first, last, [](value_t n) { return Int(n); })) {};
	template<class value_t> 
	requires (std::is_arithmetic_v<value_t>)
	List(std::initializer_list<value_t> il) : List(FromRange(il, [](value_t n) { return Int(n); })) {};
	template<template <class...> class range_t, class value_t>
	requires (std::is_arithmetic_v<value_t>)
	List(range_t<value_t> l) : List(FromRange(l, [](value_t n) { return Int(n); })) {};
*/
};
/*
class Var::Map : public Var {
public:
	Map(const map_t& m) : Var(0, nullptr, {}, m, type_t::Map) {};
	Map(const std::initializer_list<std::pair<const Var, Var>>& m) : Map((map_t)m) {};
	Map(const list_t& l1, const list_t& l2) : Map(std::map<Var, Var>(l1, l2)) {};
	template<class key_range_t, class value_range_t>
	Map(const key_range_t& k_list, const value_range_t& v_list) : Map(std::map<Var, Var>(List::FromRange(k_list), List::FromRange(v_list))) {};
};
*/
#endif // VAR_HPP

int main() {
	Var v1 = Var::Int();
	std::string s = "Hello World!";
	std::vector<float> l = {0.f, 2.3, 3.6};
	Var v = Var::List((std::string)"");
//	Var v = Var::List(l);
	Var::List::main();
	return 0;
}

