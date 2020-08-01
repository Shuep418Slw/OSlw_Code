/*(Ver.=0.97)
 * OSLW_memory.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#include "OSLW_include.h"

lw_32 OSlwMemorySizeInitial(OSlwMemSizeSTU *ps, lw_u16 len)
{
#if OSLW_MEM_ALLOC_FAST
	int i = 15;
	len = len < sizeof(int) ? sizeof(int) : len;
	do
	{
		if (len & ((lw_u16)1 << i ))
		{
			break;
		}

	} while (--i);

	
	if (!_OSLW_IS_2_POW(len))//����2^n Ҫ��+1
	{
		i++;
	}
	i = i > 15 ? 15 : i;//�޷�

	ps->MovNum = i;
	ps->ModNum = (((lw_u32)1) << (i)) - 1;
	ps->SectionSize = OSLW_MEM_SIZE_MPY(1, *ps);
#else
    ps->SectionSize = len;
		return 0;
#endif // OSLW_MEM_ALLOC_FAST

}

void * OSlwMemoryGlobalCalloc(struct OSLW_MEMORY_BASIC_STRUCT *pMU, lw_u32 dsize)
{

	void *p;
	OSLW_assert(!(pMU));
	p = pMU->Malloc(pMU, dsize);

	OSLW_assert(!(p));

	memset(p, 0, dsize);

	return p;
}


void *OSlwMemoryExAlloc(lw_mem pMU, lw_u32 dsize)
{
	OSLW_assert(!(pMU));
	OSLW_assert(!(((OSlwMemoryExSTU *)pMU)->Malloc));
	return (((OSlwMemoryExSTU *)pMU)->Malloc(dsize));
}

void *OSlwMemoryExCalloc(lw_mem pMU, lw_u32 dsize)
{
	OSLW_assert(!(pMU));
	OSLW_assert(!(((OSlwMemoryExSTU *)pMU)->Calloc));
	return (((OSlwMemoryExSTU *)pMU)->Calloc(dsize));
}

void *OSlwMemoryRealloc(lw_mem pMU,void *p, lw_u32 dsize)
{
	OSLW_assert(!(pMU));
	OSLW_assert(!(((OSlwMemoryExSTU *)pMU)->ReAlloc));
	return (((OSlwMemoryExSTU *)pMU)->ReAlloc(p, dsize));
}

void *OSlwMemoryExFree(lw_mem pMU,void *p)
{
	OSLW_assert(!(pMU));
	OSLW_assert(!(((OSlwMemoryExSTU *)pMU)->Free));
	((OSlwMemoryExSTU *)pMU)->Free(p);
	return p;
}

void OSlwMemoryExInital(OSlwMemoryExSTU *pMU, void *malloc_fun, void *calloc_fun, void *free_fun, void *realloc_fun)
{
	OSLW_assert(!(pMU));
	pMU->basic.Malloc = OSlwMemoryExAlloc;
	pMU->basic.Calloc = OSlwMemoryExCalloc;
	pMU->basic.ReAlloc = OSlwMemoryRealloc;
	pMU->basic.Free = OSlwMemoryExFree;

	pMU->Malloc = (void *(*)(lw_u32))malloc_fun;
	pMU->Calloc = (void *(*)(lw_u32))calloc_fun;
	pMU->Free = (void (*)(void *))free_fun;
	pMU->ReAlloc = (void *(*)(void *, lw_u32))realloc_fun;


}

#if OSLW_MEMORY_IMPORT_MAP

lw_32 OSlwMemoryMapInital
(
    OSlwMemoryMapSTU *pMU,//�洢���ṹ��
    void *pMem,//�ڴ�ص�ַ �ܳ���=lenMem*(MemSize+sizeof(void *))
    lw_u16 lenMem,//�ڴ�Ƭ����
    lw_u16 MemSize,//�ڴ�Ƭ��С(sizeof)
    lw_u32 sizeofmem
)
{
	void *pMemInfo = pMem;
    lw_u32 reallen;
    lw_u16 lenMemInfo;

    OSLW_assert(!(pMU));
    OSLW_assert(!(pMem));
    OSLW_assert(!(MemSize));
    OSLW_assert(!(sizeofmem));


    lenMemInfo=lenMem;
	OSlwMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = OSLW_MEM_SIZE_MPY(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	if (reallen > sizeofmem)//���»���Ƭ��С
	{
		//OSLW_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = sizeofmem / (pMU->basic.MemSectionSize.SectionSize + sizeof(pMU->Mem.pData));
		reallen = OSLW_MEM_SIZE_MPY(lenMem, pMU->basic.MemSectionSize) + lenMem * sizeof(pMU->Mem.pData);
	}


	pMU->Mem.pData = (void *)((size_t)pMem + (lenMem * sizeof(pMU->Mem.pData)));
    pMU->Mem.uData=lenMem;//Ϊ�˱�̷���û��ʹ���泤��
    pMU->MemInfo.pData=pMemInfo;
    pMU->MemInfo.uData=lenMemInfo;//Ϊ�˱�̷���û��ʹ���泤��
	pMU->pMemInfoButtom = (void *)((size_t)pMU->MemInfo.pData + pMU->MemInfo.uData * sizeof(pMU->Mem.pData));


    (pMU->basic.Malloc)=(void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemoryMapAlloc;
    pMU->basic.Free=(void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *))OSlwMemoryMapFree;
    pMU->basic.ReAlloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *, lw_u32))OSlwMemoryMapReAlloc;
	pMU->basic.Calloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemoryGlobalCalloc;
    pMU->basic.memMax = lenMem;//�ڴ�Ƭ�ܴ�С
    pMU->basic.memSurplus = pMU->basic.memMax;//ʣ���С

	pMU->pMemButtom = (void *)((size_t)pMem + reallen);

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;

}

void* OSlwMemoryMapAlloc(OSlwMemoryMapSTU *pMU,lw_u32 dsize)
{

    size_t parr,phead= (size_t)NULL;
    void **pinfoarr;
    lw_u32 len;
    lw_u16 count=0;


    //lw_u16 *pMemButtom=(lw_u16 *)pMU->Mem.pData+pMU->Mem.Data*pMU->MemMapSize,*pMemInfoButtom=(lw_u16 *)pMU->MemInfo.pData+pMU->MemInfo.Data*sizeof(pMU->Mem.pData);

    OSLW_assert(!(pMU));


    parr = (size_t)(pMU->Mem.pData);
    pinfoarr = pMU->MemInfo.pData;
    if (dsize == 0)
    {
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = OSLW_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);

    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.97)
    if(len * pMU->MemSectionSize < dsize)
    	len++;
    	*/

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if (OSLW_MEM_SIZE_MPY(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    if (pMU->basic.memSurplus < len)//û���㹻���ڴ� ��������
    {
        return NULL;
    }

    do {
        //������ڴ�����ʹ��
        if((*pinfoarr)==NULL)
        {
            count++;//������++
            if(phead == (size_t)(NULL))//���֮ǰû���ҵ���
                phead=parr;//�������ڴ�Ƭͷ��
        }
        else//����Ѿ�����ʹ��
        {
            count=0;//����������
            phead= (size_t)NULL;//ͷ���ͷ�
        }
        if(count==len)//����ҵ����㹻�������ڴ�Ƭ
        {
            while (count--) *pinfoarr-- = (void *)(phead);//���ڴ�Ƭ����
            count=len;//�������ָ�����
            break;
        }


    } while((parr+=pMU->basic.MemSectionSize.SectionSize) < (size_t)(pMU->pMemButtom) && (size_t)(pinfoarr++) < (size_t)(pMU->pMemInfoButtom));

    if(count == len)//���������� == Ҫ�󳤶�
    {
        pMU->basic.memSurplus -= len;
        return (void *)phead;
    }
    else
    {
        return NULL;
    }

    //return NULL;

}

//------------------------------------------
//<������>_OSlwMemoryMapFind</������>
//<����˵��>ӳ�����ڴ���� ���Һ���</����˵��>
//<����˵��>pmu �ڴ������ p:������ڴ� pLen(��ΪNULL):��ǰָ����ڴ� pNum(��ΪNULL):��ǰָ����ڴ�+֮��Ŀ����ڴ�Ƭ���� </����˵��>
//<���˵��>lw_32 �ڴ�Ƭ��Ӧ������ -1��ʾû�и��ڴ�</���˵��>
//------------------------------------------
static lw_32 _OSlwMemoryMapFind(OSlwMemoryMapSTU *pMU, void *p,lw_u16 *pLen,lw_u16 *pNum)
{
    lw_u16 count = 0, len = 0;
    lw_u16 headernum;
    void **pinfoarr;

    OSLW_assert(!(pMU));
    OSLW_assert(!(p));

    //���ڷ�Χ֮�� �Ҳ���
    if
    (
        p < pMU->Mem.pData
        ||
        p > pMU->pMemButtom
    )
    {
        return (lw_32)-1;
    }

    //headernum = (size_t)((size_t)p - (size_t)(pMU->Mem.pData))/((size_t)(pMU->MemSectionSize));//����ƫ��
	headernum = (lw_u16)(OSLW_MEM_SIZE_DIV((size_t)((size_t)p - (size_t)(pMU->Mem.pData)), (pMU->basic.MemSectionSize))); //����ƫ��
    if (pNum && pLen)
    {
        pinfoarr = pMU->MemInfo.pData;
        pinfoarr += headernum;
        while ((*pinfoarr == p) || (*pinfoarr==NULL) && (pinfoarr != pMU->pMemInfoButtom))
        {
            if (*pinfoarr == p)
            {
                len++;
            }
            count++;
            pinfoarr++;
        }

        *pNum = count;
        *pLen = len;
    }
    return (lw_32)headernum;

}

void * OSlwMemoryMapFree(OSlwMemoryMapSTU *pMU,void *p)
{
    /*(Ver.=0.97)
    	lw_u8 **pinfoarr=pMU->MemInfo.pData;
    	lw_u8 **pMemInfoButtom;
    	lw_u8 FindFlag = 0;
    	lw_u32 count = 0;

    	//���ڷ�Χ֮�� �������ͷ�
    	if
    	(
    		p < pMU->Mem.pData
    		||
    		p > pMU->pMemButtom
    	)
    	{
    		return NULL;
    	}

    	do{
    		//������ڴ��ʹ������p
    		if(*pinfoarr == p)
    		{
    			*pinfoarr = NULL;//�ָ�Ϊ��
    			count++;
    			FindFlag = 1;//��Ѱ����־λΪ1
    		}
    		else if(FindFlag)//���û���ҵ��ұ�־λΪ1 ˵���������
    		{
    			break;
    		}

    	}while((pinfoarr++) < (lw_u8 **)(pMU->pMemInfoButtom));

    	pMU->basic.memSurplus += count;
    	*/

    lw_32 findres;
    lw_u32 count = 0;
    void **pinfoarr;

    OSLW_assert(!(pMU));


    findres = _OSlwMemoryMapFind(pMU, p, NULL,NULL);
    if (findres < 0)//û���ҵ�
    {
        return NULL;
    }
    pinfoarr = ((void **)pMU->MemInfo.pData)+ findres;

    while (*pinfoarr == p)
    {
        *pinfoarr = NULL;//�ָ�Ϊ��
        count++;
        pinfoarr++;
    }
    pMU->basic.memSurplus += count;


    return p;

}

void* OSlwMemoryMapReAlloc(OSlwMemoryMapSTU *pMU,void *p, lw_u32 dsize)
{
    lw_u16 count = 0, len, nowlen;
    lw_32 index;
    void **pinfoarr;
    void *pAlloc;

    OSLW_assert(!(pMU));


    if (p==NULL)//ֱ�ӷ����ڴ�
    {
        return OSlwMemoryMapAlloc(pMU, dsize);
    }

    if (dsize == 0)//ֱ���ͷ��ڴ�
    {
        return OSlwMemoryMapFree(pMU, p);
    }

    index = _OSlwMemoryMapFind(pMU, p, &nowlen, &count);
    if (index < 0 || count ==0)
    {
        OSLW_assert(1);
        return NULL;
    }

    //len = dsize / pMU->MemSectionSize;
    len = OSLW_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.97)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if ((lw_u32)(OSLW_MEM_SIZE_MPY(len, pMU->basic.MemSectionSize)) < dsize)
		len++;
    if (count >= len)//����Ҫ���·���
    {

        if (nowlen > len)//��ǰ���ȴ���Ҫ�󳤶� ��Ҫ�ͷŶ��೤��
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + nowlen - 1;//β��
            count = nowlen - len;
            pMU->basic.memSurplus += count;
            do
            {
                *pinfoarr-- = NULL;
            } while (--count);

        }
        else if (nowlen < len)//��ǰ����С��Ҫ�󳤶� ���볤��
        {
            pinfoarr = ((void **)pMU->MemInfo.pData) + index + len - 1;//β��
            count = len - nowlen;
            pMU->basic.memSurplus -= count;
            do
            {
                *pinfoarr-- = p;
            } while (--count);


        }
        else;//����Ҫ����
        return p;
    }
    else//���·���
    {
        pAlloc = OSlwMemoryMapAlloc(pMU, dsize);//����
        memcpy(pAlloc, p, OSLW_MEM_SIZE_MPY(nowlen, pMU->basic.MemSectionSize));//�����ڴ�����

        //�ͷ����� ���õ���free ��Լʱ��
        pinfoarr = ((void **)pMU->MemInfo.pData) + index;

        count = 0;
        while (*pinfoarr == p)
        {
            *pinfoarr = NULL;//�ָ�Ϊ��
            count++;
            pinfoarr++;
        }
        pMU->basic.memSurplus += count;

        return pAlloc;
    }

   // OSLW_assert(1);

   // return NULL;

}

