#include "SVBase.h"

#ifdef _CELLOS_LV2
	#include <sys/process.h>
	SYS_PROCESS_PARAM(1001,0x100000)
#endif

#ifdef DONT_USE_EXCEPTION
	s32 volatile exception_depth = 1;
	jmp_buf exception_buf[NUM_EXCEPTIONS];
	cptr8 exception_data[NUM_EXCEPTIONS];
#endif
