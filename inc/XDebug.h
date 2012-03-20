#ifndef INC_BACKTRACE_HPP
#define INC_BACKTRACE_HPP

#if _MSC_VER > 1000
	#pragma once
#endif

#ifndef __cplusplus
#error "This C++ header was included in C file"
#endif

#include <stdlib.h>
#include <stdio.h>

#if defined(WIN32)
	#define DEBUG_BREAK(msg) { fprintf(stderr, "Debug break: '%s'\n", msg); DebugBreak(); }
#elif defined (__GNUC__)
	#define DEBUG_BREAK(msg) { fprintf(stderr, "Debug break: '%s'\n", msg); *(char*)(NULL) = 0xFF; }	// SEGV!!!
#else
	#define DEBUG_BREAK(msg) { fprintf(stderr, "Debug break: '%s' <skipped>\n", msg); }
#endif

#if defined(__GLIBC__)
	#define HAS_BACKTRACE
#else
	#undef HAS_BACKTRACE
#endif

#ifdef HAS_BACKTRACE

int backtrace_save(void** addrs, int maxlen);

size_t backtrace_print(char* buff, size_t maxlen, void* const* addrs, int skip);

#endif

#endif // INC_BACKTRACE_HPP
