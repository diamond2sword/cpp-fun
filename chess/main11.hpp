#include <array>
#include <string>
#include <vector>
#include "see.hpp"


#define s_assert(e) static_assert(e, #e)


template <class Tp> using ptr = Tp*;
template <class Tp> using vec = std::vector<Tp>;
using str = std::string;
template <class Tp, size_t N> using arr = std::array<Tp, N>;
template <size_t N1, size_t N2, class Tp> using arr_2d = arr<arr<Tp, N2>, N1>;


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


template <class __value_t>
class __default_board : public __board<8, 8, __value_t> {};


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

using __attr = int;
using __attrs = vec<__attr>;

class __piece {
public:
	__attr name;
	__attrs attrs;
	__piece() : name(-1), attrs({}) {}
	void remove(__attrs attrs) {
		for (__attr& __a : attrs)
			std::erase(this->attrs, __a);
	}
	void add(__attrs attrs) {
		for (__attr& __a : attrs) {
			auto it = std::find(this->attrs.begin(), this->attrs.end(), __a);
			if (it == this->attrs.end())
				this->attrs.push_back(__a);
		}
	}
	__piece(__attr __name, __attrs attrs = {}) : name(__name), attrs(attrs) {}
};



template <int __nrows, int __ncols>
class __game {
	using __piece = __piece;
	using __board = __board<__nrows, __ncols, __piece>;
	using __moves = vec<const __move<__piece, __pos>>;
public:
	__board board = {};
	__moves moves = {};
	size_t nmoves() const {return moves.size();};
};
using default_game = __game<8, 8>;

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

template <class __move_data>
class __replace_move_data {
public:
	__move_data removed;
	__move_data added;
	__replace_move_data(__move_data __removed, __move_data __added) : removed(__removed), added(__added) {}
};


template <int __nrows, int __ncols>
class __board_ptr : public __board<__nrows, __ncols, __piece> {
	using __board = __board<__nrows, __ncols, __piece>;
	using __move_data = __move_data<__piece, __pos>;
	using __replace_move_data = __replace_move_data<__move_data>;
public:
	ptr<__board> __the_board;
	__pos pos;
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
		return set(this->pos + __p);
	}
	__piece& piece() const {
		return __the_board->data[this->pos.row][this->pos.col];
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
	__replace_move_data edit(__attrs __to_be_removed, __attrs __to_be_added) const {
		__move_data __rm_data = __move_data(this->piece(), this->pos);
		this->piece().remove(__to_be_removed);
		this->piece().add(__to_be_added);
		return __replace_move_data(__rm_data, __move_data(this->piece(), this->pos));		
	}
	__board_ptr(ptr<__board> __the_b, __pos __p = __pos()) : __the_board(__the_b), pos(__p) {};
};


#endif
