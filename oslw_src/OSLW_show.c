/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_show.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"

#if OSLW_OS_ENABLE

#if OSLW_TASK_SHOW_ENABLE

OSlwTaskSTU OSlwInlineTaskShow,*pta_Show=&OSlwInlineTaskShow;

OSlwTimerSTU OSlwTimerShowUpdate=OSLW_TIMER_DEFAULT;

#if OSLW_SHOW_SCREEN==OSLW_SHOW_NO_KEEP

OSlwTimerSTU OSlwTimerShowIrp=OSLW_TIMER_DEFAULT;

#endif

/*(Ver.=0.9~)(Beg.=0.9)
OSlwShowParaDateSTU* OSlwShowCursorMove(OSlwShowParaDateSTU *pSPD,lw_16 dir)
{
	OSlwShowCursorSTU *pSC= &(pSPD->Cursor);
	lw_u8 sta,len=pSPD->pPC->ParaLength,indexbk=pSPD->HeadIndex;
	if(len==0)
	{
		return NULL;
	}

#if OSLW_SHOW_MOVE_METHOD==OSLW_SHOW_MOVE_CURSOR_FREE
	if(dir>0)
	{
		sta=((lw_u8)(++(pSC->Position_forList) >= pSPD->pPC->ParaLength)<<1) |((lw_u8)(++(pSC->Position_forScreen) >= OSLW_SHOW_WIN_LENGTH ));
	}
	else if(dir<(lw_8)0)
	{
		sta=((lw_8)((pSC->Position_forList)-- <= 0)<<1) |((lw_8)((pSC->Position_forScreen)-- <= 0 ));
	}
	else{
		;
	}

	switch(sta)
	{
		case 0:
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_OnlyCursor);
		break;

		case 1://光标屏幕越界 数组未越界
			pSPD->HeadIndex+=dir;
			pSC->Position_forScreen-=dir;
			pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);
		break;

		case 2://数组越界 光标位越界 说明参数较少屏幕显示不满
			pSPD->HeadIndex=pSC->Position_forList=pSC->Position_forScreen=0;//正常情况下是向下移动出现的
			pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);
		break;

		case 3://同时越界 代表需要显示另一端
			if(dir>0)
			{
				pSPD->HeadIndex=pSC->Position_forList=pSC->Position_forScreen=0;
				pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
				OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);

			}
			else if(dir<0)
			{
				pSC->Position_forScreen=OSLW_SHOW_WIN_LENGTH-1;
				pSPD->HeadIndex=pSPD->pPC->ParaLength-OSLW_SHOW_WIN_LENGTH;
				pSC->Position_forList=pSPD->pPC->ParaLength-1;
				pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
				OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);

			}
			else
			{

			}

		break;

		default:

		break;


	}

#elif OSLW_SHOW_MOVE_METHOD==OSLW_SHOW_MOVE_WIN_FREE

	if(dir>0)
	{

		sta=((lw_u8)(++(pSC->Position_forList) >= len)<<1) |((lw_u8)(((++pSPD->HeadIndex) + OSLW_SHOW_WIN_LENGTH) > len ))|(pSC->Position_forScreen != OSLW_SHOW_MOVE_CURSOR_POS);
	}
	else if(dir<(lw_8)0)
	{
		sta=((lw_u8)((pSC->Position_forList)-- == 0)<<1) |((lw_u8)((pSPD->HeadIndex)-- == 0 ))|(pSC->Position_forScreen != OSLW_SHOW_MOVE_CURSOR_POS);
	}
	else{
		;
	}
	switch(sta)
	{
		case 0://两遍都没有越界 滑窗
			//pSC->Position_forList++;
			pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);
		break;

		case 1://滑窗到达底部或者顶部 或者光标没有到达指定位置
			pSPD->HeadIndex=indexbk;
			//pSC->Position_forList+=dir;
			pSC->Position_forScreen+=dir;
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_OnlyCursor);
		break;

		case 2://数组越界 光标位越界 说明参数较少屏幕显示不满
			pSPD->HeadIndex=pSC->Position_forList=pSC->Position_forScreen=0;//正常情况下是向下移动出现的
			pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
			OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);
		break;

		case 3://同时越界 代表需要显示另一端
			if(dir>0)
			{
				pSPD->HeadIndex=pSC->Position_forList=pSC->Position_forScreen=0;
				pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
				OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);
			}
			else if(dir<0)
			{
				pSC->Position_forScreen=OSLW_SHOW_WIN_LENGTH-1;
				pSPD->HeadIndex=pSPD->pPC->ParaLength-OSLW_SHOW_WIN_LENGTH;
				pSC->Position_forList=pSPD->pPC->ParaLength-1;
				pSPD->ppPHead=&(pSPD->pPC->pParaList[pSPD->HeadIndex]);
				OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);

			}
			else
			{

			}
		break;

		default:

		break;

	}



#endif
	return pSPD;

}

OSlwShowParaDateSTU* OSlwShowPageTurn(OSlwShowParaDateSTU *pSPD,lw_u8 *pPage)
{

	pSPD->PCIndex=*pPage=(*pPage)%OSLW_CORE_PARA_CTRL_NUM;

	pSPD->HeadIndex=0;

	pSPD->pPC=& pSPD->pOS->ParaCtrlList[pSPD->PCIndex];

	pSPD->ppPHead=pSPD->pPC->pParaList;

	pSPD->Cursor.Position_forList=pSPD->Cursor.Position_forScreen=0;

	OSlwShowScreenUpdate(pSPD,OSlwShowUpdateMethod_All);

	return pSPD;
}






OSlwShowParaDateSTU* OSlwShowScreenUpdate(OSlwShowParaDateSTU *pSPD,OSlwShowUpdateMethodNUM method)
{

	lw_u8 n,headindexbk=pSPD->HeadIndex,indexmax=pSPD->pPC->ParaLength;

	OSlwParameterSTU **ppp=pSPD->ppPHead;

	OSLW_assert(!pSPD);

	switch(method)
	{
		case OSlwShowUpdateMethod_OnlyDate:
			for(n=0;n<OSLW_SHOW_WIN_LENGTH;n++)
			{
				if(++headindexbk>indexmax)
					break;

				OSlwShowParaDate(pSPD,n,OSLW_SHOW_WIN_LENGTH,*(ppp)++);

			}

		break;

		case OSlwShowUpdateMethod_All:
			for(n=0;n<OSLW_SHOW_WIN_LENGTH;n++)
			{
				if(++headindexbk>indexmax)
					break;
				OSlwShowParaId(pSPD,n,OSLW_SHOW_WIN_LENGTH,*(ppp));
				OSlwShowParaDate(pSPD,n,OSLW_SHOW_WIN_LENGTH,*(ppp)++);

			}
			OSlwShowCursor(pSPD);
		break;

		case OSlwShowUpdateMethod_OnlyCursor:

			OSlwShowCursor(pSPD);

		break;


		default:
		break;
	}

	return pSPD;
}



*/