#endif //OSLW_MEMORY_IMPORT_MAP



#if OSLW_MEMORY_IMPORT_SIMPLE

lw_32 OSlwMemorySimpleInital(OSlwMemorySimpleSTU *pMU,
                             void *pMem,
                             lw_u16 lenMem,
                             lw_u16 MemSize,
                             lw_u32 sizeofmem
                            )
{
    lw_u32 reallen;
    OSLW_assert(!(pMU));
    OSLW_assert(!(pMem));


    pMU->basic.Malloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemorySimpleAlloc;
    pMU->basic.Free = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *))OSlwMemorySimpleFree;
    pMU->basic.ReAlloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *, lw_u32))OSlwMemorySimpleReAlloc;
	pMU->basic.Calloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemoryGlobalCalloc;

    //pMU->MemSectionSize = MemSize;
    OSlwMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = MemSize*lenMem;

	if (reallen > sizeofmem)
	{
		OSLW_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = sizeofmem / pMU->basic.MemSectionSize.SectionSize;
		reallen = MemSize*lenMem;
	}



    pMU->basic.memSurplus = pMU->basic.memMax = lenMem;

    pMU->Mem.pData = pMem;
    pMU->pMemNow=(size_t)pMem;
    pMU->Mem.uData = lenMem;


	pMU->pMemButtom = (size_t)pMem + reallen;

	memset(pMem, 0, reallen);

	return reallen - sizeofmem;
}


