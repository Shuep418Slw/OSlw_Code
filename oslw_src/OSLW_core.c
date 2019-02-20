/*
 * OSLW_core.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

//------------------------------------------
//<函数名>OSlwTaskDispatch</函数名>
//<功能说明>任务调度器 操作系统核心</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskDispatch(OSlwCoreSTU *pOS)
{

	OSLW_assert(!pOS);

	if(!(pOS->DispatchEable))//判断调度器是否上锁
	{
		while(pOS->pCurrentTask->TaskStatus != OSlwTaskStatus_Ready);//直接等待任务进入准备状态

		return ;
	}
	if(pOS->InitialFlagGroup.all)//判断初始化标志组是否为0
	{//如果不为0 说明还有尚未初始化的任务
		for(pOS->n1_Dispatch=0;pOS->n1_Dispatch<pOS->TaskRunningNum;pOS->n1_Dispatch++)//循环查找未初始化的任务  //---------可以采用算法优化--------//
		{
			pOS->Temp=pOS->TaskRunningList[pOS->n1_Dispatch];
			if((pOS->InitialFlagGroup.all & ((lw_u64)1<<(pOS->Temp))) && //如果初始化标志组的某一位为1
					(pOS->pTaskList[pOS->Temp]))//且这个任务已经注册了
			{
				pOS->pCurrentTask=pOS->pTaskList[pOS->Temp];

				if(!SAVE(pOS->DispatchBreakPoint))//等待任务跳回来
				{
					if(pOS->pCurrentTask->TaskFun)//跳转之前先判断函数指针是否为0
					{
						pOS->pCurrentTask->TaskFun(pOS->pCurrentTask);
					}//end if

				}//end if
				pOS->InitialFlagGroup.all&=(~((lw_u64)1<<(pOS->Temp)));//清除初始化标志位
				pOS->ReadyFlagGroup.all|=((lw_u64)1<<(pOS->Temp));
				pOS->pLastTask=pOS->pCurrentTask;
				pOS->pCurrentTask->TaskStatus=OSlwTaskStatus_Ready;
			}//end if
		}//end for
	}
	if(pOS->ReadyFlagGroup.all & (~((lw_u64)1<<(OSLW_TASK_NUM_MAX-1))))//屏蔽最低优先级--空闲任务 查看任务是否准备完毕
	{
		for(pOS->n2_Dispatch=0;pOS->n2_Dispatch<pOS->TaskRunningNum;pOS->n2_Dispatch++)//循环查找就绪的任务  //---------可以采用算法优化--------//
		{
			if(pOS->ReadyFlagGroup.all & ((lw_u64)1<<(pOS->TaskRunningList[pOS->n2_Dispatch]))) //如果就绪标志组的某一位为1
			{
				pOS->LastTaskIndex=pOS->CurrentTaskIndex;
				pOS->pLastTask=pOS->pCurrentTask;//记录上一次任务

				pOS->CurrentTaskIndex=pOS->TaskRunningList[pOS->n2_Dispatch];
				pOS->pCurrentTask=pOS->pTaskList[pOS->CurrentTaskIndex];//更新本次任务
				pOS->pCurrentTask->TaskStatus=OSlwTaskStatus_Running;//更改任务状态
				LOAD(pOS->pCurrentTask->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1],pOS->LastTaskIndex);//任务跳转
			}//end if
		}//end for
	}//end if
	else//说明当前没有任务准备就绪 执行空闲任务
	{
		pOS->pLastTask=pOS->pCurrentTask;
		pOS->LastTaskIndex=pOS->CurrentTaskIndex;
		pOS->pCurrentTask=pOS->pTaskList[OSLW_TASK_NUM_MAX-1];
		LOAD(pOS->pCurrentTask->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1],pOS->LastTaskIndex);
	}

	return;
}

//------------------------------------------
//<函数名>OSlwClockCallBack</函数名>
//<功能说明>操作系统时钟节拍回掉 此函数请放入定时器中断!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwClockCallBack(OSlwCoreSTU *pOS)
{
	OSLW_assert(!pOS);

	lw_u8 n=0;
	lw_u8 tindex=0;
	OSlwTaskSTU *pta;
	if(pOS->CoreStatus!=OSlwCoreStatus_Running)
		return;
	for(;n<pOS->TaskRunningNum;n++)//循环任务列表
	{
		tindex=pOS->TaskRunningList[n];
		pta=pOS->pTaskList[tindex];//取得任务控制类
		//判断任务状态
		if(pta->TaskStatus==OSlwTaskStatus_Wait_ForSleep)//如果是 等待睡眠状态
		{
			if(!(--(pta->SleepCount)))//睡眠寄存器-- 如果为0，说明的是时间到了
			{
				pOS->ReadyFlagGroup.all|=(1<<tindex);
				pta->TaskStatus=OSlwTaskStatus_Ready;//转换任务状态 为准备态
			}//end if

		}//end if

	}//end for

	pOS->pTaskList[OSLW_TASK_NUM_MAX-1]->PrivateMem.m8[0]++;


}

//------------------------------------------
//<函数名>OSlwCoreInitial</函数名>
//<功能说明>操作系统内核初始化函数 初始化内核寄存器 创建系统内部任务</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------

void OSlwCoreInitial(OSlwCoreSTU *pOS)
{
	OSLW_assert(!pOS);
	memset(pOS,0,sizeof(OSlwCoreSTU));//清空内容
	memset(pOS->TaskRunningList,OSLW_TASK_NUM_MAX+1,sizeof(pOS->TaskRunningList));//执行表初始化

	//函数指针初始化
	pOS->ClockCBFun=OSlwClockCallBack;
	pOS->DispatchFun=OSlwTaskDispatch;
	pOS->TaskAppendFun=OSlwTaskAppend;
	pOS->StartFun=OSlwCoreStart;
	//内部任务 空闲任务
	OSlwTaskInit(//空闲任务初始化
			&OSlwInlineTaskBoring,
			OSLW_TASK_NUM_MAX-1,
			"~_~",
			0,
			(void *)0,
			OSlwInlineTaskBoringExe
			);
			
	OSlwTaskAppend(pOS,&OSlwInlineTaskBoring);//空闲任务追加


}

//------------------------------------------
//<函数名>OSlwTaskAppend</函数名>
//<功能说明>操作系统追加任务 !!!!!!该函数不管任务控制块的初始化!!!!!!!!!</功能说明>
//<输入说明>pOS:this pta:追加任务控制块</输入说明>
//<输出说明>pOS:this</输出说明>
//------------------------------------------
OSlwCoreSTU *OSlwTaskAppend(OSlwCoreSTU *pOS,OSlwTaskSTU *pta)
{
	OSLW_assert(!pOS);
	OSLW_assert(!pta);

	if(pOS->pTaskList[pta->Priority])//判断该任务号是否已经被注册
	{
		pOS->CoreError=OSlwCoreError_PriorityRedefine;//内核异常置起
		return pOS;//返回
	}

	pta->pOS=pOS;//任务所述的操作系统指针


	pOS->pTaskList[pta->Priority]=pta;//任务控制块存入相应位置

	pOS->TaskRunningList[pOS->TaskRunningNum]=pta->Priority;//任务优先级保存入运行表格
	
	pOS->TaskRunningNum++;//任务运行数+1
	
	_quicksort(pOS->TaskRunningList,pOS->TaskRunningNum+20,sizeof(pOS->TaskRunningList[0]),comp);//对运行表格进行排序 自小向大

	pOS->InitialFlagGroup.all|=((lw_u64)1<<pta->Priority);//内核初始化标志组置起

	return pOS;
}

//------------------------------------------
//<函数名>OSlwCoreStart</函数名>
//<功能说明>OSLW开始 !!!!!请在所以任务控制块与操作系统内核初始化完成之后再调用!!!!!!</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwCoreStart(OSlwCoreSTU *pOS)
{

	pOS->DispatchEable=1;//调度器使能
	pOS->CoreStatus=OSlwCoreStatus_Running;//任务内核转换为运行态
	pOS->DispatchFun(pOS);//执行任务调度

	while(1)//!!!!!!!!!!!!正常情况下不会进入这个循环!!!!!!!!!!!!!!!!!!!!!
	{
		;
	}

}


//------------------------------------------
//<函数名>comp</函数名>
//<功能说明>比较函数 用于优先级排序</功能说明>
//------------------------------------------
lw_16 comp(const void*a,const void*b)
{

	return (*(lw_u8*)a-*(lw_u8*)b);

}

//------------------------------------------
//<函数名>_quicksort</函数名>
//<功能说明>快速排序</功能说明>
//------------------------------------------
void _quicksort (void *const pbase, size_t total_elems, size_t size,_compar_fn cmp)
{
  char *base_ptr = (char *) pbase;

  const size_t max_thresh = MAX_THRESH * size;

  if (total_elems == 0)
    /* Avoid lossage with unsigned arithmetic below.  */
    return;

  if (total_elems > MAX_THRESH)
    {
      char *lo = base_ptr;
      char *hi = &lo[size * (total_elems - 1)];
      stack_node stack[STACK_SIZE];
      stack_node *top = stack;

      PUSH (NULL, NULL);

      while (STACK_NOT_EMPTY)
        {
          char *left_ptr;
          char *right_ptr;
	  char *mid = lo + size * ((hi - lo) / size >> 1);

	  if ((*cmp) ((void *) mid, (void *) lo) < 0)
	    SWAP (mid, lo, size);
	  if ((*cmp) ((void *) hi, (void *) mid) < 0)
	    SWAP (mid, hi, size);
	  else
	    goto jump_over;
	  if ((*cmp) ((void *) mid, (void *) lo) < 0)
	    SWAP (mid, lo, size);
	jump_over:;

	  left_ptr  = lo + size;
	  right_ptr = hi - size;
	  do
	    {
	      while ((*cmp) ((void *) left_ptr, (void *) mid) < 0)
		left_ptr += size;

	      while ((*cmp) ((void *) mid, (void *) right_ptr) < 0)
		right_ptr -= size;

	      if (left_ptr < right_ptr)
		{
		  SWAP (left_ptr, right_ptr, size);
		  if (mid == left_ptr)
		    mid = right_ptr;
		  else if (mid == right_ptr)
		    mid = left_ptr;
		  left_ptr += size;
		  right_ptr -= size;
		}
	      else if (left_ptr == right_ptr)
		{
		  left_ptr += size;
		  right_ptr -= size;
		  break;
		}
	    }
	  while (left_ptr <= right_ptr);

          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
		/* Ignore both small partitions. */
                POP (lo, hi);
              else
		/* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
	    /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
	      /* Push larger left partition indices. */
              PUSH (lo, right_ptr);
              lo = left_ptr;
            }
          else
            {
	      /* Push larger right partition indices. */
              PUSH (left_ptr, hi);
              hi = right_ptr;
            }
        }
    }

#define min(x, y) ((x) < (y) ? (x) : (y))
  {
    char *const end_ptr = &base_ptr[size * (total_elems - 1)];
    char *tmp_ptr = base_ptr;
    char *thresh = min(end_ptr, base_ptr + max_thresh);
    char *run_ptr;
    for (run_ptr = tmp_ptr + size; run_ptr <= thresh; run_ptr += size)
      if ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0)
        tmp_ptr = run_ptr;

    if (tmp_ptr != base_ptr)
      SWAP (tmp_ptr, base_ptr, size);

    run_ptr = base_ptr + size;
    while ((run_ptr += size) <= end_ptr)
      {
	tmp_ptr = run_ptr - size;
	while ((*cmp) ((void *) run_ptr, (void *) tmp_ptr) < 0)
	  tmp_ptr -= size;

	tmp_ptr += size;
        if (tmp_ptr != run_ptr)
          {
            char *trav;

	    trav = run_ptr + size;
	    while (--trav >= run_ptr)
              {
                char c = *trav;
                char *hi, *lo;

                for (hi = lo = trav; (lo -= size) >= tmp_ptr; hi = lo)
                  *hi = *lo;
                *hi = c;
              }
          }
      }
  }
}
