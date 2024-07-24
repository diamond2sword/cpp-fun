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
	using list_t = std::vector<Var>;
	enum class type_t;
	using id_t = unsigned long long int;
	using id_list_t = std::vector<id_t>;
	using iter_t = std::__wrap_iter<Var*>;
private:
	static id_t instance_counter_;
	static list_t instance_list_;
private:
	const id_t id_;
	value_t value_;
	id_t its_id_;
	id_list_t id_list_;
	const enum type_t type_;
private:
	Var(const id_t id, const type_t t, const value_t n, const id_t its_id, const id_list_t l);
protected:
	Var(const id_t id, const type_t t);
public:
	Var(const id_t id);
public:
	template <class var_t, class... args_t>
	static const Var New(args_t&&... args);
private:
	static iter_t Find(const id_t id);
public:
	bool Exists() const;
private:
	Var& operator*() const;
public:
	value_t& value() const;
	id_t& its_id() const;
	id_list_t& id_list() const;
	type_t type() const;
	id_t id() const;
private:
	static std::string ToString(const type_t t);
	static std::string ToString(const Var& v);
	static std::string ToStringTree(const Var& v);
public:
	static std::string ToString();
	std::string ToStringTree() const;
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
	Pair,
	Map,
	Err,
};

Var::id_t Var::instance_counter_ = 1;
Var::list_t Var::instance_list_ = {};
	
Var::Var(const id_t id, const type_t t, const value_t n, const id_t its_id, const id_list_t l) : id_(id), type_(t), value_(n), its_id_(its_id), id_list_(l) {};

Var::Var(const id_t id, const type_t t) : Var(id, t, 0, 0, {}) {};

Var::Var(const id_t id) : Var(id, type_t::Ref) {};

template <class var_t, class... args_t>
const Var Var::New(args_t&&... args) {
	const id_t id = instance_counter_++;
	Var v = var_t(id);
	instance_list_.push_back(std::move(v));
	const Var v_ref = Var(id);
	var_t::Assign(v_ref, std::forward<args_t>(args)...);
	return v_ref;
};

Var::iter_t Var::Find(const id_t id) {
	return std::find_if(
		std::begin(instance_list_), 
		std::end(instance_list_),
		[id](const Var& v) { 
			return v.id_ == id; 
		}
	);
};

bool Var::Exists() const {
	if (type_ != type_t::Ref) {
		std::__throw_runtime_error("bool Var::Exists() const: type_ != type_t::Ref");
	}
	return Find(id_) != std::cend(instance_list_);
};

Var& Var::operator*() const {
	auto it = Find(id_);
	if (type_ != type_t::Ref) {
		std::__throw_runtime_error("Var& Var::operator*() const: type_ != type_t::Ref");
	}
	if (it == std::cend(instance_list_)) {
		std::__throw_runtime_error("Var& Var::operator*() const: it == std::cend(instance_list_)");
	}
	return *it;
};

Var::value_t& Var::value() const { return (**this).value_; };
Var::id_t& Var::its_id() const { return (**this).its_id_; };
Var::id_list_t& Var::id_list() const { return (**this).id_list_; };
Var::type_t Var::type() const { return (**this).type_; };
Var::id_t Var::id() const { return (**this).id_; };

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
	
std::string Var::ToStringTree(const Var& v) {
	std::string list_s = "";
	for (int i = 0; i < v.id_list().size(); i++) {
		try {
			list_s += (i == 0 ? "" : ", ") + ToString(Var(v.id_list().at(i)));
		} catch (const std::exception e) {
			list_s += (i == 0 ? "" : ", ") + (std::string)"\"Error\""; 
		}
	}
	list_s = "[" + list_s + "]";
	std::string result_s = 
		"\"n\": " + std::to_string(v.value()) + 
		", \"its_id\": ";
	try {
		result_s += (
			v.type() != type_t::Ptr || v.its_id() == 0 ? std::to_string(v.its_id())
			: Var(v.its_id()).ToString()
		);
	} catch (const std::exception e) {
		result_s += "ERROR";
	}
	result_s +=
		", \"l\": " + list_s +
		", \"t\": " + "\"" + ToString(v.type()) + "\"" +
		", \"id\": " + std::to_string(v.id());
	return "{" + result_s + "}";
};

std::string Var::ToString(const Var& v) {
	std::string list_s = "";
	for (int i = 0; i < v.id_list().size(); i++) {
		list_s += (i == 0 ? "" : ", ") + std::to_string(v.id_list().at(i));
	}
	list_s = "[" + list_s + "]";
	const std::string result_s = 
		"\"n\": " + std::to_string(v.value()) + 
		", \"its_id\": " + std::to_string(v.its_id()) +
		", \"l\": " + list_s +
		", \"t\": " + "\"" + ToString(v.type()) + "\"" +
		", \"id\": " + std::to_string(v.id());
	return "{" + result_s + "}";
};

