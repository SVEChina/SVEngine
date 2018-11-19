#ifndef __BASE_H__
#define __BASE_H__

#include <time.h>
#include <math.h>
#include <float.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <assert.h>

#include "SVTypes.h"

//
#ifdef _CELLOS_LV2
	#include <sys/paths.h>
	#define PATH_PREFIX	SYS_APP_HOME"/"
#elif _ANDROID
	#define PATH_PREFIX	"/sdcard/"
#endif

//
#ifdef _WIN32
	#define sv_inline		__forceinline
	#define sv_fasetcall	__fastcall
	#define sv_restrict	__restrict
#elif _ANDROID
	#define sv_inline		__inline__
	#define sv_fasetcall
	#define sv_restrict	__restrict
#else
	#define sv_inline		__inline__ __attribute__ ((always_inline))
	#define sv_fasetcall
	#define sv_restrict	__restrict
#endif

//
#define UNUSED(EXP)		(void)EXP;

//
#ifdef __GNUC__
	#define LIKELY(EXP)		__builtin_expect((EXP),1)
	#define UNLIKELY(EXP)	__builtin_expect((EXP),0)
#else
	#define LIKELY(EXP)		(EXP)
	#define UNLIKELY(EXP)	(EXP)
#endif

//
#ifdef USE_ASSERT
	#include "Log.h"
	#ifdef assert
		#undef assert
	#endif
	#ifdef NDEBUG
		#define assert(EXP)	(static_cast<void>(0))
	#else
		#if _LINUX
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: %s: Assertion: '%s'\n",__FILE__,__LINE__,__ASSERT_FUNCTION,#EXP); } }
		#elif defined(_MACOS) || defined(_IOS)
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: %s: Assertion: '%s'\n",__FILE__,__LINE__,__PRETTY_FUNCTION__,#EXP); } }
		#else
			#define assert(EXP) { if(UNLIKELY(EXP)) { } else { Log::fatal("%s:%d: Assertion: '%s'\n",__FILE__,__LINE__,#EXP); } }
		#endif
	#endif
#endif

//
#ifdef DONT_USE_EXCEPTION
	#include "Log.h"
	#define NUM_EXCEPTIONS	32
	extern volatile s32 exception_depth;
	extern jmp_buf exception_buf[NUM_EXCEPTIONS];
	extern cptr8 exception_data[NUM_EXCEPTIONS];
	#ifdef _CELLOS_LV2
		#include <cell/atomic.h>
		#define TRY { cellAtomicIncr32((uint32_t*)&exception_depth); assert(exception_depth < NUM_EXCEPTIONS && "try(): stack overflow"); if(setjmp(exception_buf[exception_depth]) == 0) { {
		#define CATCH(EXP) } cellAtomicDecr32((uint32_t*)&exception_depth); } else { EXP = exception_data[cellAtomicDecr32((uint32_t*)&exception_depth)]; assert(exception_depth >= 1 && "catch(): stack underflow"); {
	#elif _ANDROID
		#include <sys/atomics.h>
		#define TRY { __atomic_inc(&exception_depth); assert(exception_depth < NUM_EXCEPTIONS && "try(): stack overflow"); if(setjmp(exception_buf[exception_depth]) == 0) { {
		#define CATCH(EXP) } __atomic_dec(&exception_depth); } else { EXP = exception_data[__atomic_dec(&exception_depth)]; assert(exception_depth >= 1 && "catch(): stack underflow"); {
	#else
		#define TRY { exception_depth++; assert(exception_depth < NUM_EXCEPTIONS && "try(): stack overflow"); if(setjmp(exception_buf[exception_depth]) == 0) { {
		#define CATCH(EXP) } exception_depth--; } else { EXP = exception_data[exception_depth--]; assert(exception_depth >= 1 && "catch(): stack underflow"); {
	#endif
	#define THROW(EXP) { if(exception_depth <= 1) Log::fatal("%s:%d: throw(): unhandled exception: %s",__FILE__,__LINE__,EXP); exception_data[exception_depth] = EXP; longjmp(exception_buf[exception_depth],1); }
	#define TRY_END } } }
#else
	#define TRY try
	#define THROW(EXP) throw(EXP)
	#define CATCH(EXP) catch(EXP)
	#define TRY_END
#endif

//
#define ALIGNED4(VALUE) (((size_t)(VALUE) + 3) & ~3)
#define ALIGNED8(VALUE) (((size_t)(VALUE) + 7) & ~7)
#define ALIGNED16(VALUE) (((size_t)(VALUE) + 15) & ~15)
#define ALIGNED128(VALUE) (((size_t)(VALUE) + 127) & ~127)
#define IS_ALIGNED4(VALUE) (((size_t)(VALUE) & 3) == 0)
#define IS_ALIGNED8(VALUE) (((size_t)(VALUE) & 7) == 0)
#define IS_ALIGNED16(VALUE) (((size_t)(VALUE) & 15) == 0)
#define IS_ALIGNED128(VALUE) (((size_t)(VALUE) & 127) == 0)
#define ASSERT_ALIGNED4(VALUE) assert(IS_ALIGNED4(VALUE))
#define ASSERT_ALIGNED8(VALUE) assert(IS_ALIGNED8(VALUE))
#define ASSERT_ALIGNED16(VALUE) assert(IS_ALIGNED16(VALUE))
#define ASSERT_ALIGNED128(VALUE) assert(IS_ALIGNED128(VALUE))

//
#if defined(USE_SSE) || defined(USE_ALTIVEC) || defined(USE_NEON)
	#ifdef _WIN32
		#define SVE_ALIGNED4(NAME) __declspec(align(4)) NAME
		#define SVE_ALIGNED8(NAME) __declspec(align(8)) NAME
		#define SVE_ALIGNED16(NAME) __declspec(align(16)) NAME
		#define SVE_ALIGNED128(NAME) __declspec(align(128)) NAME
	#else
		#define SVE_ALIGNED4(NAME) NAME __attribute__ ((aligned(4)))
		#define SVE_ALIGNED8(NAME) NAME __attribute__ ((aligned(8)))
		#define SVE_ALIGNED16(NAME) NAME __attribute__ ((aligned(16)))
		#define SVE_ALIGNED128(NAME) NAME __attribute__ ((aligned(128)))
	#endif
#else
	#define SVE_ALIGNED4(NAME) NAME
	#define SVE_ALIGNED8(NAME) NAME
	#define SVE_ALIGNED16(NAME) NAME
	#define SVE_ALIGNED128(NAME) NAME
#endif

//
#ifdef USE_MEMORY
	#include "Memory.h"
#endif

//
class Base {
		
	public:
		
		#ifdef USE_MEMORY
			static sv_inline void *operator new(size_t size) { return ::operator new(size); }
			static sv_inline void *operator new[](size_t size) { return ::operator new[](size); }
			static sv_inline void operator delete(void *ptr) { ::operator delete(ptr); }
			static sv_inline void operator delete[](void *ptr) { ::operator delete[](ptr); }
			static sv_inline void operator delete(void *ptr,size_t size) { ::operator delete(ptr,size); }
			static sv_inline void operator delete[](void *ptr,size_t size) { ::operator delete[](ptr,size); }
		#endif
};

#endif /* __BASE_H__ */
