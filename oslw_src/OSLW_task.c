/*(Ver.=0.97)
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
//<������>OSlwTaskInit</������>
//<����˵��>�����ʼ������</����˵��>
//<����˵��>pta:������ƿ�ָ��  priority:���ȼ�  name:����(����:OSLW_TASK_NAME_LENGTH_MAX) ExMemLen:�ⲿ�洢���� ExMemarr:�ⲿ�洢��ַ fun:������</����˵��>
//<���˵��>void</���˵��>
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

    memset(pta,0,sizeof(OSlwTaskSTU));//���������ƿ�
    pta->Priority=priority;//���ȼ�
    pta->TaskFun = fun;//������

	
#if !(OSLW_SIMPLE_MODE)	
		strncpy((char *)pta->Name,(char *)name,OSLW_TASK_NAME_LENGTH_MAX);//����
#endif		
	
#if OSLW_SIMPLE_LEVEL<3 && OSLW_GIFT_EN
    OSlwGiftConciergeInitial(&(pta->Concierge));//����ϵͳ��ʼ��
    pta->Concierge.pta=pta;//��������ָ��
    if(ExMemarr!=NULL && ExMemLen!=0)//��� �ⲿ�洢��������
    {
        pta->ExternMem.uData=ExMemLen;
        pta->ExternMem.pData=ExMemarr;
    }

#endif


}

//------------------------------------------
//<������>OSlwInlineTaskBoringExe</������>
//<����˵��>����������</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
#if OSLW_STEP_RUNNING

void OSlwInlineTaskBoringExe(OSlwTaskSTU *_pta)
{
    static OSlwTaskSTU *pta=NULL;

		
		
    pta = _pta;

		pta->pOS->InitialFlagGroup.all&=(~((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1)));//�����ʼ����־λ
		pta->pOS->ReadyFlagGroup.all|=((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1));		
		pta->TaskStatus=OSlwTaskStatus_Running;

		OSlwTaskDispatch(pta->pOS);

		pta->pOS->CurrentTaskIndex=OSLW_TASK_NUM_MAX-1;
		
#if !OSLW_SPEED_RUNNING						
		while (pta->TaskGroupFlag.CurrentStatus.all) {
				OSLW_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
		}
		OSLW_LOW_POW_DISABLE();
#endif

		pta->TaskGroupFlag.CurrentStatus.all = 0x01;//���			
}

#else
void OSlwInlineTaskBoringExe(OSlwTaskSTU *_pta)
{
    static OSlwTaskSTU *pta;

    pta = _pta;
	
#if !(OSLW_SIMPLE_MODE)
    OSlw_5_2(pta);//���� ���������ʼ�����
    for(;;)//��ѭ��
    {

        //while(!(pta_last->PrivateMem.m8[0])) {;} //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
		pta->pOS->CurrentTaskIndex = OSLW_TASK_NUM_MAX - 1;
#if !OSLW_SPEED_RUNNING			
        while (pta->TaskGroupFlag.CurrentStatus.all) {
            OSLW_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
        }
        OSLW_LOW_POW_DISABLE();
#endif
				
				
        pta->TaskGroupFlag.CurrentStatus.all = 0x01;//���	
        OSlwTaskDispatch(pta->pOS);
    }

    OSlw_2_5(pta);
#else//����ģʽ
		pta->pOS->InitialFlagGroup.all&=(~((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1)));//�����ʼ����־λ
		pta->pOS->ReadyFlagGroup.all|=((OSlwGroupAllType)1<<(OSLW_TASK_NUM_MAX-1));		
		pta->TaskStatus=OSlwTaskStatus_Running;
		do
		{
			OSlwTaskDispatch(pta->pOS);

			pta->pOS->CurrentTaskIndex=OSLW_TASK_NUM_MAX-1;
			
#if !OSLW_SPEED_RUNNING						
			while (pta->TaskGroupFlag.CurrentStatus.all) {
					OSLW_LOW_POW_ENABLE();    //�ȴ��������һ�� !!!!!�����ڴ�ʱ����͹���ģʽ!!!!!!!!
			}
			OSLW_LOW_POW_DISABLE();
#endif

			
			pta->TaskGroupFlag.CurrentStatus.all=1;//���			
		
		}while(1);
		
		
#endif
}

#endif

//------------------------------------------
//<������>OSlwInlineTaskParameterExe</������>
//<����˵��>��������������</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------

//!!!!!!!!!!!!Ϊ�˷�ֹ���ֱ������Ķϵ㲻֧�־ֲ��������� �Ƽ�����ʹ��ȫ�ֱ���!!!!!!!!!!!!!!!!!!!!!!!!
/*(Ver.=0.97)
OSlwParaCtrlSTU *ppc_1;
OSlwGiftReceiveSTU *pgr_1;
OSlwGiftPostmanSTU *pp_1;
void OSlwInlineTaskParameterExe(OSlwTaskSTU *pta)
{

	OSLW_TASK_LOOP(pta_1)//ѭ����ʼ
	{
		OSlwGiftReceiveWait(pta_1);//�ȴ����ݽ���
		do
		{
			pgr_1=&(pta_1->Concierge.giftR);//�õ�������
			pp_1=pgr_1->pHead;//�õ��ʵ�Ա
			pp_1->Status=pp_1->Method;//�Ѿ�ǩ��

			//�ж������б��Ƿ��������б�
//			if(pp_1->pGU->PrivateUnit.ParaCmd.ListID > OSLW_CORE_PARA_CTRL_NUM)
//			{
//				continue;
//			}

#if !(OSLW_SIMPLE_MODE)

//			ppc_1=& pta_1->pOS->ParaCtrlList[pp_1->pGU->PrivateUnit.ParaCmd.ListID];
//			ppc_1->CmdExeFun(ppc_1,(void *)(pp_1->pGU));//ֱ�ӵ���ִ�к���OSlwParaCtrlCmdExe

		}while(pgr_1->MoveNextFun(pgr_1));//ѭ���б�

#else
			OSlwParaCtrlCmdExe(ppc_1,pp_1->pGU);
			//OSlwParaCtrlCmdExe(&(pta->pOS->ParaCtrl),& (pta->Concierge.giftR.pHead->pGU->PrivateUnit.ParaCmd));
		}while(OSlwGiftReceiveMoveNext(pgr_1));
#endif


	}

}
*/
//------------------------------------------
//<������>OSlwTaskSleep</������>
//<����˵��>����˯�ߺ���</����˵��>
//<����˵��>pta:this, n :˯�߽�����</����˵��>
//<���˵��>void</���˵��>
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
//<������>OSlwTaskWaitGroupFlag</������>
//<����˵��>����ȴ���־�麯��</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void OSlwTaskWaitGroupFlag(OSlwTaskSTU *pta)
{

    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGroupFlag;//....
    OSLW_TASK_DISPATH_EXE(pta);

}


