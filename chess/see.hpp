#undef NDEBUG
#include <cassert> 
#include <iostream>

#ifndef SEE_HPP
#define SEE_HPP

static size_t see_counter = 0;
#define __see(v) "[" << #v << " = " << v << "]"
#define __newlined(s) s << "\n"
#define __cout(s) std::cout << s
#define __println(s) __cout(__newlined(s))

#define see(v) ++see_counter; __println(__see(see_counter) << __see(v));
#define seeifnot(e, v) if (!e) { __println(__see(e) << ", " << __see(v)); } assert(e);

#endif // SEE_HPP
