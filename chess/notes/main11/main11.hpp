#include <algorithm>
#include <array>
#include <string>
#include <vector>
#include <map>
#include <__functional/function.h>
#include "see.hpp"


#define s_assert(e) static_assert(e, #e)


template <class Tp> using ptr = Tp*;
template <class Tp> using vec = std::vector<Tp>;
using str = std::string;
template <class Tp, size_t N> using arr = std::array<Tp, N>;
template <size_t N1, size_t N2, class Tp> using arr_2d = arr<arr<Tp, N2>, N1>;
template <class Tp> using fun = std::function<Tp>;
template <class Tp> using transform = fun<Tp(Tp)>;
template <class Tp> using conditional_transform = fun<transform<Tp>(Tp)>;
template <class Tp> 
class pipe_result {
public:
	Tp object;
	bool is_success;
	pipe_result(Tp __object, bool __is_success) : object(__object), is_success(__is_success) {}
	pipe_result operator|(transform<pipe_result> __t) {
		return this->is_success ? __t(*this) : *this;
	}
};
template <class Kp, class Tp> using map = std::map<Kp, Tp>;



#ifndef MAIN_10_HPP
#define MAIN_10_HPP


template <int __nrows, int __ncols, class __value_t>
class __board {
	s_assert(__nrows > 0);
	s_assert(__ncols > 0);
public:
	static const size_t nrows = __nrows;
	static const size_t ncols = __ncols;
	arr_2d<__nrows, __ncols, __value_t> data;
};


template <class __piece, class __pos>
class __move_data {
public:
	__piece piece;
	__pos pos;
	__move_data(__piece piece, __pos pos) : piece(piece), pos(pos) {};
};


template <class __piece, class __pos>
class __move {
public:
	vec<__move_data<__piece, __pos>> removed;
	vec<__move_data<__piece, __pos>> added;
};


template <int __value>
class __board_data {
	s_assert(__value >= 0);
public:
	size_t value = __value; 
};

using __board_data_0 = __board_data<0>;


class __pos {
public:
	int row;
	int col;
	__pos(int __row, int __col) : row(__row), col(__col) {};
	__pos() : __pos(0, 0) {};
};
template <class Tp>
using conditional_transform = fun<transform<Tp>(Tp)>;

using __attr = int;
using __attrs = vec<__attr>;

class __piece {
public:
	__attr name;
	__attrs attrs;
	__piece() : name(-1), attrs({}) {}
	__piece(__attr __name, __attrs attrs = {}) : name(__name), attrs(attrs) {}
};

__pos& operator+=(__pos& __p1, __pos __p2) {
	__p1.row += __p2.row;
	__p1.col += __p2.col;
	return __p1;
}

__pos& operator-=(__pos& __p1, __pos __p2) {
	return __p1 += __pos(-__p2.row, -__p2.col);
}

__pos operator+(__pos __p1, __pos __p2) {
	return __p1 += __p2;
}

__pos operator-(__pos __p1, __pos __p2) {
	return __p1 -= __p2;
}

enum class __north {
	north,
	east,
	south,
	west,
};

__pos operator*(__pos __p, __north __n) {
	switch (__n) {
	case __north::north: return __p;
	case __north::east: return __pos(__p.col, -__p.row);
	case __north::west: return __pos(-__p.col, __p.row);
	case __north::south: return __pos() - __p;
	}
}

__north operator*(__north __n1, __north __n2) {
	return __north((int(__n1) + int(__n2)) % 4);
}

__north& operator*=(__north& __n1, __north __n2) {
	return __n1 = __n1 * __n2;
}

template <class __move_data>
class __replace_move_data {
public:
	__move_data removed;
	__move_data added;
	__replace_move_data(__move_data __removed, __move_data __added) : removed(__removed), added(__added) {}
};


