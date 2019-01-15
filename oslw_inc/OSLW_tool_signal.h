#ifndef OSLW_TOOL_SIGNAL_H_
#define OSLW_TOOL_SIGNAL_H_
#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_memory.h"

#if !(OSLW_SIMPLE_LEVEL >= 3)

typedef struct OSLW_TOOL_SYSTEM_BASIC_STRUCT {
	OSlwMat In;
	OSlwMat State;
	OSlwMat Out;

}OSlwToolSysBasicSTU;





#endif // !(OSLW_SIMPLE_LEVEL >= 3)


#endif // !OSLW_TOOL_SIGNAL_H_
