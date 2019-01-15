#ifndef OSLW_TOOL_STRING_H_
#define OSLW_TOOL_STRING_H_
#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_memory.h"

#if !(OSLW_SIMPLE_LEVEL >= 4)


ParaType OSlwToolStringToParaType(const lw_u8 *str);
lw_32 OSlwToolStringToInt(const lw_u8 *str);

lw_u8* OSlwToolString_itoa(lw_32 value, lw_u8 *str, lw_32 radix);
lw_u8* OSlwToolString_utoa(lw_u32 value, lw_u8 *str, lw_32 radix);
lw_u8* OSlwToolString_gcvt(ParaType value, lw_32 ndigit, lw_u8 *buf);
void* OSlwToolStringVSprintf(lw_8 *buf, const lw_8 *fmt, va_list args);
void* OSlwToolStringSprintf(lw_8 *buf, const lw_8 *fmt, ...);

#define LwStr2F(S) OSlwToolStringToParaType(S)
#define LwStr2I(S) OSlwToolStringToInt(S)
#define LwI2Str(S,D) OSlwToolString_itoa(S,D,10)
#define LwF2Str(S,D) OSlwToolString_gcvt(D,4,S)
#define LwSprintf OSlwToolStringSprintf

#define _LW_STR_SKIP_FIND(SS,S,C) 	do{(SS) = (S);while ( *(S) && *(S)++ != (C));(S)[-1] = 0;}while(0)
#define _LW_STR_SKIP_COPY(SS,S,C) 	do{lw_u8 *_buf=(SS);while ( *(S) && *(S) != (C)){*(_buf)++=*(S)++;}(_buf)[0] = 0;}while(0)

#endif // !(OSLW_SIMPLE_LEVEL >= 3)


#endif // !OSLW_TOOL_STRING_H_
