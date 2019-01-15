/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_gift.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

#if OSLW_OS_ENABLE && OSLW_GIFT_EN

#if !(OSLW_SIMPLE_LEVEL >= 3)
//------------------------------------------
//<函数名>OSlwGiftConciergeInitial</函数名>
//<功能说明>门卫管理系统 初始化</功能说明>
//<输入说明>pgc:this</输入说明>
//<输出说明>void</输出说明>
//------------------------------------------


void OSlwGiftConciergeInitial(OSlwGiftConciergeSTU *pgc)
{

    OSLW_assert(!pgc);
    memset(pgc,0,sizeof(OSlwGiftConciergeSTU));//初始化清零
    OSlwToolDListInitial(&(pgc->giftR.List), 0, NULL);
#if !(OSLW_SIMPLE_MODE)
    pgc->giftR.MoveNextFun=OSlwGiftReceiveMoveNext;//双向链表移动成员函数
    pgc->giftR.SlipNextFun=OSlwGiftReceiveSlipNext;//滑动函数
    pgc->giftR.AppendFun=OSlwGiftReceiveAppend;//追加成员函数
#endif
    pgc->giftR.pGC=pgc;//接收器 指针

    //OSlwGiftTransmitInitial(&(pgc->giftT));

    // pgc->giftT.pGC=pgc;//发送器 指针

}


//逐渐放弃 采用内存分配机制代替
#if 0
void OSlwGiftTransmitInitial(OSlwGiftTransmitSTU *pgt)
{
    int n=0;

    OSLW_assert(!pgt);
#if !(OSLW_SIMPLE_MODE)
    pgt->IdleFun=OSlwGiftTransmitIdle;//得到空闲成员索引函数
#endif

    for(; n<OSLW_GIFT_POSTMAN_MAX; n++) //快递员初始化
    {
        pgt->PostmanList[n].PostmanID = n;
        pgt->PostmanList[n].Status = GiftPostmanStatus_Free;//设置状态为自由
        pgt->PostmanList[n].pGT = pgt;//快递员 所属指针初始化
    }

}
#endif


//------------------------------------------
//<函数名>OSlwGiftReceiveAppend</函数名>
//<功能说明>礼物发送函数</功能说明>
//<输入说明>pgr:this pgp:快递员指针</输入说明>
//<输出说明>双向链表头部</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftReceiveAppend(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU *pgp)
{

    OSLW_assert(!pgr);
    OSLW_assert(!pgp);

#if 0

    if(!(pgr->GiftNum))//判断当前礼物数量是否==0
    {   //为0

        pgr->pHead=pgr->pTail=pgp;//首尾相同
        pgr->pHead->pPrior=NULL;//上一个为空
    }
    else
    {   //不为0
        pgr->pTail->pNext=pgp;//当前尾部的下一个接上
        pgp->pPrior=pgr->pTail;//pgp的前一个为当前尾部
        pgr->pTail=pgp;	//更新尾部
    }

    pgp->pGT->PostmanFlag.all|=((lw_u64)1<<pgp->ID);//发送标志组置起

    pgp->Status=GiftPostmanStatus_InDelivery;//快递员状态为正在投递
    pgr->pTail->pNext=NULL;//尾部下一个为空
    pgr->GiftMax++;//最大数++
    pgr->GiftNum++;//礼物数量++

    return pgr->pHead;//返回头部
#endif // 0

    pgr->List.AppendFUN(&(pgr->List), &(pgp->node));

    //逐渐放弃 采用内存分配机制代替
#if 0
    if(pgp->pGT)//说明是有主邮递员
    {
        pgp->pGT->PostmanFlag.all |= ((lw_u64)1 << pgp->PostmanID);//发送标志组置起
        if(pgp->Status!=GiftPostmanStatus_Free)//如果快递员 状态为自由 才可以放入等待链表
        {
            //如果不为自由
            pgr->pGC->pta->pOS->CoreError=OSlwCoreError_GiftStatusError;//将内核错误标志置起
            return NULL;//返回NULL
        }

    }
#endif

    pgp->Status = GiftPostmanStatus_InDelivery;//快递员状态为正在投递
    return pgp;

}


//------------------------------------------
//<函数名>OSlwGiftReceiveMoveNext</函数名>
//<功能说明>礼物接收器移动 用于 do{...}while(..move..)循环</功能说明>
//<输入说明>pgr:this</输入说明>
//<输出说明>当前头部</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftReceiveMoveNext(OSlwGiftReceiveSTU *pgr)
{

    OSlwGiftPostmanSTU *p,*q;
    OSlwMemoryBasicSTU *pmem;

    OSLW_assert(!pgr);

    if(pgr->List.NowLen<=1)//如果数量已经为0 不用再移动
    {
        return NULL;
    }
#if 0
    p=pgr->pHead;//取得同步
    if(pgr->GiftNum==1)//如果数量已经==1
    {
        pgr->pHead=NULL;//释放头部
        pgr->pTail=NULL;//释放尾部
        q=NULL;//返回NULL 因为采用do {}while() 循环
    }
    else
    {
        pgr->pHead=p->pNext;//头部下一个
        q=pgr->pHead;//返回当前头部
    }
    pgr->GiftNum--;//礼物数量--

    if(p->Status == p->Method)//如果之前任务状态 与 模式系统
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1<<p->ID);//清除发送标志
        p->Status=GiftPostmanStatus_Free;//并且状态为自由

    }
#endif // 0


    p = (OSlwGiftPostmanSTU *)pgr->List.Head.con.pNext;//取得除了头结点的第一个

    if (!p)
    {
        return NULL;
    }
    //逐渐放弃 采用内存分配机制代替