OSlwShowExtParaDataSTU *OSlwShowExtParaAppendFun(OSlwShowExtParaCtrlSTU *pSEPC,OSlwShowExtParaDataSTU *pSEPD)
{

    OSLW_assert(!pSEPC);
    OSLW_assert(!pSEPD);

    pSEPC->pTail->con.pNext=pSEPD;

    pSEPD->pLast=pSEPC->pTail;

    pSEPD->ID=pSEPC->LengthMax++;

    pSEPC->pTail=	pSEPD;

    pSEPD->ID=pSEPC->Length;

    pSEPC->Length++;

    return pSEPD;
}


OSlwShowExtParaDataSTU *OSlwShowExtParaDeleteFun(OSlwShowExtParaCtrlSTU *pSEPC,lw_u16 id)
{

    OSlwShowExtParaDataSTU *psed;

    OSLW_assert(!pSEPC);

    if(id==0 || pSEPC->Length ==1 )//表示Head
    {
        return NULL;
    }
    if(id>pSEPC->Length)
    {
        psed=pSEPC->pTail;
        pSEPC->pTail=pSEPC->pTail->pLast;
        pSEPC->pTail->con.pNext=NULL;
        pSEPC->Length--;
        return psed;
    }

    psed=&(pSEPC->Head);

    while(psed)
    {
        if(psed->ID == id)
        {
            psed->pLast->con.pNext=psed->con.pNext;
            psed->con.pNext->pLast=psed->pLast;
            break;
        }
        psed = psed->con.pNext;

    }

    return psed;
}


