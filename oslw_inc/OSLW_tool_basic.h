/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef OSLW_TOOL_BASIC_H_
#define OSLW_TOOL_BASIC_H_


#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_memory.h"
#include "OSLW_tool_define.h"

#if OSLW_TOOL_IMPORT_SLIP_WIN|| OSLW_TOOL_IMPORT_ALL
//--------------------
//<滑窗>
//pMasterData 主数据池
//pBufferData 缓冲数据池
//MDataLen_forSize_t 主数据对于size_t的大小
//BufferLength 缓冲区已存数据大小!!!!!!!!!用户必须保证缓冲区足够!!!!!!!!!!!!!
//MDataLockFlag 主数据锁存标志 如果为1 表示主数据锁存 数据填入缓冲器
//AppendFun 成员函数 psw:this pd:数据首地址 num:数据长度
//</滑窗>
//--------------------
typedef struct OSLW_TOOL_SLIPWIN_STRUCT{

	void *pMasterData;
	void *pBufferData;

	lw_u32 MDataLen_forSize_t;
	
	
	lw_u16 BufferLength;	
	lw_u8 MDataLockFlag;
	
	struct OSLW_TOOL_SLIPWIN_STRUCT* (*AppendFun)(struct OSLW_TOOL_SLIPWIN_STRUCT *psw,void *pd,lw_u8 num);

}  OSlwToolSlipwinSTU;

OSlwToolSlipwinSTU* OSlwToolSlipwinAppend(OSlwToolSlipwinSTU *psw,void *pd,lw_u8 num);


#define OSLW_TOOL_SLIPWIN_DEFAULT(MD,BUF,DTYPE,LEN) {(MD),(BUF),(sizeof(DTYPE)*LEN),0,0,OSlwToolSlipwinAppend};

#define OSLW_TOOL_MEMCPY(P,Q,N) do {while((N)--)*(P)++=*(Q)++;}while(0)

#endif //OSLW_TOOL_IMPORT_SLIP_WIN


#if OSLW_TOOL_IMPORT_DLIST|| OSLW_TOOL_IMPORT_ALL
//-------------------
//<链表节点>
//</链表节点>
//--------------------
typedef struct OSLW_TOOL_DLIST_NODE_STRUCT{

	OSlwToolDListNodeConnectSTU con;
	OSlwExternMemorySTU Key;
	OSlwExternMemorySTU Data;
	
}OSlwToolDListNodeSTU;


typedef enum{
	OSlwToolDListIndexMode_byNum=0,
	OSlwToolDListIndexMode_byId=1

}OSlwToolDListIndexModeNUM;

typedef lw_8(*pOSlwDlistCmpFun)(OSlwToolDListNodeSTU *pDLN, lw_u16 data, lw_u16 count, void *pdata);
#define OSLW_TOOL_DLIST_FUN_PARA_DEFAULT pOSlwDlistCmpFun pCmpFun,lw_u16 data,void *pdata


