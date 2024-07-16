#include "main6-1.hpp"
	
Var::Type Var::DeduceType() const {
	Type type = {};
	value_t scope = 0;
	DeduceType(&type, &scope);
	return type;
};
void Var::DeduceType(Type* type_ptr, value_t* scope_ptr) const {
	// for new scope
	if (*scope_ptr == type_ptr->size()) {
		type_ptr->push_back(base_type_);
	}

	// for unsimilar Type
	if (base_type_ != type_ptr->at(*scope_ptr)) {
		type_ptr->at(*scope_ptr) = TypeBase::Var;
		for (int i = 0; i < type_ptr->size() - *scope_ptr + 1; i++) {
			type_ptr->pop_back();
		}
		return;
	}
	
	// for List and Ptr
	if (base_type_ == TypeBase::List || base_type_ == TypeBase::Ptr) {
		(*scope_ptr)++;
		// for each children
		TransformChildrenConst([&] (const Var& child_v) -> Var {
			// for Var scope
			if (type_ptr->at(*scope_ptr) == TypeBase::Var) {
				return Null();
			}
			// otherwise
			child_v.DeduceType(type_ptr, scope_ptr);
			return Null();
		});
		(*scope_ptr)--;
		return;
	}

	// for Int, Null, Empty
	// Do nothing
};

Var Var::TransformChildrenConst(const std::function<Var(const Var&)>& transform) const {
	if (base_type_ == TypeBase::List) {
		std::vector<Var> v_l = {};
		for (const Var& v : list_) {
			v_l.push_back(transform(v));
		}
		return Var(v_l);
	}
	if (base_type_ == TypeBase::Ptr) {
		 return transform(*(*this));
	}
	return Null();
};

Var Var::operator&() {
	return Var(this);
};
Var Var::operator*() const {
	return base_type_ != TypeBase::Ptr || pointer_ == nullptr ? Null() : *pointer_;
};

Var::Var(const std::vector<Var*>& l) : list_(PtrList(l)), base_type_(TypeBase::List) {}; // List Ptr
Var::Var(const std::vector<Var>& l) : list_(std::move(l)), base_type_(TypeBase::List)  {}; // List Var
Var::Var(const std::string& s) : list_(List(s)), base_type_(TypeBase::List) {}; // List Int from string
Var::Var(const std::vector<value_t>& s) : list_(List(s)), base_type_(TypeBase::List) {}; // List Int
Var Var::Ptr(Var& v) { return Var(&v); }; // Ptr
Var::Var(Var* p) : pointer_(std::move(p)), base_type_(TypeBase::Ptr) {}; // Ptr
Var Var::NullPtr() { return Var(TypeBase::Ptr); }; // Ptr Null
Var::Var(const value_t& c) : value_(std::move(c)), base_type_(TypeBase::Int) {}; // Int from char
Var::Var() : base_type_(TypeBase::Empty) {}; // Empty
Var Var::Null() { return Var(TypeBase::Null); }; // Null
Var::Var(const TypeBase& t) : base_type_(std::move(t)) {}; // any from TypeBase

std::vector<Var> Var::PtrList(const std::vector<Var*>& ptr_l) {
	std::vector<Var> l = {};
	l.reserve(ptr_l.size());
	for (Var* p: ptr_l) {
		l.push_back(p);
	}
	return l;
};
std::vector<Var> Var::List(const std::string& s) {
	std::vector<Var> l = {};
	l.reserve(s.size());
	for (const value_t& c : s) {
		l.push_back(Var(c));
	}
	return l;
};
std::vector<Var> Var::List(const std::vector<value_t>& s) {
	std::vector<Var> l = {};
	l.reserve(s.size());
	for (const value_t& c : s) {
		l.push_back(Var(c));
	}
	return l;
};
