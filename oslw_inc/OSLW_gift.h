/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_gift.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_GIFT_H_
#define OSLW_GIFT_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"

#if !(OSLW_SIMPLE_LEVEL >= 3)


//------------------------------------------
//<共用体名>OSlwGiftUnitUN</共用体名>
//<共用体功能>礼物模块私有单元 就有多重访问功能</共用体功能>
//------------------------------------------
typedef struct OSLW_GIFT_UNIT_UNION{

	

	struct OSLW_PARAMETER_STRUCT *pParaData;
	struct {
		lw_u8 ComID;
		lw_u8 RxD[OSLW_GIFT_COMMUN_RXD_LENGTH];
	} ComCmd;
	
	lw_u8 str[OSLW_GIFT_UNIT_STR_LENGTH];
	lw_u8 KeyBoardInput;
}OSlwGiftUnitUN;



//------------------------------------------
//<结构体名>OSlwGiftExternUnitSTU</结构体名>
//<结构体功能>礼物外部存储空间</结构体功能>
//------------------------------------------
//typedef struct OSLW_GIFT_EXTERN_UNIT_STRUCT{
//	lw_16 Length;
//	void *arr;
//}OSlwGiftExternUnitSTU;

typedef OSlwExternMemorySTU OSlwGiftExternUnitSTU;

//------------------------------------------
//<枚举>GiftUnitStatusNUM</枚举>
//<枚举功能>礼物状态类型</枚举功能>
//------------------------------------------
typedef enum{
	GiftUnitStatus_NAN=0,
	GiftUnitStatus_Free=1,
	GiftUnitStatus_InDelivery,
	GiftUnitStatus_Received
}GiftUnitStatusNUM;

//------------------------------------------
//<枚举>GiftPostmanStatusNUM</枚举>
//<枚举功能>快递员状态类型</枚举功能>
//------------------------------------------
typedef enum{

	GiftPostmanStatus_NAN=0,//为初始化
	GiftPostmanStatus_Free=1,//自由状态
	GiftPostmanStatus_InDelivery,//正在派送
	GiftPostmanStatus_Received,//已签收 表示目标任务已经接受
	GiftPostmanStatus_Return//返回 表示目标任务已经返回(用于数据具有双向性的模式)
}GiftPostmanStatusNUM;


//------------------------------------------
//<结构体名>OSlwGiftUnitSTU</结构体名>
//<结构体功能>礼物单元空间 包含一片私有与外部单元</结构体功能>
//------------------------------------------
typedef struct OSLW_GIFT_UNIT_STRUCT{
	
	OSlwGiftExternUnitSTU ExternUnit;//礼物外部单元
	OSlwGiftUnitUN PrivateUnit;//礼物私有单元
	
	
}OSlwGiftUnitSTU;


//------------------------------------------
//<结构体名>OSlwGiftPostmanSTU</结构体名>
//<结构体功能>快递员 结构体 本质为双向链表的节点</结构体功能>
//------------------------------------------
typedef struct OSLW_GIFT_POSTMAN_STRUCT{

	OSlwToolDListNodeSTU node;
	
	lw_u8 GroupFlag;
	
	lw_u8 PostmanID;

	struct OSLW_TASK_STRUCT *pSource,*pAim;//指向源任务与目标任务的指针 暂时无用
	
	struct OSLW_MEMORY_BASIC_STRUCT *pmem;
	
	GiftPostmanStatusNUM Status,Method;//快递员当前状态与目标 当目标与状态相同时 表示快递员使命结束
		
	struct OSLW_GIFT_TRANSMIT_STRUCT *pGT;//指向所属发送单位的指针


}OSlwGiftPostmanSTU;


