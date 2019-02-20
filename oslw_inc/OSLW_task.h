/*
 * OSLW_task.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_TASK_H_
#define OSLW_TASK_H_

#include <setjmp.h>
#include "OSLW_define.h"
//------------------------------------------
//<结构体名>OSlwTaskPrivateMemorySTU</结构体名>
//<结构体功能>任务私有存储器 每片长度为TASK_PRIVATE_MEMORY_SECTION_LENGTH</结构体功能>
//------------------------------------------
typedef struct OSLW_TASK_PRIVATE_MEMORY_STRUCT{
	lw_u8 m8[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	lw_u16 m16[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	lw_u32 m32[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	lw_u64 m64[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	lw_sf msf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	lw_df mdf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
}OSlwTaskPrivateMemorySTU;


//------------------------------------------
//<结构体名>OSlwTaskExternMemorySTU</结构体名>
//<结构体功能>任务外部存储器 长度为Length 首地址为arr</结构体功能>
//------------------------------------------
typedef struct OSLW_TASK_EXTERN_MEMORY_STRUCT{
	lw_u16 Length;
	void *arr;
}OSlwTaskExternMemorySTU;

//------------------------------------------
//<枚举>OSlwTaskStatusNUM</枚举>
//<枚举功能>任务状态类型</枚举功能>
//------------------------------------------
typedef enum{OSlwTaskStatus_Init=0,//初始化
	OSlwTaskStatus_Ready,//任务准备完毕 等待执行
	OSlwTaskStatus_Running,//任务正在运行
	OSlwTaskStatus_Wait_ForSleep,//任务等待 等待原因为睡眠
	OSlwTaskStatus_Wait_ForSignal,//任务等待 等待原因为等待信号量
	OSlwTaskStatus_Wait_ForMessage,//任务等待 等待原因为等待消息
	OSlwTaskStatus_Frozen//任务冻结
}OSlwTaskStatusNUM;

//------------------------------------------
//<结构体名>OSlwTaskSTU</结构体名>
//<结构体功能>任务控制块</结构体功能>
//------------------------------------------
typedef struct OSLW_TASK_STRUCT{
	//成员变量
	lw_u8 Priority;//优先级

	lw_8 Name[OSLW_TASK_NAME_LENGTH_MAX+1];//名字 长度由OSLW_TASK_NAME_LENGTH_MAX控制

	OSlwTaskPrivateMemorySTU PrivateMem;//任务私有存储器
	
	OSlwTaskExternMemorySTU ExternMem;//任务外部存储器

	_BP BreakPoint[OSLW_TASK_BREAKPOINT_DEEP];//断点列表 表示支持的函数嵌套次数

	lw_u16 BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP];//断点返回表 表示各个断点的返回值

	OSlwTaskStatusNUM TaskStatus;//任务状态

	lw_u8 TaskFrozenFlag;
	
	lw_u32 SleepCount;//睡眠计数器
	
	struct OSLW_CORE_STRUCT *pOS;//操作系统指针 指向任务所属操作系统
	
	//成员函数
	void (*TaskFun)(struct OSLW_TASK_STRUCT *pta);//任务函数
	void (*SleepFun)(struct OSLW_TASK_STRUCT *pta,lw_u32 n);//睡眠函数
}OSlwTaskSTU;


//函数声明
extern OSlwTaskSTU OSlwInlineTaskBoring;
void OSlwInlineTaskBoringExe(OSlwTaskSTU *pta);
void OSlwTaskSleep(OSlwTaskSTU *pta,lw_u32 n);
void OSlwTaskInit(OSlwTaskSTU *pta,
		lw_u8 priority,
		lw_8 *name,
		lw_u16 ExMemLen,
		void *ExMemarr,
		void (*fun)(struct OSLW_TASK_STRUCT *pta)
		);


#define OSLW_TASK_BENGIN(A)		do{if(!SAVE(A->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1]))\
{\
	A->TaskStatus=OSlwTaskStatus_Ready;\
	LOAD(A->pOS->DispatchBreakPoint,A->Priority);\
}}while(0)
	

#define OSLW_TASK_LOOP(A) OSLW_TASK_BENGIN(A); for(;;)



#endif /* OSLW_TASK_H_*/
