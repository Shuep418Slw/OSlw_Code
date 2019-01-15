/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_core.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"


#if OSLW_OS_ENABLE


//------------------------------------------
//<函数名>OSlwTaskDispatch</函数名>
//<功能说明>任务调度器 操作系统核心</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskDispatch(OSlwCoreSTU *_pOS)
{
    static OSlwCoreSTU *pOS;
    OSLW_assert(!_pOS);

    pOS = _pOS;

    if(!(pOS->DispatchEable))//判断调度器是否上锁
    {
        while(pOS->pCurrentTask->TaskStatus != OSlwTaskStatus_Ready);//直接等待任务进入准备状态

        return ;
    }
		
    if(pOS->InitialFlagGroup.all)//判断初始化标志组是否为0
    {   //如果不为0 说明还有尚未初始化的任务
        for(pOS->n1_Dispatch=0; 
			pOS->n1_Dispatch<pOS->TaskRunningNum
#if OSLW_SIMPLE_MODE
			-1
#endif			
			; pOS->n1_Dispatch++) //循环查找未初始化的任务  //---------可以采用算法优化--------//
        {
            pOS->Temp=pOS->TaskRunningList[pOS->n1_Dispatch];
            if((pOS->InitialFlagGroup.all & ((lw_u64)1<<(pOS->Temp))) && //如果初始化标志组的某一位为1
                    (pOS->pTaskList[pOS->Temp]))//且这个任务已经注册了
            {
                pOS->pCurrentTask=pOS->pTaskList[pOS->Temp];
#if !OSLW_SIMPLE_MODE
                if(!_OSLW_SAVE(pOS->DispatchBreakPoint))//等待任务跳回来
                {
                    if(pOS->pCurrentTask->TaskFun)//跳转之前先判断函数指针是否为0
                    {
                        pOS->pCurrentTask->TaskFun(pOS->pCurrentTask);
                    }//end if

                }//end if
#else
								if(pOS->pCurrentTask->TaskFun)//跳转之前先判断函数指针是否为0
								{
										pOS->pCurrentTask->TaskFun(pOS->pCurrentTask);
								}//end if								
#endif
								
                pOS->InitialFlagGroup.all&=(~((lw_u64)1<<(pOS->Temp)));//清除初始化标志位
                pOS->ReadyFlagGroup.all|=((lw_u64)1<<(pOS->Temp));
								
#if	!OSLW_SIMPLE_MODE															
                pOS->pLastTask=pOS->pCurrentTask;
                pOS->pCurrentTask->TaskStatus=OSlwTaskStatus_Ready;
#endif
								
            }//end if
        }//end for
    }
#if (OSLW_SIMPLE_MODE)
	do{	
#endif	//(OSLW_SIMPLE_MODE)		
    pOS->_flag_BK = pOS->ReadyFlagGroup.all & (~((lw_u64)1 << (OSLW_TASK_NUM_MAX - 1)));
    if(pOS->_flag_BK)//屏蔽最低优先级--空闲任务 查看任务是否准备完毕
    {
        pOS->_ReadyTaskIndex = 0;
        if (pOS->TaskRunningNum > __OSLW_LOG2_TASK_NUM  )//如果任务数量比较多 说明二分法搜索时间复杂度较低
        {

            //二分法
            for (pOS->n2_Dispatch = 0; pOS->n2_Dispatch < __OSLW_LOG2_TASK_NUM - 2; pOS->n2_Dispatch++)//二分法折半到4 直接switch case
            {
                /*(Ver.=0.9~)(Beg.=0.9)
                	TASK_NUM == 8 为例
                	ReadyFlagGroup=0b10010010
                	ReadyFlagGroup & 0b00001111 相当于取出低4位
                	判断低四位是否为0
                	如果低四位 不为0 说明 低四位有被置起的数据
                	反之 被置起的位在高4

                */
                if (pOS->_flag_BK & pOS->_mask_list[pOS->n2_Dispatch])
                {
                    //如果不为0
                    pOS->_flag_BK &= pOS->_mask_list[pOS->n2_Dispatch];//屏蔽高位
                }
                else
                {
                    pOS->_flag_BK = (pOS->_flag_BK >> pOS->_move_reg[pOS->n2_Dispatch]) & pOS->_mask_list[pOS->n2_Dispatch];//得到高位
                    pOS->_ReadyTaskIndex += pOS->_move_reg[pOS->n2_Dispatch];
                }

            }//end for

            if (pOS->_flag_BK < 16)
            {
                switch (pOS->_flag_BK)
                {
                case 8:
                    pOS->_ReadyTaskIndex += 3;
                    break;
                case 4:
                    pOS->_ReadyTaskIndex += 2;
                    break;
                case 12:
                    pOS->_ReadyTaskIndex += 2;
                    break;
                case 2:
                    pOS->_ReadyTaskIndex += 1;
                    break;
                case 6:
                    pOS->_ReadyTaskIndex += 1;
                    break;
                case 10:
                    pOS->_ReadyTaskIndex += 1;
                    break;
                case 14:
                    pOS->_ReadyTaskIndex += 1;
                    break;

                default://末尾是1
                    break;
                }
            }
            else
            {
                OSLW_assert(1);//不应该>=16
            }
        }// end if (pOS->TaskRunningNum >= __OSLW_LOG2_TASK_NUM - 1 )
        else
        {
            for (pOS->n2_Dispatch = 0; pOS->n2_Dispatch<pOS->TaskRunningNum; pOS->n2_Dispatch++) //循环查找就绪的任务  //---------采用算法优化--------//
            {
                pOS->_ReadyTaskIndex = pOS->TaskRunningList[pOS->n2_Dispatch];
                if (pOS->ReadyFlagGroup.all & ((lw_u64)1 << (pOS->_ReadyTaskIndex))) //如果就绪标志组的某一位为1
                {
                    break;
                }//end if
            }//end for
        }//end else
				
				
#if	!OSLW_SIMPLE_MODE							
        pOS->LastTaskIndex = pOS->CurrentTaskIndex;
        pOS->pLastTask = pOS->pCurrentTask;//记录上一次任务
#endif
				
				
        pOS->CurrentTaskIndex = pOS->_ReadyTaskIndex;
        pOS->pCurrentTask = pOS->pTaskList[pOS->CurrentTaskIndex];//更新本次任务
        //pOS->pCurrentTask->TaskStatus=OSlwTaskStatus_Running;//更改任务状态
				
        _OSLW_LOAD(pOS->pCurrentTask->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP - 1], pOS->LastTaskIndex,pOS->pCurrentTask);//任务跳转

    }//end if
    else//说明当前没有任务准备就绪 执行空闲任务
    {
        
#if (OSLW_SIMPLE_MODE)			
				pOS->pCurrentTask=pOS->pTaskList[OSLW_TASK_NUM_MAX-1];
				break;
#else
			  pOS->pLastTask=pOS->pCurrentTask;
        pOS->LastTaskIndex=pOS->CurrentTaskIndex;
        pOS->pCurrentTask=pOS->pTaskList[OSLW_TASK_NUM_MAX-1];
        _OSLW_LOAD(pOS->pCurrentTask->BreakPoint[OSLW_TASK_BREAKPOINT_DEEP-1],pOS->LastTaskIndex,pOS->pCurrentTask);
#endif//(OSLW_SIMPLE_MODE)	
    }

		
		
		
		
#if (OSLW_SIMPLE_MODE)
	}while(1);
