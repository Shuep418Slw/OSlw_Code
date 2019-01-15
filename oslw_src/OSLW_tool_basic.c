/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"


#if OSLW_TOOL_IMPORT_SLIP_WIN || OSLW_TOOL_IMPORT_ALL
OSlwToolSlipwinSTU* OSlwToolSlipwinAppend(OSlwToolSlipwinSTU *psw,void *pd,lw_u8 num)
{
    register lw_8 *p,*q;
    register lw_u32 n,n1;

    if(psw->MDataLockFlag)//如果锁存
    {
        p=(lw_8 *)psw->pBufferData+psw->BufferLength;//得到当前缓冲区首地址
        q=(lw_8 *)pd;
        n=num;
        OSLW_TOOL_MEMCPY(p,q,n);//复制黏贴
        psw->BufferLength+=num;

    }
    else
    {
        n=num+psw->BufferLength;//存入数据长度==新存数据长度+缓冲区数据长度
        p=(lw_8 *)psw->pMasterData;
        q=p+n;
        n1=psw->MDataLen_forSize_t-n;
        OSLW_TOOL_MEMCPY(p,q,n1);//先滑窗

        n=psw->BufferLength;
        q=psw->pBufferData;
        OSLW_TOOL_MEMCPY(p,q,n);//再填入缓冲器数据 如果缓冲器长0 则不执行
        psw->BufferLength=0;

        q=(lw_8 *)pd;

        while((num)--)//复制黏贴新数据
            *(p)++=*(q)++;




    }
    return psw;
}
#endif //OSLW_TOOL_IMPORT_SLIP_WIN


#if !(OSLW_SIMPLE_LEVEL >= 4)
#if OSLW_TOOL_IMPORT_DLIST || OSLW_TOOL_IMPORT_ALL
void OSlwToolDListInitial(OSlwToolDListSTU *pDL,lw_u8 len,void *p)
{


    OSLW_assert(!pDL);
    memset(pDL,0,sizeof(OSlwToolDListSTU));
    pDL->Head.Key.uData=0;
    pDL->pTail=&pDL->Head;
    pDL->Head.Data.uData=len;
    pDL->Head.Data.pData=p;
    pDL->MaxLen++;
    pDL->NowLen++;
    pDL->AppendFUN=OSlwToolDListAppend;
    pDL->IndexOfFUN=OSlwToolDListIndexOf;
    pDL->InsertFUN=OSlwToolDListInsert;
    pDL->RemoveFUN=OSlwToolDListRemove;


    return;
}



lw_8 OSlwToolDListCmpId(OSlwToolDListNodeSTU *pDLN,lw_u16 data,lw_u16 count,void *pdata)
{
    return pDLN->Key.uData == data;
}

lw_8 OSlwToolDListCmpCount(OSlwToolDListNodeSTU *pDLN, lw_u16 data, lw_u16 count, void *pdata)
{

    return count == data;
}


OSlwToolDListNodeSTU* OSlwToolDListIndexOf(OSlwToolDListSTU *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    register OSlwToolDListNodeSTU *pDLN;
    lw_u16 count=0;

    OSLW_assert(!(pDL));
    pDLN = &pDL->Head;
    /*(Ver.=0.9~)(Beg.=0.9)
        if(mode== OSlwToolDListIndexMode_byId)
        {
            while(!(pDLN->Id == id)) pDLN= (OSlwToolDListNodeSTU *)pDLN->con.pNext;
            if(pDLN->Id == id)return pDLN;
            else return NULL;
        }
        else if(mode == OSlwToolDListIndexMode_byNum && id < pDL->NowLen)
        {
            while(count++ != id) pDLN=(OSlwToolDListNodeSTU *)(pDLN->con.pNext);
            return pDLN;
        }
        else
            return NULL;
    */

    if (pCmpFun)//如果定义判断函数
    {
        do
        {
            if (pCmpFun(pDLN, data, count, pdata))//如果判断函数返回1
            {
                break;
            }
            count++;
            pDLN = (OSlwToolDListNodeSTU *)(pDLN->con.pNext);

        } while (pDLN);

        return pDLN;
    }
    else
    {
        return NULL;
    }


}