std::string Var::ToString() {
	std::string result_s = "";
	for (int i = 0; i < instance_list_.size(); i++) {
		result_s += (i == 0 ? "" : ", ") + ToString(Var(instance_list_[i].id_));
	}
	return "{\"Var::instance_list_\": [" + result_s + "]}";
};

std::string Var::ToStringTree() const {
	return ToStringTree(*this);
};

class Null : public Var {
private:
	Null(const id_t id) : Var(id, type_t::Null) {};

	static void Assign(const Var& v) {};
public:
	friend class Var;
};
	
class Free : public Var {
private:
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	Free(const id_t id) : Var(id, type_t::Free) {};
	
	static void Assign(const Var& v, const value_t n, const id_t its_id, const id_list_t& id_l) {
		v.value() = n;
		v.its_id() = its_id;
		v.id_list() = id_l;
	};

	static void Assign(const Var& v) {};
public:
	friend class Var;
};

class Int : public Var {
private:
	Int(const id_t id) : Var(id, type_t::Int) {};
	template<class value_t>
	requires std::is_arithmetic_v<value_t>
	static void Assign(const Var& v, const value_t n) {
		v.value() = n;
	};
	
	static void Assign(const Var& v) {};
public:
	friend class Var;
};

class Ptr : public Var {
private:
	Ptr(const id_t id) : Var(id, type_t::Ptr) {};
	static void Assign(const Var& v, const id_t its_id) {
		v.its_id() = its_id;
	};
	static void Assign(const Var& v, const Var& other_v) {
		Assign(v, other_v.id());
	};

	static void Assign(const Var& v) {
		Assign(v, New<Null>());
	};
public:
	friend class Var;
};

class List : public Var {
private:
	List(const id_t id) : Var(id, type_t::List) {};
private:
	static void Assign(const Var& v, const id_list_t& id_l) {
		v.id_list() = id_l;
	};

	template <class in_iter_t>
	requires (!std::is_convertible_v<in_iter_t, Var>)
	static void Assign(const Var& v, in_iter_t first, const in_iter_t last) {
		id_list_t result_l = {};
		for (; first != last; first++) {
			const Var child_v = New<Int>(*first);
			result_l.push_back(child_v.id());
		}
		v.id_list() = result_l;
	}

	template <class range_t>
	requires (!std::is_convertible_v<range_t, Var>)
	static void Assign(const Var& v, const range_t& l) {
		Assign(v, std::begin(l), std::cend(l));
	};

	static void Assign(const Var& v) {};
public:
	static int main();
public:
	friend class Var;
};

class Pair : public Var {
private:
	Pair(const id_t id) : Var(id, type_t::Pair) {};

	template <class value_t>
	requires std::is_arithmetic_v<value_t>
	static void Assign(const Var& v, value_t key_n, const Var& value_v) {
		v.value() = key_n;
		v.its_id() = value_v.id();
	};
public:
	friend class Var;
};

class Map : public Var {
private:
	Map(const id_t id) : Var(id, type_t::Map) {};
	
	template <class value_t>
	requires std::is_arithmetic_v<value_t>
	static void Update(const Var& v, value_t key_n, const Var& value_v) {
		auto it = std::find_if(std::begin(v.id_list()), std::end(v.id_list()), [key_n](const id_t id) {
			return Var(id).value() == key_n;
		});
		if (it != std::cend(v.id_list())) {
			Var(*it).its_id() = value_v.id();
		}
		const Var pair_v = New<Pair>(key_n, value_v);
		v.id_list().push_back(pair_v.id());
	};

	static void Remove(const Var& v, const Var& key_v) {
		auto it = std::find_if(std::begin(v.id_list()), std::end(v.id_list()), [&key_v](const id_t id) {
			return Var(id).value() == key_v.value();
		});
	};
public:
	friend class Var;
};

class Err : public Var {
private:
	Err(const id_t id) : Var(id, type_t::Err) {};
public:
	friend class Var;
};

#endif // VAR_HPP

int main() { 
	std::cout << "Hello World";
/*	std::vector<int> l = {0, 1, 3, 4};
	Var v1 = List(0, (std::initializer_list<int>){0, 1, 2, 3});
	Var v2 = Var::New<List>((std::initializer_list<int>){0, 1, 3, 4});
	Var v3 = List(0, "Hello");
	std::cout << v2.ToString();*/




	Var::main(); 
	List::main();
	return 0;
}

int Var::main() {
	//Var v = New<List>(100);
	Var v2 = New<List>("Hello");

	std::cout << v2.ToStringTree() << std::endl;
	std::cout << v2.ToString() << std::endl;
	//std::string s = "Hello World";
	//Var v = Var::New<List>(s);
	/*
	std::cout << "Hello World!";
	for (const Var& v : Var::instance_list_) {
		std::cout << v.ToString();
	}*/
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