void * OSlwMemorySimpleAlloc(OSlwMemorySimpleSTU *pMU, lw_u32 dsize)
{
    lw_u32 len, offsetlen = 0;
    void *pbuf = NULL;
    OSLW_assert(!(pMU));

    //len = dsize / pMU->MemSectionSize;
    len = OSLW_MEM_SIZE_DIV(dsize, pMU->basic.MemSectionSize);
    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    /*(Ver.=0.97)
    if (len * pMU->MemSectionSize < dsize)
    	len++;
    */
    //offsetlen = len* pMU->MemSectionSize;//�õ��ڴ�ʵ��ƫ����

	//�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
	if (OSLW_MEM_SIZE_MPY(len, pMU->basic.MemSectionSize) < dsize)
		len++;

    offsetlen = OSLW_MEM_SIZE_MPY(len, pMU->basic.MemSectionSize);//�õ��ڴ�ʵ��ƫ����

    if (pMU->pMemNow + offsetlen < pMU->pMemButtom)
    {
        pbuf = (void *)(pMU->pMemNow);
        pMU->pMemNow += offsetlen;
        pMU->basic.memSurplus -= len;
        return pbuf;

    }
    else
    {
        return NULL;
    }

}


void * OSlwMemorySimpleFree(OSlwMemorySimpleSTU *pMU, void *p)
{
    OSLW_assert(1);
    //���ڴ���䲻Ӧ���ͷ� ��Ҫ�����û�

    return NULL;

}

