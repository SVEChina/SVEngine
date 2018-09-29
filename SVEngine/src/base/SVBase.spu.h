#ifndef __BASE_SPU_H__
#define __BASE_SPU_H__

#include <math.h>
#include <stdlib.h>

#include <spu_intrinsics.h>
#include <spu_printf.h>
#include <spu_mfcio.h>
#include <vec_types.h>

//
#define sv_inline	__inline__
#define sv_restrict	__restrict

//
#define LIKELY(EXP)		__builtin_expect((EXP),1)
#define UNLIKELY(EXP)	__builtin_expect((EXP),0)

//
#ifdef assert
	#undef assert
#endif
#ifdef NDEBUG
	#define assert(EXP)	(static_cast<void>(0))
#else
	#define assert(EXP) { if(UNLIKELY(EXP)) { } else { spu_printf("%s:%d: Assertion: '%s'\n",__FILE__,__LINE__,#EXP); spu_stop(0x3fff); } }
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
#define ATTRIBUTE_ALIGNED4(NAME) NAME __attribute__ ((aligned(4)))
#define ATTRIBUTE_ALIGNED8(NAME) NAME __attribute__ ((aligned(8)))
#define ATTRIBUTE_ALIGNED16(NAME) NAME __attribute__ ((aligned(16)))
#define ATTRIBUTE_ALIGNED128(NAME) NAME __attribute__ ((aligned(128)))

#endif /* __BASE_SPU_H__ */