#endif//(OSLW_SIMPLE_MODE)					
		
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

    lw_u8 n=0;
    lw_u8 tindex=0;
    OSlwTaskSTU *pta;

    OSLW_assert(!pOS);

    if(pOS->CoreStatus!=OSlwCoreStatus_Running)
        return;
    pOS->NormalRunningTimerCount++;

	if (pOS->_CPU_Usage_count >= OSLW_CPU_USAGE_MAX)
	{
		pOS->_CPU_Usage_count = 0;
		pOS->CPU_Usage = pOS->_CPU_Usage_cal_reg;
		pOS->_CPU_Usage_cal_reg = OSLW_CPU_USAGE_MAX;
	}
	else
	{
		pOS->_CPU_Usage_count++;
		if (pOS->CurrentTaskIndex==OSLW_TASK_NUM_MAX-1)//如果是空闲任务
		{
			pOS->_CPU_Usage_cal_reg--;//减计数 如果完全是空闲任务 计数器会减到0
		}
	}

    for(; n<pOS->TaskRunningNum-1; n++) //循环任务列表
    {
        tindex=pOS->TaskRunningList[n];
        pta=pOS->pTaskList[tindex];//取得任务控制类
        //判断任务状态
        if(pta->TaskFrozenFlag) continue;//如果是冻结状态 不用管


		if (pta->BackToSleep.bits.timer_start_flag)
		{
			pta->BackToSleep.bits.work_time_count++;
		}

        switch((pta->TaskStatus))
        {
					
					
        case OSlwTaskStatus_Running:
#if !(OSLW_SIMPLE_LEVEL >= 3)
            pta->RunningTimeSinceStart++;
            pta->RunningTimeSinceRunning++;
						break;
#endif // !(OSLW_SIMPLE_LEVEL >= 3)
				
				
				
        case OSlwTaskStatus_Wait_ForSleep://等待睡眠状态
            if(!(--(pta->SleepCount)))//睡眠寄存器-- 如果为0，说明的是时间到了
            {
                OSlwCoreTaskIsReady(pOS,pta,tindex);//转换任务状态 为准备态
            }//end if
            break;
						
#if OSLW_GIFT_EN						
						
#if OSLW_SIMPLE_LEVEL < 3
        case OSlwTaskStatus_Wait_ForGiftReceive://等待接收状态
            if(pta->Concierge.giftR.List.NowLen > 1)//如果接收数量不为0 就可以准备就绪
            {
                OSlwCoreTaskIsReady(pOS,pta,tindex);//转换任务状态 为准备态
            }
            break;
#elif OSLW_SIMPLE_LEVEL == 3
        case OSlwTaskStatus_Wait_ForGiftReceive://等待接收状态
            if(pta->Concierge.pNext && pta->Concierge.pLast)//如果接收数量不为0 就可以准备就绪
            {
                OSlwCoreTaskIsReady(pOS,pta,tindex);//转换任务状态 为准备态
            }
            break;
						
#endif // !(OSLW_SIMPLE_LEVEL >= 4)						
						
#endif //OSLW_GIFT_EN						
						
            //逐渐放弃 鸡肋
#if 0
        case OSlwTaskStatus_Wait_ForGiftTransmit://等待发送完成
            if(!(pta->Concierge.giftT.PostmanFlag.all))//如果标志组==0 代表全部发送相应完成
            {
                OSlwCoreTaskIsReady(pOS,pta,tindex);//转换任务状态 为准备态
            }
            break;
#endif
						
						
						
						
        case OSlwTaskStatus_Wait_ForGroupFlag:
            if(pta->TaskGroupFlag.CurrentStatus.all & pta->TaskGroupFlag.AimStatus.all)
            {
                OSlwCoreTaskIsReady(pOS,pta,tindex);//转换任务状态 为准备态
            }
            break;
						
						
						
						
        default:
            break;


        }//end switch

    }//end for


    for(n=0; n<pOS->TimerCtrl.Length; n++)
    {
        OSlwTimerClockCallBack(pOS->TimerCtrl.pTimerList[n]);
    }

    pOS->pTaskList[OSLW_TASK_NUM_MAX - 1]->TaskGroupFlag.AimStatus.all = 0;



}



