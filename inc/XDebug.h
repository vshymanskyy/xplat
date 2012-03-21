#ifndef INC_XDEBUG_HPP
#define INC_XDEBUG_HPP

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <XPlat.h>

#if defined(WIN32)
	#define XDEBUG_BREAK() { DebugBreak(); }
#else
	#define XDEBUG_BREAK() { *(char*)(NULL) = 0xFF; } // SEGV!!!
#endif

#define XDEBUG_PRINT_FAIL(...) // TODO

#define XASSERT(expr) { if (!(expr)) { XDEBUG_PRINT_FAIL("Assertion '" #expr "' failed at %s:%d", CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_EQ(expr, val, fmt) { if (!((expr) == (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " == " #val "' failed ( " fmt " != " fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_NE(expr, val, fmt) { if (!((expr) != (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " != " #val "' failed ( " fmt " == " fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_GT(expr, val, fmt) { if (!((expr) >  (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " > "  #val "' failed ( " fmt " <= " fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_GE(expr, val, fmt) { if (!((expr) >= (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " >= " #val "' failed ( " fmt " < "  fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_LT(expr, val, fmt) { if (!((expr) <  (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " < "  #val "' failed ( " fmt " >= " fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }
#define XASSERT_LE(expr, val, fmt) { if (!((expr) <= (val))) { XDEBUG_PRINT_FAIL("Assertion '" #expr " <= " #val "' failed ( " fmt " > "  fmt " ) at %s:%d", (expr), (val), CURR_FILE, CURR_LINE); XDEBUG_BREAK(); } }


#endif // INC_XDEBUG_HPP
