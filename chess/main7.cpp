#ifndef VAR_HPP
#define VAR_HPP

#include <__type_traits/is_char_like_type.h>
#include <charconv>
#include <complex>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <initializer_list>
#include <iomanip>
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
	const id_t id_;
	value_t value_;
	id_t its_id_;
	list_t list_;
	const enum type_t type_;
public:
	value_t& value() const;
	id_t& its_id() const;
	list_t& list() const;
	type_t type() const;
	id_t id() const;
protected:
	Var(const id_t id, const value_t n, const id_t its_id, const list_t& l, const type_t t) : id_(id), value_(n), its_id_(its_id), list_(l), type_(t) {};
protected:
	Var(const value_t n, const id_t its_id, const list_t& l, const type_t t) : Var(instance_counter_++, n, its_id, l, t) {};
private:
	Var(const id_t id);
public:
	static bool ExistsIn(const id_t id, const list_t& l);
	static bool Exists(const id_t id);
	bool Exists() const;
public:
	static Var Ref(const id_t its_id);
public:
	void Add(id_t id) const;
public:
	template <class var_t, class... args_t>
	static Var Make(args_t&&... args);
public:
	template <class var_t, class... args_t>
	static void AddIn(list_t& l, args_t&&... args);
public:
	template <class var_t, class... args_t>
	static Var New(args_t&&... args);
public:
	template <class var_t, class... args_t>
	void Add(args_t&&... args) const;
/*protected:
	Var(const value_t n, const id_t its_id, const list_t& l, const type_t t);*/
public:
	Var& operator*() const;
private:
	static std::string ToString(const type_t t);
	static std::string ToString(const Var& v);
public:
	std::string ToString() const;
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

Var::value_t& Var::value() const { return (**this).value_; };
Var::id_t& Var::its_id() const { return (**this).its_id_; };
Var::list_t& Var::list() const { return (**this).list_; };
Var::type_t Var::type() const { return (**this).type_; };
Var::id_t Var::id() const { return (**this).id_; };

Var::Var(const id_t id) : Var(id, 0, 0, {}, type_t::Ref) {};

template <class var_t, class... args_t>
Var Var::Make(args_t&&... args) {
	return var_t(std::forward<args_t>(args)...);
};

template <class var_t, class... args_t>
void Var::AddIn(list_t& l, args_t&&... args) {
	l.push_back(Make<var_t>(std::forward<args_t>(args)...));
};

template <class var_t, class... args_t>
Var Var::New(args_t&&... args) {
	const id_t id = instance_counter_++;
	AddIn<var_t>(instance_list_, id, std::forward<args_t>(args)...);
	return Var(id);
};

template <class var_t, class... args_t>
void Var::Add(args_t&&... args) const {
	list().push_back(New<var_t>(std::forward<args_t>(args)...));
};

bool Var::ExistsIn(const id_t id, const list_t& l) {
	auto it = std::find_if(
		std::begin(l), 
		std::end(l),
		[id](const Var& v) { 
			return v.id_ == id; 
		}
	);
	return it != std::cend(l);
};

bool Var::Exists(const id_t id) {
	return ExistsIn(id, instance_list_);
};

bool Var::Exists() const {
	if (type_ != type_t::Ref) {
		std::cout << ToString(); // BUG
		std::__throw_runtime_error("bool Var::Exists() const: type_ != type_t::Ref: ");
	}
	return Exists(id_);
};

Var Var::Ref(const id_t id) {
	if (!Exists(id)) {
		std::__throw_runtime_error("Var Var::Ref(id_t id): !Exists(its_id)");
	}
	return Var(id);
}

void Var::Add(const id_t id) const {
	if (ExistsIn(id, list())) {
		std::__throw_runtime_error("void Var::Add(const id_t id) const: ExistsIn(id, list())");
	}
	list().push_back(Var::Ref(id));
};

Var& Var::operator*() const {
	if (!Exists()) {
		std::__throw_runtime_error("Var& Var::operator*() const: !Exists()");
	}
	return instance_list_[id_];
};