//------------------------------------------
//<函数名>OSlwCoreInitial</函数名>
//<功能说明>操作系统内核初始化函数 初始化内核寄存器 创建系统内部任务</功能说明>
//<输入说明>pOS:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwCoreInitial(OSlwCoreSTU *pOS)
{
    int i;

    OSLW_assert(!pOS);
    memset(pOS,0,sizeof(OSlwCoreSTU));//清空内容
    memset(pOS->TaskRunningList,OSLW_TASK_NUM_MAX+1,sizeof(pOS->TaskRunningList));//执行表初始化

	pOS->_CPU_Usage_cal_reg = OSLW_CPU_USAGE_MAX;

    //任务调度器初始化赋值
#if OSLW_TASK_NUM_MAX<=8
    pOS->_mask_list[0] = 0x0f;

    pOS->_move_reg[0] = 4;

#elif OSLW_TASK_NUM_MAX>8 && OSLW_TASK_NUM_MAX<=16
    pOS->_mask_list[0] = 0x00ff;
    pOS->_mask_list[1] = 0x000f;

    pOS->_move_reg[0] = 8;
    pOS->_move_reg[1] = 4;
#elif OSLW_TASK_NUM_MAX>16 && OSLW_TASK_NUM_MAX<=32
    pOS->_mask_list[0] = 0x0000ffff;
    pOS->_mask_list[1] = 0x000000ff;
    pOS->_mask_list[2] = 0x0000000f;

    pOS->_move_reg[0] = 16;
    pOS->_move_reg[1] = 8;
    pOS->_move_reg[2] = 4;

#elif OSLW_TASK_NUM_MAX>32 && OSLW_TASK_NUM_MAX<=64
    pOS->_mask_list[0] = 0x00000000ffffffff;
    pOS->_mask_list[1] = 0x000000000000ffff;
    pOS->_mask_list[2] = 0x00000000000000ff;
    pOS->_mask_list[3] = 0x000000000000000f;

    pOS->_move_reg[0] = 32;
    pOS->_move_reg[1] = 16;
    pOS->_move_reg[2] = 8;
    pOS->_move_reg[3] = 4;

#else
#error "TASK NUM IS OVER"
#endif


    //函数指针初始化
    pOS->StartFun = OSlwCoreStart;
    pOS->TimerAppendFun = OSlwTimerAppend;
    pOS->TaskAppendFun = OSlwTaskAppend;
    /*(Ver.=0.9~)(Beg.=0.9)
    #if !(OSLW_SIMPLE_MODE)
    pOS->ClockCBFun=OSlwClockCallBack;
    pOS->DispatchFun=OSlwTaskDispatch;
    pOS->TaskAppendFun=OSlwTaskAppend;
    pOS->StartFun=OSlwCoreStart;
    pOS->TimerAppendFun=OSlwTimerAppend;
    #endif
    */
    //内存控制初始化

//	//参数控制初始化
//	for(i=0;i<OSLW_CORE_PARA_CTRL_NUM;i++)
//	{
//
//		pOS->ParaCtrlList[i].pOS=pOS;
//#if !(OSLW_SIMPLE_MODE)
//		pOS->ParaCtrlList[i].CmdExeFun=OSlwParaCtrlCmdExe;
//#endif
//	}

    //内部任务 空闲任务

    OSlwTaskInit(//空闲任务初始化
        &(pOS->OSlwInlineTaskBoring),
        OSLW_TASK_NUM_MAX-1,
        "~_~",
        0,
        (void *)0,
        OSlwInlineTaskBoringExe
    );
    OSlwTaskAppend(pOS, &(pOS->OSlwInlineTaskBoring));//空闲任务追加


    /*(Ver.=0.9~)(Beg.=0.9)
    	OSlwTaskInit(
    			&OSlwInlineTaskParameter,
    			1,
    			"T_T",
    			sizeof(OSlwParaCtrlSTU)*OSLW_CORE_PARA_CTRL_NUM,
    			(void *)(pOS->ParaCtrlList),
    			OSlwInlineTaskParameterExe
    			);
    	OSlwTaskAppend(pOS,&OSlwInlineTaskParameter);//参数管理任务
    */
/*(Ver.=0.9~)(Beg.=0.9)
    for(i=0; i<OSLW_CORE_PARA_PAGE_NUM; i++)
    {

        OSlwRealParaListInitial(&pOS->ParaBook[i],"");
        pOS->ParaBook[i].PageId=i;
        pOS->ParaBook[i].pOS=pOS;

    }
*/
#if OSLW_TASK_COMMUN_ENABLE //如果使能通信任务
//	for(i=0;i<OSLW_CORE_COMMUN_LIST_LENGTH;i++)//循环通信控制器列表
//	{
//		//指针初始化
//		pOS->Comm[i].pRx=pOS->Comm[i].RxList;
//		pOS->Comm[i].pTx=pOS->Comm[i].pTxTail=pOS->Comm[i].TxList;

//		//滑窗初始化
//		pOS->Comm[i].ComRX.AppendFun=OSlwToolSlipwinAppend;
//		pOS->Comm[i].ComRX.MDataLen_forSize_t=(sizeof(lw_u8)*OSLW_COMMUN_RX_LENGTH);
//		pOS->Comm[i].ComRX.pBufferData=pOS->Comm[i].RxListBuf;
//		pOS->Comm[i].ComRX.pMasterData=pOS->Comm[i].RxList;

//		//函数指针初始化
//		pOS->Comm[i].isReadyFun	= OSlwCommunIsReady;
//		pOS->Comm[i].TxReadyFun = OSlwCommunTxReady;
//
//		//操作系统指针
//		pOS->Comm[i].pOS=pOS;
//		pOS->Comm[i].Address=OSLW_COMMUN_ADDRESS;//地址
//
//		//通信发送模块初始化 用于收到数据之后向通信任务发送
//		OSlwGiftTransmitInitial(&(pOS->Comm[i].giftT));
//		pOS->Comm[i].giftT.IdleIndexFun=OSlwGiftTransmitIdleIndex;
//	}
//
//	OSlwTaskInit(
//			&OSlwInlineTaskCommun,
//			2,
//			"^v^",
//			sizeof(OSlwCommunSTU),
//			(void *)(&(pOS->Comm)),
//			OSlwInlineTaskCommunExe
//			);
//	OSlwTaskAppend(pOS,&OSlwInlineTaskCommun);//系统通信任务

#endif

#if OSLW_MEMORY_ENABLE
//逐渐放弃 鸡肋

#if 0
    OSlwMemoryMapInital(&(pOS->GiftMem),(pOS->GiftBuf), OSLW_CORE_GIFT_NUM,sizeof(OSlwGiftUnitSTU),sizeof(pOS->GiftBuf));
    OSlwMemoryMapInital(&(pOS->PostmanMem), (pOS->PostmanBuf), OSLW_CORE_POSTMAN_NUM, sizeof(OSlwGiftPostmanSTU), sizeof(pOS->PostmanBuf));
#endif


#endif




#if OSLW_TASK_SHOW_ENABLE

    pOS->Show.pOS=pOS;

    OSlwShowParaCtrlInitial(&(pOS->Show.PageList[0]),&(pOS->Show)
                            ,OSLW_SHOW_WIN_LENGTH,0,(void *)(&pOS->ParaCtrlList[0]),
                            OSlwShowParaCtrlCursorLock_Pvt,OSlwShowParaCtrlCursorUnLock_Pvt,OSlwShowParaCtrlCursorInput_Pvt
                            //NULL,NULL,NULL
                            ,NULL,NULL,OSlwDriverShowDrawCursor);

    OSlwShowParaCtrlInitial(&(pOS->Show.PageList[1]),&(pOS->Show),
                            OSLW_SHOW_WIN_LENGTH,0,(void *)(&pOS->ParaCtrlList[1]),
                            NULL,NULL,NULL,
                            NULL,NULL,OSlwDriverShowDrawCursor);

    pOS->Show.Cursor.pSPC=pOS->Show.PageList;

    pOS->Show.CursorMoveFun=OSlwShowCursorMoveFun;
    pOS->Show.DrawPageNumFun=OSlwDriverShowDrawPageNum;
    pOS->Show.InforProcFun=OSlwShowInforProc;
    pOS->Show.PageTrunFun=OSlwShowPageTrun;
    pOS->Show.UpdateFun=OSlwShowUpdate;

#if OSLW_SHOW_SCREEN == OSLW_SHOW_NO_KEEP

    OSlwTimerShowIrp.Mode=OSlwTimerMode_Continue;
    OSlwTimerShowIrp.Count=OSlwTimerShowIrp.CountMax=500/OSLW_CORE_TICK_MS;
    OSlwTimerShowIrp.AlarmCBFun=OSlwTimerShowIrpALFun;
    OSlwTimerShowIrp.pLeader=(void *)pOS;

    OSlwTimerShowIrp.ExtMem.Data=sizeof(pOS->Show.ShowBuffer);
    OSlwTimerShowIrp.ExtMem.pData=(void *)pOS->Show.ShowBuffer;

    OSlwTimerAppend(pOS,&OSlwTimerShowIrp);
#endif


    OSlwTimerShowUpdate.Mode=OSlwTimerMode_Continue;
    OSlwTimerShowUpdate.Count=OSlwTimerShowUpdate.CountMax=1000/OSLW_CORE_TICK_MS;
    OSlwTimerShowUpdate.AlarmCBFun=OSlwTimerShowUpdateALFun;
    OSlwTimerShowUpdate.pLeader=(void *)pOS;

    OSlwTimerShowUpdate.ExtMem.Data=sizeof(OSlwTaskSTU);
    OSlwTimerShowUpdate.ExtMem.pData=(void *)pta_Show;

    OSlwTimerAppend(pOS,&OSlwTimerShowUpdate);

    OSlwTaskInit(
        &OSlwInlineTaskShow,
        OSLW_TASK_NUM_MAX-2,
        "=v=",
        sizeof(OSlwShowSTU),
        (void *)(&(pOS->Show)),
        OSlwInlineTaskShowExe
    );
    OSlwTaskAppend(pOS,&OSlwInlineTaskShow);//系统显示任务



#endif

#if OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN

	OSlwStrDbgInitial(&(pOS->StrDbg));

	pOS->StrDbgFun = OSlwStrDbgFun;

#endif // OSLW_SIMPLE_LEVEL<4 && OSLW_STR_DBG_EN

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
    _quicksort(pOS->TaskRunningList,pOS->TaskRunningNum,sizeof(pOS->TaskRunningList[0]),comp);//对运行表格进行排序 自小向大
    pOS->InitialFlagGroup.all|=((lw_u64)1<<pta->Priority);//内核初始化标志组置起

    return pOS;
}

