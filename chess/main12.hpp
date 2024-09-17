#include <algorithm>
#include <array>
#include <atomic>
#include <concepts>
#include <cstdio>
#include <iterator>
#include <string>
#include <sys/cdefs.h>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <__functional/function.h>
#include "see.hpp"

#define s_assert(__location, __e) static_assert(__e, __STRING(__location: __e));
#define __location() __FILE_NAME__: __LINE__

template <class _Tp> using Ptr = _Tp*;
template <class _Tp> using Vec = std::vector<_Tp>;
using Str = std::string;
template <class _Tp, size_t _N> using Arr = std::array<_Tp, _N>;
template <class _Kp, class _Tp> using Map = std::map<_Kp, _Tp>;

template <class _Tp, int _N1, size_t _N2>
class _Arr2d {
	using type = Arr<Arr<_Tp, _N2>, _N1>;
};

template <class _Tp, size_t _N1, size_t _N2> using Arr2d = Arr<Arr<_Tp, _N2>, _N1>;
template <class _Tp> using Vec2d = Vec<Vec<_Tp>>;
template <class _Tp> using Fn = std::function<_Tp>;
template <class _Tp> using Transform = Fn<_Tp(_Tp)>;
template <class _Tp> using ConditionalTransform = Fn<Transform<_Tp>(_Tp)>;
template <class _Tp> 
class PipeResult {
public:
	_Tp object;
	bool is_success;
	PipeResult(const _Tp& __object, bool __is_success) : object(__object), is_success(__is_success) {}
	PipeResult operator|(Transform<PipeResult> __t) {
		return this->is_success ? __t(*this) : *this;
	}
};

template <class _Tp>
concept is_iterable_impl = requires(_Tp __t) {
	begin(__t) != end(__t);
	++std::declval<decltype(begin(__t))&>();
	*begin(__t);
};


#ifndef MAIN_11_HPP
#define MAIN_11_HPP


template <class _Tp>
concept is_pos = requires(_Tp& __t, _Tp::Limit& __t_limit) {
	{ __t.assert_satisfies(__t_limit) } -> std::convertible_to<void>;
};


class Pos2d {
public:
	class Limit;
	int row;
	int col;
	Pos2d(int __row, int __col) : row(__row), col(__col) {}
	void assert_satisfies(const Limit& __limit) const;
};

class Pos2d::Limit {
public:
	int nrows;
	int ncols;
	Limit(int __nrows, int __ncols) : nrows(__nrows), ncols(__ncols) {
		see_assert_greater(__nrows, 0);
		see_assert_greater(__ncols, 0);
	}
};

void Pos2d::assert_satisfies(const Limit& __limit) const {
	see_assert_in_index_range(this->row, __limit.nrows);
	see_assert_in_index_range(this->col, __limit.ncols);
}

template <class _CellData, class _Data, class _Pos>
class Board {
	s_assert(__location(), std::is_constructible_v<_Data>);
	s_assert(__location(), std::is_constructible_v<_CellData>);
	s_assert(__location(), is_pos<_Pos>);
public:
	using _PosLimit = typename _Pos::Limit;
	const _PosLimit pos_limit; 
	_Data data;
	virtual _CellData& __at(const _Pos& __pos) = 0;
	_CellData& at(const _Pos& __pos) {
		__pos.assert_satisfies(pos_limit);
		return __at(__pos);
	}
	Board(const _PosLimit& __pos_limit, const _Data& __data = _Data()) : pos_limit(__pos_limit), data(__data) {}
};

template <class _CellData, int _Nrows, int _Ncols,
	class _Data = Arr2d<_CellData, _Nrows, _Ncols>,
	class _Pos = Pos2d,
	class _Board = Board<_CellData, _Data, _Pos>>
class ArrBoard : public _Board {
	s_assert(__location(), _Nrows > 0);
	s_assert(__location(), _Ncols > 0);
public:
	using _PosLimit = typename _Pos::Limit;
	_CellData& __at(const _Pos& __pos) final {
		return _Board::data[__pos.row][__pos.col];
	}
	ArrBoard(const _CellData& __cell_data) : _Board(_PosLimit(_Nrows, _Ncols)) {
		std::fill_n(_Board::data[0].begin(), _Ncols, __cell_data);
		if (_Nrows > 1)
			std::fill_n(_Board::data.begin() + 1, _Nrows - 1, _Board::data[0]);
	}
	ArrBoard() : ArrBoard(_CellData()) {}
};

template <class _CellData,
	class _Data = Vec2d<_CellData>,
	class _Pos = Pos2d,
	class _Board = Board<_CellData, _Data, _Pos>>
class VecBoard : public _Board {
public:
	using _PosLimit = typename _Pos::Limit;
	_CellData& __at(const _Pos& __pos) final {
		return _Board::data[__pos.row][__pos.col];
	}
	VecBoard(const _PosLimit& __pos_limit, const _CellData& __cell_data) : _Board(__pos_limit, _Data(__pos_limit.nrows, Vec<_CellData>(__pos_limit.ncols, __cell_data))) {}
	VecBoard(const _PosLimit& __pos_limit) : VecBoard(__pos_limit, _CellData()) {}
};



class A {
public:
	//A() = delete;
};

#endif //MAIN_11_HPP


