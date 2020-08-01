/*(Ver.=0.97)
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
 //<�ṹ����>OSlwTaskPrivateMemorySTU</�ṹ����>
 //<�ṹ�幦��>����˽�д洢�� ÿƬ����ΪTASK_PRIVATE_MEMORY_SECTION_LENGTH</�ṹ�幦��>
 //------------------------------------------
typedef struct OSLW_TASK_PRIVATE_MEMORY_STRUCT {
	lw_u8 m8[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u16 m16[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u32 m32[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_u64 m64[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_sf msf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
	//lw_df mdf[TASK_PRIVATE_MEMORY_SECTION_LENGTH];
}OSlwTaskPrivateMemorySTU;


//------------------------------------------
//<�ṹ����>OSlwTaskExternMemorySTU</�ṹ����>
//<�ṹ�幦��>�����ⲿ�洢�� ����ΪLength �׵�ַΪarr</�ṹ�幦��>
//------------------------------------------
//typedef struct OSLW_TASK_EXTERN_MEMORY_STRUCT{
//	lw_u16 Length;
//	void *arr;
//}OSlwTaskExternMemorySTU;

typedef OSlwExternMemorySTU OSlwTaskExternMemorySTU;

//------------------------------------------
//<ö��>OSlwTaskStatusNUM</ö��>
//<ö�ٹ���>����״̬����</ö�ٹ���>
//------------------------------------------
typedef enum {
	OSlwTaskStatus_Init = 0,//��ʼ��
	OSlwTaskStatus_Ready = 1,//����׼����� �ȴ�ִ��
	OSlwTaskStatus_Running = 2,//������������
	OSlwTaskStatus_Wait_ForSleep,//����ȴ� �ȴ�ԭ��Ϊ˯��
	OSlwTaskStatus_Wait_ForSignal,//����ȴ� �ȴ�ԭ��Ϊ�ȴ��ź���

	OSlwTaskStatus_Wait_ForGiftReceive,//����ȴ� �ȴ�ԭ��Ϊ�ȴ���Ϣ�������

	OSlwTaskStatus_Wait_ForGroupFlag,//����ȴ� �ȴ�ԭ��ȴ���־��

	OSlwTaskStatus_Wait_ForGiftTransmit,
	OSlwTaskStatus_Frozen//���񶳽�
}OSlwTaskStatusNUM;


typedef struct OSLW_TASK_GROUP_FLAG_STRUCT
{

	OSlwCoreGroupUN CurrentStatus;
	OSlwCoreGroupUN AimStatus;


}OSlwTaskGroupFlagSTU;


//------------------------------------------
//<�ṹ����>OSlwTaskSTU</�ṹ����>
//<�ṹ�幦��>������ƿ�</�ṹ�幦��>
//------------------------------------------
typedef struct OSLW_TASK_STRUCT {
	//��Ա����


#if !(OSLW_SIMPLE_MODE)	
	volatile _BP BreakPoint[OSLW_TASK_BREAKPOINT_DEEP];//�ϵ��б� ��ʾ֧�ֵĺ���Ƕ�״���
	volatile lw_8 Name[OSLW_TASK_NAME_LENGTH_MAX + 1];//���� ������OSLW_TASK_NAME_LENGTH_MAX����	
	volatile lw_u16 BreakPointReturn[OSLW_TASK_BREAKPOINT_DEEP];//�ϵ㷵�ر� ��ʾ�����ϵ�ķ���ֵ

#else
	volatile _BP BreakPoint;
#endif	




#if OSLW_SIMPLE_LEVEL<3

#if OSLW_GIFT_EN	
	OSlwGiftConciergeSTU Concierge;
#endif //OSLW_GIFT_EN	

	lw_u32 RunningTimeSinceStart;
	lw_u16 RunningTimeSinceRunning;
	lw_u16 TimeSliceMax;
	OSlwTaskExternMemorySTU ExternMem;//�����ⲿ�洢��	

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


#elif OSLW_SIMPLE_LEVEL==3 && OSLW_GIFT_EN
	OSlwToolDListNodeConnectSTU Concierge;
#else

#endif



	volatile lw_u32 SleepCount;//˯�߼�����
	volatile OSlwTaskGroupFlagSTU TaskGroupFlag;
	//OSlwTaskPrivateMemorySTU PrivateMem;//����˽�д洢��
	struct OSLW_CORE_STRUCT *pOS;//����ϵͳָ�� ָ��������������ϵͳ

	//��Ա����

	void(*TaskFun)(struct OSLW_TASK_STRUCT *pta);//������
	/*(Ver.=0.97)
#if !(OSLW_SIMPLE_MODE)
	void (*SleepFun)(struct OSLW_TASK_STRUCT *pta,lw_u32 n);//˯�ߺ���
	void (*WaitGRFun)(struct OSLW_TASK_STRUCT *pta);//
	void (*WaitGTFun)(struct OSLW_TASK_STRUCT *pta);//
	void (*WaitGFFun)(struct OSLW_TASK_STRUCT *pta);//
	struct OSLW_TASK_STRUCT*(*GiftTFun)
		(struct OSLW_TASK_STRUCT *pta, struct OSLW_TASK_STRUCT *pta_aim, OSlwGiftPostmanSTU *pGP, OSlwGiftUnitSTU *pgu, GiftPostmanStatusNUM method);
#endif
*/
	volatile lw_u8 TaskFrozenFlag : 1;
	volatile OSlwTaskStatusNUM TaskStatus : 7;//����״̬

	lw_u8 Priority;//���ȼ�
}OSlwTaskSTU;