//------------------------------------------
//<函数名>OSlwParaAppend</函数名>
//<功能说明>参数追加任务 向OS追加参数</功能说明>
//<输入说明>pOS:this num:参数列表编号 常见有0(输入寄存器参数) 1(保持寄存器参数) *p:参数指针</输入说明>
//<输出说明>this</输出说明>
//------------------------------------------
/*(Ver.=0.9~)(Beg.=0.9)
OSlwCoreSTU* OSlwParaAppend(OSlwCoreSTU *pOS,lw_u8 num,struct OSLW_PARAMETER_STRUCT *p)
{

	OSlwParaCtrlSTU *pPC;

	OSLW_assert(!pOS);
	OSLW_assert(!p);

	num= num % OSLW_CORE_PARA_CTRL_NUM;//% 保证列表编号不会越界
	pPC = & (pOS->ParaCtrlList[num]);// 取得系统参数控制列表的指针
	if (pPC->ParaLength >= OSLW_PARA_LIST_LENGRH)//如果参数数量已经超过列表总长度
	{
		pOS->CoreError=OSlwCoreError_ParaListLengthOver;//设置错误标志
		return pOS;
	}
	p->ParaId=pPC->ParaLength;//设置参数编号
	pPC->pParaList[pPC->ParaLength]=p;//保存参数
	pPC->ParaLength++;//数量+1

	return pOS;

}
*/