OSlwToolDListNodeSTU* OSlwToolDListAppend(struct OSLW_TOOL_DLIST_STRUCT *pDL,OSlwToolDListNodeSTU *pDLN)
{

    OSLW_assert(!(pDL));
    OSLW_assert(!(pDLN));

    /*(Ver.=0.9~)(Beg.=0.9)
    pDL->pTail->con.pNext=(OSlwToolDListNodeConnectSTU *)pDLN;//尾部的下一个为新的
    pDLN->con.pLast=(OSlwToolDListNodeConnectSTU *)pDL->pTail;//新节点的上一个为原来尾部
    */

    OSLW_TOOL_DLIST_NODE_CONNECT(pDL->pTail, pDLN)

    pDL->pTail=pDLN;
    pDLN->Key.uData =pDL->MaxLen++;//ID赋予
    pDL->NowLen++;

    return pDLN;



}
OSlwToolDListNodeSTU* OSlwToolDListInsert
(struct OSLW_TOOL_DLIST_STRUCT *pDL,OSlwToolDListNodeSTU *pDLN, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    register OSlwToolDListNodeSTU *pDLNbuf;
    OSLW_assert(!(pDL));
    OSLW_assert(!(pDLN));

    pDLNbuf=pDL->IndexOfFUN(pDL,pCmpFun,data,pdata);
    if(!(pDLNbuf))
    {
        if(pDLNbuf != pDL->pTail)
        {
            //pDLNbuf1=(OSlwToolDListNodeSTU *)(pDLNbuf->con.pNext);//得到下一个
            pDLN->Key.uData =pDL->MaxLen++;//ID赋予
            pDL->NowLen++;
            /*(Ver.=0.9~)(Beg.=0.9)
            pDLNbuf->con.pNext=(OSlwToolDListNodeConnectSTU *)pDLN;//上一个的下一个为新的
            pDLN->con.pLast=(OSlwToolDListNodeConnectSTU *)pDLNbuf;//新的的上一个为原来的

            pDLN->con.pNext=(OSlwToolDListNodeConnectSTU *)pDLNbuf1;//二号对接
            pDLNbuf1->con.pLast=(OSlwToolDListNodeConnectSTU *)pDLN;*/
            OSLW_TOOL_DLIST_NODE_INSERT(pDLNbuf, pDLN);
        }
        else
        {
            pDL->AppendFUN(pDL,pDLN);
        }
        return pDLN;

    }
    else
        return NULL;


}



//内置函数
OSlwToolDListNodeSTU* _OSlwToolDListRemoveNode
(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSlwToolDListNodeSTU *pDLNbuf)
{

    if((pDLNbuf) && pDLNbuf != &(pDL->Head))//0号为head 不可以释放
    {
        if(pDLNbuf == pDL->pTail)
        {
            pDLNbuf->con.pLast->pNext=NULL;//尾部指针清零
            pDL->pTail=(OSlwToolDListNodeSTU *)pDLNbuf->con.pLast;//尾巴更新
        }
        else
        {
            /*(Ver.=0.9~)(Beg.=0.9)
            pDLNbuf->con.pLast->pNext=pDLNbuf->con.pNext;//上一个的下一个为下一个
            pDLNbuf->con.pNext->pLast=pDLNbuf->con.pLast;//下一个的上一个为上一个
            */
            OSLW_TOOL_DLIST_NODE_DELECT(pDLNbuf)
        }
        pDL->NowLen--;

        return (OSlwToolDListNodeSTU *)pDLNbuf->con.pNext;

    }


    return (OSlwToolDListNodeSTU *)pDLNbuf->con.pNext;


}

OSlwToolDListNodeSTU* OSlwToolDListRemove
(struct OSLW_TOOL_DLIST_STRUCT *pDL, OSLW_TOOL_DLIST_FUN_PARA_DEFAULT)
{
    OSlwToolDListNodeSTU *pDLNbuf;
    OSLW_assert(!(pDL));
    pDLNbuf = pDL->IndexOfFUN(pDL, pCmpFun, data, pdata);
    /*(Ver.=0.9~)(Beg.=0.9)
        if((pDLNbuf) && data !=0)//0号为head 不可以释放
        {
            if(pDLNbuf == pDL->pTail)
            {
                pDLNbuf->con.pLast->pNext=NULL;//尾部指针清零
                pDL->pTail=(OSlwToolDListNodeSTU *)pDLNbuf->con.pLast;//尾巴更新
            }
            else
            {
                #if 0
                pDLNbuf->con.pLast->pNext=pDLNbuf->con.pNext;//上一个的下一个为下一个
                pDLNbuf->con.pNext->pLast=pDLNbuf->con.pLast;//下一个的上一个为上一个
								#endif
                OSLW_TOOL_DLIST_NODE_DELECT(pDLNbuf)
            }
            pDL->NowLen--;

            return (OSlwToolDListNodeSTU *)pDLNbuf->con.pNext;

        }
        else
            return NULL;
    */

    if((pDLNbuf) && data !=0)//0号为head 不可以释放
    {
        return _OSlwToolDListRemoveNode(pDL,pDLNbuf);

    }
    else
        return NULL;


}
#endif //OSLW_TOOL_IMPORT_DLIST




