/*
 * OSLW_task.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

OSlwTaskSTU OSlwInlineTaskBoring;

//------------------------------------------
//<函数名>OSlwTaskInit</函数名>
//<功能说明>任务初始化函数</功能说明>
//<输入说明>pta:任务控制块指针  priority:优先级  name:名字(长度:OSLW_TASK_NAME_LENGTH_MAX) ExMemLen:外部存储长度 ExMemarr:外部存储地址 fun:任务函数</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskInit(OSlwTaskSTU *pta,
		lw_u8 priority,
		lw_8 *name,
		lw_u16 ExMemLen,
		void *ExMemarr,
		void (*fun)(struct OSLW_TASK_STRUCT *pta)
		)
{
	memset(pta,0,sizeof(OSlwTaskSTU));//清空任务控制块
	pta->Priority=priority;//优先级
	strncpy(pta->Name,name,OSLW_TASK_NAME_LENGTH_MAX);//名字
	pta->TaskFun=fun;//任务函数
	pta->SleepFun=OSlwTaskSleep;//睡眠函数
	if(ExMemarr!=NULL && ExMemLen!=0)//如果 外部存储器有意义
	{
		pta->ExternMem.Length=ExMemLen;
		pta->ExternMem.arr=ExMemarr;
	}


}

//------------------------------------------
//<函数名>OSlwInlineTaskBoringExe</函数名>
//<功能说明>空闲任务函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwInlineTaskBoringExe(OSlwTaskSTU *pta)
{

	
	OSLW_TASK_BENGIN(pta);//声明 空闲任务初始化完成
	for(;;)//死循环
	{
		
		while(!(pta->PrivateMem.m8[0])){;}//等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
		pta->PrivateMem.m8[0]=0;//清零
		pta->pOS->DispatchFun(pta->pOS);//执行任务调度
						
	}




}

//------------------------------------------
//<函数名>OSlwTaskSleep</函数名>
//<功能说明>任务睡眠函数</功能说明>
//<输入说明>pta:this, n :睡眠节拍数</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskSleep(OSlwTaskSTU *pta,lw_u32 n)
{
	pta->SleepCount=n;
	pta->TaskStatus=OSlwTaskStatus_Wait_ForSleep;
	pta->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<(pta->Priority));
	pta->pOS->DispatchFun(pta->pOS);
}
