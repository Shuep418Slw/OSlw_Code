/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_show.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef OSLW_SHOW_H_
#define OSLW_SHOW_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"

//V1.0
typedef enum{

	
	OSlwShowDateStatus_Display,
	OSlwShowDateStatus_Change
	
}OSlwShowDateStatusNUM ;


typedef enum{
	OSlwShowUpdateMethod_OnlyDate,
	OSlwShowUpdateMethod_OnlyCursor,
	OSlwShowUpdateMethod_All
}OSlwShowUpdateMethodNUM ;

typedef enum{
	OSlwShowCursorStatus_Free,
	OSlwShowCursorStatus_Lock
}OSlwShowCursorStatusNUM ;

typedef struct OSLW_SHOW_CURSOR_STRUCT{
		
	OSlwShowCursorStatusNUM Status;
	
	lw_16 Position_forList,Position_forScreen,Position_forScreenLast;
	
	lw_u8 Position_forPage;
	
	struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC;
	
	
}OSlwShowCursorSTU;


typedef struct OSLW_SHOW_PRIVATE_PARA_CTRL_STRUCT{
	OSlwParameterSTU **ppPHead;
	lw_u8 HeadIndex;		
	OSlwParaCtrlSTU *pPC;
}OSlwShowPvtParaCtrlSTU;


typedef struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT{
	lw_u16 ID;
	struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *pLast,*pNext;	
	OSlwExternMemorySTU mem;
	
	void (*DrawFun)(struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf);
	void (*ShowIdFun)(struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf);
}OSlwShowExtParaDataSTU;


typedef struct OSLW_SHOW_EXTERN_PARA_CTRL_STRUCT{
	OSlwShowExtParaDataSTU Head,*pTail,*pNow;
	lw_u16 Length,NowId,LengthMax;
	
	OSlwShowExtParaDataSTU *(*AppendFun)(struct OSLW_SHOW_EXTERN_PARA_CTRL_STRUCT *pSEPC,OSlwShowExtParaDataSTU *pSEPD);
	
	OSlwShowExtParaDataSTU *(*DeleteFun)(struct OSLW_SHOW_EXTERN_PARA_CTRL_STRUCT *pSEPC,lw_u16 id);
	
}OSlwShowExtParaCtrlSTU;


typedef struct OSLW_SHOW_PARA_CTRL_STRUCT{
	lw_u8 ID;
	OSlwCoreGroupUN TypeFlag;
	union{
		OSlwShowPvtParaCtrlSTU PvtCtrl;
		OSlwShowExtParaCtrlSTU ExtCtrl;
	}ParaCtrl;
	
	lw_u8 ShowLength;
	struct OSLW_SHOW_STRUCT *pS;
	
	lw_u16 *pParaLength,*pHeadId;
	
	void *(*HeadMoveNextFun)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_16 len,void *pbuf);
	
	void (*DrawMoreFun)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);
	
	void (*DrawCursorFun)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,OSlwShowCursorSTU *pSC);	
	
	void (*CursorLockFun)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
	void (*CursorUnlockFun)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
	void (*CursorInput)(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
	
}OSlwShowParaCtrlSTU;


typedef struct OSLW_SHOW_STRUCT{
	
	
	OSlwShowCursorSTU Cursor;
	
	OSlwShowParaCtrlSTU PageList[OSLW_SHOW_PAGE_LIST_LEN];
	
	lw_u8 length;
	
	#if OSLW_SHOW_SCREEN == OSLW_SHOW_NO_KEEP
	
	lw_u8 ShowBuffer[OSLW_SHOW_BUFFER_LEN];
	
	#endif
	
	void (*UpdateFun)(struct OSLW_SHOW_STRUCT *pS);
	
	void (*InforProcFun)(struct OSLW_SHOW_STRUCT *pS,lw_u8 ch);
		
	void (*PageTrunFun)(struct OSLW_SHOW_STRUCT *pS,lw_u8 *ppageid);
	
	void (*CursorMoveFun)(struct OSLW_SHOW_STRUCT *pS,lw_8 dir);
	
	void (*DrawPageNumFun)(struct OSLW_SHOW_STRUCT *pS);
	
	struct OSLW_CORE_STRUCT *pOS;
	
}OSlwShowSTU;


extern struct OSLW_TASK_STRUCT OSlwInlineTaskShow,*pta_Show;
extern struct OSLW_TIMER_STRUCT OSlwTimerShowUpdate;

#if OSLW_SHOW_SCREEN==OSLW_SHOW_NO_KEEP
extern struct OSLW_TIMER_STRUCT OSlwTimerShowIrp;
#endif


OSlwShowExtParaDataSTU *OSlwShowExtParaAppendFun(OSlwShowExtParaCtrlSTU *pSEPC,OSlwShowExtParaDataSTU *pSEPD);
OSlwShowExtParaDataSTU *OSlwShowExtParaDeleteFun(OSlwShowExtParaCtrlSTU *pSEPC,lw_u16 id);

void OSlwShowParaCtrlDrawMore_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);
void OSlwShowParaCtrlDrawMore_Ext(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf);

void *OSlwShowParaCtrlHeadMove_Pvt(OSlwShowParaCtrlSTU *pSPC,lw_16 len,void *pbuf);
void *OSlwShowParaCtrlHeadMove_Ext(OSlwShowParaCtrlSTU *pSPC,lw_16 len,void *pbuf);



void OSlwShowParaCtrlCursorLock_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
void OSlwShowParaCtrlCursorUnLock_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);
void OSlwShowParaCtrlCursorInput_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf);



void OSlwShowPageTrun(struct OSLW_SHOW_STRUCT *pS,lw_u8 *ppageid);
void OSlwShowCursorMoveFun(struct OSLW_SHOW_STRUCT *pS,lw_8 dir);
void OSlwShowInforProc(OSlwShowSTU *pS,lw_u8 ch);
void OSlwShowUpdate(OSlwShowSTU *pS);


void OSlwShowParaCtrlInitial(OSlwShowParaCtrlSTU *pSPC,OSlwShowSTU *pS,lw_u8 showlen,
	lw_u8 len,void *arr,
	void *lock,void *unlock,void *input,	
	void *drawdata,void *showid,void *drawcursor);

extern void OSlwDriverShow_Clr(OSlwShowSTU *pS,lw_u8 cmd,void *pbuf);

extern void OSlwDriverShowDrawParaDate(OSlwShowPvtParaCtrlSTU *psppc,lw_u8 pos,lw_u8 len,OSlwParameterSTU *pp,void *pbuf);
extern void OSlwDriverShowDrawParaId(OSlwShowPvtParaCtrlSTU *psppc,lw_u8 pos,lw_u8 len,OSlwParameterSTU *pp);
extern void OSlwDriverShowDrawCursor(OSlwShowParaCtrlSTU *pSPC,OSlwShowCursorSTU *pSC);
extern void OSlwDriverShowDrawPageNum(OSlwShowSTU *pS);

extern void OSlwTimerShowUpdateALFun(struct OSLW_TIMER_STRUCT *pt);
extern void OSlwTimerShowIrpALFun(struct OSLW_TIMER_STRUCT *pt);

	
	
#define OSLW_DRIVER_SHOW_EXT_FUN_DEFAULT(FUN_NAME) void \
	FUN_NAME(struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *pSEPD,lw_u16 id,lw_u16 len,void *pbuf)

#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_SHOW_H_ */