void* OSlwMemorySimpleReAlloc(OSlwMemorySimpleSTU *pMU, void *p, lw_u32 dsize)
{
    OSLW_assert(1);
    //���ڴ���䲻Ӧ�����·��� ��Ҫ�����û�

    return NULL;
}

#endif //OSLW_MEMORY_IMPORT_SIMPLE



#if OSLW_MEMORY_IMPORT_LIST

lw_32 OSlwMemoryListInital(OSlwMemoryListSTU *pMU,
                           void *pMem,
                           lw_u16 lenMem,
                           lw_u16 MemSize,
                           lw_u32 sizeofmem
                          )
{
    lw_u32 reallen;
    //lw_32 data_return;
    lw_u32 dsize = 0;
//    lw_u16 len;
    //OSlwMemoryListNodeSTU *ptail;

    OSLW_assert(!(pMU));
    OSLW_assert(!(pMem));
    OSLW_assert(!(MemSize));
    OSLW_assert(!(sizeofmem));


    pMU->basic.Malloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemoryListAlloc;
    pMU->basic.Free = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *))OSlwMemoryListFree;
    pMU->basic.ReAlloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, void *, lw_u32))OSlwMemoryListReAlloc;
	pMU->basic.Calloc = (void *(*)(struct OSLW_MEMORY_BASIC_STRUCT *, lw_u32))OSlwMemoryGlobalCalloc;
    //pMU->MemSectionSize = MemSize;

    OSlwMemorySizeInitial(&(pMU->basic.MemSectionSize), MemSize);
	reallen = OSLW_MEM_SIZE_MPY (lenMem, pMU->basic.MemSectionSize)+  sizeof(OSlwMemoryListNodeSTU);
	if (reallen > sizeofmem)
	{
		//OSLW_assert(1);//�����Ϊ�쳣 ��Ҫ�����û�
		lenMem = (sizeofmem - sizeof(OSlwMemoryListNodeSTU)) / pMU->basic.MemSectionSize.SectionSize;
		reallen = OSLW_MEM_SIZE_MPY(lenMem, pMU->basic.MemSectionSize) + sizeof(OSlwMemoryListNodeSTU);
	}


    pMU->Mem.uData = lenMem;
    pMU->Mem.pData = pMem;
	pMU->basic.memSurplus = pMU->basic.memMax = OSLW_MEM_SIZE_MPY(lenMem, pMU->basic.MemSectionSize);
	pMU->pMemButtom = (void *)((size_t)pMem + reallen);
	memset(pMem, 0, reallen);
	pMU->basic.memSurplus = pMU->basic.memMax = OSLW_MEM_SIZE_MPY(lenMem, pMU->basic.MemSectionSize);


    pMU->ptail = (OSlwMemoryListNodeSTU *)((size_t)pMU->pMemButtom - sizeof(OSlwMemoryListNodeSTU));
    pMU->phead = pMem;

    OSLW_TOOL_DLIST_NODE_CONNECT(pMU->phead, pMU->ptail);

    OSLW_MEM_LIST_STD_SIZE(pMU, dsize);//�ڴ��С�淶��

    pMU->phead->len = dsize;
    pMU->basic.memSurplus -= dsize;

	return reallen - sizeofmem;

}


