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

#endif // SEE_HPP
