/*(Ver.=0.9~)
 * OSLW_core.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_CORE_H_
#define OSLW_CORE_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_memory.h"
#include "OSLW_show.h"
#include "OSLW_communication.h"
#include "OSLW_timer.h"
#include "OSLW_task.h"
#include "OSLW_string_debug.h"
#include <limits.h>
#include <stdlib.h>

typedef int (*_compar_fn)(const void *,const void *);


//------------------------------------------
//<枚举>OSlwCoreErrorNUM</枚举>
//<枚举功能>枚举内核错误类型</枚举功能>
//------------------------------------------
typedef enum{
	OSlwCoreError_NoError=0,//无错误
	OSlwCoreError_PriorityRedefine,//任务优先级重定义错误
	OSlwCoreError_GiftStatusError,
	OSlwCoreError_ParaListLengthOver,
	OSlwCoreError_TimerListLengthOver,
	OSlwCoreError_FixedMalloc
}OSlwCoreErrorNUM;

//------------------------------------------
//<枚举>OSlwCoreStatusNUM</枚举>
//<枚举功能>枚举内核状态类型</枚举功能>
//------------------------------------------
typedef enum{
	OSlwCoreStatus_NAN=0,//未定义状态
	OSlwCoreStatus_Initial=1,//初始化中
	OSlwCoreStatus_Running=0x0a,//正常运行中
	OSlwCoreStatus_Error=0x05//内核错误
}OSlwCoreStatusNUM;



#if OSLW_OS_ENABLE
//------------------------------------------
//<结构体名>OSlwParameter_toForm</结构体名>
//<结构体功能>操作系统内核结构体</结构体功能>
//------------------------------------------
typedef struct OSLW_CORE_STRUCT{
	//成员变量
#if !OSLW_SIMPLE_MODE
	_BP DispatchBreakPoint;//调度器断点
#endif
	
	OSlwCoreGroupUN InitialFlagGroup;//初始化标志组
	
	OSlwCoreGroupUN ReadyFlagGroup;//准备完成标志组

	struct OSLW_TASK_STRUCT *pTaskList[OSLW_TASK_NUM_MAX];//注册过的任务的控制块

	struct OSLW_TASK_STRUCT *pCurrentTask;//当前任务控制块

#if !OSLW_SIMPLE_MODE	
	struct OSLW_TASK_STRUCT *pLastTask;//之前任务的优先级
#endif
	
	struct OSLW_TASK_STRUCT OSlwInlineTaskBoring;

	OSlwTimerCtrlSTU TimerCtrl;
	
	//OSlwShowSTU Show;

	lw_u8 DispatchEable:1;//调度器使能标志 0:关闭调度器 1:开启调度器
	OSlwCoreStatusNUM CoreStatus:7;//内核状态
	OSlwCoreErrorNUM CoreError:8;//内核错误
	
	lw_u8 TaskRunningNum;//当前注册任务数量
	
	lw_u8 TaskRunningList[OSLW_TASK_NUM_MAX];//以及注册的任务的优先级 用于快速查询任务
	
	lw_u8 CurrentTaskIndex;//当前任务的优先级
	
#if !OSLW_SIMPLE_MODE	
	lw_u8 LastTaskIndex;//之前任务的优先级
#endif

#if OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN

	struct OSLW_STRING_DEBUG_STRUCT StrDbg;

#endif // OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN



	
	OSlwGroupAllType _flag_BK;
	OSlwGroupAllType _mask_list[__OSLW_LOG2_TASK_NUM-2];
	OSlwGroupAllType _move_reg[__OSLW_LOG2_TASK_NUM-2];

#if OSLW_TASK_COMMUN_ENABLE==1
	//struct OSLW_COMMUN_STRCUT Comm[OSLW_CORE_COMMUN_LIST_LENGTH];
#endif

	//OSlwParaListCtrlSTU ParaBook[OSLW_CORE_PARA_PAGE_NUM];
	//lw_u16 ParaIdCount;

#if OSLW_MEMORY_ENABLE
	//逐渐放弃 鸡肋
#if 0
	OSlwMemoryMapSTU GiftMem;
	lw_u8 GiftBuf[OSLW_MEM_MAP_LEN(sizeof(OSlwGiftUnitSTU),OSLW_CORE_GIFT_NUM)];

	OSlwMemoryMapSTU PostmanMem;
	lw_u8 PostmanBuf[OSLW_MEM_MAP_LEN(sizeof(OSlwGiftPostmanSTU), OSLW_CORE_POSTMAN_NUM)];
#endif

#endif




	//成员函数

	void(*StartFun)(struct OSLW_CORE_STRUCT *pOS);//系统开始函数
	struct OSLW_CORE_STRUCT *(*TaskAppendFun)(struct OSLW_CORE_STRUCT *pOS, struct OSLW_TASK_STRUCT *pta);//任务注册函数
	struct OSLW_CORE_STRUCT *(*TimerAppendFun)(struct OSLW_CORE_STRUCT *pOS, OSlwTimerSTU *pT);
	/*(Ver.=0.9~)
#if !(OSLW_SIMPLE_MODE)
	void (*DispatchFun)(struct OSLW_CORE_STRUCT *pOS);//调度器函数
	void (*ClockCBFun)(struct OSLW_CORE_STRUCT *pOS);//系统时钟回掉函数 !!!!!!!!!不建议以成员函数调用!!!!!!!!!
	void (*StartFun)(struct OSLW_CORE_STRUCT *pOS);//系统开始函数
	struct OSLW_CORE_STRUCT *(*TaskAppendFun)(struct OSLW_CORE_STRUCT *pOS,struct OSLW_TASK_STRUCT *pta);//任务注册函数
	struct OSLW_CORE_STRUCT *(*ParaAppendFun)(struct OSLW_CORE_STRUCT *pOS,lw_u8 num,struct OSLW_PARAMETER_STRUCT *p);
	struct OSLW_CORE_STRUCT *(*TimerAppendFun)(struct OSLW_CORE_STRUCT *pOS,OSlwTimerSTU *pT);
#endif
*/