void OSlwShowParaCtrlDrawMore_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf)
{

    OSlwShowPvtParaCtrlSTU *pSPD=&pSPC->ParaCtrl.PvtCtrl;
    OSlwShowCursorSTU *psc=&(pSPC->pS->Cursor);
    lw_u8 n,headindexbk=pSPD->HeadIndex,indexmax=pSPD->pPC->ParaLength,i=headindexbk;

    lw_16 lockedid=psc->Status==OSlwShowCursorStatus_Lock?psc->Position_forList:-1;
    OSlwParameterSTU **ppp=pSPD->ppPHead;

    OSLW_assert(!pSPC);

    for(n=0; n<pSPC->ShowLength && ++headindexbk<=indexmax && n<=num ; n++,i++)
    {

        if(FlagForAllUpdate) OSlwDriverShowDrawParaId(pSPD,n,pSPC->ShowLength,*(ppp));
        if(i!=lockedid)
            OSlwDriverShowDrawParaDate(pSPD,n,pSPC->ShowLength,*(ppp)++,NULL);
        else
            OSlwDriverShowDrawParaDate(pSPD,n,pSPC->ShowLength,*(ppp)++,&lockedid);

    }


}

void OSlwShowParaCtrlDrawMore_Ext(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 num,lw_u8 FlagForAllUpdate,void *pbuf)
{

    OSlwShowExtParaDataSTU *psepd=pSPC->ParaCtrl.ExtCtrl.pNow;
    lw_u16 id=pSPC->ParaCtrl.ExtCtrl.NowId,showlen=pSPC->ShowLength,datalen=pSPC->ParaCtrl.ExtCtrl.Data,i=0;

    OSLW_assert(!pSPC);

    do
    {
        if(psepd->DrawFun)
            psepd->DrawFun(psepd,i,pSPC->ShowLength,NULL);
        if(FlagForAllUpdate && psepd->ShowIdFun)
            psepd->ShowIdFun(psepd,i,pSPC->ShowLength,NULL);
        psepd=psepd->con.pNext;
        i++;
    } while(psepd &&  ++id<datalen && --showlen && --num );

}

void *OSlwShowParaCtrlHeadMove_Pvt(OSlwShowParaCtrlSTU *pSPC,lw_16 len,void *pbuf)
{

    lw_16 reallen=len>=0?len:-len,realid,paralen=(*(pSPC->pParaLength));
    lw_8 dir=len>=0?1:-1;
    OSLW_assert(!pSPC);
    if(!pbuf)
    {
        reallen=(reallen % (*(pSPC->pParaLength)))*dir;

        realid=(lw_16)(*(pSPC->pHeadId)) + reallen;

        realid=realid<0?realid+paralen:realid;
        realid=realid>=paralen?realid-paralen:realid;


        pSPC->ParaCtrl.PvtCtrl.HeadIndex=realid;

        pSPC->ParaCtrl.PvtCtrl.ppPHead=& (pSPC->ParaCtrl.PvtCtrl.pPC->pParaList[realid]);
    }
    else if(*((lw_8 *)pbuf) > 0)
    {
        pSPC->ParaCtrl.PvtCtrl.HeadIndex=paralen-pSPC->ShowLength;

        pSPC->ParaCtrl.PvtCtrl.ppPHead=& (pSPC->ParaCtrl.PvtCtrl.pPC->pParaList[pSPC->ParaCtrl.PvtCtrl.HeadIndex]);

    }
    else
    {

        pSPC->ParaCtrl.PvtCtrl.HeadIndex=0;

        pSPC->ParaCtrl.PvtCtrl.ppPHead= (pSPC->ParaCtrl.PvtCtrl.pPC->pParaList);

    }
    return(void *)pSPC;
}