//--------------------
//<双向链表>
//</双向链表>
//--------------------
typedef struct OSLW_TOOL_DLIST_STRUCT{
	OSlwToolDListNodeSTU Head,*pTail;
	lw_u16 MaxLen,NowLen;
	OSlwToolDListNodeSTU* (*IndexOfFUN)(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
	OSlwToolDListNodeSTU* (*AppendFUN)(struct OSLW_TOOL_DLIST_STRUCT *pDL,OSlwToolDListNodeSTU *pDLN);
	OSlwToolDListNodeSTU* (*InsertFUN)(struct OSLW_TOOL_DLIST_STRUCT *pDL,OSlwToolDListNodeSTU *pDLN, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
	OSlwToolDListNodeSTU* (*RemoveFUN)(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
	
}OSlwToolDListSTU;

#define OSLW_TOOL_DLIST_FUN_PARA_DEFAULT pOSlwDlistCmpFun pCmpFun,lw_u16 data,void *pdata


#if !(OSLW_SIMPLE_LEVEL >= 4)
OSlwToolDListNodeSTU* OSlwToolDListIndexOf(OSlwToolDListSTU *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
OSlwToolDListNodeSTU* OSlwToolDListAppend(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSlwToolDListNodeSTU *pDLN);
OSlwToolDListNodeSTU* OSlwToolDListInsert(struct OSLW_TOOL_DLIST_STRUCT *pDL,OSlwToolDListNodeSTU *pDLN, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
OSlwToolDListNodeSTU* OSlwToolDListRemove(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT);
OSlwToolDListNodeSTU* _OSlwToolDListRemoveNode (struct OSLW_TOOL_DLIST_STRUCT *pDL, OSlwToolDListNodeSTU *pDLNbuf);
#endif // !(OSLW_SIMPLE_LEVEL >= 4)


#define OSLW_TOOL_DLIST_NODE_CONNECT(P1,P2) 	(P1)->con.pNext=(OSlwToolDListNodeConnectSTU *)(P2);/*(Ver.=0.9~)(Beg.=0.9)尾部的下一个为新的*/\
(P2)->con.pLast = (OSlwToolDListNodeConnectSTU *)(P1);/*(Ver.=0.9~)(Beg.=0.9)新节点的上一个为原来尾部*/

#define OSLW_TOOL_DLIST_NODE_INSERT(P1,P_INSERT) (P_INSERT)->con.pLast = (OSlwToolDListNodeConnectSTU *)(P1);\
(P_INSERT)->con.pNext = (OSlwToolDListNodeConnectSTU *)(P1)->con.pNext;\
(P1)->con.pNext->pLast = (OSlwToolDListNodeConnectSTU *)(P_INSERT);\
(P1)->con.pNext = (OSlwToolDListNodeConnectSTU *)(P_INSERT);

#define OSLW_TOOL_DLIST_NODE_DELECT(P) 			(P)->con.pLast->pNext=(P)->con.pNext;/*(Ver.=0.9~)(Beg.=0.9)上一个的下一个为下一个*/\
(P)->con.pNext->pLast = (P)->con.pLast;/*(Ver.=0.9~)(Beg.=0.9)下一个的上一个为上一个*/


void OSlwToolDListInitial(OSlwToolDListSTU *pDL,lw_u8 len,void *p);

lw_8 OSlwToolDListCmpId(OSlwToolDListNodeSTU *pDLN, lw_u16 data, lw_u16 count, void *pdata);
lw_8 OSlwToolDListCmpCount(OSlwToolDListNodeSTU *pDLN, lw_u16 data, lw_u16 count, void *pdata);

#endif //OSLW_TOOL_IMPORT_DLIST



#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_HASH_LIST|| OSLW_TOOL_IMPORT_ALL

typedef lw_u8 (*HashCalFunType)(void *pData);
lw_u8 HashCalStringBKDR(void *pData);


typedef lw_8(*HashKeyCmpFunType)(void *p1, void *p2);
lw_8 HashCmpString(void *p1, void *p2);

//--------------------
//<哈希表>
//</哈希表>
//--------------------
typedef struct OSLW_TOOL_HASH_STRUCT {
	OSlwToolDListNodeSTU *pHashList[OSLW_TOOL_HASH_LIST_MAX];
	lw_u8 HashListLength;

	HashCalFunType HashCalFun;
	HashKeyCmpFunType HashKeyCmp;

	lw_u8(*SelectFun)(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult, OSlwToolDListNodeSTU **ppDLN);
	OSlwToolDListNodeSTU* (*AppendFun)(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, OSlwToolDListNodeSTU *pDLN);
	OSlwToolDListNodeSTU* (*DeleteFun)(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey);

}OSlwToolHashSTU;


void OSlwToolHashInitial(OSlwToolHashSTU *pH, lw_u8 length, HashCalFunType pHashFun, HashKeyCmpFunType pHashKeyCmpFun);
lw_u8 OSlwToolHashSelect(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult,OSlwToolDListNodeSTU **ppDLN);
OSlwToolDListNodeSTU* OSlwToolHashAppend(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, OSlwToolDListNodeSTU *pDLN);
OSlwToolDListNodeSTU* OSlwToolHashDelete(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey);

#endif //OSLW_TOOL_IMPORT_HASH_LIST


#if OSLW_TOOL_IMPORT_TABLE|| OSLW_TOOL_IMPORT_ALL
typedef struct OSLW_TOOL_TABLE_STRUCT{

	OSlwExternMemorySTU Row;

	OSlwExternMemorySTU Col;

	lw_u32 ColSize;

	OSlwMemoryBasicSTU *pmem;

	void *(*ReadFun)(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row,lw_32 col,lw_u16 *pLen);
	lw_32 (*WriteFun)(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len);
	
	
}OSlwToolTableSTU;



void OSlwToolTableInitial(OSlwToolTableSTU *pT, lw_u16 RowLen, lw_u16 *pCol, lw_u16 ColLen, OSlwMemoryBasicSTU *pmem, void *pTable, lw_u32 *pColReal);
void * OSlwToolTableRead(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, lw_u16 *pLen);
lw_32 OSlwToolTableWrite(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len);

#endif //OSLW_TOOL_IMPORT_TABLE

#endif // !(OSLW_SIMPLE_LEVEL >= 4)

#endif


