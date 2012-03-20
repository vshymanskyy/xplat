#ifndef INCLUDED_XPLAT_H
#define INCLUDED_XPLAT_H

/* http://sourceforge.net/apps/mediawiki/predef/index.php */

/*********************************************************
 * HELPERS
 *********************************************************/
#define X_CONCAT(x,y) x ## y					/*  */
#define X_STRINGIFY(x) #x						/*  */
#define X_STRINGIFY_DEF(x) X_STRINGIFY(x)		/* Makes string from value of define */

#define BIT(val, i) ((size_t)(*(val)) & ((size_t)1 << (i)))
#define OFFSETOF(type, member) ((size_t)&(((type*)0)->member))
#define COUNTOF(arr) (sizeof(arr)/sizeof(arr[0]))

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(a) (((a) < 0) ? -(a) : (a))

/*********************************************************
 * COMPILER DETECTION
 *********************************************************/
#if defined(__GNUC__)
	#define COMPILER_GCC
	#define COMPILER_STR "GCC"
	#define COMPILER_VER_STR __VERSION__
	#define GCC_SPECIFIC(expr) expr
#elif defined(__CYGWIN__)
	#define COMPILER_CYGWIN
	#define COMPILER_STR "CYGWIN"
#elif defined(__MINGW32__)
	#define COMPILER_MINGW
	#define COMPILER_STR "MinGW"
	#define COMPILER_VER (__MINGW32_MAJOR_VERSION * 100 + __MINGW32_MINOR_VERSION) /* VVRR */
#elif defined(__TURBOC__)
	#define COMPILER_TURBO_C
	#define COMPILER_STR "Turbo C"
#elif defined(__BORLANDC__)
	#define COMPILER_BORLAND_CPP
	#define COMPILER_STR "Borland C++"
	#define COMPILER_VER __BORLANDC__
#elif defined(__INTEL_COMPILER)
	#define COMPILER_ICC
	#define COMPILER_STR "ICC"
	#define COMPILER_VER __INTEL_COMPILER /* VRP */
	#define ICC_SPECIFIC(expr) expr
#elif defined(_MSC_VER)
	#define COMPILER_MSVC
	#define COMPILER_STR "MSVC"
	#define COMPILER_VER _MSC_VER /* VVRR */
	#define COMPILER_FULL_VER _MSC_FULL_VER /* VVRRPPPP or VVRRPPPPP */
	#define MSVC_SPECIFIC(expr) expr
#elif defined(__TINYC__)
	#define COMPILER_TINY_C
	#define COMPILER_STR "Tiny C"
#else
	#define COMPILER_UNKNOWN
#endif

/* Fill unknown compiler data */
#ifndef COMPILER_VER
	#define COMPILER_VER 0
#endif
#ifndef COMPILER_FULL_VER
	#define COMPILER_FULL_VER COMPILER_VER
#endif
#ifndef COMPILER_VER_STR
	#if COMPILER_VER == 0
		#define COMPILER_VER_STR "[unknown]"
	#else
		#define COMPILER_VER_STR PREPROC_STRINGIFY_DEF(COMPILER_VER)
	#endif
#endif
#ifndef COMPILER_FULL_VER_STR
	#if COMPILER_FULL_VER == 0
		#define COMPILER_FULL_VER_STR COMPILER_VER_STR
	#else
		#define COMPILER_FULL_VER_STR PREPROC_STRINGIFY_DEF(COMPILER_FULL_VER)
	#endif
#endif

#ifndef GCC_SPECIFIC
#	define GCC_SPECIFIC()
#endif
#ifndef MSVC_SPECIFIC
#	define MSVC_SPECIFIC()
#endif
#ifndef ICC_SPECIFIC
#	define ICC_SPECIFIC()
#endif

/*********************************************************
 * TARGET PLATFORM
 *********************************************************/
