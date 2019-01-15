/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_communication.h
 *
 *  Created on: 2017-7-26
 *      Author: slw
 */

#ifndef OSLW_COMMUN_H_
#define OSLW_COMMUN_H_

#include "OSLW_define.h"
#include "OSLW_tool.h"
#include "OSLW_gift.h"



typedef struct OSLW_MODBUS_STRCUT{


	
	OSlwToolSlipwinSTU ComRX;
	
	lw_u8 Address;
	lw_u8 TxList[OSLW_COMMUN_TX_LENGTH];
	lw_u16 TxLength;
	lw_u8 *pTx;
	lw_u8 *pTxTail,*pTxHead;
	
	lw_u8 RxList[OSLW_COMMUN_RX_LENGTH+1];
	lw_u16 RxLength;
	lw_u8 *pRx;
	
	lw_u8 RxListBuf[OSLW_COMMUN_RX_LENGTH/2+1];

	lw_u8 ReadyLenList[OSLW_COMMUN_READY_LEN_LIST_LENGTH];
	
	lw_8 (*isReadyFun)(struct OSLW_MODBUS_STRCUT *pcom);
	void (*TxReadyFun)(struct OSLW_MODBUS_STRCUT *pcom);
	
}OSlwModBusCoreSTU;


extern struct OSLW_TASK_STRUCT OSlwInlineTaskCommun;

void OSlwInlineTaskCommunExe(struct OSLW_TASK_STRUCT *pta);
void OSlwModBusCallBack(struct OSLW_CORE_STRUCT *pOS,lw_u8 ComID,lw_u8 *pdata,lw_u16 len);
lw_8 OSlwModBusIsReady(void *pcom);
void OSlwModBusTxReady(void *pcom);
lw_u16 CRC16(lw_u8 *puchMsg, lw_u16 usDataLen) ;

extern void OSlwModBusTransmit(void *pc);

#define MODBUS_CONNECT_UINT8_TO_UINT16(A,B) (((lw_u32)(A))<<8)+B
#define MODBUS_CONNECT_INT8_TO_INT16(A,B) (((lw_32)(A))<<8)+B

#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_COMMUN_H_ */