#if OSLW_TOOL_IMPORT_HASH_LIST || OSLW_TOOL_IMPORT_ALL
lw_u8 HashCalStringBKDR(void *pData)
{
    register lw_u8 *str = (lw_u8 *)pData;
    register lw_u8 hash = 0;
    register lw_u8 ch ;

    OSLW_assert(!(pData));

    while ((ch=(lw_u8)(*str++)))
    {
        hash += ((hash << 7) + (hash << 1) + hash + ch);//hash=hash*131+ch;
    }

    return hash;
}

lw_8 HashCmpString(void *p1, void *p2)
{
    return strcmp((lw_8 *)p1, (lw_8 *)p2);
}



void OSlwToolHashInitial(OSlwToolHashSTU *pH, lw_u8 length, HashCalFunType pHashFun, HashKeyCmpFunType pHashKeyCmpFun)
{

    OSLW_assert(!(pH));
    OSLW_assert(!(pHashFun));
    OSLW_assert(!(pHashKeyCmpFun));
    OSLW_assert(!(length));

    if (length > OSLW_TOOL_HASH_LIST_MAX)
    {
        OSLW_assert(1);
        length = OSLW_TOOL_HASH_LIST_MAX;
    }


    memset(pH, 0, sizeof(OSlwToolHashSTU));

    pH->HashListLength = length;
    pH->HashCalFun = pHashFun;
    pH->HashKeyCmp = pHashKeyCmpFun;

    pH->SelectFun = OSlwToolHashSelect;
    pH->AppendFun = OSlwToolHashAppend;
    pH->DeleteFun = OSlwToolHashDelete;

    return;
}




lw_u8 OSlwToolHashSelect(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, lw_u8 *pIndexResult, OSlwToolDListNodeSTU **ppDLN)
{
    lw_u8 key_index;
    OSlwToolDListNodeSTU *pDLN = NULL;
    OSlwToolDListNodeSTU *pDLNlast = NULL;

    OSLW_assert(!(pH));
    OSLW_assert(!(pKey));

    key_index = pH->HashCalFun(pKey) % pH->HashListLength;

    pDLN = pH->pHashList[key_index];
    while (pDLN)
    {
        if (!(pH->HashKeyCmp(pDLN->Key.pData,pKey)))
        {
            break;
        }
        pDLNlast = pDLN;
        pDLN = (OSlwToolDListNodeSTU *)(pDLN->con.pNext);

    }
    if (pIndexResult)
        *pIndexResult = key_index;



    if (pDLN)//判断是否找到
    {
        if (ppDLN)//要不要返回
        {
            *ppDLN = pDLN;
        }
        return 1;
    }
    else//没有找到返回前一个节点
    {
        if (ppDLN)
        {
            *ppDLN = pDLNlast;
        }
        return 0;
    }
    return 0;

}

OSlwToolDListNodeSTU* OSlwToolHashAppend(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey, OSlwToolDListNodeSTU *pDLN)
{
    lw_u8 key_index;
    lw_u8 res;
    OSlwToolDListNodeSTU *pdln;

    OSLW_assert(!(pH));
    OSLW_assert(!(pKey));
    OSLW_assert(!(pDLN));

    res = pH->SelectFun(pH, pKey, &key_index,&pdln);
    if (res)//不满足追加条件
    {
        OSLW_assert(1);
        return NULL;

    }
    else//满足追加条件
    {
        //pdln = pH->pHashList[key_index];
        if (pdln)//如果不是空
        {
            OSLW_TOOL_DLIST_NODE_CONNECT(pdln, pDLN);
        }
        else//为空 直接对应数组填入
        {
            pH->pHashList[key_index] = pDLN;
						pDLN->con.pLast = NULL;
        }
        pDLN->con.pNext = NULL;
        pDLN->Key.pData = pKey;

    }
    return pDLN;

}