#if OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN
	lw_u8 *(*StrDbgFun)(struct OSLW_CORE_STRUCT *pOS, lw_u8 *instr);
#endif // OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN

	//部分临时中间变量
	lw_u8 n1_Dispatch,n2_Dispatch;
	lw_u8 Temp;
	lw_u8 temp2;
	lw_u8 _ReadyTaskIndex;

	//稳定运行时间计数
	lw_u64 NormalRunningTimerCount;

#if OSLW_SIMPLE_LEVEL >= 3
	lw_u8 CPU_Usage;
	lw_u8 _CPU_Usage_cal_reg;
	lw_u8 _CPU_Usage_count;
#else
	lw_u16 CPU_Usage;
	lw_u16 _CPU_Usage_cal_reg;
	lw_u16 _CPU_Usage_count;
#endif // OSLW_SIMPLE_LEVEL >= 3



}OSlwCoreSTU;



//函数声明
void OSlwTaskDispatch(OSlwCoreSTU *_pOS);
void OSlwClockCallBack(OSlwCoreSTU *pOS);
void OSlwCoreInitial(OSlwCoreSTU *pOS);
OSlwCoreSTU* OSlwTaskAppend(OSlwCoreSTU *pOS,struct OSLW_TASK_STRUCT *pta);
OSlwCoreSTU* OSlwParaAppend(OSlwCoreSTU *pOS,lw_u8 num,struct OSLW_PARAMETER_STRUCT *p);
OSlwCoreSTU* OSlwTimerAppend(OSlwCoreSTU *pOS,OSlwTimerSTU *pT);

void OSlwCoreStart(OSlwCoreSTU *pOS);
void _quicksort (void *const pbase, size_t total_elems, size_t size,_compar_fn cmp);
lw_16 comp(const void*a,const void*b);


#endif //OSLW_OS_ENABLE


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

//低功耗宏
//快速模式下没有
#if !OSLW_SPEED_RUNNING
#define OSLW_LOW_POW_ENABLE() ((void)0)
#define OSLW_LOW_POW_DISABLE() ((void)0)
#endif