template <int __nrows, int __ncols>
class __board_ptr {
	using __board = __board<__nrows, __ncols, __piece>;
	using __move_data = __move_data<__piece, __pos>;
	using __replace_move_data = __replace_move_data<__move_data>;
public:
	ptr<__board> __the_board;
	__pos pos;
	__north north;
	__piece& piece() const {
		return this->__the_board->data[this->pos.row][this->pos.col];
	}
	__move_data remove() const {
		__move_data __m_data = __move_data(this->piece(), this->pos);
		this->piece() = __piece();
		return	__m_data;
	}
	__move_data add(__piece __new_p) const {
		this->piece() = __new_p;
		return __move_data(__new_p, this->pos);
	}
	__replace_move_data replace(__piece __new_p) const {
		return __replace_move_data(this->remove(), this->add(__new_p));
	}
	void remove_attrs(__attrs attrs) const {
		for (__attr& __a : attrs) {
			auto __it = std::find(this->piece().attrs.begin(), this->piece().attrs.end(), __a);
			this->piece().attrs.erase(__it);
		}
	}
	void add_attrs(__attrs attrs) const {
		for (__attr __a : attrs)
			this->piece().attrs.push_back(__a);
	}
	void set_attrs(__attrs attrs) const {
		this->piece().attrs = attrs;
	}
	bool has_all_of(__attrs attrs) const {
		__attrs __this_attrs = this->piece().attrs;
		for (__attr __a : attrs) {
			auto it = std::find(__this_attrs.begin(), __this_attrs.end(), __a);
			if (it == __this_attrs.cend()) return false;
			__this_attrs.erase(it);
		}
		return true;	
	}
	bool lacks_any_of(__attrs attrs) const {
		return !this->has_all_of(attrs);
	}
	bool has_none_of(__attrs attrs) const {
		__attrs __this_attrs = this->piece().attrs;
		for (__attr __a : attrs) {
			auto it = std::find(__this_attrs.begin(), __this_attrs.end(), __a);
			if (it != __this_attrs.cend()) return false;
			__this_attrs.erase(it);
		}
		return true;
	}
	bool has_any_of(__attrs attrs) const {
		return !this->has_none_of(attrs);
	}
	bool is_named(__attr __name) const {
		return __name == this->piece().name;
	}
	bool is_equal_to(__piece __p) const {
		return this->is_named(__p.name) && this->has_attrs(__p.attrs) && this->piece().attrs.size() == __p.attrs.size();
	}
	__replace_move_data edit(__attrs __to_be_removed, __attrs __to_be_added) const {
		__move_data __rm_data = __move_data(this->piece(), this->pos);
		this->remove_attrs(__to_be_removed);
		this->add_attrs(__to_be_added);
		return __replace_move_data(__rm_data, __move_data(this->piece(), this->pos));		
	}
	bool is_in_board() const {
		return 
			this->pos.row >= 0 &&
			this->pos.row < this->__the_board->nrows &&
			this->pos.col >= 0 &&
			this->pos.col < this->__the_board->ncols;
	}
	__pos set(__pos __p) {
		__pos __then_p = this->pos;
		this->pos = __p;
		if (!this->is_in_board())
			this->pos = __then_p;
		return this->pos;
	}
	__pos move(__pos __p) {
		return set(this->pos + __p * this->north);
	}
	__north face(__north __n) {
		return this->north = __n;
	}
	__north turn(__north __n) {
		return this->north *= __n;
	}
	void remember() {}
	void forget() {}
	void take() {}
	void give() {}
	void reset() {}
	__board_ptr(ptr<__board> __the_b, __pos __p = __pos()) : __the_board(__the_b), pos(__p) {}
	__board_ptr() : __the_board(nullptr), pos(__pos()) {}
};


template <int __nrows, int __ncols>
class __board_ptr_result : public pipe_result<__board_ptr<__nrows, __ncols>> {
	using __move_data = __move_data<__piece, __pos>;
	using __replace_move_data = __replace_move_data<__move_data>;
	using __moves = vec<const __replace_move_data>;
	using __board_ptr = __board_ptr<__nrows, __ncols>;
	using __pipe_result = pipe_result<__board_ptr>;
public:
	__moves moves;
	__board_ptr_result(__board_ptr __bp, bool __is_success) : __pipe_result(__bp, __is_success) {};
	__board_ptr_result operator|(transform<__board_ptr_result> __t) {
		return this->is_success ? __t(*this) : *this;
	}
};

template <int __nrows, int __ncols>
class __move_gen {
	using __move_data = __move_data<__piece, __pos>;
	using __replace_move_data = __replace_move_data<__move_data>;
	using __moves = vec<const __replace_move_data>;
	using __moves_map = map<__pos, __moves>;
	using __board_ptr = __board_ptr<__nrows, __ncols>;
	using __norths = vec<__north>;
	using __board_ptr_transforms = vec<transform<__board_ptr>>;
public:
	__board_ptr board_ptr;
	__board_ptr_transforms board_ptr_transforms;
};

template <int __nrows, int __ncols>
class __game {
	using __board = __board<__nrows, __ncols, __piece>;
	using __moves = vec<const __move<__piece, __pos>>;
	using __board_ptr = __board_ptr<__nrows, __ncols>;
public:
	__board board;
	__moves moves;
	__board_ptr board_ptr;
	size_t nmoves() const {return this->moves.size();}
	__game() : board(__board()), moves({}) {
		this->board_ptr = __board_ptr(&board);
	}
};
using default_game = __game<8, 8>;

int main2() {
	using bp = __board_ptr<8, 8>;
	using bpp_res = __board_ptr_result<8, 8>;
	using bp_t = transform<bpp_res>;
	bp_t f = [](bpp_res __r) -> bpp_res { return __r; };
	bpp_res(bp(), true) | f;
	return 0 ;
}
	

#endif
