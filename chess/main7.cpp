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
#include <forward_list>

class Var { 
protected:
	using value_t = long long int;
	using id_t = unsigned long long int;
	using list_t = std::vector<Var>;
	enum class type_t;
private:
	static id_t instance_counter_;
	static list_t instance_list_;
private:
	value_t value_;
	id_t its_id_;
	list_t list_;
	const enum type_t type_;
	const id_t id_;
private:
	Var(value_t n, id_t its_id, const list_t& l, type_t t, id_t id) : value_(n), its_id_(its_id), list_(l), type_(t), id_(id) {};
private:
	Var(id_t its_id);
public:
	static bool Exists(id_t id);
	bool Exists() const;
public:
	static Var Ref(id_t its_id);
protected:
	Var(value_t n, id_t its_id, const list_t& l, type_t t);
public:
	Var& operator*() const;
public:
	type_t type() const;
	id_t id() const;
	list_t& list() const;
	value_t& value() const;
	id_t&  its_id() const;
public:
	static int main();
};

enum class Var::type_t {
	Ref,
	Null,
	Free,
	Int,
	Ptr,
	List,
	Map,
	Err,
};

Var::id_t Var::instance_counter_ = 0;
Var::list_t Var::instance_list_ = {};

Var::Var(id_t its_id) : Var(0, 0, {}, type_t::Ref, its_id) {};

bool Var::Exists(id_t id) {
	auto it = std::find_if(
		std::begin(instance_list_), 
		std::end(instance_list_),
		[&id](const Var& v) { 
			return v.id_ == id; 
		}
	);
	return it != instance_list_.cend();
};

bool Var::Exists() const {
	if (type_ != type_t::Ref) {
		std::__throw_runtime_error("bool Var::Exists() const: type_ != type_t::Ref");
	}
	return Exists(id_);
};

Var& Var::operator*() const {
	if (!Exists()) {
		std::__throw_runtime_error("Var& Var::operator*() const: !Exists()");
	}
	return instance_list_[id_];
};

Var::value_t& Var::value() const { return (**this).value_; };
Var::id_t& Var::its_id() const { return (**this).its_id_; };
Var::list_t& Var::list() const { return (**this).list_; };
Var::type_t Var::type() const { return (**this).type_; };
Var::id_t Var::id() const { return (**this).id_; };

Var Var::Ref(id_t id) {
	if (!Exists(id)) {
		std::__throw_runtime_error("Var Var::Ref(id_t id): !Exists(its_id)");
	}
	return Var(id);
}

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
		if (!Exists(its_id)) {
			std::__throw_runtime_error("Ptr(id_t its_id): !Exists(its_id)");
		}
	};
	Ptr(const Var& v) : Ptr(v.id()) {};

	template <class var_t>
	requires std::is_convertible_v<var_t, Var> && (!std::is_reference_v<var_t>)
	Ptr(var_t v) : Ptr(v.id()) {};

	Ptr() : Ptr(Null()) {};
};

class List : public Var {
public:
	List(const list_t& l = {}) : Var(0, 0, l, type_t::List) {
		for (const Var& v : l) if (!v.Exists()) {
			std::__throw_runtime_error("List(const list_t& l = {}): !v.Exists()");
		}
	};

	template <class in_iter_t>
	requires (!std::is_convertible_v<in_iter_t, Var>)
	List(in_iter_t first, const in_iter_t last) : List() {
		for (; first != last; first++) {
			Var::list().push_back(Int(*first));
		}
	}

	template <class range_t>
	requires (!std::is_convertible_v<range_t, Var>)
	List(const range_t& l) : List(std::begin(l), std::end(l)) {};
public:
	static int main();
};

class Err : public Var {
	Err() : Var(0, 0, {}, type_t::Err) {};
};

#endif // VAR_HPP

int main() { 
	Var::main(); 
	List::main();
	return 0;
}

int Var::main() {
/*
	Var v = Int();
	std::cout << (*v).value_;
	Var l = ::List({v, v});
	Var l2 = ::List("");
	std::string s = "";
	Var l3 = ::List(s);
	Var l4 = ::List(u8"18181818");
*/
	return 0;
};

int List::main() {
/*
	std::string s = "";
	std::array<int, 0> a = {};
	List l2 = List();
	Var l3 = List();
	Var l = List();
*/
	List v = "Hello";
	for (const Var& v : v.list()) std::cout << v.value() << ", ";
	std::cout << std::endl;
	return 0;
};