#if defined(COMPILER_GCC) || defined(COMPILER_ICC)
	#if defined(__i386__)
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I386
	#elif defined(__i486__)
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I486
	#elif defined(__i586__)
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I586
	#elif defined(__i686__)
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I686
	#elif defined(__amd64__)
		#define TARGET_ARCH_X86_64
	#elif defined(__ia64__)
		#define TARGET_ARCH_IA64
	#elif defined(__powerpc__)
		#define TARGET_ARCH_PPC
	#elif defined(__sparc__)
		#define TARGET_ARCH_SPARC
	#elif defined(__mips__)
		#define TARGET_ARCH_MIPS
	#elif defined(__arm__)
		#define TARGET_ARCH_ARM
	#elif defined(__thumb__)
		#define TARGET_ARCH_ARM_THUMB
	#elif defined(__AVR__)
		#define TARGET_ARCH_AVR
	#endif
#elif defined(COMPILER_MSVC)
	#if defined(_M_IX86) && _M_IX86 == 300
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I386
	#elif defined(_M_IX86) && _M_IX86 == 400
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I486
	#elif defined(_M_IX86) && _M_IX86 == 500
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I586
	#elif defined(_M_IX86) && _M_IX86 == 600
		#define TARGET_ARCH_X86
		#define TARGET_ARCH_VER_I686
	#elif defined(_M_X64)
		#define TARGET_ARCH_X86_64
	#elif defined(_M_IA64)
		#define TARGET_ARCH_IA64
	#elif defined(_M_PPC)
		#define TARGET_ARCH_PPC
	#elif defined(_M_MRX000)
		#define TARGET_ARCH_MIPS
	#elif defined(_M_ARM)
		#define TARGET_ARCH_ARM
	#endif
#endif

/* Fill target arch data */
#if defined(TARGET_ARCH_X86)
	#define TARGET_ARCH_STR "x86"
#elif defined(TARGET_ARCH_X86_64)
	#define TARGET_ARCH_STR "x86-64"
#elif defined(TARGET_ARCH_IA64)
	#define TARGET_ARCH_STR "ia64"
#elif defined(TARGET_ARCH_PPC)
	#define TARGET_ARCH_STR "PowerPC"
#elif defined(TARGET_ARCH_SPARC)
	#define TARGET_ARCH_STR "SPARC"
#elif defined(TARGET_ARCH_MIPS)
	#define TARGET_ARCH_STR "MIPS"
#elif defined(TARGET_ARCH_ARM)
	#define TARGET_ARCH_STR "ARM"
#elif defined(TARGET_ARCH_ARM_THUMB)
	#define TARGET_ARCH_STR "ARM-Thumb"
#elif defined(TARGET_ARCH_AVR)
	#define TARGET_ARCH_STR "AVR"
#else
	#define TARGET_ARCH_UNKNOWN
	#define TARGET_ARCH_STR "[unknown]"
#endif

/*********************************************************
 * TARGET OS DETECTION
 *********************************************************/
#if   defined (__unix__) && defined (ANDROID)
	#define TARGET_OS_UNIX
	#define TARGET_OS_TYPE_ANDROID
	#define TARGET_OS_STR "Android"
#elif defined (__unix__) && defined (__linux__)
	#define TARGET_OS_UNIX
	#define TARGET_OS_TYPE_LINUX
	#define TARGET_OS_STR "Linux"
#elif defined (__unix__) && defined (__OpenBSD__)
	#define TARGET_OS_UNIX
	#define TARGET_OS_TYPE_OPEN_BSD
	#define TARGET_OS_STR "OpenBSD"
#elif defined (__unix__) && defined (__NetBSD__)
	#define TARGET_OS_UNIX
	#define TARGET_OS_TYPE_NET_BSD
	#define TARGET_OS_STR "NetBSD"
#elif defined (__minix)
	#define TARGET_OS_MINIX
	#define TARGET_OS_TYPE_MINIX
	#define TARGET_OS_STR "MINIX"
#elif defined (__unix__)
	#define TARGET_OS_UNIX
	#define TARGET_OS_TYPE_UNIX
	#define TARGET_OS_STR "Unix"
