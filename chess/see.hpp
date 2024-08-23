#include <regex>
#undef NDEBUG
#include <cassert> 
#include <iostream>

#ifndef SEE_HPP
#define SEE_HPP

static size_t see_counter = 0;
#define __(s) << s
#define __see(v) __(#v)__(" = ")__(v)
#define __newlined(__s) __s __("\n")
#define __cout(__s) std::cout __s
#define __println(__s) __cout(__newlined(__s))

#define see(v) __println(__("[")__(++see_counter)__("] see: ")__see(v));
#define seeifnot(e, v) if (!e) { __println(__see(e)__(", ")__see(v)); } assert(e);

#define see_assert(e, v1, v2) if (!(e)) {see(v1); see(v2); assert(e);}
#	define see_assert_neq(v1, v2) see_assert(v1 != v2, v1, v2);
#	define see_assert_eq(v1, v2) see_assert(v1 == v2, v1, v2);
#	define see_assert_greater(v1, v2) see_assert(v1 > v2, v1, v2); 
#	define see_assert_lesser(v1, v2) see_assert(v1 < v2, v1, v2); 
#	define see_assert_lesserequal(v1, v2) see_assert(v1 <= v2, v1, v2); 
#	define see_assert_greaterequal(v1, v2) see_assert(v1 >= v2, v1, v2);

#define see_assert_in_range(n, begin_n, end_n) \
	see_assert_greaterequal(n, begin_n); \
	see_assert_lesser(n, end_n);
#define see_assert_in_index_range(n, end_n) see_assert_in_range(n, 0, end_n);


#endif // SEE_HPP