//断点保存 
#define REMEMBER(x,r) r=_OSLW_SAVE((x));if(!(r))


#if !OSLW_SIMPLE_MODE

//调度准备 A为任务指针
#define DISPATCH_READY(A) A->BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP - 1] = _OSLW_SAVE((A->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP - 1]));\
	if (!(A->BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP - 1]) && A->TaskStatus == OSlwTaskStatus_Running)

//调度返回 A为任务指针
#define DISPATCH_RETURN(A) else{A->TaskStatus = OSlwTaskStatus_Running;}

#else

#define DISPATCH_READY(A) 	case __LINE__:(A)->BreakPoint=__LINE__;\
			if((A)->TaskStatus == OSlwTaskStatus_Ready){(A)->TaskStatus=OSlwTaskStatus_Running;}else

#define DISPATCH_RETURN(A) return;

#endif


			
//任务开始			
		
#if OSLW_SIMPLE_MODE 
#define OSlw_5_2(A) 	switch((A)->BreakPoint){\
		default:\
		case 0:\
		case __LINE__:(A)->BreakPoint=__LINE__;\
		if((A)->TaskStatus == OSlwTaskStatus_Init){(A)->TaskStatus=OSlwTaskStatus_Running;return;}
		

#define	OSlw_2_5(A) }

#else

#define OSlw_5_2(A)		do{if(!_OSLW_SAVE(A->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1]))\
{\
	A->TaskStatus=OSlwTaskStatus_Ready;\
	_OSLW_LOAD(A->pOS->DispatchBreakPoint,A->Priority,NULL);\
}}while(0)


#define OSlw_2_5(A) ;

#endif
	
	
	
#define OSLW_TASK_BENGIN(A) OSlw_5_2((A))

//既济卦 万事俱备
#define OSlwJiJi(A) OSlw_5_2(A)

//未济卦	 仍欠东风
#define OSlwWeiJi(A) OSlw_2_5(A)			


//任务子函数调用
//<输入>LV:嵌套等级等级(在任务函数中直接使用调用则等级为0 在下一级为1 依次类推) pta:任务指针 F:被调用的函数</输入>
//<示例> TaskSubFunCall(0,pta,fun1); pta任务调用fun1嵌套等级0 </示例> 
//!!!!!!!!!!!!void fun1(OSlwTaskSTU *pta,void *p)!!!!!!!!!!!!!!!!!!!!!!!!!

#define OSlwTaskSubFunCall(LV,PTA,F) do {REMEMBER(PTA->BreakPoint[LV],PTA->BreakPointReturn[LV])\
																				{F(PTA,(void *)(&(PTA->BreakPoint[LV])));}}while(0)

																				
																																				
																				
//任务子函数返回
//!!!!!!与TaskSubFunCall成对调用 用于返回!!!!!!																				
#define OSlwTaskSubFunReturn(PTA,P) do { _OSLW_LOAD((*((_BP *)(P))),PTA->Priority,(PTA));}while(0)


#define OSlwCoreTaskIsReady(POS,PTA,ID) 					do{POS->ReadyFlagGroup.all|=(1<<ID);\
					PTA->TaskStatus=OSlwTaskStatus_Ready;}while(0)

					
					
					

//睡眠N个时钟
//<输入>A:任务控制块指针 N:周期</输入>
#if OSLW_SIMPLE_MODE
#define OSlwSleepN(A,N) do {DISPATCH_READY(A){ OSlwTaskSleep(A,N-1);DISPATCH_RETURN(A)}} while(0)
#else
#define OSlwSleepN(A,N) do {DISPATCH_READY(A){ OSlwTaskSleep(A,N-1);}DISPATCH_RETURN(A)} while(0)
#endif


#define OSlwSleepSec(A,SEC,MS) OSlwSleepN(A,((((lw_u32)(SEC)*1000+(MS)))/(lw_u32)OSLW_CORE_TICK_MS))

