/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_task.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

#if OSLW_OS_ENABLE

//OSlwTaskSTU OSlwInlineTaskBoring,*pta_last=&OSlwInlineTaskBoring;

//OSlwTaskSTU OSlwInlineTaskParameter,*pta_1=&OSlwInlineTaskParameter;

//------------------------------------------
//<函数名>OSlwTaskInit</函数名>
//<功能说明>任务初始化函数</功能说明>
//<输入说明>pta:任务控制块指针  priority:优先级  name:名字(长度:OSLW_TASK_NAME_LENGTH_MAX) ExMemLen:外部存储长度 ExMemarr:外部存储地址 fun:任务函数</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskInit(
    OSlwTaskSTU *pta,
    lw_u8 priority,
    lw_8 *name,
    lw_u16 ExMemLen,
    void *ExMemarr,
    void (*fun)(struct OSLW_TASK_STRUCT *pta)
)
{

    OSLW_assert(!pta);
    OSLW_assert(!fun);

    memset(pta,0,sizeof(OSlwTaskSTU));//清空任务控制块
    pta->Priority=priority;//优先级
    pta->TaskFun = fun;//任务函数

	
#if !(OSLW_SIMPLE_MODE)	
		strncpy((char *)pta->Name,(char *)name,OSLW_TASK_NAME_LENGTH_MAX);//名字
#endif		
	
#if OSLW_SIMPLE_LEVEL<3 && OSLW_GIFT_EN
    OSlwGiftConciergeInitial(&(pta->Concierge));//门卫系统初始化
    pta->Concierge.pta=pta;//门卫所属指针
    if(ExMemarr!=NULL && ExMemLen!=0)//如果 外部存储器有意义
    {
        pta->ExternMem.uData=ExMemLen;
        pta->ExternMem.pData=ExMemarr;
    }

#endif


}

//------------------------------------------
//<函数名>OSlwInlineTaskBoringExe</函数名>
//<功能说明>空闲任务函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------

void OSlwInlineTaskBoringExe(OSlwTaskSTU *_pta)
{
    static OSlwTaskSTU *pta;

    pta = _pta;
	
#if !(OSLW_SIMPLE_MODE)
    OSlw_5_2(pta);//声明 空闲任务初始化完成
    for(;;)//死循环
    {

        //while(!(pta_last->PrivateMem.m8[0])) {;} //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
		pta->pOS->CurrentTaskIndex = OSLW_TASK_NUM_MAX - 1;
#if !OSLW_SPEED_RUNNING			
        while (pta->TaskGroupFlag.AimStatus.all) {
            OSLW_LOW_POW_ENABLE();    //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
        }
        OSLW_LOW_POW_DISABLE();
#endif
				
				
        pta->TaskGroupFlag.AimStatus.all = 0x01;//清除
        OSlwTaskDispatch(pta->pOS);
    }

    OSlw_2_5(pta);
#else//极简模式
		pta->pOS->InitialFlagGroup.all&=(~((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1)));//清除初始化标志位
		pta->pOS->ReadyFlagGroup.all|=((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1));		
		pta->TaskStatus=OSlwTaskStatus_Running;
		do
		{
			OSlwTaskDispatch(pta->pOS);

			pta->pOS->CurrentTaskIndex=OSLW_TASK_NUM_MAX-1;
			
#if !OSLW_SPEED_RUNNING						
			while (pta->TaskGroupFlag.AimStatus.all) {
					OSLW_LOW_POW_ENABLE();    //等待任务完成一次 !!!!!可以在此时进入低功耗模式!!!!!!!!
			}
			OSLW_LOW_POW_DISABLE();
#endif

			
			pta->TaskGroupFlag.AimStatus.all = 0x01;//清除			
		
		}while(1);
		
		
#endif
}

//------------------------------------------
//<函数名>OSlwInlineTaskParameterExe</函数名>
//<功能说明>参数管理任务函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------