#elif defined (_WIN32) && defined (_WIN32_WCE)
	#define TARGET_OS_WINDOWS
	#define TARGET_OS_TYPE_WINCE
	#define TARGET_OS_STR "WinCE"
#elif defined (_WIN32) && defined (_WIN64)
	#define TARGET_OS_WINDOWS
	#define TARGET_OS_TYPE_WIN64
	#define TARGET_OS_STR "Win64"
#elif defined (_WIN32)
	#define TARGET_OS_WINDOWS
	#define TARGET_OS_TYPE_WIN32
	#define TARGET_OS_STR "Win32"
#elif defined (MSDOS) || defined (__MSDOS__) || defined (_MSDOS) || defined (__DOS__)
	#define TARGET_OS_MSDOS
	#define TARGET_OS_TYPE_MSDOS
	#define TARGET_OS_STR "MS-DOS"
#elif defined (macintosh) || defined (Macintosh)
	#define TARGET_OS_MAC
	#define TARGET_OS_TYPE_MAC_OS9
	#define TARGET_OS_STR "Mac OS 9"
#elif defined (__APPLE__) && defined (__MACH__)
	#define TARGET_OS_MAC
	#define TARGET_OS_TYPE_MAC_OSX
	#define TARGET_OS_STR "Mac OS X"
#elif defined (__BEOS__)
	#define TARGET_OS_BEOS
	#define TARGET_OS_TYPE_BEOS
	#define TARGET_OS_STR "BeOS"
#elif defined (__SYMBIAN32__)
	#define TARGET_OS_SYMBIAN
	#define TARGET_OS_TYPE_SYMBIAN
	#define TARGET_OS_STR "Symbian OS"
#elif defined (__nucleus__)
	#define TARGET_OS_NUCLEUS
	#define TARGET_OS_TYPE_NUCLEUS
	#define TARGET_OS_STR "Nucleus RTOS"
#elif defined (__ECOS)
	#define TARGET_OS_ECOS
	#define TARGET_OS_TYPE_ECOS
	#define TARGET_OS_STR "eCos"
#else
	#define TARGET_OS_UNKNOWN
	#define TARGET_OS_STR "[unknown]"
#endif

/*********************************************************
 * CAPABILITIES
 *********************************************************/
#if defined(COMPILER_GCC) && defined(__EXCEPTIONS)
	#define X_CAPS_EXCEPTIONS
#elif defined(COMPILER_MSVC) && defined(_CPPUNWIND)
	#define X_CAPS_EXCEPTIONS
#endif

#if defined(COMPILER_GCC) && defined(__GXX_RTTI)
	#define X_CAPS_RTTI
#elif defined(COMPILER_MSVC) && defined(_CPPRTTI)
	#define X_CAPS_RTTI
#endif

#if defined(COMPILER_GCC) && defined(__cplusplus)
	#define X_CAPS_CPLUSPLUS
#elif defined(COMPILER_MSVC) && defined(__cplusplus)
	#define X_CAPS_CPLUSPLUS
#endif

#if defined(COMPILER_GCC) && defined(__GXX_EXPERIMENTAL_CXX0X__)
	#define CAPS_CXX0X
#endif

#if defined(COMPILER_GCC) && defined(OPENMP)
	#define X_CAPS_OPENMP
#elif defined(COMPILER_MSVC) && defined(_OPENMP)
	#define X_CAPS_OPENMP
#endif

#if defined(COMPILER_MSVC) && defined(__MSVC_RUNTIME_CHECKS)
	#define X_CAPS_MSVC_RUNTIME_CHECKS
#endif

#if defined(COMPILER_MSVC) && defined(_MT)
	#define X_CAPS_MSVC_MULTITHREADED_LIBS
#endif

#if defined(COMPILER_GCC) || defined(COMPILER_ICC)
	#define X_CAPS_VARIADIC_MACROS_BY_NAME
#elif defined(COMPILER_MSVC)
	#define X_CAPS_VARIADIC_MACROS_VA_ARGS
#else
	#error "Variadic macros type not detected"
#endif