#if OSLW_GIFT_EN

//------------------------------------------
//<������>OSlwTaskWaitGiftReceice</������>
//<����˵��>���յȴ�����</����˵��>
//<����˵��>pta:this</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void OSlwTaskWaitGiftReceice(OSlwTaskSTU *pta)
{


    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGiftReceive;//����״̬Ϊ�ȴ��������
    OSLW_TASK_DISPATH_EXE(pta);
}


//------------------------------------------
//<������>OSlwTaskWaitGiftTransmit</������>
//<����˵��>���͵ȴ�����</����˵��>
//<����˵��>pta:this/����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
void OSlwTaskWaitGiftTransmit(OSlwTaskSTU *pta)
{

    OSLW_assert(!pta);

    pta->TaskStatus=OSlwTaskStatus_Wait_ForGiftTransmit;//....
    OSLW_TASK_DISPATH_EXE(pta);

}


//------------------------------------------
//<������>OSlwTaskGiftTransmit</������>
//<����˵��>���﷢�ͺ���</����˵��>
//<����˵��>pta:this pta_aim:Ŀ������ָ�� ID:���ԱID pgu:���ﵥԪ��ָ�� method:����ģʽ</����˵��>
//<���˵��>void</���˵��>
//------------------------------------------
#if !(OSLW_SIMPLE_LEVEL >= 3)
OSlwTaskSTU *OSlwTaskGiftTransmit(OSlwTaskSTU *pta,OSlwTaskSTU *pta_aim,OSlwGiftPostmanSTU *pGP,OSlwGiftUnitSTU *pgu,GiftPostmanStatusNUM method)
{

	OSlwGiftPostmanSTU *p = NULL;


    OSLW_assert(!pta);
    OSLW_assert(!pta_aim);
    OSLW_assert(!pgu);

    if (pGP)
    {
        p = pGP;//ȡ�ÿ��Ա
    }
    else if(pta)
    {
        //p = OSlwGiftTransmitIdle(&(pta->Concierge.giftT));//�������һ�����Ա
        OSLW_assert(1);
        if (!p)//���ȫ����������
        {
            OSLW_assert(1);
        }
    }
    else
    {
        OSLW_assert(1);
        return NULL;
    }

    p->node.Data.pData=pgu;//��������

    p->Method=method;//������ģʽ

    p->pSource = pta;
    p->pAim = pta_aim;

    OSlwGiftReceiveAppend(&(pta_aim->Concierge.giftR),p);

    return pta;//����this
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

	if(pta->Concierge.pLast ==NULL)//����Ϊ��
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
    register OSlwGroupAllType data;

    if ((lw_16)task_price >=0 )
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


#if !(OSLW_SIMPLE_LEVEL >= 3)
lw_u16 OSlwTaskToc(OSlwTaskSTU *pta) {
	lw_u16 _res = pta->BackToSleep.bits.work_time_count; 
	pta->BackToSleep.bits.timer_start_flag = 0; 
	pta->BackToSleep.bits.work_time_count = 0;
	return _res;
}
#endif

#endif


