/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_timer.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef OSLW_TIMER_H_
#define OSLW_TIMER_H_

#include "OSLW_define.h"

typedef enum{
	OSlwTimerMode_Once,
	OSlwTimerMode_Continue
}OSlwTimerModeNUM;


typedef struct OSLW_TIMER_STRUCT{
	lw_u8 ID;
	lw_u32 Count,CountMax;
	lw_u8 TimerEnableFlag;
	
	OSlwTimerModeNUM Mode;
	
	void *pLeader;
	
	OSlwExternMemorySTU ExtMem;
	
	void (*ClockCBFun)(struct OSLW_TIMER_STRUCT *pt);
	void (*AlarmCBFun)(struct OSLW_TIMER_STRUCT *pt);
	
	
}OSlwTimerSTU;


typedef struct OSLW_TIMER_CTRL_STRUCT{
	OSlwTimerSTU *pTimerList[OSLW_TIMER_CTRL_LIST_LEN];
	lw_u8 Length;
	
}OSlwTimerCtrlSTU;


void OSlwTimerClockCallBack(OSlwTimerSTU *pt);

#define OSLW_TIMER_DEFAULT(H,M,S,MS) {0,\
(((lw_u32)H*3600l+(lw_u32)M*60L+(lw_u32)S)*1000l+MS)/(lw_u32)OSLW_CORE_TICK_MS,\
(((lw_u32)H*3600l+(lw_u32)M*60L+(lw_u32)S)*1000l+MS)/(lw_u32)OSLW_CORE_TICK_MS,\
0,OSlwTimerMode_Once,NULL,{0,NULL},OSlwTimerClockCallBack,NULL}


#define OSlwTimerSetup(T,H,M,S,MS) do{(T).Count=(T).CountMax=(((lw_u32)H*3600l+(lw_u32)M*60L+(lw_u32)S)*1000l+MS)/(lw_u32)OSLW_CORE_TICK_MS;}while(0)

#define OSlwTimerPause(T)	do{(T).TimerEnableFlag=0;}while(0)

#define OSlwTimerRun(T) do{(T).TimerEnableFlag=1;}while(0)

#define OSlwTimerStop(T) do{(T).TimerEnableFlag=0;(T).Count=(T).CountMax;}while(0)

#define OSlwTimerStart(T,H,M,S,MS) do{(T).Count=(((lw_u32)H*3600l+(lw_u32)M*60L+(lw_u32)S)*1000l+MS)/(lw_u32)OSLW_CORE_TICK_MS;(T).TimerEnableFlag=1;}while(0)

#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_TIMER_H_ */
