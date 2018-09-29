//
// SVTypes.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef SV_CORE_TYPES_H
#define SV_CORE_TYPES_H

#include "SVCompileConfig.h"

//! 8 bit unsigned variable.
/** This is a typedef for unsigned char, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int8		u8;
#else
typedef unsigned char u8;
#endif

//! 8 bit signed variable.
/** This is a typedef for signed char, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int8			s8;
#else
typedef signed char s8;
#endif

//! 8 bit character variable.
/** This is a typedef for char, it ensures portability of the engine. */
typedef char c8;

typedef char* ptr8;

typedef unsigned char* ptru8;

typedef const char* cptr8;

//! 16 bit unsigned variable.
/** This is a typedef for unsigned short, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int16	u16;
#else
typedef unsigned short u16;
#endif

//! 16 bit signed variable.
/** This is a typedef for signed short, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int16			s16;
#else
typedef signed short s16;
#endif



//! 32 bit unsigned variable.
/** This is a typedef for unsigned int, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int32	u32;
#else
typedef unsigned int u32;
#endif

//! 32 bit signed variable.
/** This is a typedef for signed int, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int32			s32;
#else
typedef signed int s32;
#endif


#ifdef _SV_HAS_S64
//! 64 bit unsigned variable.
/** This is a typedef for 64bit uint, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int64			u64;
#elif __GNUC__
#if __WORDSIZE == 64
typedef unsigned long int			u64;
#else
__extension__ typedef unsigned long long u64;
#endif
#else
typedef unsigned long long			u64;
#endif

//! 64 bit signed variable.
/** This is a typedef for 64bit int, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int64					s64;
#elif __GNUC__
#if __WORDSIZE == 64
typedef long int				s64;
#else
__extension__ typedef long long s64;
#endif
#else
typedef long long				s64;
#endif
#endif	// _SV_HAS_S64

//! 32 bit floating point variable.
/** This is a typedef for float, it ensures portability of the engine. */
typedef float f32;

//! 64 bit floating point variable.
/** This is a typedef for double, it ensures portability of the engine. */
typedef double f64;


//NSSV_E

/////////////////////////////////////////////////////////////

#include <wchar.h>

#ifdef SV_WINDOWS_API
//! Defines for s{w,n}printf_irr because s{w,n}printf methods do not match the ISO C
//! standard on Windows platforms.
//! We want int snprintf_irr(char *str, size_t size, const char *format, ...);
//! and int swprintf_irr(wchar_t *wcs, size_t maxlen, const wchar_t *format, ...);
#if defined(_MSC_VER) && _MSC_VER > 1310 && !defined (_WIN32_WCE)
#define swprintf_irr swprintf_s
#define snprintf_irr sprintf_s
#elif !defined(__CYGWIN__)
#define swprintf_irr _snwprintf
#define snprintf_irr _snprintf
#endif

// define the wchar_t type if not already built in.
#ifdef _MSC_VER
#ifndef _WCHAR_T_DEFINED
//! A 16 bit wide character type.
/**
    Defines the wchar_t-type.
    In VS6, its not possible to tell
    the standard compiler to treat wchar_t as a built-in type, and
    sometimes we just don't want to include the huge stdlib.h or wchar.h,
    so we'll use this.
*/
typedef unsigned short wchar_t;
#define _WCHAR_T_DEFINED
#endif // wchar is not defined
#endif // microsoft compiler
#else
#define swprintf_irr swprintf
#define snprintf_irr snprintf


#endif // SV_WINDOWS_API

//! Type name for character type used by the file system.
/** Should the wide character version of the FileSystem be used it is a
16 bit character variable. Used for Unicode Filesystem and Unicode strings.
Else it is a 8 bit character variable. Used for ansi Filesystem and non-unicode
strings
*/
#if defined(_SV_WCHAR_FILESYSTEM)
typedef wchar_t fschar_t;
#define _SV_TEXT(X) L##X
#else
typedef char fschar_t;
#define _SV_TEXT(X) X
#endif



//! define a break macro for debugging.
#if defined(_DEBUG)

#if defined(SV_WINDOWS_API) && defined(_MSC_VER) && !defined (_WIN32_WCE)
#if defined(WIN64) || defined(_WIN64) // using portable common solution for x64 configuration
#include <crtdbg.h>
#define _SV_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_CrtDbgBreak();}
#else
#define _SV_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_asm int 3}
#endif
#else
#include "assert.h"
#define _SV_DEBUG_BREAK_IF( _CONDITION_ ) assert( !(_CONDITION_) );
#endif
#else
#define _SV_DEBUG_BREAK_IF(_CONDITION_)
#endif

//! Defines a deprecated macro which generates a warning at compile time
/** The usage is simple
For typedef:		typedef _SV_DEPRECATED_ int test1;
For classes/structs:	class _SV_DEPRECATED_ test2 { ... };
For methods:		class test3 { _SV_DEPRECATED_ virtual void foo() {} };
For functions:		template<class T> _SV_DEPRECATED_ void test4(void) {}
**/

#if defined(IGNORE_DEPRECATED_WARNING)
#define _SV_DEPRECATED_
#elif _MSC_VER >= 1310 //vs 2003 or higher
#define _SV_DEPRECATED_ __declspec(deprecated)
#elif (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1)) // all versions above 3.0 should support this feature
#define _SV_DEPRECATED_  __attribute__ ((deprecated))
#else
#define _SV_DEPRECATED_
#endif

//! Defines an override macro, to protect virtual functions from typos and other mismatches
/** Usage in a derived class:
virtual void somefunc() _SV_OVERRIDE_;
*/
#if (((__GNUC__ > 4) || ((__GNUC__ == 4) && (__GNUC_MINOR__ >= 7))) && (defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L))
#define _SV_OVERRIDE_ override
#elif (_MSC_VER >= 1600) /* supported since MSVC 2010 */
#define _SV_OVERRIDE_ override
#elif (__clang_major__ >= 3)
#define _SV_OVERRIDE_ override
#else
#define _SV_OVERRIDE_
#endif

// memory debugging
#if defined(_DEBUG) && defined(SV_EXPORTS) && defined(_MSC_VER) && \
    (_MSC_VER > 1299) && !defined(_IRR_DONT_DO_MEMORY_DEBUGGING_HERE) && !defined(_WIN32_WCE)

#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#define DEBUG_CLIENTBLOCK new( _CLIENT_BLOCK, __FILE__, __LINE__)
#include <stdlib.h>
#include <crtdbg.h>
#define new DEBUG_CLIENTBLOCK
#endif

//! ignore VC8 warning deprecated
/** The Microsoft compiler */
#if defined(SV_WINDOWS_API) && defined(_MSC_VER) && (_MSC_VER >= 1400)
//#pragma warning( disable: 4996)
//#define _CRT_SECURE_NO_DEPRECATE 1
//#define _CRT_NONSTDC_NO_DEPRECATE 1
#endif


//! creates four CC codes used in SVENGINE for simple ids
/** some compilers can create those by directly writing the
code like 'code', but some generate warnings so we use this macro here */
#define MAKE_SV_ID(c0, c1, c2, c3) \
        ((sv::u32)(sv::u8)(c0) | ((sv::u32)(sv::u8)(c1) << 8) | \
        ((sv::u32)(sv::u8)(c2) << 16) | ((sv::u32)(sv::u8)(c3) << 24 ))

#if defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#define _strcmpi(a,b) strcmpi(a,b)
#endif

#endif // SV_TYPES_H