/*(Ver.=0.9~)(Beg.=0.9)
OSlwCoreSTU* OSlwParaAppend(OSlwCoreSTU *pOS,lw_u8 PageId,OSlwParameterSTU *pP)
{
    OSlwParaListCtrlSTU *pPLC;

    OSLW_assert(!pOS);
    OSLW_assert(!pP);

    PageId= PageId % OSLW_CORE_PARA_PAGE_NUM;
    pPLC = & (pOS->ParaBook[PageId]);// 取得系统参数控制列表的指针

    if (pPLC->ParaLength >= OSLW_PARA_LIST_LENGRH)//如果参数数量已经超过列表总长度
    {
        pOS->CoreError=OSlwCoreError_ParaListLengthOver;//设置错误标志
        return pOS;
    }

    pP->ParaId=pOS->ParaIdCount++;
    pPLC->pParaList[pPLC->ParaLength]=pP;
    pPLC->ParaLength++;

    return pOS;

}
*/

OSlwCoreSTU* OSlwTimerAppend(OSlwCoreSTU *pOS,OSlwTimerSTU *pT)
{
    if(pOS->TimerCtrl.Length >= OSLW_TIMER_CTRL_LIST_LEN)
    {
        pOS->CoreError=OSlwCoreError_TimerListLengthOver;
        return NULL;
    }

    pT->ID=pOS->TimerCtrl.Length;

    pOS->TimerCtrl.pTimerList[pOS->TimerCtrl.Length++]=pT;

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
#if !(OSLW_SIMPLE_MODE)
    OSlwTaskDispatch(pOS);//执行任务调度
#else
    pOS->OSlwInlineTaskBoring.TaskFun(&(pOS->OSlwInlineTaskBoring));
#endif
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
//<功能说明>快速排序</功能说明>//
//by 互联网
//------------------------------------------
void _quicksort (void *const pbase, size_t total_elems, size_t size,_compar_fn cmp)
{
    char *base_ptr = (char *) pbase;

    const size_t max_thresh = MAX_THRESH * size;

    if (total_elems == 0)
        /*(Ver.=0.9~)(Beg.=0.9) Avoid lossage with unsigned arithmetic below.  */
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
jump_over:
            ;

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
                    /*(Ver.=0.9~)(Beg.=0.9) Ignore both small partitions. */
                    POP (lo, hi);
                else
                    /*(Ver.=0.9~)(Beg.=0.9) Ignore small left partition. */
                    lo = left_ptr;
            }
            else if ((size_t) (hi - left_ptr) <= max_thresh)
                /*(Ver.=0.9~)(Beg.=0.9) Ignore small right partition. */
                hi = right_ptr;
            else if ((right_ptr - lo) > (hi - left_ptr))
            {
                /*(Ver.=0.9~)(Beg.=0.9) Push larger left partition indices. */
                PUSH (lo, right_ptr);
                lo = left_ptr;
            }
            else
            {
                /*(Ver.=0.9~)(Beg.=0.9) Push larger right partition indices. */
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

#endif

