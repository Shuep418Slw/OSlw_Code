/*
 * OSLW_parameter.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_PARAMETER_H_
#define OSLW_PARAMETER_H_

#include "OSLW_define.h"


//------------------------------------------
//<结构体名>OSlwParameter</结构体名>
//<成员变量>
//Parameter:实际参数
//ParameterShallow:实际参数的影子寄存器
//delt:影子参数的增量
//Q:格式化因子 用于格式化
//</成员变量>
//<成员函数>
//toFormFun:格式化函数
//setFun:设置函数 影子寄存器存入
//UpSelfFun:参数自增
//DownSelfFun:参数自减
//</成员函数>
//------------------------------------------

typedef struct OSLW_PARAMETER_STRUCT{
	ParaType Parameter;
	ParaType ParameterShallow;
	ParaType delt;
	lw_u8 Q;

	struct OSLW_PARAMETER_STRUCT* (*toFormFun)(struct OSLW_PARAMETER_STRUCT *p,lw_u8 arr[5]);
	struct OSLW_PARAMETER_STRUCT* (*setFun)(struct OSLW_PARAMETER_STRUCT *p);
	struct OSLW_PARAMETER_STRUCT* (*UpSelfFun)(struct OSLW_PARAMETER_STRUCT *p);
	struct OSLW_PARAMETER_STRUCT* (*DownSelfFun)(struct OSLW_PARAMETER_STRUCT *p);

}OSlwParameterSTU;

//函数声明
OSlwParameterSTU *OSlwParameter_toForm(OSlwParameterSTU *p,lw_u8 arr[5]);
OSlwParameterSTU *OSlwParameter_set(OSlwParameterSTU *p);
OSlwParameterSTU *OSlwParameter_UpSelf(OSlwParameterSTU *p);
OSlwParameterSTU *OSlwParameter_DownSelf(OSlwParameterSTU *p);


//<宏组说明>数据类型定义 定义参数所用的算法</宏组说明>
#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMpy(A,B) (_IQmpy(A,B))
	#define _ParaDiv(A,B) (_IQdiv(A,B))
	#define _ParaInt(A) (_IQint(A))
	#define _ParaFrac(A) (_IQfrac(A))
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_FLOAT
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMpy(A,B) ((A)*(B))
	#define _ParaDiv(A,B) ((A)/(B))
	#define _ParaInt(A) ((int)(A))
	#define _ParaFrac(A) ((A)-int(A))
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_DOUBLE
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMpy(A,B) ((A)*(B))
	#define _ParaDiv(A,B) ((A)/(B))
	#define _ParaInt(A) ((int)(A))
	#define _ParaFrac(A) ((A)-int(A))
#else
	#error "MATH TYPE IS NOT DEFINED"
#endif


#endif /* OSLW_PARAMETER_H*/