//等待接收完成
//<输入>A:任务控制块指针</输入>
#if OSLW_SIMPLE_MODE
#define OSlwGiftReceiveWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGiftReceice(A);DISPATCH_RETURN(A)}} while(0)
#else
#define OSlwGiftReceiveWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGiftReceice(A);}DISPATCH_RETURN(A)} while(0)
#endif

#define OSlwWaitGift(A) OSlwGiftReceiveWait((A))

//等待发送完成
//<输入>A:任务控制块指针</输入>
#if OSLW_SIMPLE_MODE
#define OSlwGiftTransmitWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGiftTransmit(A);DISPATCH_RETURN(A)}} while(0)
#else
#define OSlwGiftTransmitWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGiftTransmit(A);}DISPATCH_RETURN(A)} while(0)
#endif

//等待标志组
//<输入>A:任务控制块指针</输入>
#if OSLW_SIMPLE_MODE
#define OSlwGroupFlagWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGroupFlag(A);DISPATCH_RETURN(A)}} while(0)
#else
#define OSlwGroupFlagWait(A) do {DISPATCH_READY(A){ OSlwTaskWaitGroupFlag(A);}DISPATCH_RETURN(A)} while(0)
#endif


#define OSlwWaitFlag(A) OSlwGroupFlagWait((A))

//等待条件
//<输入>A:任务控制块指针 COND:跳出循环的条件 NUM:计数器变量 由外部定义传入 MS:每次延时所用的毫秒</输入>
#define OSlwWaitCondition(A,COND,NUM,MS)  do{OSlwSleepSec((A),0,(MS));}while(--NUM && (COND))

//放弃cpu使用权
//<输入>A:任务控制块指针</输入>
#define OSlwGiveUp(A) OSlwSleepN((A),2)

//任务停止
//<输入>A:任务控制块指针</输入>
#define OSlwTaskStop(A) do{(A)->TaskFrozenFlag=1;OSlwGiveUp(A);}while(0)

//满足条件放弃CPU使用权
//<输入>A:任务控制块指针 COND:满足条件</输入>
#define OSlwConditionGiveUp(A,COND) do{\
									if (COND)\
									{\
										OSlwGiveUp((A));\
									}\
									}while(0)


//设置闹钟
//<输入>A:任务控制块指针</输入>
#define OSlwSetAlarm(A,SEC,MS) do{(A)->TimeSliceMax=((((lw_u32)(SEC)*1000+(MS)))/(lw_u32)OSLW_CORE_TICK_MS);}while(0)


//看闹钟
//<输入>A:任务控制块指针</输入>
#define OSlwWatchAlarm(A) OSlwConditionGiveUp((A),((A)->RunningTimeSinceRunning >= TimeSliceMax))

//拍卖
//<输入>A:任务控制块指针</输入>
#define OSlwAuction(A,PRICE) OSlwConditionGiveUp((A),(OSlwTaskAuctionJudge((A),(lw_8)(PRICE))))


//测试程序运行时长 开始
//<输入>A:任务控制块指针</输入>
#define OSlwTic(A) do{(A)->BackToSleep.bits.timer_start_flag=1;}while(0);


//测试程序运行时长 结束
//<输入>A:任务控制块指针</输入>
#define OSlwToc(A) OSlwTaskToc((A))

//设置一共休息时间
//<输入>A:任务控制块指针</输入>
#define OSlwSetRest(A,S,MS) do{(A)->BackToSleep.bits.all_time_count=((S)*1000+(MS))/(OSLW_CORE_TICK_MS);}while(0);


//设置休息时间
//<输入>A:任务控制块指针</输入>
#define OSlwRest(A) do{lw_u16 _res_t=OSlwToc((A));if((A)->BackToSleep.bits.all_time_count >(1+ _res_t)){OSlwSleepN(A,(A)->BackToSleep.bits.all_time_count - _res_t);}}while(0);


//#define OSlwSleepN(A,N) do {DISPATCH_READY(A){ A->SleepFun(A,N-1);}} while(0)

#endif /*(Ver.=0.9~) OSLW_CORE_H_ */