//------------------------------------------
//<结构体名>OSlwGiftReceiveSTU</结构体名>
//<结构体功能>礼物接收器结构体 本质为双向链表的控制单元</结构体功能>
//------------------------------------------
typedef struct OSLW_GIFT_RECEIVE_STRUCT{
	//成员变量

	OSlwToolDListSTU List;
	
	struct OSLW_GIFT_CONCIERGE_STRUCT *pGC;//指向自己所属门卫机构的指针
	
	//成员函数
#if !(OSLW_SIMPLE_MODE)	
	struct OSLW_GIFT_POSTMAN_STRUCT *(*AppendFun)(struct OSLW_GIFT_RECEIVE_STRUCT *pgr,struct OSLW_GIFT_POSTMAN_STRUCT *pgp);//追加函数
	struct OSLW_GIFT_POSTMAN_STRUCT *(*MoveNextFun)(struct OSLW_GIFT_RECEIVE_STRUCT *pgr);//移动函数 
	struct OSLW_GIFT_POSTMAN_STRUCT *(*SlipNextFun)(struct OSLW_GIFT_RECEIVE_STRUCT *pgr,struct OSLW_GIFT_POSTMAN_STRUCT **ppGP);//滑动函数 	
#endif
}OSlwGiftReceiveSTU;




//------------------------------------------
//<结构体名>OSlwGiftTransmitSTU</结构体名>
//<结构体功能>礼物发送器结构体</结构体功能>
//逐渐放弃 采用内存分配机制代替
//------------------------------------------
#if 0
typedef struct OSLW_GIFT_TRANSMIT_STRUCT{
	
	OSlwGiftPostmanSTU PostmanList[OSLW_GIFT_POSTMAN_MAX];//快递员列表 存储一部分快递员
	
	union OSLW_CORE_GROUP_UNION PostmanFlag;//状态标志组 为0表示发送完成 一位代表一个快递员 
	
	struct OSLW_GIFT_CONCIERGE_STRUCT *pGC;	//指向自己所属门卫机构的指针
#if !(OSLW_SIMPLE_MODE)		
	OSlwGiftPostmanSTU* (*IdleFun)(struct OSLW_GIFT_TRANSMIT_STRUCT *pgt);
#endif	
}OSlwGiftTransmitSTU;
#endif // 0

//------------------------------------------
//<结构体名>OSlwGiftConciergeSTU</结构体名>
//<结构体功能>门卫机构结构体</结构体功能>
//------------------------------------------
typedef struct OSLW_GIFT_CONCIERGE_STRUCT{

	
	OSlwGiftReceiveSTU giftR;//接收
	//OSlwGiftTransmitSTU giftT;//发送

	struct OSLW_TASK_STRUCT *pta;//指向自己所属任务的指针
	
}OSlwGiftConciergeSTU;


//函数声明
void OSlwGiftConciergeInitial(OSlwGiftConciergeSTU *pgc);

//逐渐放弃 采用内存分配机制代替
#if 0
void OSlwGiftTransmitInitial(OSlwGiftTransmitSTU *pgt);
OSlwGiftPostmanSTU * OSlwGiftTransmitIdle(OSlwGiftTransmitSTU *pgt);
#endif


OSlwGiftPostmanSTU * OSlwGiftReceiveAppend(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU *pgp);
OSlwGiftPostmanSTU * OSlwGiftReceiveMoveNext(OSlwGiftReceiveSTU *pgr);
OSlwGiftPostmanSTU * OSlwGiftReceiveSlipNext(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU **ppGP);
OSlwGiftPostmanSTU * OSlwGiftReceiveForeah(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU **ppGP);


OSlwGiftPostmanSTU * OSlwGiftAllocPostman(struct OSLW_MEMORY_BASIC_STRUCT *pmem);

#endif // !(OSLW_SIMPLE_LEVEL >= 3)


#define OSLW_GIFT_RECE_GIFT(PTA) do{OSlwTaskReceOnceGift(PTA);}while(0)

#define OSLW_GIFT_SIGNE(PGP) do{(PGP)->Method=(PGP)->Status;}while(0)

#define OSLW_GIFT_SEND_GIFT(PTA,PM) do{OSlwGiftReceiveAppend(&((PTA)->Concierge.giftR),(PM));}while(0)

#define OSLW_GIFT_GET_GIFT(PTA) (OSlwTaskGetOnceGift(PTA))