void * OSlwMemoryListAlloc(OSlwMemoryListSTU *pMU, lw_u32 dsize)
{

    OSlwMemoryListNodeSTU *p, *q;
    void *preturn = NULL;
		register size_t buf1;

    OSLW_assert(!(pMU));
    OSLW_assert(!(dsize));
    /*(Ver.=0.97)
    MemSectionSize = pMU->MemSectionSize;
    dsize += sizeof(OSlwMemoryListNodeSTU);
    len = dsize / MemSectionSize;

    //�õ����û�Ԥ���ڴ�Ҫ����ڴ�Ƭ����
    if ((lw_u32)len * MemSectionSize < dsize)
    	len++;

    dsize = (lw_u32)len *MemSectionSize;
    */

    if (dsize == 0)
    {
        return NULL;
    }


    OSLW_MEM_LIST_STD_SIZE(pMU, dsize);//�ڴ��С�淶��

    if (pMU->basic.memSurplus < dsize)//û���㹻���ڴ� ��������
    {
        return NULL;
    }

    p = pMU->phead;
    do
    {

        if (p < pMU->phead || p>pMU->ptail )//pԽ������쳣
        {
            OSLW_assert(1);
            return NULL;
        }
				buf1=((size_t)(p->con.pNext) - (size_t)(p) - p->len);
        if (buf1 >= dsize)//������ʣ��
        {
            q = (OSlwMemoryListNodeSTU *)(((size_t)p) + p->len);
            /*(Ver.=0.97)
            			q->con.pLast = (OSlwToolDListNodeConnectSTU *)p;
            			q->con.pNext = (OSlwToolDListNodeConnectSTU *)p->con.pNext;

            			p->con.pNext->pLast = (OSlwToolDListNodeConnectSTU *)q;
            			p->con.pNext = (OSlwToolDListNodeConnectSTU *)q;
            */
            OSLW_TOOL_DLIST_NODE_INSERT(p, q);

            q->len = dsize;
            preturn = (void *)((size_t)q + sizeof(OSlwMemoryListNodeSTU));
            pMU->basic.memSurplus -= dsize;

            break;
        }

        p = (OSlwMemoryListNodeSTU *)(p->con.pNext);

    } while (p->con.pNext);


    return preturn;

}