//!!!!!!!!!!!!为了防止部分编译器的断点不支持局部变量保存 推荐尽量使用全局变量!!!!!!!!!!!!!!!!!!!!!!!!
/*(Ver.=0.9~)(Beg.=0.9)
OSlwParaCtrlSTU *ppc_1;
OSlwGiftReceiveSTU *pgr_1;
OSlwGiftPostmanSTU *pp_1;
void OSlwInlineTaskParameterExe(OSlwTaskSTU *pta)
{

	OSLW_TASK_LOOP(pta_1)//循环开始
	{
		OSlwGiftReceiveWait(pta_1);//等待数据介绍
		do
		{
			pgr_1=&(pta_1->Concierge.giftR);//得到接收器
			pp_1=pgr_1->pHead;//得到邮递员
			pp_1->Status=pp_1->Method;//已经签收

			//判断请求列表是否长于已有列表
//			if(pp_1->pGU->PrivateUnit.ParaCmd.ListID > OSLW_CORE_PARA_CTRL_NUM)
//			{
//				continue;
//			}

#if !(OSLW_SIMPLE_MODE)

//			ppc_1=& pta_1->pOS->ParaCtrlList[pp_1->pGU->PrivateUnit.ParaCmd.ListID];
//			ppc_1->CmdExeFun(ppc_1,(void *)(pp_1->pGU));//直接调用执行函数OSlwParaCtrlCmdExe

		}while(pgr_1->MoveNextFun(pgr_1));//循环列表

#else
			OSlwParaCtrlCmdExe(ppc_1,pp_1->pGU);
			//OSlwParaCtrlCmdExe(&(pta->pOS->ParaCtrl),& (pta->Concierge.giftR.pHead->pGU->PrivateUnit.ParaCmd));
		}while(OSlwGiftReceiveMoveNext(pgr_1));
#endif


	}

}
*/
//------------------------------------------
//<函数名>OSlwTaskSleep</函数名>
//<功能说明>任务睡眠函数</功能说明>
//<输入说明>pta:this, n :睡眠节拍数</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskSleep(OSlwTaskSTU *pta,lw_u32 n)
{


    OSLW_assert(!pta);

    if(n==0)
        return;
    pta->SleepCount=n;
    pta->TaskStatus=OSlwTaskStatus_Wait_ForSleep;
    OSLW_TASK_DISPATH_EXE(pta);
}

//------------------------------------------
//<函数名>OSlwTaskWaitGroupFlag</函数名>
//<功能说明>任务等待标志组函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskWaitGroupFlag(OSlwTaskSTU *pta)
{

    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGroupFlag;//....
    OSLW_TASK_DISPATH_EXE(pta);

}


#if OSLW_GIFT_EN

//------------------------------------------
//<函数名>OSlwTaskWaitGiftReceice</函数名>
//<功能说明>接收等待函数</功能说明>
//<输入说明>pta:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskWaitGiftReceice(OSlwTaskSTU *pta)
{


    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGiftReceive;//任务状态为等待礼物接收
    OSLW_TASK_DISPATH_EXE(pta);
}


//------------------------------------------
//<函数名>OSlwTaskWaitGiftTransmit</函数名>
//<功能说明>发送等待函数</功能说明>
//<输入说明>pta:this/输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
void OSlwTaskWaitGiftTransmit(OSlwTaskSTU *pta)
{

    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGiftTransmit;//....
    OSLW_TASK_DISPATH_EXE(pta);

}