#define OSLW_GIFT_GET_MAN(PTA) ((OSlwGiftPostmanSTU *)(PTA->Concierge.giftR.List.Head.con.pNext))

#define OSLW_GIFT_NEXT(PTA) ((PTA)->Concierge.giftR.MoveNextFun(&(PTA->Concierge.giftR)))

#define OSLW_GIFT_NUM(PTA) (((PTA)->Concierge.giftR.List.NowLen) & 0xFFFE)

#define OSLW_GIFT_JUDGE_FLAG(PGP,NUM,COND) if((((PGP)->GroupFlag)!=NUM) && (COND)) continue;

#define OSLW_GIFT_SLIP(PTA,PGP) (OSlwGiftReceiveSlipNext(&(PTA->Concierge.giftR),&(PGP)))

#define OSLW_GIFT_SG_SF(PTA,PM,NUM) do{(PM)->GroupFlag=(NUM);OSLW_GIFT_SEND_GIFT((PTA),(PM));OSLW_FLAG_SET((PTA),(NUM));}while(0)

#define OSLW_FLAG_POP(PTA,NUM) ((PTA)->TaskGroupFlag.CurrentStatus.bits.bit##NUM)

#define OSLW_FLAG_GET(PTA,NUM) ((((PTA)->TaskGroupFlag.CurrentStatus.all)>>(NUM))&1)

#define OSLW_FLAG_CLR(PTA,NUM) do {(PTA)->TaskGroupFlag.CurrentStatus.all &= (~(1<<(NUM)));}while(0)

#define OSLW_FLAG_SET(PTA,NUM) do {(PTA)->TaskGroupFlag.CurrentStatus.all |= ((1<<(NUM)));}while(0)





#if OSLW_SIMPLE_LEVEL ==3
void * OSlwGiftReceiveMoveNext(OSlwToolDListNodeConnectSTU *pgr);


//取消部分宏定义
#undef OSLW_GIFT_SEND_GIFT
#undef OSLW_GIFT_GET_GIFT
#undef OSLW_GIFT_RECE_GIFT
#undef OSLW_GIFT_SIGNE
#undef OSLW_GIFT_SEND_GIFT
#undef OSLW_GIFT_GET_GIFT
#undef OSLW_GIFT_GET_MAN
#undef OSLW_GIFT_NEXT
#undef OSLW_GIFT_JUDGE_FLAG
#undef OSLW_GIFT_NUM




#define OSLW_GIFT_NEXT(PTA) (OSlwGiftReceiveMoveNext(&((PTA)->Concierge)))
#define OSLW_GIFT_GET_GIFT(PTA) ((void *)((lw_u8 *)((PTA)->Concierge.pNext) + (sizeof(OSlwToolDListNodeConnectSTU))))
#define OSLW_GIFT_GET_MAN(PTA) ((void *)(PTA)->Concierge.pNext)


#endif

typedef OSlwGiftPostmanSTU LwPostman;

#define LwGetMan OSLW_GIFT_GET_MAN 
#define LwGetGift(PGP) ((PGP)->node.Data.pData) 
#define LwSignGift OSLW_GIFT_SIGNE
#define LwNextGift OSLW_GIFT_SLIP
#define LwCheckGift OSLW_GIFT_JUDGE_FLAG
#define LwSendGift OSLW_GIFT_SEND_GIFT

#define LwCallPost(PMEM) OSlwGiftAllocPostman((void *)((PMEM)))

#define LwForeachGift(PTA,PGP) for((PGP)=((OSlwGiftPostmanSTU *)&((PTA)->Concierge.giftR.List.Head));OSlwGiftReceiveForeah(&((PTA)->Concierge.giftR),&(PGP));)

#define LwSetFlag OSLW_FLAG_SET
#define LwClearFlag OSLW_FLAG_CLR
#define LwGetFlag OSLW_FLAG_GET
#define LwPopFlag OSLW_FLAG_POP

#define LwSendGiFg OSLW_GIFT_SG_SF

#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_GIFT_H_ */