/*
Var::Var(const value_t n, const id_t its_id, const list_t& l, const type_t t) : Var(instance_counter_++) {
	instance_list_.push_back(Var(n, its_id, l, t, id_));
};*/

std::string Var::ToString(const type_t t) {
	switch (t) {
	case type_t::Ref: return "Ref";
	case type_t::Null: return "Null";
	case type_t::Free: return "Free";
	case type_t::Int: return "Int";
	case type_t::Ptr: return "Ptr";
	case type_t::List: return "List";
	case type_t::Map: return "Map";
	case type_t::Err: return "Err";
	default: return "TypeNotImplemented";
	}
};
	
std::string Var::ToString(const Var& v) {
	std::string list_s = "";
	for (int i = 0; i < v.list().size(); i++) {
		list_s += (i == 0 ? "" : ", ") + ToString(v.list().at(i));
	}
	list_s = "[" + list_s + "]";
	const std::string result_s = 
		"\"n\": " + std::to_string(v.value()) + 
		", \"its_id\": " + std::to_string(v.its_id()) +
		", \"l\": " + list_s +
		", \"t\": " + ToString(v.type()) +
		", \"id\": " + std::to_string(v.id());
	return "{" + result_s + "}";
};

std::string Var::ToString() const {
	std::cout << "Printing: "; // BUG
	return ToString(*this);
};

class Null : public Var {
public:
	Null(const id_t id) : Var(id, 0, 0, {}, type_t::Null) {};
};
	
class Free : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Free(const id_t id, const value_t n, const id_t its_id, const list_t& l) : Var(id, n, its_id, l, type_t::Free) {};
	
	Free(const id_t id) : Free(id, 0, 0, {}) {};
};

class Int : public Var {
public:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Int(const id_t id, const value_t n) : Var(id, n, 0, {}, type_t::Int) {};
	
	Int(const id_t id) : Int(id, 0) {};
};

class Ptr : public Var {
public:
	Ptr(const id_t id, const id_t its_id) : Var(id, 0, its_id, {}, type_t::Ptr) {
		if (!Exists(its_id)) {
			std::__throw_runtime_error("Ptr(const id_t id, const id_t its_id): !Exists(its_id)");
		}
	};
	Ptr(const id_t id, const Var& v) : Ptr(id, v.id()) {};

	template <class var_t>
	requires std::is_convertible_v<var_t, Var> && (!std::is_reference_v<var_t>)
	Ptr(const id_t id, const var_t v) : Ptr(id, v.id()) {};

	Ptr(const id_t id) : Ptr(id, New<Null>()) {}; // TODO: Var<Null>()
};

class List : public Var {
public:
	List(const list_t& l = {}) : Var(0, 0, l, type_t::List) {
		for (const Var& v : l) if (!v.Exists()) {
			std::__throw_runtime_error("List(const id_t id, const list_t& l = {}): !v.Exists()");
		}
	};

	template <class in_iter_t>
	requires (!std::is_convertible_v<in_iter_t, Var>)
	List(in_iter_t first, const in_iter_t last) : List() {
		for (; first != last; first++) {
			list().push_back(New<Int>(*first));
			std::cout << ToString() << std::endl; // BUG
		}
	}

	template <class range_t>
	requires (!std::is_convertible_v<range_t, Var>)
	List(const range_t& l) : List(std::begin(l), std::cend(l)) {};

	template <class value_t>
	List(const std::initializer_list<value_t>& l) : List(std::begin(l), std::end(l)) {};
public:
	static int main();
};

class Err : public Var {
	Err(const id_t id) : Var(id, 0, 0, {}, type_t::Err) {};
};

#endif // VAR_HPP

int main() { 
	//Var v = Var::New<List>("Hello");
	std::string s = "Hello";
	std::vector<int> l = {0, 1, 3, 4};
	Var v1 = List(0, {0, 1, 2, 3});
	Var v2 = Var::New<List>(0, {0, 1, 3, 4});
	Var v3 = List(0, "Hello");
	std::cout << v2.ToString();





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
/*	std::cout << "v.list().size(): " << v.ToString() << std::endl;
	for (const Var& v : v.list()) std::cout << v.value() << ", ";
	std::cout << std::endl;*/
	return 0;
};
