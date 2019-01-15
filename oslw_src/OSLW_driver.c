/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_driver.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"
//#include "usart.h"
//#include "OLED.h"
//#include "led.h"
/*(Ver.=0.9~)(Beg.=0.9)
void OSlwCommunTransmit(OSlwCommunSTU *pc)
{

	int i=0;
	while((pc->TxLength)--)
	{
		while((USART1->SR&0X40)==0);//循环发送,直到发送完毕
		USART1->DR = (u8) (pc->TxList[i]);
		i++;
	}

	pc->pTxHead = pc->pTxTail = pc->TxList;
	pc->TxLength=0;


}
*/



#if OSLW_OS_ENABLE
#if OSLW_TASK_SHOW_ENABLE
void OSlwDriverShow_Clr(OSlwShowSTU *pS,lw_u8 cmd,void *pbuf)
{

    OLED_CLS();

}

void OSlwDriverShowDrawParaDate(OSlwShowPvtParaCtrlSTU *psppc,lw_u8 pos,lw_u8 len,OSlwParameterSTU *pp,void *pbuf)
{
    /*(Ver.=0.9~)(Beg.=0.9)
    lw_u8 str[20];
    if(pbuf==NULL)
    	if(pp->pExtParameter)
    		sprintf(str,"%f",_ParaToF(*(pp->pExtParameter)));
    	else
    	sprintf(str,"%f",_ParaToF(pp->Parameter));
    else
    	sprintf(str,"%f",_ParaToF(pp->ParameterShallow));
    OLED_P6x8Str(32,(pos+1)*6/(len),str);
    */
}


void OSlwDriverShowDrawParaId(OSlwShowPvtParaCtrlSTU *psppc,lw_u8 pos,lw_u8 len,OSlwParameterSTU *pp)
{

    //printf("%d_%d %d:\n",pos,len,pp->ParaId);
    lw_u8 str[20];
    sprintf(str,"%d:",pp->ParaId);
    OLED_P6x8Str(15,(pos+1)*6/(len),str);


}

void OSlwDriverShowDrawCursor(OSlwShowParaCtrlSTU *pSPC,OSlwShowCursorSTU *pSC)
{

//	printf("X%d\n",pSC->Position_forScreenLast);
//	pSC->Position_forScreenLast=pSC->Position_forScreen;
//	printf("V%d\n",pSC->Position_forScreen);
    OLED_CLS8x12(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength));
    pSC->Position_forScreenLast=pSC->Position_forScreen;
    if(pSC->Status == OSlwShowCursorStatus_Free)
        OLED_P6x8(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength),">");
    else
        OLED_P6x8(0,(pSC->Position_forScreenLast+1)*6/(pSC->pSPC->ShowLength),":");
}


void OSlwDriverShowDrawPageNum(OSlwShowSTU *pS)
{

//	printf("This is page %d\n",pS->Cursor.Position_forPage);

    lw_u8 str[20];
    sprintf(str,"PAGE: %d",pS->Cursor.Position_forPage);
    OLED_P6x8Str(0,0,str);

}

void OLED_CLS_win(void)
{
    int y,x;
    for(y=1; y<8; y++)
    {
        OLED_WrCmd(0xb0+y);
        OLED_WrCmd(0x01);
        OLED_WrCmd(0x10);
        for(x=0; x<X_WIDTH; x++)
            OLED_WrDat(0,0);
    }
}

OSLW_DRIVER_SHOW_EXT_FUN_DEFAULT(OSlwDriverShowDATA1)
{
    lw_u16 i=pSEPD->mem.Data/sizeof(lw_u8);
    lw_u8 *p=pSEPD->mem.pData;
    OLED_CLS_win();
    while(i--)
    {
        OLED_PutPixel(i+20,p[i]);
    }
}

OSLW_DRIVER_SHOW_EXT_FUN_DEFAULT(OSlwDriverShowDATA2)
{
    auto lw_u16 i=pSEPD->mem.Data/sizeof(lw_u8)+20;

    lw_u8 *p=pSEPD->mem.pData;

    OLED_CLS_win();
    OLED_Rectangle(p[0],p[1],p[2],p[3],0);

}


OSLW_DRIVER_SHOW_EXT_FUN_DEFAULT(OSlwDriverShowEXTid)
{



}

void OSlwDriverShowDrawCursorExt(OSlwShowParaCtrlSTU *pSPC,OSlwShowCursorSTU *pSC)
{

}


void OSlwTimerShowUpdateALFun(OSlwTimerSTU *pt)
{


    OSlwInlineTaskShow.TaskGroupFlag.CurrentStatus.bits.bit0=1;


    return;
}

void OSlwTimerShowIrpALFun(OSlwTimerSTU *pt)
{



}
#endif
#endif

