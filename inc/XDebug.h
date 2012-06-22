#ifndef INC_XDEBUG_H
#define INC_XDEBUG_H

#include <XPlat.h>

#include <stdio.h>


#if defined DEBUG || defined _DEBUG
#define X_DBG(expr) expr
#else
#define X_DBG(expr)
#endif

#if defined (TARGET_OS_WINDOWS)
	#include <Windows.h>
	#define X_DBG_BREAK() DebugBreak();
#elif defined (TARGET_OS_UNIX)
	#include <signal.h>
	#define X_DBG_BREAK() raise(SIGTRAP);
#else
	#define X_DBG_BREAK() *(char*)(NULL) = 0xFF; // SEGV!!!
#endif



#define X_DEBUG_PRINT_FAIL(...) { fprintf(stderr, __VA_ARGS__); X_DBG_BREAK(); }

#define X_FATAL(msg) { X_DEBUG_PRINT_FAIL("Fatal: " #msg " at %s:%d", __FILE__, __LINE__); }

#define X_ASSERT(expr) { if (!(expr)) { X_DEBUG_PRINT_FAIL("Assertion '" #expr "' failed at %s:%d", __FILE__, __LINE__); } }
#define X_ASSERT_EQ(expr, val, fmt) { if (!((expr) == (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " == " #val "' failed ( " fmt " != " fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }
#define X_ASSERT_NE(expr, val, fmt) { if (!((expr) != (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " != " #val "' failed ( " fmt " == " fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }
#define X_ASSERT_GT(expr, val, fmt) { if (!((expr) >  (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " > "  #val "' failed ( " fmt " <= " fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }
#define X_ASSERT_GE(expr, val, fmt) { if (!((expr) >= (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " >= " #val "' failed ( " fmt " < "  fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }
#define X_ASSERT_LT(expr, val, fmt) { if (!((expr) <  (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " < "  #val "' failed ( " fmt " >= " fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }
#define X_ASSERT_LE(expr, val, fmt) { if (!((expr) <= (val))) { X_DEBUG_PRINT_FAIL("Assertion '" #expr " <= " #val "' failed ( " fmt " > "  fmt " ) at %s:%d", (expr), (val), __FILE__, __LINE__); } }

#endif // INC_XDEBUG_H