void *OSlwShowParaCtrlHeadMove_Ext(OSlwShowParaCtrlSTU *pSPC,lw_16 len,void *pbuf)
{
    lw_16 reallen=len>=0?len:-len,realid,paralen=(*(pSPC->pParaLength)),nowid=pSPC->ParaCtrl.ExtCtrl.NowId;
    lw_8 dir=len>=0?1:-1;
    OSlwShowExtParaCtrlSTU *psep=&pSPC->ParaCtrl.ExtCtrl;

    OSLW_assert(!pSPC);

    if(!pbuf)
    {
        reallen=(reallen % (*(pSPC->pParaLength)))*dir;

        realid=(lw_16)(*(pSPC->pHeadId)) + reallen;

        realid=realid<0?realid+paralen:realid;
        realid=realid>=paralen?realid-paralen:realid;

        reallen=reallen*dir;
        psep->NowId= realid;
        if(reallen==0)
            return (void *)pSPC;
        else if(realid > nowid)//向后移动
        {
            do
            {
                psep->pNow=psep->pNow->con.pNext;
            } while(--reallen && psep->pNow->con.pNext);

        }
        else
        {
            do
            {
                psep->pNow=psep->pNow->pLast;
            } while(--reallen && psep->pNow->pLast);
        }

    }
    else if(*((lw_8 *)pbuf) > 0)
    {
        lw_u8 len=pSPC->ShowLength;
        psep->pNow=psep->pTail;
        while(--len && psep->pNow)
        {
            psep->pNow=psep->pNow->pLast;

        }
        psep->NowId=psep->pNow->ID;

    }
    else
    {

        psep->NowId=0;

        psep->pNow=& psep->Head;

    }
    return(void *)pSPC;
}

void OSlwShowPageTrun(struct OSLW_SHOW_STRUCT *pS,lw_u8 *ppageid)
{


    OSlwShowParaCtrlSTU *pspc,*pspclast=pS->Cursor.pSPC;

    lw_8 buf=(lw_8)-1;

    OSLW_assert(!pS);
    OSLW_assert(!ppageid);

    *ppageid=(*ppageid) % (pS->length);

    pS->Cursor.Position_forPage=*ppageid;

    pS->Cursor.Position_forList=pS->Cursor.Position_forScreen=0;

    pS->Cursor.pSPC=pspc=&pS->PageList[pS->Cursor.Position_forPage];

    OSlwDriverShow_Clr(pS,0,NULL);

    if((pS->DrawPageNumFun))
        pS->DrawPageNumFun(pS);

    pspc->HeadMoveNextFun(pspc,0,&buf);

    pspc->DrawMoreFun(pspc,0xff,1,NULL);

    if(pS->Cursor.Status == OSlwShowCursorStatus_Lock && (pspclast->CursorUnlockFun))
        pspclast->CursorUnlockFun(pspclast,0,NULL);
    else
        pspc->DrawCursorFun(pspc,&pS->Cursor);


}


void OSlwShowParaCtrlCursorLock_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf)
{
    OSlwShowCursorSTU *psc=&(pSPC->pS->Cursor);

    if(psc->Status == OSlwShowCursorStatus_Lock)
        return;
    else
    {
        OSlwParameterSTU *pP=psc->pSPC->ParaCtrl.PvtCtrl.pPC->pParaList[psc->Position_forList];
        psc->Status=OSlwShowCursorStatus_Lock;
        OSlwParameter_SelfSL(pP,OSLW_PARA_SELF_CMD_SAVE);
        pSPC->DrawCursorFun(pSPC,psc);
    }
    return;

}


void OSlwShowParaCtrlCursorUnLock_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf)
{
    OSlwShowCursorSTU *psc=&(pSPC->pS->Cursor);

    if(psc->Status == OSlwShowCursorStatus_Free)
        return;
    else
    {
        OSlwParameterSTU *pP=psc->pSPC->ParaCtrl.PvtCtrl.pPC->pParaList[psc->Position_forList];
        psc->Status=OSlwShowCursorStatus_Free;
        OSlwParameter_SelfSL(pP,OSLW_PARA_SELF_CMD_LOAD);
        pSPC->DrawCursorFun(pSPC,psc);
    }
    return;

}


void OSlwShowParaCtrlCursorInput_Pvt(struct OSLW_SHOW_PARA_CTRL_STRUCT *pSPC,lw_u8 cmd,void *pbuf)
{
    OSlwShowCursorSTU *psc=&(pSPC->pS->Cursor);
    if(psc->Status == OSlwShowCursorStatus_Free)
        return;
    else
    {
        OSlwParameterSTU *pP=psc->pSPC->ParaCtrl.PvtCtrl.pPC->pParaList[psc->Position_forList];
        switch(cmd)
        {

        case OSLW_SHOW_CMD_UP:
            OSlwParameter_SelfUp(pP,0);
            break;


        case OSLW_SHOW_CMD_DOWN:
            OSlwParameter_SelfDown(pP,0);
            break;


        default:

            break;
        }


    }

}