//��������
extern OSlwTaskSTU OSlwInlineTaskBoring;
extern OSlwTaskSTU OSlwInlineTaskParameter;

void OSlwInlineTaskBoringExe(OSlwTaskSTU *pta);
void OSlwInlineTaskParameterExe(OSlwTaskSTU *pta);
void OSlwInlineTaskShowExe(OSlwTaskSTU *pta);
void OSlwTaskSleep(OSlwTaskSTU *pta, lw_u32 n);
void OSlwTaskWaitGroupFlag(OSlwTaskSTU *pta);
void OSlwTaskWaitGiftReceice(OSlwTaskSTU *pta);
void OSlwTaskWaitGiftTransmit(OSlwTaskSTU *pta);
OSlwGroupAllType OSlwTaskAuctionJudge(OSlwTaskSTU *pta, lw_8 task_price);
lw_u16 OSlwTaskToc(OSlwTaskSTU *pta);
#if !(OSLW_SIMPLE_LEVEL >= 3)
OSlwGiftUnitSTU * OSlwTaskGetOnceGift(OSlwTaskSTU *pta);
void OSlwTaskReceOnceGift(OSlwTaskSTU *pta);
OSlwTaskSTU *OSlwTaskGiftTransmit(OSlwTaskSTU *pta, OSlwTaskSTU *pta_aim, OSlwGiftPostmanSTU *pGP, OSlwGiftUnitSTU *pgu, GiftPostmanStatusNUM method);
#endif // !(OSLW_SIMPLE_LEVEL >= 3)

#if OSLW_SIMPLE_LEVEL == 3
void * OSlwTaskDelivery(OSlwTaskSTU *pta, OSlwMemoryBasicSTU *pmem, void *src, lw_u32 len);
#endif

void OSlwTaskInit(OSlwTaskSTU *pta,
	lw_u8 priority,
	lw_8 *name,
	lw_u16 ExMemLen,
	void *ExMemarr,
	void(*fun)(struct OSLW_TASK_STRUCT *pta)
);



#if OSLW_SIMPLE_MODE

#define OSLW_TASK_DISPATH_EXE(PTA) do{\
(PTA)->RunningTimeSinceRunning=0;\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
return;}while(0)

#else

#define OSLW_TASK_DISPATH_EXE(PTA) do{\
(PTA)->RunningTimeSinceRunning=0;\
(PTA)->pOS->ReadyFlagGroup.all&=~((lw_u64)1<<((PTA)->Priority));\
OSlwTaskDispatch((PTA)->pOS);\
}while(0)	


#endif



#endif /*(Ver.=0.97) OSLW_TASK_H_*/