//------------------------------------------
//<函数名>OSlwTaskGiftTransmit</函数名>
//<功能说明>礼物发送函数</功能说明>
//<输入说明>pta:this pta_aim:目标任务指针 ID:快递员ID pgu:礼物单元的指针 method:发送模式</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------
#if !(OSLW_SIMPLE_LEVEL >= 3)
OSlwTaskSTU *OSlwTaskGiftTransmit(OSlwTaskSTU *pta,OSlwTaskSTU *pta_aim,OSlwGiftPostmanSTU *pGP,OSlwGiftUnitSTU *pgu,GiftPostmanStatusNUM method)
{

    OSlwGiftPostmanSTU *p;


    OSLW_assert(!pta);
    OSLW_assert(!pta_aim);
    OSLW_assert(!pgu);

    if (pGP)
    {
        p = pGP;//取得快递员
    }
    else if(pta)
    {
        //p = OSlwGiftTransmitIdle(&(pta->Concierge.giftT));//随机分配一个快递员
        OSLW_assert(1);
        if (!p)//如果全部被用上了
        {
            OSLW_assert(1);
        }
    }
    else
    {
        OSLW_assert(1);
        return NULL;
    }

    p->node.Data.pData=pgu;//带上礼物

    p->Method=method;//告诉他模式

    p->pSource = pta;
    p->pAim = pta_aim;

    OSlwGiftReceiveAppend(&(pta_aim->Concierge.giftR),p);

    return pta;//返回this
}



OSlwGiftUnitSTU * OSlwTaskGetOnceGift(OSlwTaskSTU *pta)
{

    OSlwGiftPostmanSTU *pGP;

    OSLW_assert(!(pta));
    pGP = (OSlwGiftPostmanSTU *)pta->Concierge.giftR.List.Head.con.pNext;
    if (pGP)
    {
        return pGP->node.Data.pData;
    }

    return NULL;
}


void OSlwTaskReceOnceGift(OSlwTaskSTU *pta)
{

    OSlwGiftPostmanSTU *pGP;

    OSLW_assert(!(pta));
    pGP = (OSlwGiftPostmanSTU *)pta->Concierge.giftR.List.Head.con.pNext;
    if (pGP)
    {
        pGP->Status = pGP->Method;
    }

    return ;
}
#endif //OSLW_GIFT_EN
#endif // !(OSLW_SIMPLE_LEVEL >= 3)

#if OSLW_SIMPLE_LEVEL==3 && OSLW_GIFT_EN
void * OSlwTaskDelivery(OSlwTaskSTU *pta,OSlwMemoryBasicSTU *pmem,void *src,lw_u32 len)
{
	
	void *p;
	OSLW_assert(!(pmem));
	OSLW_assert(!(src));
	len+=sizeof(OSlwToolDListNodeConnectSTU);
	p=pmem->Malloc(pmem,len);
	if(!pmem) 
	{
		OSLW_assert(1);
		return NULL;
	}
	((OSlwToolDListNodeConnectSTU *)p)->pLast=(void *)pmem;
	
	memcpy((void *)((size_t)p+sizeof(OSlwToolDListNodeConnectSTU)),src,len);

	if(pta->Concierge.pLast ==NULL)//链表为空
	{
		pta->Concierge.pNext=(OSlwToolDListNodeConnectSTU *)(p);
		pta->Concierge.pLast=(OSlwToolDListNodeConnectSTU *)(p);	
	}
	else
	{
		pta->Concierge.pLast->pNext=(OSlwToolDListNodeConnectSTU *)(p);
		pta->Concierge.pLast=(OSlwToolDListNodeConnectSTU *)(p);
	}
	
	
	return p;
}

#endif



OSlwGroupAllType OSlwTaskAuctionJudge(OSlwTaskSTU *pta,lw_8 task_price)
{
    register lw_u32 data;

    if (task_price >=0 )
    {
        data = pta->Priority + task_price;
        data = data > (OSLW_TASK_NUM_MAX - 1) ? (OSLW_TASK_NUM_MAX - 1) : data;

    }
    else
    {
        data = ((OSlwGroupAllType)1 << (OSLW_TASK_NUM_MAX - 1)) - 1;
    }

    return (pta->pOS->ReadyFlagGroup.all) & (((OSlwGroupAllType)1<<data) - 1);

}

lw_u16 OSlwTaskToc(OSlwTaskSTU *pta) {
	lw_u16 _res = pta->BackToSleep.bits.work_time_count; 
	pta->BackToSleep.bits.timer_start_flag = 0; 
	pta->BackToSleep.bits.work_time_count = 0;
	return _res;
}


#endif