#if 0
    if (p->Status == p->Method && (p->pGT))//如果任务状态 与 目标模式一致 且 邮递员有主
    {
        p->pGT->PostmanFlag.all &= ~((lw_u64)1 << p->PostmanID);//清除发送标志
        p->Status = GiftPostmanStatus_Free;//并且状态为自由

    }
#endif

    q = (OSlwGiftPostmanSTU *)(pgr->List.RemoveFUN(&(pgr->List), OSlwToolDListCmpCount, 1, NULL));


    return q;
}



//------------------------------------------
//<函数名>OSlwGiftReceiveSlipNext</函数名>
//<功能说明>礼物接收器移动 用于 do{...}while(..slip(,&pgp)..)循环</功能说明>
//<输入说明>pgr:this,ppGP:指向指针的指针</输入说明>
//<输出说明>修改之后的指针值</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftReceiveSlipNext(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU **ppGP)
{
    OSlwGiftPostmanSTU *p;

    OSLW_assert(!pgr);
    OSLW_assert(!ppGP);
    OSLW_assert(!(*ppGP));

    p=(OSlwGiftPostmanSTU *)((*ppGP)->node.con.pNext);

    /*(Ver.=0.9~)(Beg.=0.9)
    if(_OSlwToolDListRemoveNode(&(pgr->List),(OSlwToolDListNodeSTU *)(*ppGP)))
    {
    	return NULL;
    }
    */



    if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
    {
        _OSlwToolDListRemoveNode(&(pgr->List),(OSlwToolDListNodeSTU *)(*ppGP));

        if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//如果释放失败
        {

            (*ppGP)=p;
            return p;

        }
    }

    (*ppGP)=p;
    return *ppGP;
}


//------------------------------------------
//<函数名>OSlwGiftReceiveForeah</函数名>
//<功能说明>遍历礼物/功能说明>
//<输入说明>pgr:this,ppGP:指向指针的指针</输入说明>
//<输出说明>下一个礼物</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftReceiveForeah(OSlwGiftReceiveSTU *pgr,OSlwGiftPostmanSTU **ppGP)
{
    OSlwGiftPostmanSTU *p;

    OSLW_assert(!pgr);
    OSLW_assert(!ppGP);
    OSLW_assert(!(*ppGP));

    p=(OSlwGiftPostmanSTU *)((*ppGP)->node.con.pNext);
	
		if((*ppGP) != (OSlwGiftPostmanSTU *)(&(pgr->List.Head)))
		{
			if((*ppGP)->Method == (*ppGP)->Status && (*ppGP)->pmem)
			{
					_OSlwToolDListRemoveNode(&(pgr->List),(OSlwToolDListNodeSTU *)(*ppGP));

					if(!((*ppGP)->pmem->Free((*ppGP)->pmem,(*ppGP))))//如果释放失败
					{

							(*ppGP)=p;
							return p;

					}
			}
			 
		}
		(*ppGP)=p;
    return *ppGP;
}

//逐渐放弃 采用内存分配机制代替
#if 0
//------------------------------------------
//<函数名>OSlwGiftTransmitIdleIndex</函数名>
//<功能说明>得到空闲邮递员函数</功能说明>
//<输入说明>pgt:this</输入说明>
//<输出说明>空闲邮递员的ID 如果返回-1表示没有空闲</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftTransmitIdle(OSlwGiftTransmitSTU *pgt)
{

    lw_u64 sta=pgt->PostmanFlag.all;

    lw_8 n=0;

    while(((1<<n)&sta) && n<OSLW_GIFT_POSTMAN_MAX) n++;//循环判断邮递员 如果判断数量超过 或者找到了 就停止循环

    return n<OSLW_GIFT_POSTMAN_MAX? (pgt->PostmanList+n):NULL;

}
#endif


//------------------------------------------
//<函数名>OSlwGiftAllocPostman</函数名>
//<功能说明>申请空闲邮递员</功能说明>
//<输入说明>pmem:内存分配函数</输入说明>
//<输出说明>NUL表示没有</输出说明>
//------------------------------------------
OSlwGiftPostmanSTU * OSlwGiftAllocPostman(OSlwMemoryBasicSTU *pmem)
{

    OSlwGiftPostmanSTU *pgp;

    OSLW_assert(!pmem);

    pgp=(OSlwGiftPostmanSTU *)pmem->Malloc(pmem,sizeof(OSlwGiftPostmanSTU));

    if(!pgp)
    {
        return pgp;
    }

    pgp->pmem=pmem;
    pgp->Method=GiftPostmanStatus_Received;


    return pgp;

}

#endif // !(OSLW_SIMPLE_LEVEL >= 3)

#if OSLW_SIMPLE_LEVEL ==3

void * OSlwGiftReceiveMoveNext(OSlwToolDListNodeConnectSTU *pgr)
{
	OSlwToolDListNodeConnectSTU *p;
	OSlwMemoryBasicSTU *pmem;
	if(pgr->pLast == NULL || pgr->pNext == NULL)
	{
		return NULL;
	}
	
	p=pgr->pNext;
	pmem=(OSlwMemoryBasicSTU *)(p->pLast);
	
	if(pgr->pLast == pgr->pNext)//只要一个消息
	{
		
		pgr->pLast=NULL;
		pgr->pNext=NULL;
		pmem->Free(pmem,p);
		
		return NULL;
	}
	else
	{
		pgr->pNext=p->pNext;
		pmem->Free(pmem,p);
		return 	pgr->pNext;	
	}
	
	
//	return NULL;
}

#endif


#endif
