#include <algorithm>
#include <charconv>
#include <climits>
#include <cmath>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include <string>



int main() {
	class Var {
	public:
		typedef long long value_t;
		enum class Type { Null, Empty, IntList, VarList, Int, Ptr, PtrList, Var};
	private:
		Var(const Type& t) : type(std::move(t)) {};
		static std::vector<Var> List(const std::vector<value_t>& s) {
			std::vector<Var> l = {};
			l.reserve(s.size());
			for (const value_t& c : s) {
				l.push_back(Var(c));
			}
			return l;
		};
		static std::vector<Var> List(const std::string& s) {
			std::vector<Var> l = {};
			l.reserve(s.size());
			for (const value_t& c : s) {
				l.push_back(Var(c));
			}
			return l;
		};
		static std::vector<Var> PtrList(const std::vector<Var*>& ptr_l) {
			std::vector<Var> l = {};
			l.reserve(ptr_l.size());
			for (Var* p: ptr_l) {
				l.push_back(p);
			}
			return l;
		};

	public:
		static Var Null() { return Var(Type::Null); };
		static Var Ptr(Var& v) { return Var(&v); };
		Var() : type(Type::Empty) {};
		Var(Var* p) : pointer(std::move(p)), type(Type::Ptr) {};
		Var(const std::vector<Var*>& l) : list(PtrList(l)), type(Type::PtrList) {};
		Var(const value_t& c) : value(std::move(c)), type(Type::Int) {};
		Var(const std::vector<value_t>& s) : list(List(s)), type(Type::IntList) {};
		Var(const std::string& s) : list(List(s)), type(Type::IntList) {};
		Var(const std::vector<Var>& l) : list(std::move(l)), type(Type::VarList)  {};

		bool Equals(const Var& v) const {
			return value == v.value;
		};

		std::string ToString() const {
			if (type == Type::Null) {
				return "NULL";
			}
			if (type == Type::Empty) {
				return "EMPTY";
			}
			if (type == Type::Ptr) {
				return "PTR";
			}
			if (type == Type::Int) {
				return std::to_string(value);
			}
			// for VarList and IntList and PtrList
			std::string s = "";
			for (int i = 0; i < list.size(); i++) {
				s += (i == 0 ? "" : ", ") + list[i].ToString();
			}
			return "{" + s + "}";
		};

		void Type(std::vector<enum Type>* type_l_ptr, value_t* scope_ptr) const {
			// for new scope
			if (*scope_ptr == type_l_ptr->size()) {
				type_l_ptr->push_back(type);
			}
			// for Var scope
			if (type_l_ptr->at(*scope_ptr) == Type::Var) {
				return;
			}
			// for unsimilar Type
			if (type != type_l_ptr->at(*scope_ptr)) {
				type_l_ptr->at(*scope_ptr) = Type::Var;
				for (int i = 0; i < type_l_ptr->size() - *scope_ptr + 1; i++) {
					type_l_ptr->pop_back();
				}
				return;
			}
			// for not list
			if (!IsList()) {
				return;
			}
			// for list
			(*scope_ptr)++;
			for (const Var& v : list) {
				v.Type(type_l_ptr, scope_ptr);
			}
			(*scope_ptr)--;
		};

		Var operator+(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value + v.value);
		};

		Var operator-(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value - v.value);
		}

		Var operator*(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value * v.value);
		}

		Var operator/(const Var& v) const {
			return type != v.type || type != Type::Int || v.value == 0 ? Null() : Var(value / v.value);
		};

		Var operator%(const Var& v) const {
			return type != v.type || type != Type::Int || v.value == 0 ? Null() : Var(value % v.value);
		};

		Var operator^(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(std::pow(value, v.value));
		};
		
		Var operator+=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int ? Null() : Var(value + v.value);
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};
			
		Var operator-=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int ? Null() : Var(value - v.value);
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};

		Var operator*=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int ? Null() : Var(value * v.value);
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};

		Var operator/=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int || v.value == 0 ? Null() : Var(value / v.value);
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};

		Var operator%=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int || v.value == 0 ? Null() : Var(value % v.value);
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};

		Var operator^=(const Var& v) {
			const Var result_v = type != v.type || type != Type::Int ? Null() : Var(std::pow(value, v.value));
			return result_v.type == Type::Null ? result_v : *this = result_v;
		};

		Var operator&&(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value && v.value);
		};

		Var operator||(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value || v.value);
		};
		
		Var operator!() const {
			return type != Type::Int ? Null() : Var(!value);
		};

		Var operator==(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value == v.value);
		};
		
		Var operator!=(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value != v.value);
		};

		Var operator<(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value < v.value);
		};

		Var operator>(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value > v.value);
		};

		Var operator<=(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value <= v.value);
		};

		Var operator>=(const Var& v) const {
			return type != v.type || type != Type::Int ? Null() : Var(value >= v.value);
		};

		Var operator++() {
			const Var v = type != Type::Int ? Null() : Var(value + 1);
			return v.type == Type::Null ? v : *this = v;
		};

		Var operator--() {
			const Var v = type != Type::Int ? Null() : Var(value - 1);
			return v.type == Type::Null ? v : *this = v;
		};

		bool IsList() const {
			return type == Type::IntList || type == Type::VarList || type == Type::PtrList;
		};
		
		Var operator*() {
			return type != Type::Ptr || pointer == nullptr ? Null() : *pointer;
		};

		Var operator&() {
			return Var(this);
		};
		
		Var operator[](const size_t n) {
			return !IsList() || n >= list.size() ? Null() : list[n];
		};

		Var operator[](const Var& v) {
			return !IsList() || v.type == Type::Int || v.value >= list.size() ? Null() : list.at(v.value);
		};
		
		Var operator[]() {
			return !IsList() || list.empty() ? Null() : list.front();
		};
		
		Var* pointer = nullptr;
		std::vector<Var> list = {};
		value_t value = 0;
		enum Type type = Type::Empty;
	};
	
	class List {
	public:
		List(std::vector<int> l) : value(l) {};
		std::vector<int*> operator&() {
			std::vector<int*> l = {};
			l.reserve(value.size());
			for (int& i : value) {
				l.push_back(&i);
			}
			return l;
		};
		std::vector<int> value;
	};





	typedef std::vector<Var> Vars;
	typedef std::vector<Var*> VarPtrs;

	Var v = Var("0");
	Var v2 = Var({Var("012345678"), Var::Null(), Var('0'), Var((Vars){})});
	Var v3 = Var(true) && Var(8);
	Var v4 = {};
	Var v5 = Var({&v});

	class Var2 {
	public:
		enum class TypeBase { Null, Empty, Var, Int, List, Ptr };
	public:
		typedef long long int value_t;
		typedef std::vector<TypeBase> Type;
	public:
		Var2 operator*() const {
			return base_type != TypeBase::Ptr || pointer == nullptr ? Null() : *pointer;
		};
	public:
		static std::vector<Var2> List(const std::vector<value_t>& s) {
			std::vector<Var2> l = {};
			l.reserve(s.size());
			for (const value_t& c : s) {
				l.push_back(Var2(c));
			}
			return l;
		};
		static std::vector<Var2> List(const std::string& s) {
			std::vector<Var2> l = {};
			l.reserve(s.size());
			for (const value_t& c : s) {
				l.push_back(Var2(c));
			}
			return l;
		};
		static std::vector<Var2> PtrList(const std::vector<Var2*>& ptr_l) {
			std::vector<Var2> l = {};
			l.reserve(ptr_l.size());
			for (Var2* p: ptr_l) {
				l.push_back(p);
			}
			return l;
		};
	public:
		Var2(const TypeBase& t) : base_type(std::move(t)) {};
		static Var2 Null() { return Var2(TypeBase::Null); };
		static Var2 Ptr(Var2& v) { return Var2(&v); };
		Var2() : base_type(TypeBase::Empty) {};
		Var2(Var2* p) : pointer(std::move(p)), base_type(TypeBase::Ptr) {};
		Var2(const std::vector<Var2*>& l) : list(PtrList(l)), base_type(TypeBase::List) {};
		Var2(const value_t& c) : value(std::move(c)), base_type(TypeBase::Int) {};
		Var2(const std::vector<value_t>& s) : list(List(s)), base_type(TypeBase::List) {};
		Var2(const std::string& s) : list(List(s)), base_type(TypeBase::List) {};
		Var2(const std::vector<Var2>& l) : list(std::move(l)), base_type(TypeBase::List)  {};
	public:
		class ToBePrinted {
		public:
			enum class PrintType {
				None,
				Int,
				Str,
				TypeBase,
				Type,
				IntPtr,
				StrPtr,
				TypeBasePtr,
				TypePtr,
				VarPtr,
			};
			typedef enum PrintType PType;
		public:
			class Printer {
			public:
				Printer(std::vector<ToBePrinted> tbp_l) : to_be_printed_l(tbp_l) {};
				void Print() const {
					std::cout << ToString();
				}
				std::string ToString() const {
					std::string s = "";
					for (const ToBePrinted& tbp : to_be_printed_l) {
						s += tbp.ToString();
					}
					return s;
				}
			public:
				Printer operator+(const Printer& p) const {
					Printer result_p = *this;
					result_p.to_be_printed_l.reserve(p.to_be_printed_l.size());
					for (const ToBePrinted& tbp : p.to_be_printed_l) {
						result_p.to_be_printed_l.push_back(tbp);
					}
					return result_p;
				};
			public:
				std::vector<ToBePrinted> to_be_printed_l = {};
			};
		public:
			std::string ToString() const {
				std::string s = "";
				switch (print_type) {
				// Non-pointer
				case PType::None:
					return "";
				case PType::Int:
					return std::to_string(int_);
				case PType::Str:
					return str_;
				case PType::TypeBase:
					return ToString(base_type_);
				case PType::Type:
					return ToString(type_);
				// Pointer
				case PType::IntPtr:
					return std::to_string(*int_ptr);
				case PType::StrPtr:
					return *str_ptr;
				case PType::TypeBasePtr:
					return ToString(*base_type_ptr);
				case PType::TypePtr:
					return ToString(*type_ptr);
				case PType::VarPtr:
					return ToString(*var_ptr);
				default:
					return "ToBePrintedNotImplemented";
				}
			};
			inline static std::string ToString(std::string s) {
				return s;
			};
			static std::string ToString(const enum TypeBase& base_type) {
				switch (base_type) {
				case TypeBase::Null:
					return "Null";
				case TypeBase::Empty:
					return "Empty";
				case TypeBase::Var:
					return "Var";
				case TypeBase::Int:
					return "Int";
				case TypeBase::List:
					return "List";
				case TypeBase::Ptr:
					return "Ptr";
				default:
					return "TypeBaseNotImplemented";
				}
			};
			inline static std::string ToString(const Type& type) {
				std::vector<std::string> s_l = {};
				for (const TypeBase& base_type : type) {
					s_l.push_back(ToString(base_type));
				}
				return ToListString(s_l);
			};
			static std::string ToString(const Var2& v) {
				switch (v.base_type) {
				case TypeBase::Null:
					return "Null";
				case TypeBase::Empty:
					return "Empty";
				case TypeBase::Var:
					return "Var";
				case TypeBase::Int:
					return std::to_string(v.value);
				case TypeBase::List: {
					std::vector<std::string> s_l = {};
					for (const Var2& v : v.list) {
						s_l.push_back(ToString(v));
					}
					return ToListString(s_l);
				}
				case TypeBase::Ptr:
					return ToString(*v);
				default:
					return "TypeBaseNotImplemented";
				}
			};
			inline static std::string ToListString(const std::vector<std::string>& s_l) {
				std::string result_s = "";
				for (int i = 0; i < s_l.size(); i++) {
					result_s += (i == 0 ? "" : ", ") + s_l[i];
				}
				return "{" + result_s + "}";
			};
		public:
			ToBePrinted() : print_type(PType::None) {};
			// Non-Pointer
			ToBePrinted(value_t i) : int_(i), print_type(PType::Int) {};
			ToBePrinted(std::string s) : str_(s), print_type(PType::Str) {};
			ToBePrinted(TypeBase bt) : base_type_(bt), print_type(PType::TypeBase) {};
			ToBePrinted(Type t) : type_(t), print_type(PType::Type) {};
			// Pointer
			ToBePrinted(value_t* i_ptr) : int_ptr(i_ptr), print_type(PType::IntPtr) {};
			ToBePrinted(std::string* s_ptr) : str_ptr(s_ptr), print_type(PType::StrPtr) {};
			ToBePrinted(TypeBase* bt_ptr) : base_type_ptr(bt_ptr), print_type(PType::TypeBasePtr) {};
			ToBePrinted(Type* t_ptr) : type_ptr(t_ptr), print_type(PType::TypePtr) {};
			ToBePrinted(Var2* v_ptr) : var_ptr(v_ptr), print_type(PType::VarPtr) {};
		public:
			value_t int_ = 0;
			std::string str_ = "";
			Type type_ = {};
			TypeBase base_type_ = TypeBase::Empty;
		public:
			value_t* int_ptr = nullptr;
			std::string* str_ptr = nullptr;
			Type* type_ptr = nullptr;
			TypeBase* base_type_ptr = nullptr;
			Var2* var_ptr = nullptr;
			PType print_type = PType::None;
		}; // class ToBePrinted
	public:
		typedef ToBePrinted TBP;
	public:
		Type DeduceType() const {
			Type type = {};
			value_t scope = 0;
			DeduceType(&type, &scope);
			return type;
		};
		void DeduceType(Type* type_ptr, value_t* scope_ptr) const {
			// for new scope
			if (*scope_ptr == type_ptr->size()) {
				type_ptr->push_back(base_type);
			}
			// for Var scope
			if (type_ptr->at(*scope_ptr) == TypeBase::Var) {
				return;
			}
			// for unsimilar Type
			if (base_type != type_ptr->at(*scope_ptr)) {
				type_ptr->at(*scope_ptr) = TypeBase::Var;
				for (int i = 0; i < type_ptr->size() - *scope_ptr + 1; i++) {
					type_ptr->pop_back();
				}
				return;
			}
			// for not list
			if (base_type != TypeBase::List) {
				return;
			}
			// for list
			(*scope_ptr)++;
			for (const Var2& v : list) {
				v.DeduceType(type_ptr, scope_ptr);
			}
			(*scope_ptr)--;
		};
	private:	
		Var2* pointer = nullptr;
		std::vector<Var2> list = {};
		value_t value = 0;
		enum TypeBase base_type = TypeBase::Empty;
	};
	
	Var2 v2_ = Var2("jsjsjsjskskicifjen");
	std::string s = "";
	Var2::Type t = v2_.DeduceType();
	Var2::TBP::Printer({Var2::TBP(t)}).Print();
		
	return 0;
}