#if defined(COMPILER_GCC) || defined(COMPILER_ICC)
	#define CAPS_INLINE_ASM_AT_T
#elif defined(COMPILER_MSVC)
	#define CAPS_INLINE_ASM_INTEL
#endif


/*********************************************************
 * Standards
 *********************************************************/
#if defined(__STDC__)
	#define STANDARD_C_1989
	#if defined(__STDC_VERSION__)
		#define STANDARD_C_1990
		#if (__STDC_VERSION__ >= 199409L)
			#define STANDARD_C_1994
		#endif
		#if (__STDC_VERSION__ >= 199901L)
			#define STANDARD_C_1999
		#endif
	#endif
#endif

#if defined(TARGET_OS_UNIX)
	#include <unistd.h>
	#if defined(_XOPEN_VERSION)
		#if (_XOPEN_VERSION >= 3)
			#define STANDARD_XOPEN_1989
		#endif
		#if (_XOPEN_VERSION >= 4)
			#define STANDARD_XOPEN_1992
		#endif
		#if (_XOPEN_VERSION >= 4) && defined(_XOPEN_UNIX)
			#define STANDARD_XOPEN_1995
		#endif
		#if (_XOPEN_VERSION >= 500)
			#define STANDARD_XOPEN_1998
		#endif
		#if (_XOPEN_VERSION >= 600)
			#define STANDARD_XOPEN_2003
		#endif
		#if (_XOPEN_VERSION >= 700)
			#define STANDARD_XOPEN_2008
		#endif
	#endif
#endif


#if defined(COMPILER_MSVC)

	/* types */
	#include <stdint.h>
	typedef intptr_t ssize_t;

	#define X_FUNC_NAME __FUNCTION__
	#define X_FUNC_DECL __FUNCDNAME__
	#define X_FUNC_SIGN __FUNCSIG__

	#define X_ENDIAN_LITTLE /* TODO: detect */

	/* function modifiers */
	#define FUNC_FORCE_INLINE __forceinline
	#define FUNC_NOINLINE __declspec(noinline)
	#define FUNC_NORETURN __declspec(noreturn)
	#define FUNC_NAKED __declspec(naked)
	#define FUNC_WARN_UNUSED_RESULT

	/* packing */
	#define PACKED
	#define PACKED_REGION_BEGIN __pragma(pack(push, 1))
	#define PACKED_REGION_END __pragma(pack(pop))

	/* branch prediction */
	#define X_LIKELY(x)       (x)
	#define X_UNLIKELY(x)     (x)

	#define strcasecmp stricmp
	#define snprintf _snprintf
	#define vsnprintf _vsnprintf

#elif defined(COMPILER_GCC) | defined (COMPILER_ICC)

	/* types */
	#include <stdint.h>
	#include <endian.h>

	#define X_FUNC_NAME __func__
	#define X_FUNC_DECL __FUNCTION__
	#define X_FUNC_SIGN __PRETTY_FUNCTION__

#if defined (LITTLE_ENDIAN)
#	define X_ENDIAN_LITTLE
#elif defined (BIG_ENDIAN)
#	define X_ENDIAN_BIG
#endif

	/* function modifiers */
	#define FUNC_FORCE_INLINE __attribute__((always_inline))
	#define FUNC_NOINLINE __declspec(noinline)
	#define FUNC_NORETURN __declspec(noreturn)
	#define FUNC_NAKED __declspec(naked)
	#define FUNC_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

	/* packing */
	#define PACKED __attribute__((packed))
	#define PACKED_REGION_BEGIN __pragma(pack(push, 1))
	#define PACKED_REGION_END __pragma(pack(pop))

	/* branch prediction */
	#define X_LIKELY(x)       __builtin_expect((x),1)
	#define X_UNLIKELY(x)     __builtin_expect((x),0)

	#define stricmp strcasecmp
	#define _snprintf snprintf
	#define _vsnprintf vsnprintf

#else
#	error "Platform not supported"
#endif

#endif /* INCLUDED_XPLAT_PLATFORM_H */