void OSlwShowCursorMoveFun(struct OSLW_SHOW_STRUCT *pS,lw_8 dir)
{

    OSlwShowCursorSTU *pSC= &(pS->Cursor);
    OSlwShowParaCtrlSTU *pspc=pSC->pSPC;
    lw_u8 sta,len=*(pS->Cursor.pSPC->pParaLength);//,indexbk=pS->Cursor.Position_forList;
    lw_u16 headid=*(pS->Cursor.pSPC->pHeadId);
    lw_8 buf=0;

    OSLW_assert(!pS);

    if(len==0)
    {
        return;
    }

#if OSLW_SHOW_MOVE_METHOD==OSLW_SHOW_MOVE_CURSOR_FREE
    if(dir>0)
    {
        sta=((lw_u8)(++(pSC->Position_forList) >= len)<<1) |((lw_u8)(++(pSC->Position_forScreen) >= pspc->ShowLength ));
    }
    else if(dir<(lw_8)0)
    {
        sta=((lw_8)((pSC->Position_forList)-- <= 0)<<1) |((lw_8)((pSC->Position_forScreen)-- <= 0 ));
    }
    else {
        ;
    }

    switch(sta)
    {
    case 0:
        pspc->DrawCursorFun(pspc,pSC);
        break;

    case 1://光标屏幕越界 数组未越界
        pspc->HeadMoveNextFun(pspc,dir,NULL);
        pSC->Position_forScreen-=dir;
        pspc->DrawMoreFun(pspc,0xff,1,NULL);
        pspc->DrawCursorFun(pspc,pSC);
        break;

    case 2://数组越界 光标位越界 说明参数较少屏幕显示不满
        pSC->Position_forList=pSC->Position_forScreen=0;//正常情况下是向下移动出现的
        buf=dir;
        pspc->HeadMoveNextFun(pspc,0,&buf);
        pspc->DrawMoreFun(pspc,0xff,1,NULL);
        pspc->DrawCursorFun(pspc,pSC);
        break;

    case 3://同时越界 代表需要显示另一端
        if(dir>0)
        {
            pSC->Position_forList=pSC->Position_forScreen=0;
            buf=-dir;
            pspc->HeadMoveNextFun(pspc,0,&buf);
            pspc->DrawMoreFun(pspc,0xff,1,NULL);
            pspc->DrawCursorFun(pspc,pSC);

        }
        else if(dir<0)
        {
            pSC->Position_forList=len-1;
            pSC->Position_forScreen=pspc->ShowLength-1;
            buf=-dir;
            pspc->HeadMoveNextFun(pspc,0,&buf);
            pspc->DrawMoreFun(pspc,0xff,1,NULL);
            pspc->DrawCursorFun(pspc,pSC);

        }
        else
        {
        }

        break;

    default:

        break;


    }

#elif OSLW_SHOW_MOVE_METHOD==OSLW_SHOW_MOVE_WIN_FREE

    if(dir>0)
    {

        sta=((lw_u8)(++(pSC->Position_forList) >= len)<<1) |((lw_u8)(((headid+1) + pspc->ShowLength) > len ))|(pSC->Position_forScreen != OSLW_SHOW_MOVE_CURSOR_POS);
    }
    else if((lw_8)dir<(lw_8)0)
    {
        sta=((lw_u8)((pSC->Position_forList)-- == 0)<<1) |((lw_u8)((headid) == 0 ))|(pSC->Position_forScreen != OSLW_SHOW_MOVE_CURSOR_POS);
    }
    else {
        ;
    }
    switch(sta)
    {
    case 0://两遍都没有越界 滑窗
        pspc->HeadMoveNextFun(pspc,dir,NULL);
        pspc->DrawMoreFun(pspc,0xff,1,NULL);
        break;

    case 1://滑窗到达底部或者顶部 或者光标没有到达指定位置
        buf=pSC->Position_forScreen;
        buf+=dir;
        buf=buf>=pspc->ShowLength?pspc->ShowLength-1:buf;
        buf=buf<=0?0:buf;
        pSC->Position_forScreen=buf;
        pspc->DrawCursorFun(pspc,pSC);
        if(pspc->ShowLength <= OSLW_SHOW_MOVE_CURSOR_POS)
        {
            pspc->HeadMoveNextFun(pspc,dir,NULL);
            pspc->DrawMoreFun(pspc,0xff,1,NULL);
        }
        break;

    case 2://数组越界 光标未越界 说明参数较少屏幕显示不满
        buf=(lw_8)-1;
        pspc->HeadMoveNextFun(pspc,0,&buf);
        pspc->DrawCursorFun(pspc,pSC);
        break;

    case 3://同时越界 代表需要显示另一端
        if(dir>0)
        {
            pSC->Position_forList=pSC->Position_forScreen=0;
            buf=-dir;
            pspc->HeadMoveNextFun(pspc,0,&buf);
            pspc->DrawMoreFun(pspc,0xff,1,NULL);
            pspc->DrawCursorFun(pspc,pSC);

        }
        else if(dir<0)
        {
            pSC->Position_forList=len-1;
            pSC->Position_forScreen=pspc->ShowLength-1;
            buf=-dir;
            pspc->HeadMoveNextFun(pspc,0,&buf);
            pspc->DrawMoreFun(pspc,0xff,1,NULL);
            pspc->DrawCursorFun(pspc,pSC);

        }
        else
        {
        }
        break;

    default:

        break;

    }



#endif
    return ;

}




