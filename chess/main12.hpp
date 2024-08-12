#include <algorithm>
#include <array>
#include <atomic>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <map>
#include <__functional/function.h>
#include "see.hpp"
#define s_assert(e) static_assert(e, #e)


template <class _Tp> using Ptr = _Tp*;
template <class _Tp> using Vec = std::vector<_Tp>;
using Str = std::string;
template <class _Tp, size_t _N> using Arr = std::array<_Tp, _N>;
template <class _Kp, class _Tp> using Map = std::map<_Kp, _Tp>;
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

template <class _CellData, class _Data>
class Board {
public:
	const int nrows;
	const int ncols;
	_Data data;
	virtual _CellData& at(int __row, int __col) = 0;
	Board(int __nrows, int __ncols, const _Data& __data) : nrows(__nrows), ncols(__ncols), data(__data) {}
	Board(int __nrows, int __ncols) : Board(__nrows, __ncols, _Data()) {
		s_assert(std::is_constructible_v<_Data>);
	}
};

template <class _CellData, int _Nrows, int _Ncols,
	class _Data = Arr2d<_CellData, _Nrows, _Ncols>,
	class _Board = Board<_CellData, _Data>>
class ArrBoard : public _Board {
	s_assert(_Nrows > 0 && _Ncols > 0);
public:
	virtual _CellData& at(int __row, int __col) final {
		see_assert_in_index_range(__row, _Board::nrows);
		see_assert_in_index_range(__col, _Board::ncols);
		return _Board::data[__row][__col];
	}
	ArrBoard(const _CellData& __cell_data) {
		_Data __data;
		std::fill_n(__data[0].begin(), _Ncols, __cell_data);
		if (_Ncols > 1)
			std::fill_n(__data.begin() + 1, _Nrows - 1, _Board::data[0]);
		*this = _Board(_Nrows, _Ncols, __data);
	}
	ArrBoard() {
		s_assert(std::is_constructible_v<_CellData>);
		*this = ArrBoard(_CellData());
	}
};

template <class _CellData,
	class _Data = Vec2d<_CellData>,
	class _Board = Board<_CellData, _Data>>
class VecBoard : public _Board {
public:
	virtual _CellData& at(int __row, int __col) final {
		see_assert_in_index_range(__row, _Board::nrows);
		see_assert_in_index_range(__col, _Board::ncols);
		return _Board::data[__row][__col];
	}
	VecBoard(int __nrows, int __ncols, const _CellData& __cell_data) {
		see_assert_greater(__nrows, 0);
		see_assert_greater(__ncols, 0);
		*this = _Board(__nrows, __ncols, _Data(__nrows, Vec<_CellData>(__ncols, __cell_data)));
	}
	VecBoard(int __nrows, int __ncols) {
		s_assert(std::is_constructible_v<_CellData>);
		see_assert_greater(__nrows, 0);
		see_assert_greater(__ncols, 0); 
		*this = VecBoard(__nrows, __ncols, _CellData());
	}
};

template <class _CellData>
VecBoard::VecBoard(int __nrows, int __ncols) {

}


class A {
public:
	A() = delete;
};

#endif //MAIN_11_HPP

