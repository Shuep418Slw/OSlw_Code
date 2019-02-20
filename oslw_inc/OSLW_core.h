/*
 * OSLW_core.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_CORE_H_
#define OSLW_CORE_H_

#include "OSLW_define.h"
#include <limits.h>
#include <stdlib.h>

typedef int (*_compar_fn)(const void *,const void *);

//------------------------------------------
//<共用体名>OSlwCoreGroupUN</共用体名>
//<共用体功能>内核标志组共用体 根据OSLW_TASK_NUM_MAX确定长度</共用体功能>
//------------------------------------------
typedef struct OSLW_CORE_GROUP_UNION{
#if OSLW_TASK_NUM_MAX<=8
	struct{
		lw_u8 bit7:1;
		lw_u8 bit6:1;
		lw_u8 bit5:1;
		lw_u8 bit4:1;
		lw_u8 bit3:1;
		lw_u8 bit2:1;
		lw_u8 bit1:1;
		lw_u8 bit0:1;
	}bits;
	lw_u8 all;
#elif OSLW_TASK_NUM_MAX>8 && OSLW_TASK_NUM_MAX<=16
	struct{
		lw_u16 bit15:1;
		lw_u16 bit14:1;
		lw_u16 bit13:1;
		lw_u16 bit12:1;
		lw_u16 bit11:1;
		lw_u16 bit10:1;
		lw_u16 bit9:1;
		lw_u16 bit8:1;
		lw_u16 bit7:1;
		lw_u16 bit6:1;
		lw_u16 bit5:1;
		lw_u16 bit4:1;
		lw_u16 bit3:1;
		lw_u16 bit2:1;
		lw_u16 bit1:1;
		lw_u16 bit0:1;
	}bits;
	lw_u16 all;
#elif OSLW_TASK_NUM_MAX>16 && OSLW_TASK_NUM_MAX<=32
	struct{
		lw_u32 bit31:1;
		lw_u32 bit30:1;
		lw_u32 bit29:1;
		lw_u32 bit28:1;
		lw_u32 bit27:1;
		lw_u32 bit26:1;
		lw_u32 bit25:1;
		lw_u32 bit24:1;
		lw_u32 bit23:1;
		lw_u32 bit22:1;
		lw_u32 bit21:1;
		lw_u32 bit20:1;
		lw_u32 bit19:1;
		lw_u32 bit18:1;
		lw_u32 bit17:1;
		lw_u32 bit16:1;
		lw_u32 bit15:1;
		lw_u32 bit14:1;
		lw_u32 bit13:1;
		lw_u32 bit12:1;
		lw_u32 bit11:1;
		lw_u32 bit10:1;
		lw_u32 bit9:1;
		lw_u32 bit8:1;
		lw_u32 bit7:1;
		lw_u32 bit6:1;
		lw_u32 bit5:1;
		lw_u32 bit4:1;
		lw_u32 bit3:1;
		lw_u32 bit2:1;
		lw_u32 bit1:1;
		lw_u32 bit0:1;
	}bits;
	lw_u32 all;
#elif OSLW_TASK_NUM_MAX>32 && OSLW_TASK_NUM_MAX<=64
	struct{
		lw_u64 bit63:1;
		lw_u64 bit62:1;
		lw_u64 bit61:1;
		lw_u64 bit60:1;
		lw_u64 bit59:1;
		lw_u64 bit58:1;
		lw_u64 bit57:1;
		lw_u64 bit56:1;
		lw_u64 bit55:1;
		lw_u64 bit54:1;
		lw_u64 bit53:1;
		lw_u64 bit52:1;
		lw_u64 bit51:1;
		lw_u64 bit50:1;
		lw_u64 bit49:1;
		lw_u64 bit48:1;
		lw_u64 bit47:1;
		lw_u64 bit46:1;
		lw_u64 bit45:1;
		lw_u64 bit44:1;
		lw_u64 bit43:1;
		lw_u64 bit42:1;
		lw_u64 bit41:1;
		lw_u64 bit40:1;
		lw_u64 bit39:1;
		lw_u64 bit38:1;
		lw_u64 bit37:1;
		lw_u64 bit36:1;
		lw_u64 bit35:1;
		lw_u64 bit34:1;
		lw_u64 bit33:1;
		lw_u64 bit32:1;
		lw_u64 bit31:1;
		lw_u64 bit30:1;
		lw_u64 bit29:1;
		lw_u64 bit28:1;
		lw_u64 bit27:1;
		lw_u64 bit26:1;
		lw_u64 bit25:1;
		lw_u64 bit24:1;
		lw_u64 bit23:1;
		lw_u64 bit22:1;
		lw_u64 bit21:1;
		lw_u64 bit20:1;
		lw_u64 bit19:1;
		lw_u64 bit18:1;
		lw_u64 bit17:1;
		lw_u64 bit16:1;
		lw_u64 bit15:1;
		lw_u64 bit14:1;
		lw_u64 bit13:1;
		lw_u64 bit12:1;
		lw_u64 bit11:1;
		lw_u64 bit10:1;
		lw_u64 bit9:1;
		lw_u64 bit8:1;
		lw_u64 bit7:1;
		lw_u64 bit6:1;
		lw_u64 bit5:1;
		lw_u64 bit4:1;
		lw_u64 bit3:1;
		lw_u64 bit2:1;
		lw_u64 bit1:1;
		lw_u64 bit0:1;
	}bits;
	lw_u64 all;
#else
	#error "TASK NUM IS OVER"
#endif
}OSlwCoreGroupUN;

//------------------------------------------
//<枚举>OSlwCoreErrorNUM</枚举>
//<枚举功能>枚举内核错误类型</枚举功能>
//------------------------------------------
typedef enum{
	OSlwCoreError_NoError=0,//无错误
	OSlwCoreError_PriorityRedefine//任务优先级重定义错误
}OSlwCoreErrorNUM;

//------------------------------------------
//<枚举>OSlwCoreStatusNUM</枚举>
//<枚举功能>枚举内核状态类型</枚举功能>
//------------------------------------------
typedef enum{
	OSlwCoreStatus_NAN=0,//未定义状态
	OSlwCoreStatus_Initial=1,//初始化中
	OSlwCoreStatus_Running=0xaa,//正常运行中
	OSlwCoreStatus_Error=0x55//内核错误
}OSlwCoreStatusNUM;


//------------------------------------------
//<结构体名>OSlwParameter_toForm</结构体名>
//<结构体功能>操作系统内核结构体</结构体功能>
//------------------------------------------
typedef struct OSLW_CORE_STRUCT{

	//成员变量
	OSlwCoreGroupUN InitialFlagGroup;//初始化标志组
	OSlwCoreGroupUN ReadyFlagGroup;//准备完成标志组

	OSlwCoreErrorNUM CoreError;//内核错误
	OSlwCoreStatusNUM CoreStatus;//内核状态

	lw_u8 DispatchEable;//调度器使能标志 0:关闭调度器 1:开启调度器
	lw_u8 TaskRunningNum;//当前注册任务数量
	lw_u8 TaskRunningList[OSLW_TASK_NUM_MAX];//以及注册的任务的优先级 用于快速查询任务

	struct OSLW_TASK_STRUCT *pTaskList[OSLW_TASK_NUM_MAX];//注册过的任务的控制块

	lw_u8 CurrentTaskIndex;//当前任务的优先级
	struct OSLW_TASK_STRUCT *pCurrentTask;//当前任务控制块

	lw_u8 LastTaskIndex;//之前任务的优先级
	struct OSLW_TASK_STRUCT *pLastTask;//之前任务的优先级

	_BP DispatchBreakPoint;//调度器断点

	//成员函数
	void (*DispatchFun)(struct OSLW_CORE_STRUCT *pOS);//调度器函数
	void (*ClockCBFun)(struct OSLW_CORE_STRUCT *pOS);//系统时钟回掉函数 !!!!!!!!!不建议以成员函数调用!!!!!!!!!
	void (*StartFun)(struct OSLW_CORE_STRUCT *pOS);//系统开始函数
	struct OSLW_CORE_STRUCT *(*TaskAppendFun)(struct OSLW_CORE_STRUCT *pOS,struct OSLW_TASK_STRUCT *pta);//任务注册函数


	//部分临时中间变量
	lw_u8 n1_Dispatch,n2_Dispatch;
	lw_u8 Temp;
	lw_u8 temp2;

}OSlwCoreSTU;



//函数声明
void OSlwTaskDispatch(OSlwCoreSTU *pOS);
void OSlwClockCallBack(OSlwCoreSTU *pOS);
void OSlwCoreInitial(OSlwCoreSTU *pOS);
OSlwCoreSTU* OSlwTaskAppend(OSlwCoreSTU *pOS,struct OSLW_TASK_STRUCT *pta);
void OSlwCoreStart(OSlwCoreSTU *pOS);
void _quicksort (void *const pbase, size_t total_elems, size_t size,_compar_fn cmp);
lw_16 comp(const void*a,const void*b);



//<排序算法宏>
#define SWAP(a, b, size)						      \
  do									      \
    {									      \
      size_t __size = (size);						      \
      char *__a = (a), *__b = (b);					      \
      do								      \
	{								      \
	  char __tmp = *__a;						      \
	  *__a++ = *__b;						      \
	  *__b++ = __tmp;						      \
	} while (--__size > 0);						      \
    } while (0)

#define MAX_THRESH 4


typedef struct
  {
    char *lo;
    char *hi;
  } stack_node;

#define STACK_SIZE	(CHAR_BIT * sizeof(size_t))
#define PUSH(low, high)	((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define	POP(low, high)	((void) (--top, (low = top->lo), (high = top->hi)))
#define	STACK_NOT_EMPTY	(stack < top)
//</排序算法宏>


//宏函数 & 宏模块

//断点保存
#define REMEMBER(x,r) r=SAVE((x));if(!(r))

//任务子函数调用
//<输入>LV:嵌套等级等级(在任务函数中直接使用调用则等级为0 在下一级为1 依次类推) pta:任务指针 F:被调用的函数</输入>
//<示例> TaskSubFunCall(0,pta,fun1); pta任务调用fun1嵌套等级0 </示例> 
//!!!!!!!!!!!!void fun1(OSlwTaskSTU *pta,void *p)!!!!!!!!!!!!!!!!!!!!!!!!!

#define OSlwTaskSubFunCall(LV,PTA,F) do {REMEMBER(PTA->BreakPoint[LV],PTA->BreakPointReturn[LV])\
																				{F(PTA,(void *)(&(PTA->BreakPoint[LV])));}}while(0)

//任务子函数返回
//!!!!!!与TaskSubFunCall成对调用 用于返回!!!!!!																				
#define OSlwTaskSubFunReturn(PTA,P) do { LOAD((*((_BP *)(P))),PTA->Priority);}while(0)

//睡眠N个时钟
//<输入>A:任务控制块指针 N:周期</输入>
#define OSlwSleepN(A,N) do {REMEMBER(A->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1],A->BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP-1]){ A->SleepFun(A,N-1);}} while(0)


#endif /* OSLW_CORE_H_ */