void OSlwShowParaCtrlInitial(OSlwShowParaCtrlSTU *pSPC,OSlwShowSTU *pS,lw_u8 showlen,
                             lw_u8 len,void *arr,
                             void *lock,void *unlock,void *input,
                             void *drawdata,void *showid,void *drawcursor)
{

    OSLW_assert(!pSPC);

    if(len)
    {
        pSPC->TypeFlag.all=0x0002;
        pSPC->pHeadId = &pSPC->ParaCtrl.ExtCtrl.NowId;
        pSPC->pParaLength=&pSPC->ParaCtrl.ExtCtrl.Data;

        pSPC->ShowLength=showlen;
        pSPC->DrawCursorFun=(void (*)(struct OSLW_SHOW_PARA_CTRL_STRUCT *, OSlwShowCursorSTU *))drawcursor;
        pSPC->DrawMoreFun=OSlwShowParaCtrlDrawMore_Ext;
        pSPC->HeadMoveNextFun=OSlwShowParaCtrlHeadMove_Ext;

        pSPC->ParaCtrl.ExtCtrl.Head.DrawFun=(void (*)(struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *,lw_u16,lw_u16,void *pbuf))drawdata;
        pSPC->ParaCtrl.ExtCtrl.Head.ShowIdFun=(void (*)(struct OSLW_SHOW_EXTERN_PARA_DATA_STRUCT *,lw_u16,lw_u16,void *pbuf))showid;
        pSPC->ParaCtrl.ExtCtrl.Head.mem.Data=len;
        pSPC->ParaCtrl.ExtCtrl.Head.mem.pData=arr;

        pSPC->ParaCtrl.ExtCtrl.pNow=pSPC->ParaCtrl.ExtCtrl.pTail=&pSPC->ParaCtrl.ExtCtrl.Head;
        pSPC->ParaCtrl.ExtCtrl.Data=pSPC->ParaCtrl.ExtCtrl.DataMax=1;

        pSPC->ParaCtrl.ExtCtrl.AppendFun=OSlwShowExtParaAppendFun;
        pSPC->ParaCtrl.ExtCtrl.DeleteFun=OSlwShowExtParaDeleteFun;
    }
    else
    {
        pSPC->TypeFlag.all=0x0001;

        pSPC->ParaCtrl.PvtCtrl.pPC=arr;

        pSPC->pHeadId =(lw_u16 *) &(pSPC->ParaCtrl.PvtCtrl.HeadIndex);
        pSPC->pParaLength=(lw_u16 *)&(pSPC->ParaCtrl.PvtCtrl.pPC->ParaLength);
        pSPC->ShowLength=showlen;
        pSPC->DrawCursorFun=(void (*)(struct OSLW_SHOW_PARA_CTRL_STRUCT *, OSlwShowCursorSTU *))drawcursor;
        pSPC->DrawMoreFun=OSlwShowParaCtrlDrawMore_Pvt;
        pSPC->HeadMoveNextFun=OSlwShowParaCtrlHeadMove_Pvt;

        pSPC->ParaCtrl.PvtCtrl.ppPHead=((OSlwParaCtrlSTU *)arr)->pParaList;

    }
    pSPC->CursorInput=input;
    pSPC->CursorLockFun=lock;
    pSPC->CursorUnlockFun=unlock;
    pSPC->ID=pS->length++;
    pSPC->pS=pS;

}




