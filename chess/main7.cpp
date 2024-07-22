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
/*private:
	class New;*/
protected:
	using value_t = long long int;
	using id_t = unsigned long long int;
	using list_t = std::vector<Var>;
	enum class type_t;
private:
	static id_t instance_counter_;
	static list_t instance_list_;
protected:
	value_t value_;
	id_t its_id_;
	list_t list_;
	const enum type_t type_;
	const id_t id_;
private:
	Var(value_t n, id_t its_id, const list_t& l, type_t t, id_t id)
		: value_(n), its_id_(its_id), list_(l), type_(t), id_(id) {};
private:
	id_t Find() const;
public:
	static bool Exists(id_t id);
	bool Exists() const;
public:
	Var(id_t its_id);
protected:
	Var(value_t n, id_t its_id, const list_t& l, type_t t);
/*private:
	class Instance;*/
/*
public:
	class Null;
	class Free;
	class Int;
	class Ptr;
	class List;
	class Map;*/
public:
	Var& operator*() const;
	type_t Type() const;
	id_t ID() const;
public:
	static int main();
};

enum class Var::type_t {
	Err,
	Ref,
	Null,
	Free,
	Int,
	Ptr,
	List,
	Map,
};

Var::id_t Var::instance_counter_ = 0;
Var::list_t Var::instance_list_ = {};

class Err : public Var {
	Err() : Var(0, 0, {}, type_t::Err) {};
};

bool Var::Exists(id_t id) {
	auto it = std::find_if(
		instance_list_.begin(), instance_list_.end(),
		[&id](const Var& v) { return v.id_ == id; });
	return it != instance_list_.cend();
};

bool Var::Exists() const {
	if (type_ != type_t::Ref) {
		std::__throw_runtime_error("bool Var::Exists() const: type_ != type_t::Ref");
	}
	return Exists(its_id_);
};

Var& Var::operator*() const {
	if (!Exists()) {
		std::__throw_runtime_error("Var& Var::operator*() const: !Exists()");
	}
	return instance_list_[its_id_];
};

Var::type_t Var::Type() const { 
	return (**this).type_;
};

Var::id_t Var::ID() const {
	return (**this).id_;
};
	
Var::Var(id_t its_id) : Var(0, its_id, {}, type_t::Ref, 0) {
	if (!Exists()) 
};

Var::Var(value_t n, id_t its_id, const list_t& l, type_t t) : Var(instance_counter_++) {
	instance_list_.push_back(Var(n, its_id, l, t, its_id_));
};

class Null : public Var {
public:
	Null() : Var(0, 0, {}, type_t::Null) {};
};
	
class Free : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Free(value_t n, id_t its_id, const list_t& l) : Var(n, its_id, l, type_t::Free) {};
	
	Free() : Free(0, 0, {}) {};
};

class Int : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Int(value_t n) : Var(n, 0, {}, type_t::Int) {};
	
	Int() : Int(0) {};
};

class Ptr : public Var {
public:
	Ptr(id_t its_id) : Var(0, its_id, {}, type_t::Ptr) {
		if (!Var(its_id).Exists()) {
			std::__throw_runtime_error("Ptr(id_t its_id): Var(its_id).Type() == type_t::Null");
		}
	};
	Ptr(const Var& v) : Ptr(v.ID()) {};

	template <class var_t>
	requires std::is_convertible_v<var_t, Var> && (!std::is_reference_v<var_t>)
	Ptr(var_t v) : Ptr(v.ID()) {};

	Ptr() : Ptr(Null()) {};
};

class List : public Var {
private:
/*	template <
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
	}; */
public:
	List(const list_t& l = {}) : Var(0, 0, l, type_t::List) {
		for (const Var& v : l) if (!v.Exists()) {
			std::__throw_runtime_error("List(const list_t& l = {}): !v.Exists()");
		}
	};

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