static lw_8 _OSlwMemoryListJudgeIllegal(OSlwMemoryListSTU *pMU, OSlwMemoryListNodeSTU *pnode)
{

    OSLW_assert(!pMU);
    OSLW_assert(!pnode);

    if (
        ((size_t)(pnode) < (size_t)(pMU->phead) || (size_t)(pnode) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pNext) < (size_t)(pMU->phead) || (size_t)(pnode->con.pNext) > (size_t)(pMU->ptail))
        ||
        ((size_t)(pnode->con.pLast) < (size_t)(pMU->phead) || (size_t)(pnode->con.pLast) > (size_t)(pMU->ptail))
    )//�ж���Χ
    {
        OSLW_assert(1);
        return 1;
    }

    //if (((size_t)pnode - (size_t)(pMU->phead)) % pMU->MemSectionSize)//�ж��Ƿ����
    if (OSLW_MEM_SIZE_MOD(((size_t)pnode - (size_t)(pMU->phead)), pMU->basic.MemSectionSize)) //�ж��Ƿ����
    {
        OSLW_assert(1);
        return 1;
    }

    return 0;

}

void * OSlwMemoryListFree(OSlwMemoryListSTU *pMU, void *p)
{
    OSlwMemoryListNodeSTU *pnode=NULL;
    //lw_u16 len;
    //lw_u32 Length;

    OSLW_assert(!(pMU));
    OSLW_assert(!(p));

    pnode = (OSlwMemoryListNodeSTU *)((size_t)p - sizeof(OSlwMemoryListNodeSTU));

    if (_OSlwMemoryListJudgeIllegal(pMU,pnode))
    {
        OSLW_assert(1);
        return NULL;
    }

    pMU->basic.memSurplus += pnode->len;

    OSLW_TOOL_DLIST_NODE_DELECT(pnode);

    pnode->con.pLast = NULL;
    pnode->con.pNext = NULL;
    pnode->len = 0;

    return pnode;

}

void* OSlwMemoryListReAlloc(OSlwMemoryListSTU *pMU, void *p, lw_u32 dsize)
{
    OSlwMemoryListNodeSTU *pnode = NULL;
    lw_u32 _dsize;
    lw_u32 reallen;
    void *preturn = NULL;

    OSLW_assert(!(pMU));

    _dsize = dsize;

    if (p == NULL)//ֱ�ӷ����ڴ�
    {
        return OSlwMemoryListAlloc(pMU, _dsize);
    }

    if (dsize == 0)//ֱ���ͷ��ڴ�
    {
        return OSlwMemoryListFree(pMU, p);
    }

    pnode = (OSlwMemoryListNodeSTU *)((size_t)p - sizeof(OSlwMemoryListNodeSTU));//ƫ�Ƶõ�����ڵ�

    _dsize = dsize;//����֮ǰ��С

    if (_OSlwMemoryListJudgeIllegal(pMU, pnode))//�ڵ�Ϸ����ж�
    {
        OSLW_assert(1);
        return NULL;
    }

    OSLW_MEM_LIST_STD_SIZE(pMU, dsize);//���ȹ淶��

    reallen = (size_t)(pnode->con.pNext) - (size_t)pnode;//�õ���ʵ����

    if (dsize <= reallen)//�ڴ��㹻
    {
        pMU->basic.memSurplus += pnode->len;
        pMU->basic.memSurplus -= dsize;
        pnode->len = dsize;
        preturn = p;
    }
    else
    {
        preturn = OSlwMemoryListAlloc(pMU, _dsize);//��������

        memcpy(preturn, p, pnode->len);//��������

        //�ͷŽڵ�
        pMU->basic.memSurplus += pnode->len;
        OSLW_TOOL_DLIST_NODE_DELECT(pnode);

        pnode->con.pLast = NULL;
        pnode->con.pNext = NULL;
        pnode->len = 0;

    }

    return preturn;

}

#endif //OSLW_MEMORY_IMPORT_LIST