void OSlwShowInforProc(OSlwShowSTU *pS,lw_u8 ch)
{
    OSlwShowParaCtrlSTU *pspc=pS->Cursor.pSPC;
    OSLW_assert(!pS);

    switch(ch)
    {
    case OSLW_SHOW_CMD_TURN:
        pS->Cursor.Position_forPage++;
        pS->PageTrunFun(pS,&(pS->Cursor.Position_forPage));
        break;

    case OSLW_SHOW_CMD_DOWN://down
        if(pS->Cursor.Status == OSlwShowCursorStatus_Lock && (pspc->CursorInput))
            pspc->CursorInput(pspc,ch,NULL);
        else
            pS->CursorMoveFun(pS,-1);
        break;

    case OSLW_SHOW_CMD_UP://up
        if(pS->Cursor.Status == OSlwShowCursorStatus_Lock && (pspc->CursorInput))
            pspc->CursorInput(pspc,ch,NULL);
        else
            pS->CursorMoveFun(pS,1);
        break;

    case OSLW_SHOW_CMD_LOCK:
        if((pspc->CursorLockFun)) pspc->CursorLockFun(pspc,0,NULL);
        break;

    case OSLW_SHOW_CMD_CAN:
        if((pspc->CursorUnlockFun)) pspc->CursorUnlockFun(pspc,0,NULL);
        break;

    case OSLW_SHOW_CMD_NEXT:
        if((pspc->CursorUnlockFun)) pspc->CursorUnlockFun(pspc,1,NULL);
        break;
    default:

        break;
    }



}

void OSlwShowUpdate(OSlwShowSTU *pS)
{
    pS->Cursor.pSPC->DrawMoreFun(pS->Cursor.pSPC,0xff,0,NULL);
}


OSlwShowSTU *pSh;
void OSlwInlineTaskShowExe(struct OSLW_TASK_STRUCT *pta)
{

    static lw_u8 n=0;
    pSh=& pta_Show->pOS->Show;
    pta_Show->TaskGroupFlag.AimStatus.all=0xffff;
    pta_Show->pOS->TimerCtrl.pTimerList[0]->TimerEnableFlag=1;
#if OSLW_SHOW_SCREEN == OSLW_SHOW_NO_KEEP
    pta_Show->pOS->TimerCtrl.pTimerList[1]->TimerEnableFlag=1;
#endif

    pSh->PageTrunFun(pSh,&n);

    OSLW_TASK_LOOP(pta)
    {

        OSlwGroupFlagWait(pta_Show);
        if(pta_Show->TaskGroupFlag.CurrentStatus.bits.bit0)
        {
            pSh->UpdateFun(pSh);
            pta_Show->TaskGroupFlag.CurrentStatus.bits.bit0=0;
        }

        if(pta_Show->TaskGroupFlag.CurrentStatus.bits.bit1 && pta_Show->Concierge.giftR.GiftNum)
        {
            do
            {
                OSlwGiftUnitSTU *pgu=pta_Show->Concierge.giftR.pHead->pGU;
                pSh->InforProcFun(pSh,pgu->PrivateUnit.KeyBoardInput);
                pta_Show->Concierge.giftR.pHead->Status=pta_Show->Concierge.giftR.pHead->Method;

            } while(pta_Show->Concierge.giftR.MoveNextFun(&pta_Show->Concierge.giftR));

            pta_Show->TaskGroupFlag.CurrentStatus.bits.bit1=0;
        }

        if(pta_Show->TaskGroupFlag.CurrentStatus.bits.bit2)
        {
            pSh->Cursor.Position_forPage++;
            pSh->PageTrunFun(pSh,&pSh->Cursor.Position_forPage);

            pta_Show->TaskGroupFlag.CurrentStatus.bits.bit2=0;
        }


    }

}
#endif


#endif


