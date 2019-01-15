/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_task.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_TASK_H_
#define OSLW_TASK_H_

#include <setjmp.h>
#include "OSLW_define.h"
#include "OSLW_gift.h"
#include "OSLW_memory.h"

//------------------------------------------
//<结构体名>OSlwTaskPrivateMemorySTU</结构体名>
//<结构体功能>任务私有存储器 每片长度为TASK_PRIVATE_MEMORY_SECTION_LENGTH</结构体功能>
//------------------------------------------
typedef struct OSLW_TASK_PRIVATE_MEMORY_STRUCT{
	lw_u8 m8[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u16 m16[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u32 m32[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u64 m64[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_sf msf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_df mdf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
}OSlwTaskPrivateMemorySTU;


//------------------------------------------
//<结构体名>OSlwTaskExternMemorySTU</结构体名>
//<结构体功能>任务外部存储器 长度为Length 首地址为arr</结构体功能>
//------------------------------------------
//typedef struct OSLW_TASK_EXTERN_MEMORY_STRUCT{
//	lw_u16 Length;
//	void *arr;
//}OSlwTaskExternMemorySTU;

typedef OSlwExternMemorySTU OSlwTaskExternMemorySTU;

//------------------------------------------
//<枚举>OSlwTaskStatusNUM</枚举>
//<枚举功能>任务状态类型</枚举功能>
//------------------------------------------
typedef enum{OSlwTaskStatus_Init=0,//初始化
	OSlwTaskStatus_Ready=1,//任务准备完毕 等待执行
	OSlwTaskStatus_Running=2,//任务正在运行
	OSlwTaskStatus_Wait_ForSleep,//任务等待 等待原因为睡眠
	OSlwTaskStatus_Wait_ForSignal,//任务等待 等待原因为等待信号量
	
	OSlwTaskStatus_Wait_ForGiftReceive,//任务等待 等待原因为等待消息接收完成
	
	OSlwTaskStatus_Wait_ForGroupFlag,//任务等待 等待原因等待标志组
	
	OSlwTaskStatus_Wait_ForGiftTransmit,
	OSlwTaskStatus_Frozen//任务冻结
}OSlwTaskStatusNUM;


typedef struct OSLW_TASK_GROUP_FLAG_STRUCT
{

	OSlwCoreGroupUN CurrentStatus;
	OSlwCoreGroupUN AimStatus;

	
}OSlwTaskGroupFlagSTU;


//------------------------------------------
//<结构体名>OSlwTaskSTU</结构体名>
//<结构体功能>任务控制块</结构体功能>
//------------------------------------------
typedef struct OSLW_TASK_STRUCT{
	//成员变量

	
#if !(OSLW_SIMPLE_MODE)	
	_BP BreakPoint[OSLW_TASK_BREAKPOINT_DEEP];//断点列表 表示支持的函数嵌套次数
	lw_8 Name[OSLW_TASK_NAME_LENGTH_MAX+1];//名字 长度由OSLW_TASK_NAME_LENGTH_MAX控制	
	lw_u16 BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP];//断点返回表 表示各个断点的返回值
	
#else
	_BP BreakPoint;
#endif	



	
#if OSLW_SIMPLE_LEVEL<3
	
#if OSLW_GIFT_EN	
	OSlwGiftConciergeSTU Concierge;
#endif //OSLW_GIFT_EN	
	
	lw_u32 RunningTimeSinceStart;
	lw_u32 RunningTimeSinceRunning;
	lw_u32 TimeSliceMax;
	OSlwTaskExternMemorySTU ExternMem;//任务外部存储器	
	


#elif OSLW_SIMPLE_LEVEL==3 && OSLW_GIFT_EN
	OSlwToolDListNodeConnectSTU Concierge;
#else

#endif
	
	union 
	{
		struct 
		{
			lw_u8 timer_start_flag : 1;
			lw_u16 work_time_count : 15;
			lw_u16 all_time_count : 16;
		}bits;

		lw_u32 all;
	}BackToSleep;
	
	lw_u32 SleepCount;//睡眠计数器
	OSlwTaskGroupFlagSTU TaskGroupFlag;
	//OSlwTaskPrivateMemorySTU PrivateMem;//任务私有存储器
	struct OSLW_CORE_STRUCT *pOS;//操作系统指针 指向任务所属操作系统
	
	//成员函数
	
	void (*TaskFun)(struct OSLW_TASK_STRUCT *pta);//任务函数
	/*(Ver.=0.9~)(Beg.=0.9)
#if !(OSLW_SIMPLE_MODE)
	void (*SleepFun)(struct OSLW_TASK_STRUCT *pta,lw_u32 n);//睡眠函数
	void (*WaitGRFun)(struct OSLW_TASK_STRUCT *pta);//
	void (*WaitGTFun)(struct OSLW_TASK_STRUCT *pta);//
	void (*WaitGFFun)(struct OSLW_TASK_STRUCT *pta);//
	struct OSLW_TASK_STRUCT*(*GiftTFun)
		(struct OSLW_TASK_STRUCT *pta, struct OSLW_TASK_STRUCT *pta_aim, OSlwGiftPostmanSTU *pGP, OSlwGiftUnitSTU *pgu, GiftPostmanStatusNUM method);
#endif	
*/
	lw_u8 TaskFrozenFlag : 1;
	OSlwTaskStatusNUM TaskStatus : 7;//任务状态
	
	lw_u8 Priority;//优先级
}OSlwTaskSTU;





//函数声明
extern OSlwTaskSTU OSlwInlineTaskBoring;
extern OSlwTaskSTU OSlwInlineTaskParameter;

void OSlwInlineTaskBoringExe(OSlwTaskSTU *pta);
void OSlwInlineTaskParameterExe(OSlwTaskSTU *pta);
void OSlwInlineTaskShowExe(OSlwTaskSTU *pta);
void OSlwTaskSleep(OSlwTaskSTU *pta,lw_u32 n);
void OSlwTaskWaitGroupFlag(OSlwTaskSTU *pta);
void OSlwTaskWaitGiftReceice(OSlwTaskSTU *pta);
void OSlwTaskWaitGiftTransmit(OSlwTaskSTU *pta);
OSlwGroupAllType OSlwTaskAuctionJudge(OSlwTaskSTU *pta,lw_8 task_price);
lw_u16 OSlwTaskToc(OSlwTaskSTU *pta);
#if !(OSLW_SIMPLE_LEVEL >= 3)
OSlwGiftUnitSTU * OSlwTaskGetOnceGift(OSlwTaskSTU *pta);
void OSlwTaskReceOnceGift(OSlwTaskSTU *pta);
OSlwTaskSTU *OSlwTaskGiftTransmit(OSlwTaskSTU *pta, OSlwTaskSTU *pta_aim, OSlwGiftPostmanSTU *pGP, OSlwGiftUnitSTU *pgu, GiftPostmanStatusNUM method);
#endif // !(OSLW_SIMPLE_LEVEL >= 3)

#if OSLW_SIMPLE_LEVEL == 3
void * OSlwTaskDelivery(OSlwTaskSTU *pta,OSlwMemoryBasicSTU *pmem,void *src,lw_u32 len);
#endif

void OSlwTaskInit(OSlwTaskSTU *pta,
		lw_u8 priority,
		lw_8 *name,
		lw_u16 ExMemLen,
		void *ExMemarr,
		void (*fun)(struct OSLW_TASK_STRUCT *pta)
		);



#if OSLW_SIMPLE_MODE

#define OSLW_TASK_DISPATH_EXE(PTA) do{\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
return;}while(0)
	
#else
	
#define OSLW_TASK_DISPATH_EXE(PTA) do{\
(PTA)->RunningTimeSinceRunning=0;\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
OSlwTaskDispatch((PTA)->pOS);\
}while(0)	
	
	
#endif



#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_TASK_H_*/
