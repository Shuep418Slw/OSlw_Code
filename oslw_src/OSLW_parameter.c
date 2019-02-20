/*
 * OSLW_parameter.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */
#include "OSLW_include.h"

//------------------------------------------
//<函数名>OSlwParameter_toForm</函数名>
//<功能说明>将parameter转换成格式 采用定点数进行存储 一共5字节 实际数值=(arr[1]*2^24+arr[2]*2^16+arr[3]*2^8+arr[4])/(2^arr[0])</功能说明>
//<输入说明>p:this  arr[]:目标地址</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

OSlwParameterSTU *OSlwParameter_toForm(OSlwParameterSTU *p,lw_u8 arr[5])
{
	fixpoint buf;
#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_DOUBLE
	buf=(fixpoint)(p->ParameterShallow*(double)(1<<p->Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
	buf=(fixpoint)(p->ParameterShallow*(float)(1<<p->Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_Q
	buf=FP_A_TO_FP_B(OSLW_GLOBAL_MATH_Q_FORM,(p->Q),(p->ParameterShallow));
#else
	buf=0;
#endif
	*arr++=p->Q;
	*arr++=((buf&0xf000)>>24);
	*arr++=((buf&0x0f00)>>16);
	*arr++=((buf&0x00f0)>>8);
	*arr++=((buf&0x000f)>>0);
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_set</函数名>
//<功能说明>将参数影子寄存器存入实际参数寄存器</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_set(OSlwParameterSTU *p)
{
	p->Parameter=p->ParameterShallow;
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_UpSelf</函数名>
//<功能说明>参数自增 parameter=parameter+delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

OSlwParameterSTU *OSlwParameter_UpSelf(OSlwParameterSTU *p)
{
	p->ParameterShallow=_ParaAdd(p->ParameterShallow,p->delt);
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_DownSelf</函数名>
//<功能说明>参数自增 parameter=parameter-delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_DownSelf(OSlwParameterSTU *p)
{
	p->ParameterShallow=_ParaSub(p->ParameterShallow,p->delt);
	return p;
}