OSlwToolDListNodeSTU* OSlwToolHashDelete(struct OSLW_TOOL_HASH_STRUCT *pH, void *pKey)
{
    lw_u8 key_index;
    lw_u8 res;
    OSlwToolDListNodeSTU *pdln;

    OSLW_assert(!(pH));
    OSLW_assert(!(pKey));

    res = pH->SelectFun(pH, pKey, &key_index, &pdln);
    if (res)//满足删除条件
    {
        if (pdln->con.pLast)//如果有上一个 说明不是头部
        {
            OSLW_TOOL_DLIST_NODE_DELECT(pdln);
        }
        else
        {
            pH->pHashList[key_index] = (OSlwToolDListNodeSTU *)(pdln->con.pNext);
            if (pdln->con.pNext)//如果有下一个
            {
                pdln->con.pNext->pLast = NULL;//清除前一个
            }
        }
        return pdln;
    }
    else
    {
        OSLW_assert(1);
        return NULL;
    }

}
#endif //OSLW_TOOL_IMPORT_HASH_LIST

#endif

#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_TABLE || OSLW_TOOL_IMPORT_ALL
void OSlwToolTableInitial(OSlwToolTableSTU *pT, lw_u16 RowLen, lw_u16 *pCol, lw_u16 ColLen, OSlwMemoryBasicSTU *pmem, void *pTable,lw_u32 *pColReal)
{

    lw_u32 col_size = 0;
    lw_u32 *pSize;
    lw_u16 _len = 0;

    OSLW_assert(!(pT));
    OSLW_assert(!(pmem));

    pT->Col.uData = ColLen;
    pT->Row.uData = RowLen;

	if (pColReal)
	{
		pSize = pT->Col.pData = pColReal;

		while (ColLen--)
		{
			col_size += *pSize++;
		}

	}
	else
	{
		pSize = pT->Col.pData = pmem->Malloc(pmem, sizeof(lw_u32) * ColLen);
		while (ColLen--)
		{
			_len = *pCol;
			*pSize = (lw_u32)(_len);
			col_size += *pSize;
			pSize++;
			pCol++;
		}
	}
    



    pT->ColSize = col_size;

    if (pTable)//如果定义了pTable 采用静态分配
    {
        pT->Row.pData = pTable;
    }
    else//全部采用动态分配
    {
        pT->Row.pData = pmem->Malloc(pmem, (RowLen * pT->ColSize));
    }

	if (pT->Row.pData == NULL)
	{
		OSLW_assert(1);
	}

	memset(pT->Row.pData, 0, (RowLen * pT->ColSize));
    

    pT->pmem = pmem;

    pT->ReadFun = OSlwToolTableRead;
    pT->WriteFun = OSlwToolTableWrite;


}


void * OSlwToolTableRead(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, lw_u16 *pLen)
{
    void *p;
    lw_u32 *pSize;
    size_t col_size = 0;

    OSLW_assert(!(pT));

    row = row < 0 ? row + pT->Row.uData : row;
    col = col < 0 ? col + pT->Col.uData : col;

    if (row < 0 || col < 0)//彻底超出维度
    {
        return NULL;
    }

    if (col >= pT->Col.uData)//列数太大 一旦固定不在修改
    {
        return NULL;
    }

    if (row >= pT->Row.uData)//行数太大
    {
        p = pT->pmem->ReAlloc(pT->pmem, pT->Row.pData, (row  * pT->ColSize));
        if (p==NULL)
        {
            return p;
        }
        pT->Row.uData = row;
    }


    pSize = pT->Col.pData;
    while (col--)
    {
        col_size += *pSize++;
    }

    if (pLen)
    {
        *pLen = *pSize;
    }

    return (void *)((size_t)(pT->Row.pData) + (row * pT->ColSize) + col_size);

}



lw_32 OSlwToolTableWrite(struct OSLW_TOOL_TABLE_STRUCT *pT, lw_32 row, lw_32 col, void *psrc, lw_u16 len)
{
    OSlwMemSizeSTU *pSize;
    lw_u16 _len;
    void *p;

    p = OSlwToolTableRead(pT, row, col, &_len);
    if (p ==NULL )
    {
        OSLW_assert(1);
        return (lw_32)((lw_u32)1 << 31);
    }
    if (len <= _len)
    {
        memcpy(p, psrc, len);
    }
    else
    {
        memcpy(p, psrc, _len);
    }

    return (lw_32)_len - (lw_32)len;
}
#endif //OSLW_TOOL_IMPORT_TABLE

#endif // !(OSLW_SIMPLE_LEVEL >= 3)