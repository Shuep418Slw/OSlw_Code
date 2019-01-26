/*(Ver.=0.93)
 * OSLW_tool.c
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#if 0



OSlwToolNNLayerActFunSTU OSlwToolSigmoid= {OSlwToolNNSigmoid_Forward,OSlwToolNNSigmoid_Backward};
OSlwToolNNLayerActFunSTU OSlwToolLinear= {OSlwToolNNLinear_Forward,OSlwToolNNLinear_Backward};
OSlwToolNNLayerActFunSTU OSlwToolReLU= {OSlwToolNNReLU_Forward,OSlwToolNNReLU_Backward};
OSlwToolNNLayerActFunSTU OSlwToolLReLU = { OSlwToolNNLeakyReLU_Forward,OSlwToolNNLeakyReLU_Backward,_ParaFrom(0.01) };
OSlwToolNNLayerActFunSTU OSlwToolTanh = { OSlwToolNNTanh_Forward,OSlwToolNNTanh_Backward };
OSlwToolNNLayerActFunSTU OSlwToolSwish = { OSlwToolNNSwish_Forward,OSlwToolNNSwish_Backward };


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNSigmoid_Forward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNSigmoid_Forward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n = pIn->length;
        for(i=0; i<n; i++)  pOut->a[i]=_ParaDiv(_ParaFint(1),_ParaAdd(_ParaFint(1),_ParaExp(_ParaMpy(pIn->a[i],_ParaFint(-1)))));
    }

    return pOut;


}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNSigmoid_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNSigmoid_Backward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n = pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=_ParaMpy(pOut->a[i],_ParaSub(_ParaFint(1),pOut->a[i]));
    }

    return pOut;
}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNTanh_Forward)
{
	lw_u32 n, i;
	ParaType _exp_x, _exp_nx;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			_exp_x = _ParaExp(pIn->a[i]);
			_exp_nx = _ParaExp(-(pIn->a[i]));
			pOut->a[i] = _ParaDiv(
				_ParaSub(_exp_x, _exp_nx),
				_ParaAdd(_exp_x, _exp_nx)
			);
		}
	}

	return pOut;

}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNTanh_Backward)
{
	lw_u32 n, i;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) 
			pIn->a[i] = _ParaSub(
			_ParaFint(1), 
			_ParaMpy(pOut->a[i], pOut->a[i])
		);
	}

	return pOut;
}


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNLinear_Forward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNLinear_Forward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pOut->a[i]=pIn->a[i];
    }

    return pOut;


}


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNLinear_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNLinear_Backward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=_ParaFint(1);
    }

    return pOut;
}


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNReLU_Forward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNReLU_Forward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pOut->a[i]=pIn->a[i]>=_ParaFrom(0)?pIn->a[i]:_ParaFrom(0);
    }

    return pOut;


}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*,OSlwToolNNReLU_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut),OSlwToolNNReLU_Backward)
{
    lw_u32 n,i;
    OSLW_assert(!(pIn));
    OSLW_assert(!(pOut));
    if(OSLW_TOOL_M_SIZE_EQU(pIn,pOut))
    {
        n= pIn->length;
        for(i=0; i<n; i++) pIn->a[i]=pOut->a[i]>=_ParaFrom(0)?_ParaFrom(1):_ParaFrom(0);
    }

    return pOut;
}


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNLeakyReLU_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNReLU_Forward)
{
	lw_u32 n, i;
	ParaType _k;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	_k = pAF->_p1;
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) pOut->a[i] = pIn->a[i] >= _ParaFrom(0) ? pIn->a[i] : _ParaMpy(_k, pIn->a[i]);
	}

	return pOut;


}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNLeakyReLU_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNReLU_Backward)
{
	lw_u32 n, i;
	ParaType _k;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	_k = pAF->_p1;
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++) pIn->a[i] = pOut->a[i] >= _ParaFrom(0) ? _ParaFrom(1) : _k;
	}

	return pOut;
}


OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNSwish_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNSwish_Forward)
{
	lw_u32 n, i;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			pOut->a[i] = _ParaMpy(pIn->a[i], 
				_ParaDiv(
					_ParaFint(1), 
					_ParaAdd(
						_ParaFint(1), 
						_ParaExp(
							_ParaMpy(
								pIn->a[i], _ParaFint(-1)
							)
						)
					)
				)
			);

		}
	}

	return pOut;


}

OSLW_TOOL_FUN
(OSlwToolMatrixSTU*, OSlwToolNNSwish_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut), OSlwToolNNSwish_Backward)
{
	lw_u32 n, i;
	ParaType _sigmoid_x;
	OSLW_assert(!(pIn));
	OSLW_assert(!(pOut));
	if (OSLW_TOOL_M_SIZE_EQU(pIn, pOut))
	{
		n = pIn->length;
		for (i = 0; i < n; i++)
		{
			_sigmoid_x= _ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMpy(pIn->a[i], _ParaFint(-1)))));
			pIn->a[i] = _ParaAdd(pOut->a[i], _ParaMpy(_sigmoid_x, _ParaSub(_ParaFint(1), pOut->a[i])));
		}
	}

	return pOut;
}
#endif // 0


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL



#if 0



//网络层初始化函数
void OSlwToolBPnnLayerInitial(OSlwToolNNLayerFullConSTU *pNNL//网络层结构体
	, OSlwToolNNLayerActFunSTU *pAF//激活函数
	, lw_u16 size_row, lw_u16 size_col//层大小
	, ParaType *pW//系数矩阵数据块
	, ParaType *pBias//偏质
	, OSlwToolRandomBasicSTU *pTRB//随机数产生序列
	, ParaType rmax, ParaType rmin, ParaType nl)//范围 与 学习速率
{


	OSLW_assert(!(pNNL));
	OSLW_assert(!(pAF));

	memset(pNNL, 0, sizeof(OSlwToolNNLayerFullConSTU));

	pNNL->pActFun = pAF;
	pNNL->nl = nl;
	pNNL->pRand = pTRB;



	OSlwToolMatrixInitial(&pNNL->w, size_row, size_col, pW);

	pNNL->size_col = size_col;
	pNNL->size_row = size_row;

	OSlwToolMatrixRandomInitial(&pNNL->w, pTRB, rmax, rmin);

	if (pBias)
	{
		OSlwToolMatrixInitial(&pNNL->BiasUN.bias, 1, size_col, pBias);
		OSlwToolMatrixRandomInitial(&pNNL->BiasUN.bias, pTRB, rmax, rmin);

	}


}

//前向传播初始化
void OSlwToolBPnnLayerForwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X)
{
	OSLW_assert(!(pNNL));
	OSlwToolMatrixInitial(&pNNL->in, 1, pNNL->w.row, pIN);
	OSlwToolMatrixInitial(&pNNL->out, 1, pNNL->w.col, pOUT);
	OSlwToolMatrixInitial(&pNNL->_out_x, 1, pNNL->w.col, pOUT_X);
}


//后向传播初始化
void OSlwToolBPnnLayerBackwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pINerr, ParaType *pdelt, ParaType *pOuterr)
{

	OSLW_assert(!(pNNL));
	OSlwToolMatrixInitial(&pNNL->inErr, 1, pNNL->w.row, pINerr);
	OSlwToolMatrixInitial(&pNNL->outErr, 1, pNNL->w.col, pOuterr);
	OSlwToolMatrixInitial(&pNNL->DeltaUN.delta, 1, pNNL->w.col, pdelt);

}


//BN初始化
/*
void OSlwToolBPnnLayerBatchNormInitial(OSlwToolNNLayerFullConSTU *pNNL, OSlwToolNNBatchNormSTU *pBN,ParaType epsi)
{

	OSLW_assert(!(pNNL));
	OSLW_assert(!(pBN));

	pNNL->pBN = pBN;
	memset(pBN, 0, sizeof(OSlwToolNNBatchNormSTU));
	
	pBN->BN_gamma = _ParaFint(1);
	pBN->BN_Epsi = epsi;
	
}
*/

/*
由于dropout 新版本舍弃
void OSlwToolBPnnLayerReSize(OSlwToolNNLayerFullConSTU *pNNL,lw_u16 r,lw_u16 c)
{

    OSLW_assert(!(pNNL));
    pOSlwToolMatrixReSize(&(pNNL->BiasUN.bias),1,c);
    pOSlwToolMatrixReSize(&(pNNL->delt),1,c);
    pOSlwToolMatrixReSize(&(pNNL->out),1,c);
    pOSlwToolMatrixReSize(&(pNNL->outErr),1,c);

    pOSlwToolMatrixReSize(&(pNNL->w),r,c);

    pOSlwToolMatrixReSize(&(pNNL->in),1,r);
    pOSlwToolMatrixReSize(&(pNNL->inErr),1,c);
    if(pNNL->BiasUN.bias.a)
    {
        pOSlwToolMatrixReSize(&(pNNL->BiasUN.bias),1,c);
    }

}
*/

/*
void  OSlwToolBPnnDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,ParaType P,ParaType *pWBuf,ParaType *pBiasUN.biasBuf,lw_u16 *pList1,lw_u16 *pList2)
{

    OSLW_assert(!(pNNL));
    OSLW_assert(!(pwbuf));
    OSLW_assert(!(pList1));
    OSLW_assert(!(pList2));


    pNNL->PDropOut=P;
    pNNL->pDropOutRow=pList1;
    pNNL->pDropOutCol=pList2;
    pNNL->pDropOutBiasUN.bias=pBiasUN.biasBuf;
    pNNL->pDropOutW=pWBuf;

}
*/

void  OSlwToolBPnnDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,OSlwToolNNDropOutSTU *pdrop, ParaType P, ParaType *pDropList)
{

	OSLW_assert(!(pNNL));
	OSLW_assert(!(pdrop));

	pNNL->pDropOut = pdrop;
	pdrop->PossDropOut = _ParaSub(_ParaFint(1), P);
	OSlwToolMatrixInitial(&(pdrop->DropOutMat), pNNL->in.row, pNNL->in.col, pDropList);

}

//神经网络层追加 使用的是tool中的双向链表
OSlwToolBPnnSTU *OSlwToolBPnnAppend(OSlwToolBPnnSTU *pBPnn, OSlwToolDListNodeSTU *pDLN, OSlwToolNNLayerFullConSTU *pNNL)
{
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pDLN));
	OSLW_assert(!(pNNL));

	pBPnn->Net.AppendFUN(&pBPnn->Net, pDLN);
	pDLN->Data.uData = sizeof(OSlwToolNNLayerFullConSTU);
	pDLN->Data.pData = (void *)pNNL;
	if (pBPnn->ParaGroupNum == 0)
	{

		OSlwToolMatrixInitial(&pBPnn->x, pNNL->in.row, pNNL->in.col, pNNL->in.a);

	}

	OSlwToolMatrixInitial(&pBPnn->y, pNNL->out.row, pNNL->out.col, pNNL->out.a);

	pBPnn->ParaGroupNum += 2;//同时加上偏置与权重

	pBPnn->WeightParaNum += pNNL->w.length;
	pBPnn->BiasParaNum += pNNL->BiasUN.bias.length;


	return pBPnn;
}


void*
OSlwToolBPnnLayerAppend_Dynamic
(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolNNLayerActFunSTU *pAF,
	lw_u16 row,
	lw_u16 col,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType Rmax, ParaType Rmin,
	ParaType nl,
	ParaType *pin, ParaType *pout,
	OSlwMemoryBasicSTU *pMem,
	ParaType DropOutEps
)
{
	void *p1 = NULL, *p2 = NULL, *p3 = NULL, *p4 = NULL, *p5 = NULL, *p6 = NULL, *p7 = NULL, *p8 = NULL;
    OSlwToolNNLayerFullConSTU *pTNNL;

    OSLW_assert(!(pBPnn));
    OSLW_assert(!(pAF));
    OSLW_assert(!(pTRB));
    OSLW_assert(!(pMem));

    p1 = pMem->Malloc(pMem, sizeof(OSlwToolNNLayerFullConSTU));
    p2 = pMem->Malloc(pMem, sizeof(ParaType)*row*col);
    p3 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);
    if (!(p1 && p2 && p3))
    {
        OSLW_assert(1);
        return NULL;
    }

    OSlwToolBPnnLayerInitial(p1, pAF, row, col, p2, p3, pTRB, Rmax, Rmin, nl);
    if (!(pout))//如果没有自定义输出
    {
        p3 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个输出
    }
    else
    {
        p3 = pout;
    }

	p8 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个X输出

    if(pBPnn->ParaGroupNum == 0)//只有输入层
    {

        if (!pin)//如果没有定义输入
        {
            p2 = pMem->Malloc(pMem, sizeof(ParaType)*row);//分配一个输入
        }
        else
        {
            p2 = pin;
        }

        p4 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * row);//in error
        p5 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//delt
        p6 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//out error

    }
    else
    {
        pTNNL = pBPnn->Net.pTail->Data.pData;//得到上一层内存地址
        p2 = pTNNL->out.a;
        p4 = pTNNL->outErr.a;//输入的误差是上一层输出误差
        p5 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//delt
        p6 = pMem->Malloc(pMem, sizeof(ParaType) * 1 * col);//out error

    }



    p7 = pMem->Malloc(pMem, sizeof(OSlwToolDListNodeSTU));
	
	if (!(p1 && p2 && p3 && p4 && p5 && p6 && p7 && p8))
	{
		OSLW_assert(1);
		return NULL;
	}

    memset(p7, 0, sizeof(OSlwToolDListNodeSTU));


    OSlwToolBPnnLayerForwardInitial(p1, p2, p3, p8);
    OSlwToolBPnnLayerBackwardInitial(p1, p4, p5, p6);
    OSlwToolBPnnAppend(pBPnn, p7, p1);

	
	if (DropOutEps > _ParaFint(0))
	{
	
		OSlwToolBPnnDropOutInitial(p1, pMem->Malloc(pMem, sizeof(OSlwToolNNDropOutSTU)), DropOutEps, pMem->Malloc(pMem, sizeof(ParaType) * 1 * col));

	}
	else
	{
		((OSlwToolNNLayerFullConSTU *)p1)->pDropOut = NULL;
	}
	

    return p1;

}



//神经网络初始化
void OSlwToolBPnnInitial(OSlwToolBPnnSTU *pBPnn)
{
    OSLW_assert(!(pBPnn));
    memset(pBPnn, 0, sizeof(OSlwToolBPnnSTU));
    OSlwToolDListInitial(&pBPnn->Net,sizeof(OSlwToolNNLayerFullConSTU),NULL);
    pBPnn->_nl_factor = _ParaFrom(1.0);

}


void OSlwToolBPnnLossInitial(
    OSlwToolBPnnSTU *pBPnn,
    ParaType *pRef,
    OSlwToolRandomBasicSTU *pRand,
    OSlwMemoryBasicSTU *pMem,
    lw_u32 set_len,
    lw_u32 sample_len,
    LossFunTYPE loss,
    OSlwToolNNTrainSaveMethodNum savem,
    OSlwToolNNTrainUpdateMethodNum updatem
)
{

    OSlwToolNNLayerFullConSTU *pTNL;
    lw_u32 *pKind;
    lw_u16 i = 0;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;

    OSLW_assert(!(pBPnn));
	OSLW_assert(!(pRef));
	//OSLW_assert(!(loss));
    OSLW_assert(!(pRand));
    OSLW_assert(!(pMem));

    pTNL=pBPnn->Net.pTail->Data.pData;




    OSlwToolMatrixInitial(&(pBPnn->ref),1,pTNL->out.col,pRef);
    pBPnn->Train.BatchSetLength = set_len - 1;
	pBPnn->Train.SampleSetLength = sample_len;
    pBPnn->Train.BatchSampleDiv = set_len / sample_len;
    pBPnn->Train.BatchSampleDiv = pBPnn->Train.BatchSampleDiv ? pBPnn->Train.BatchSampleDiv : 1;

    pBPnn->Train.LossFun = loss;
    pBPnn->Train.pRand = pRand;
    pBPnn->Train.Flag.SaveMethod = savem;
    pBPnn->Train.Flag.UpdateMethod = updatem;

	pKind = pMem->Malloc(pMem, sizeof(lw_u32)*(pBPnn->ParaGroupNum + 1));
    pKind[i++] = PARA_MEM_CAL(1);
    //得到每一列的大小
    pDLN = pBPnn->Net.pTail;
    do {
        pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
        pKind[i++] = PARA_MEM_CAL(pNNL->w.length);
		pKind[i++] = PARA_MEM_CAL(pNNL->BiasUN.bias.length);

        pDLN = (OSlwToolDListNodeSTU *)pDLN->con.pLast;
    } while (pDLN->Key.uData);


    if (pBPnn->Train.Flag.SaveMethod == OSlwToolNNTrainSaveMethod_AllSave)
    {
        OSlwToolTableInitial(&(pBPnn->Train.DeltTable), sample_len, NULL, pBPnn->ParaGroupNum + 1, pMem, NULL, pKind);//完全存储就会全部存下
    }
    else
    {
        OSlwToolTableInitial(&(pBPnn->Train.DeltTable), 1, NULL,  pBPnn->ParaGroupNum + 1, pMem, NULL, pKind);//节约存储只会存一条delt
    }

    //pMem->Free(pMem, pKind);

    //pBPnn->loss=loss;
}

void OSlwToolBPnnReguInitial(OSlwToolBPnnSTU *pBPnn, OSlwToolNNReguTypeNUM ReguType, ParaType lambda)
{

	OSLW_assert(!pBPnn);

	pBPnn->Regu.ReguType = ReguType;
	pBPnn->Regu.Lambda = lambda;

}

/*(Ver.=0.93)
//对网络减肥
//dropout 效果不好
OSlwToolBPnnSTU* OSlwToolBPnnDropOutStart(OSlwToolBPnnSTU *_pBPnn)
{
    static OSlwToolBPnnSTU *pBPnn;
    static OSlwToolDListNodeSTU  *pDLN;
    static OSlwToolNNLayerFullConSTU *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    OSLW_assert(!(pBPnn));

    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return NULL;

    pDLN=(OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;

    do
    {
        pNNL=(OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
        if(!(pNNL->pDropOutW && pNNL->pDropOutCol && pNNL->pDropOutRow))
        {
            return NULL;
        }

        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        count_row=count_col;
        count_col=0;
        //准备过程 得到具体的行和列
        if(pDLN == (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext)
        {

            count_row=pNNL->size_row;
            for(i=0; i<pNNL->size_row; i++) //行不要dropout
                *prow++=i;
            for(j=0; j<pNNL->size_col; j++) //列要
            {
                if(pNNL->pRand->rand(pNNL->pRand,_ParaFint(1),_ParaFint(0)) > pNNL->PDropOut)
                {
                    *pcol++ = j;
                    count_col++;
                }//end if
            }//end for
        }//end if
        else if(pDLN == pBPnn->Net.pTail)
        {
            count_col=pNNL->size_col;
            for(j=0; j<pNNL->size_col; j++) //行不要dropout
                *pcol++=j;

        }
        else
        {

            for(j=0; j<pNNL->size_col; j++) //列dropout
            {
                if(pNNL->pRand->rand(pNNL->pRand,_ParaFint(1),_ParaFint(0)) > pNNL->PDropOut)
                {
                    *pcol++ = j;
                    count_col++;
                }// end if
            }//end for
        }//end else

        //开始具体dropout W
        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        pbuf=pNNL->pDropOutW;
        for(i=0; i<count_row; i++)
            for(j=0; j<count_col; j++)
                *pbuf++=_ParaDiv(OSLW_TOOL_M_GET(pNNL->w,prow[i],pcol[j]),_ParaFrom(1));

        //交换doupoutW与w矩阵
        pbuf=pNNL->w.a;
        pNNL->w.a=pNNL->pDropOutW;
        pNNL->pDropOutW=pbuf;


        //Dropout BiasUN.bias 与 BiasUN.bias
        pbuf=pNNL->pDropOutBiasUN.bias;
        if(pNNL->BiasUN.bias.a)
        {
            for(j=0; j<count_col; j++)
                *pbuf++=_ParaDiv(pNNL->w.a[pcol[j]],_ParaFrom(1));
        }

        pbuf=pNNL->BiasUN.bias.a;
        pNNL->BiasUN.bias.a=pNNL->pDropOutBiasUN.bias;
        pNNL->pDropOutBiasUN.bias=pbuf;


        //调整层大小
        OSlwToolBPnnLayerReSize(pNNL,count_row,count_col);

        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pNext;

    } while(pDLN);

    return pBPnn;

}

//恢复网络
OSlwToolBPnnSTU* OSlwToolBPnnDropOutStop(OSlwToolBPnnSTU *_pBPnn)
{
    static OSlwToolBPnnSTU *pBPnn;
    static OSlwToolDListNodeSTU  *pDLN;
    static OSlwToolNNLayerFullConSTU *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    OSLW_assert(!(pBPnn));

    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return NULL;

    pDLN=(OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;

    do
    {
        pNNL=(OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
        if(!(pNNL->pDropOutW && pNNL->pDropOutCol && pNNL->pDropOutRow))
        {
            return NULL;
        }
        count_row=pNNL->w.row;
        count_col=pNNL->w.col;

        //交换doupoutW与w矩阵
        pbuf=pNNL->w.a;
        pNNL->w.a=pNNL->pDropOutW;
        pNNL->pDropOutW=pbuf;

        //dropoutW恢复
        prow=pNNL->pDropOutRow;
        pcol=pNNL->pDropOutCol;
        pbuf=pNNL->pDropOutW;
        for(i=0; i<count_row; i++)
            for(j=0; j<count_col; j++)
            {
                OSLW_TOOL_M_GET(pNNL->w,prow[i],pcol[j])=_ParaMpy(*pbuf,_ParaFrom(1));
                pbuf++;
            }


        //BiasUN.bias 恢复
        pbuf=pNNL->BiasUN.bias.a;
        pNNL->BiasUN.bias.a=pNNL->pDropOutBiasUN.bias;
        pNNL->pDropOutBiasUN.bias=pbuf;

        for(j=0; j<count_col; j++)
        {
            *(pNNL->BiasUN.bias.a+pcol[j])=_ParaMpy(*pbuf,_ParaFrom(1));
            pbuf++;
        }


        //调整层大小
        OSlwToolBPnnLayerReSize(pNNL,pNNL->size_row,pNNL->size_col);

        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pNext;
    } while(pDLN);

    return pBPnn;


}
*/

//前向传播
OSlwToolBPnnSTU *OSlwToolBPnnRunning(OSlwToolBPnnSTU *_pBPnn)
{

    OSlwToolBPnnSTU *pBPnn;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    OSLW_assert(!(_pBPnn));
    pBPnn=_pBPnn;
    if(!(pBPnn->ParaGroupNum))
        return _pBPnn;
    pDLN=(OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
    do {
        pNNL=(OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)//如果有定义BN
		{
			OSlwToolMatrixSTU stas;//用于统计矩阵
			ParaType stas_res;
			stas.row = stas.col = stas.length = 2;
			stas.a = &stas_res;

			if (pBPnn->Train.Flag.Status != OSlwToolNNTrainStatus_Using)//在训练状态
			{

				OSlwToolMatrixSTU m1;
				OSlwToolMatrixSTU m2;

				pOSlwToolMatrixMPYA(&(pNNL->BiasUN._x_out_N), &(pNNL->in), &(pNNL->w), &(stas));
				pNNL->pBN->NowVar = pOSlwToolMatrixVar(&(pNNL->BiasUN._x_out_N), stas.a, &(pNNL->BiasUN._x_out_N), pNNL->pBN->BN_Epsi);

				m1.length = 1;
				m1.a = &(pNNL->pBN->BN_gamma);


				m2.length = 1;
				m2.a = &(pNNL->pBN->BN_beta);

				pOSlwToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->BiasUN._x_out_N), &(m1), &(m2));

				pNNL->pBN->NowMean = stas_res;
				
				//位移法记录总的平均值与方差
				pNNL->pBN->RealMean = _ParaAdd(
					_ParaMpy(pNNL->pBN->RealMean, _ParaFrom(0.99)),
					_ParaMpy(pNNL->pBN->NowMean, _ParaFrom(0.01))
				);

				pNNL->pBN->RealVar = _ParaAdd(
					_ParaMpy(pNNL->pBN->RealVar, _ParaFrom(0.99)),
					_ParaMpy(pNNL->pBN->NowVar, _ParaFrom(0.01))
				);


			}
			else
			{
				OSlwToolMatrixSTU m1;
				OSlwToolMatrixSTU m2;
				//k=gamma/sqrt(var+e)
				ParaType k = _ParaDiv(
					pNNL->pBN->BN_gamma, 
					_ParaSqrt(
						_ParaAdd(pNNL->pBN->RealVar, pNNL->pBN->BN_Epsi)
					)
				);
				//b=beta-mean/sqrt(var+e)
				ParaType b = _ParaSub(
					pNNL->pBN->BN_beta,
					_ParaDiv(
						pNNL->pBN->RealMean,
						_ParaSqrt(
							_ParaAdd(pNNL->pBN->RealVar, pNNL->pBN->BN_Epsi)
						)
					)
				);

				m1.length = m2.length = 1;
				m1.a = &k;
				m2.a = &b;

				pOSlwToolMatrixMPYA(&(pNNL->BiasUN._x_out_N), &(pNNL->in), &(pNNL->w), &(stas));

				pOSlwToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->BiasUN._x_out_N), &(m1), &(m2));
		
			}
			

		}
		else
		{
			pOSlwToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->in), &(pNNL->w), &(pNNL->BiasUN.bias));
		}
		*/

		if (pBPnn->Train.Flag.Status != OSlwToolNNTrainStatus_Using && pNNL->pDropOut)
		{
			lw_u32 i = pNNL->size_col;
			ParaType *pd = pNNL->pDropOut->DropOutMat.a;
			ParaType *pin = pNNL->in.a;
			ParaType poss = pNNL->pDropOut->PossDropOut;
			ParaType _poss_data;
			while (i--)
			{

				_poss_data = pNNL->pRand->rand(pNNL->pRand, _ParaFrom(1), _ParaFrom(0));
				if (_poss_data <= poss)//小于保留概率 保留
				{
					*pd = _ParaDiv(_ParaFint(1), poss);
				}
				else
				{
					*pd = _ParaFint(0);
				}

				*pin = _ParaMpy(*pin, *pd);

				pin++;
				pd++;
			}


		}

		pOSlwToolMatrixMPYA(&(pNNL->_out_x), &(pNNL->in), &(pNNL->w), &(pNNL->BiasUN.bias));
        pNNL->pActFun->Forward(pNNL->pActFun,&(pNNL->_out_x),&(pNNL->out));
        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pNext;

    } while(pDLN);


    return pBPnn;
}



OSlwToolBPnnSTU *OSlwToolBPnnErrCalu(OSlwToolBPnnSTU *pBPnn)
{


    OSlwToolNNLayerFullConSTU *pTNL;
    OSLW_assert(!(pBPnn));

    pTNL=pBPnn->Net.pTail->Data.pData;

    if(pBPnn->Train.LossFun)
    {

        pBPnn->Error  = pBPnn->Train.LossFun(&(pTNL->outErr),&(pBPnn->ref),&(pTNL->out));

    }

    return pBPnn;

}


/*(Ver.=0.93)
//旧版本
//后向传播
OSlwToolBPnnSTU *OSlwToolBPnnUpdate(OSlwToolBPnnSTU *_pBPnn)
{

    OSlwToolBPnnSTU *pBPnn;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    lw_u16 i,j;
    ParaType temp;
    OSLW_assert(!(_pBPnn));
    pBPnn=_pBPnn;

    if(!(pBPnn->LayerNumber))
        return _pBPnn;
    pDLN=pBPnn->Net.pTail;
    //假设w（m*n）
    do {
        pNNL=(OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
        pNNL->pActFun->Backward(&pNNL->out,&pNNL->out);
        //delta(1*n)=outerr(1*n).*(dy)
        for(i=0; i<pNNL->out.col; i++)
            pNNL->delt.a[i]=_ParaMpy(pNNL->outErr.a[i],pNNL->out.a[i]);

        //输入误差=delta(1*n)*w'(n*m)
        for(i=0; i<pNNL->w.row; i++)
        {
            temp=_ParaFint(0);
            for(j=0; j<pNNL->w.col; j++)
            {
                temp=_ParaAdd(temp,_ParaMpy(pNNL->delt.a[j],OSLW_TOOL_M_GET(pNNL->w,i,j)));
            }
            pNNL->inErr.a[i]=temp;

        }
        //dw(m*n)=x'(m*1)*delta(1*n)
        for(i=0; i<pNNL->w.row; i++)
        {
            for(j=0; j<pNNL->w.col; j++)
            {
                temp=_ParaMpy(pNNL->in.a[i],pNNL->delt.a[j]);
                OSLW_TOOL_M_GET(pNNL->w,i,j)=_ParaAdd(OSLW_TOOL_M_GET(pNNL->w,i,j),_ParaMpy(temp,pNNL->nl));//别忘记学习速度
            }
        }
        if(pNNL->BiasUN.bias.a)
        {
            for(j=0; j<pNNL->w.col; j++)
            {
                pNNL->BiasUN.bias.a[j]=_ParaAdd(pNNL->BiasUN.bias.a[j], _ParaMpy(pNNL->nl,pNNL->delt.a[j]));
            }
        }


        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pLast;
    } while(pDLN->Key.uData);


    return pBPnn;
}
*/





OSlwToolBPnnSTU *OSlwToolBPnnDeltCalu(OSlwToolBPnnSTU *_pBPnn,lw_u16 _batch_index)
{

    OSlwToolBPnnSTU *pBPnn;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    lw_u16 i, j, count = 1, len;
	ParaType temp, nl, _regu_k_U, _regu_k_B, *_pdw, *_pw;
	OSlwToolMatrixSTU dw, dbias;
    OSlwToolTableSTU *ptable;

    OSLW_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;


    pDLN = pBPnn->Net.pTail;
    ptable = &(pBPnn->Train.DeltTable);
	
	_regu_k_B = _ParaMpy(pBPnn->Regu.Lambda, _ParaFrom(pBPnn->WeightParaNum));
	

    //假设w（m*n）
    do 
	{
        pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
		pNNL->pActFun->Backward(pNNL->pActFun, &pNNL->_out_x, &pNNL->out);

		//取出dw
		dw.col = pNNL->w.col;
		dw.a = ptable->ReadFun(ptable, _batch_index, count++, &len);

		nl = _ParaMpy(pNNL->nl, pBPnn->_nl_factor);
		
		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)//如果使能BN
		{
			
			ParaType *pdgamma, *pdbeta;
			lw_u32 i = pNNL->_out_x.length;
			ParaType _sum_dg = _ParaFrom(0);
			ParaType _sum_db = _ParaFrom(0);
			ParaType *pdx_n = pNNL->DeltaUN._d_x_N.a;
			ParaType *px_n = pNNL->BiasUN._x_out_N.a;

			ParaType *pdout = pNNL->_out_x.a;
			ParaType *pdnext;

			ParaType _sum_dvar = _ParaFrom(0);
			ParaType _sum_dmean = _ParaFrom(0);
			ParaType _div_sqrt_var;
			ParaType _factor_xn;


			//第零步 计算dout也就是delta
			for (i = 0; i < pNNL->_out_x.col; i++)
				pdout[i] = _ParaMpy(pNNL->outErr.a[i], pdout[i]);

			//第一步 取出 dgamma dbeta
			pdgamma = ptable->ReadFun(ptable, _batch_index, count++, &len);
			pdbeta = pdgamma + 1;



			//第二步(算法经过化简) 计算dgamma dbeta dnorm(向量) sumdvar sumdmean
			while (i--)
			{
				//dbeta = np.sum(dout, axis=0)
				_sum_db = _ParaAdd(_sum_db, *pdout);

				//dgamma = np.sum(x_normalized * dout, axis=0)
				_sum_dg = _ParaAdd(_sum_dg, _ParaMpy(*pdout, *px_n));

				//dx_normalized = gamma * dout
				*pdx_n = _ParaMpy(*pdout, pNNL->pBN->BN_gamma);


				//sumdvar=sum(dx_normalized[1:n] * x_normalized[1:n])
				_sum_dvar = _ParaAdd(_sum_dvar, _ParaMpy(*pdx_n, *px_n));

				//sumdmean=sum(dx_normalized[1:n])
				_sum_dmean = _ParaAdd(_sum_dmean, *pdx_n);

				pdx_n++;
				px_n++;
				pdout++;
			}

			*pdgamma = _ParaAdd(*pdgamma, _ParaMpy(_sum_dg, nl));
			*pdbeta = _ParaAdd(*pdbeta, _ParaMpy(_sum_db, nl));

			//第三部 部分常数计算
			_div_sqrt_var = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(pNNL->pBN->NowVar, pNNL->pBN->BN_Epsi)));
			_factor_xn = _ParaMpy(-_div_sqrt_var, _ParaDiv(_sum_dvar, _ParaFint(pNNL->_out_x.length)));
			_sum_dmean = _ParaDiv(_sum_dmean, _ParaFint(pNNL->_out_x.length));
			_sum_dmean = _ParaMpy(_sum_dmean, _div_sqrt_var);

			//第四步 dx计算
			pdx_n = pNNL->DeltaUN._d_x_N.a;
			px_n = pNNL->BiasUN._x_out_N.a;
			pdnext = pNNL->DeltaUN.delta.a;

			i = pNNL->_out_x.length;

			while (i--)
			{
				*pdnext = _ParaMpy(_div_sqrt_var, *pdx_n);
				*pdnext = _ParaAdd(*pdnext, _ParaMpy(_factor_xn, *px_n));
				*pdnext = _ParaSub(*pdnext, _sum_dmean);
				//*pdnext = _ParaMpy(*pdnext, 0);
				pdx_n++;
				px_n++;
				pdnext++;

			}
			

		}
		else
		{
			dbias.a = ptable->ReadFun(ptable, _batch_index, count++, &len);//取出dbias
			
			//delta(1*n)=outerr(1*n).*(dy)
			for (i = 0; i<pNNL->_out_x.col; i++)
				pNNL->DeltaUN.delta.a[i] = _ParaMpy(pNNL->outErr.a[i], pNNL->_out_x.a[i]);
		}
		*/

		dbias.a = ptable->ReadFun(ptable, _batch_index, count++, &len);//取出dbias

		//delta(1*n)=outerr(1*n).*(dy)
		for (i = 0; i<pNNL->_out_x.col; i++)
			pNNL->DeltaUN.delta.a[i] = _ParaMpy(pNNL->outErr.a[i], pNNL->_out_x.a[i]);



        //dw(m*n)=x'(m*1)*delta(1*n)
        for (i = 0; i<pNNL->w.row; i++)
        {
            for (j = 0; j<pNNL->w.col; j++)
            {
                temp = _ParaMpy(pNNL->in.a[i], pNNL->DeltaUN.delta.a[j]);
                //OSLW_TOOL_M_GET(pNNL->w, i, j) = _ParaAdd(OSLW_TOOL_M_GET(pNNL->w, i, j), _ParaMpy(temp, pNNL->nl));//别忘记学习速度
                OSLW_TOOL_M_GET(dw, i, j) = _ParaAdd(OSLW_TOOL_M_GET(dw, i, j), _ParaMpy(temp, nl));//别忘记学习速度
            }
        }

        //if (pNNL->pBN == NULL)
        {
            for (j = 0; j<pNNL->w.col; j++)
            {
                dbias.a[j] = _ParaAdd(dbias.a[j], _ParaMpy(nl, pNNL->DeltaUN.delta.a[j]));
            }
        }

		//输入误差=delta(1*n)*w'(n*m)
		for (i = 0; i<pNNL->w.row; i++)
		{
			temp = _ParaFint(0);
			for (j = 0; j<pNNL->w.col; j++)
			{
				temp = _ParaAdd(temp, _ParaMpy(pNNL->DeltaUN.delta.a[j], OSLW_TOOL_M_GET(pNNL->w, i, j)));
			}
			pNNL->inErr.a[i] = temp;

		}

		if (pNNL->pDropOut)
		{
			pOSlwToolMatrixDot(&(pNNL->inErr), &(pNNL->inErr), &(pNNL->pDropOut->DropOutMat));
		}




		//正则化
		switch (pBPnn->Regu.ReguType)
		{
		case OSlwToolNNReguType_NAN:

			break;

		case OSlwToolNNReguType_L2:
			_pdw = dw.a;
			_pw = pNNL->w.a;
			
			_regu_k_U = _ParaMpy(_regu_k_B,pNNL->nl);//别忘记学习速率

			i = pNNL->w.length;

			while (i--)
			{
				*_pdw = _ParaSub(*_pdw, _ParaMpy(_regu_k_U, *_pw));
				_pdw++;
				_pw++;
			}


			break;

		case OSlwToolNNReguType_L1:
			_pdw = dw.a;
			_pw = pNNL->w.a;

			_regu_k_U = _ParaMpy(_regu_k_B, pNNL->nl);//别忘记学习速率

			i = pNNL->w.length;

			while (i--)
			{
				*_pdw = _ParaSub(*_pdw, _ParaMpy(_regu_k_U, (*_pw > _ParaFint(0) ? _ParaFrom(1) : _ParaFrom(-1))));
				_pdw++;
				_pw++;
			}
			break;

		default:
			break;
		}

        pDLN = (OSlwToolDListNodeSTU *)pDLN->con.pLast;
    } while (pDLN->Key.uData);


    return pBPnn;
}


ParaType *OSlwToolBPnnGradForInput(OSlwToolBPnnSTU *_pBPnn)
{

	OSlwToolBPnnSTU *pBPnn;
	OSlwToolDListNodeSTU  *pDLN;
	OSlwToolNNLayerFullConSTU *pNNL;
	lw_u16 i, j;
	ParaType temp, *pret_in_err;

	OSLW_assert(!(_pBPnn));
	pBPnn = _pBPnn;

	if (!(pBPnn->ParaGroupNum))
		return NULL;


	pNNL = pBPnn->Net.pTail->Data.pData;

	pOSlwToolMatrixSet(&(pNNL->outErr), _ParaFint(1), NULL);//输出误差为1 方便直接求导

	pDLN = pBPnn->Net.pTail;
	do
	{
		pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
		pNNL->pActFun->Backward(pNNL->pActFun, &pNNL->_out_x, &pNNL->out);
		for (i = 0; i<pNNL->w.row; i++)
		{
			temp = _ParaFint(0);
			for (j = 0; j<pNNL->w.col; j++)
			{
				temp = _ParaAdd(temp, _ParaMpy(pNNL->DeltaUN.delta.a[j], OSLW_TOOL_M_GET(pNNL->w, i, j)));
			}
			pNNL->inErr.a[i] = temp;

		}

		pret_in_err = pNNL->inErr.a;
		pDLN = (OSlwToolDListNodeSTU *)pDLN->con.pLast;
	} while (pDLN->Key.uData);


	return pret_in_err;
}


OSlwToolBPnnSTU *_OSlwToolBPnnReviewOnce(OSlwToolBPnnSTU *_pBPnn,lw_u16 batch_num, OSlwToolMatrixSTU *pmat_k)
{
	OSlwToolBPnnSTU *pBPnn;
	OSlwToolDListNodeSTU  *pDLN;
	OSlwToolNNLayerFullConSTU *pNNL;
	OSlwToolTableSTU *ptable;
	lw_u16 count = 1, len;
	OSlwToolMatrixSTU dw, dbias;

	OSLW_assert(!(_pBPnn));
	pBPnn = _pBPnn;

	if (!(pBPnn->ParaGroupNum))
		return _pBPnn;
	ptable = &(pBPnn->Train.DeltTable);

	pDLN = pBPnn->Net.pTail;
	do {
		pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
		//取出dw 与 dbias
		dw.length = pNNL->w.length;
		dw.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		//更新w
		pOSlwToolMatrixMPYA(&(pNNL->w), &(dw), pmat_k, &(pNNL->w));

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL->pBN)
		{

		ParaType *pdgamma, *pdbeta;

		pdgamma = ptable->ReadFun(ptable, batch_num, count++, &len);
		pdbeta = pdgamma + 1;

		pNNL->pBN->BN_beta = _ParaAdd(pNNL->pBN->BN_beta, _ParaMpy(*pdbeta, temp));
		pNNL->pBN->BN_gamma = _ParaAdd(pNNL->pBN->BN_gamma, _ParaMpy(*pdgamma, temp));

		}
		else
		{
		dbias.length = pNNL->BiasUN.bias.length;
		dbias.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		pOSlwToolMatrixMPYA(&(pNNL->BiasUN.bias), &(dbias), &mat_k, &(pNNL->BiasUN.bias));
		}
		*/

		dbias.length = pNNL->BiasUN.bias.length;
		dbias.a = ptable->ReadFun(ptable, batch_num, count++, &len);
		pOSlwToolMatrixMPYA(&(pNNL->BiasUN.bias), &(dbias), pmat_k, &(pNNL->BiasUN.bias));

		pDLN = (OSlwToolDListNodeSTU *)pDLN->con.pLast;
	} while (pDLN->Key.uData);



	return pBPnn;
}


OSlwToolBPnnSTU *OSlwToolBPnnReview(OSlwToolBPnnSTU *_pBPnn)
{

    OSlwToolBPnnSTU *pBPnn;
    lw_u16 batch_num;
    ParaType temp;
	OSlwToolMatrixSTU mat_k;
    OSlwToolTableSTU *ptable;

    OSLW_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;
    ptable = &(pBPnn->Train.DeltTable);
	temp = _ParaDiv(_ParaFint(1), _ParaFrom(pBPnn->Train.SampleSetLength));
	mat_k.length = 1;
	mat_k.a = &temp;

    //更新 w 与 bias
    for ( batch_num = 0; batch_num < pBPnn->Train.SampleCount+1; batch_num++)
    {
		_OSlwToolBPnnReviewOnce(pBPnn, batch_num, &mat_k);

    }

    //清空dw与dbias
    memset(ptable->Row.pData, 0, ptable->Row.uData * ptable->ColSize);


    //状态重置
    pBPnn->Train.count = 0;
    pBPnn->Train.SampleCount = 0;
    pBPnn->Train.Flag.Status = OSlwToolNNTrainStatus_Wait;

    return pBPnn;
}



OSlwToolBPnnSTU *OSlwToolBPnnTrain(OSlwToolBPnnSTU *_pBPnn)
{
    OSlwToolBPnnSTU *pBPnn;
    OSlwToolNNTrainSTU *pTr;

    OSLW_assert(!(_pBPnn));
    pBPnn = _pBPnn;

    if (!(pBPnn->ParaGroupNum))
        return _pBPnn;

    pTr = &(pBPnn->Train);
    switch (pTr->Flag.Status)
    {
    case OSlwToolNNTrainStatus_Wait:

        if (!(pTr->count % pTr->BatchSampleDiv))//满足采样间隔要求
        {
            pTr->SampleRandNum = pTr->pRand->randint(pTr->pRand, pTr->count + pTr->BatchSampleDiv, pTr->count);
        }

        if (pTr->count ==pTr->SampleRandNum)//等到了
        {
            if (pTr->Flag.SaveMethod == OSlwToolNNTrainSaveMethod_AllSave)//如果是完全存储
            {
                pTr->SampleCount++;//向后存
            }

            OSlwToolBPnnErrCalu(pBPnn);
            OSlwToolBPnnDeltCalu(pBPnn, pTr->SampleCount);
        }

        if (pTr->count == pTr->BatchSetLength)//采集满了
        {
            pTr->Flag.Status = OSlwToolNNTrainStatus_Complete;
            if (pTr->Flag.UpdateMethod == OSlwToolNNTrainUpdateMethod_Auto)//如果自动更新
            {
                OSlwToolBPnnReview(pBPnn);
            }
        }
        else
        {
            pTr->count++;
        }


        break;

    case OSlwToolNNTrainStatus_Complete:

        break;

	case OSlwToolNNTrainStatus_Using:

		break;

    default:
        pBPnn->Train.Flag.Status = OSlwToolNNTrainStatus_Wait;
        break;
    }

    return _pBPnn;
}


//
OSlwToolBPnnSTU *OSlwToolBPnnCopy(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2)
{
    OSlwToolDListNodeSTU  *pDLN1, *pDLN2;
    OSlwToolNNLayerFullConSTU *pNNL1, *pNNL2;

    OSLW_assert(!(pBPnn1));
    OSLW_assert(!(pBPnn2));

    if ((pBPnn1->ParaGroupNum == 0 || pBPnn2->ParaGroupNum ==0) || pBPnn1->ParaGroupNum != pBPnn2->ParaGroupNum)
        return NULL;

    pDLN1 = (OSlwToolDListNodeSTU *)pBPnn1->Net.Head.con.pNext;
    pDLN2 = (OSlwToolDListNodeSTU *)pBPnn2->Net.Head.con.pNext;
    do {
        pNNL1 = (OSlwToolNNLayerFullConSTU *)pDLN1->Data.pData;
        pNNL2 = (OSlwToolNNLayerFullConSTU *)pDLN2->Data.pData;
		
		
		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL1->pBN && pNNL2->pBN)
		{
			memcpy(pNNL1->pBN, pNNL2->pBN, sizeof(OSlwToolNNBatchNormSTU));
		}
		else
		{
			pOSlwToolMatrixSet(&(pNNL1->BiasUN.bias), 0, &(pNNL2->BiasUN.bias));//复制偏置
		}
		*/
        

		pOSlwToolMatrixSet(&(pNNL1->BiasUN.bias), 0, &(pNNL2->BiasUN.bias));//复制偏置
        pOSlwToolMatrixSet(&(pNNL1->w), 0, &(pNNL2->w));//复制权重

        pDLN1 = (OSlwToolDListNodeSTU *)pDLN1->con.pNext;
        pDLN2 = (OSlwToolDListNodeSTU *)pDLN2->con.pNext;

    } while (pDLN2 && pDLN2);

    return pBPnn1;

}


//软替换函数 用于新老网络参数更新（DDPG）
OSlwToolBPnnSTU *OSlwToolBPnnSoftReplace(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2,ParaType tu)
{
	OSlwToolDListNodeSTU  *pDLN1, *pDLN2;
	OSlwToolNNLayerFullConSTU *pNNL1, *pNNL2;

	OSLW_assert(!(pBPnn1));
	OSLW_assert(!(pBPnn2));

	if ((pBPnn1->ParaGroupNum == 0 || pBPnn2->ParaGroupNum == 0) || pBPnn2->ParaGroupNum != pBPnn1->ParaGroupNum)
		return NULL;

	pDLN1 = (OSlwToolDListNodeSTU *)pBPnn1->Net.Head.con.pNext;
	pDLN2 = (OSlwToolDListNodeSTU *)pBPnn2->Net.Head.con.pNext;

	do {
		pNNL1 = (OSlwToolNNLayerFullConSTU *)pDLN1->Data.pData;
		pNNL2 = (OSlwToolNNLayerFullConSTU *)pDLN2->Data.pData;

		/*
		//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
		if (pNNL1->pBN && pNNL2->pBN)
		{
			pNNL1->pBN->BN_gamma = _ParaAdd(
				_ParaMpy(pNNL1->pBN->BN_gamma, _ParaSub(_ParaFrom(1), tu)), 
				_ParaMpy(pNNL2->pBN->BN_gamma, tu)
			);

			pNNL1->pBN->BN_beta = _ParaAdd(
				_ParaMpy(pNNL1->pBN->BN_beta, _ParaSub(_ParaFrom(1), tu)),
				_ParaMpy(pNNL2->pBN->BN_beta, tu)
			);

		}
		else
		{
			pOSlwToolMatrix_RATIO_ADD(
				&(pNNL1->BiasUN.bias),
				_ParaSub(_ParaFrom(1), tu), &(pNNL1->BiasUN.bias),
				tu, &(pNNL2->BiasUN.bias)
			);
		}
		*/

		pOSlwToolMatrix_RATIO_ADD(
			&(pNNL1->BiasUN.bias),
			_ParaSub(_ParaFrom(1), tu), &(pNNL1->BiasUN.bias),
			tu, &(pNNL2->BiasUN.bias)
		);

		pOSlwToolMatrix_RATIO_ADD(
			&(pNNL1->w),
			_ParaSub(_ParaFrom(1), tu), &(pNNL1->w),
			tu, &(pNNL2->w)
		);

		pDLN1 = (OSlwToolDListNodeSTU *)pDLN1->con.pNext;
		pDLN2 = (OSlwToolDListNodeSTU *)pDLN2->con.pNext;

	} while (pDLN2 && pDLN2);

	return pBPnn1;

}
#endif // 0

lw_ptr OSlwToolBPnnLayerForwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr OSlwToolBPnnLayerBackwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr OSlwToolBPnnLayerUpdateDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr OSlwToolBPnnLayerNNmallocDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward) { return 0; }
lw_ptr OSlwToolBPnnLayerClearDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { LW_MAT_CLR(&(pNNSLB->out)); return 0; }
lw_ptr OSlwToolBPnnLayerDataInitDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr OSlwToolBPnnLayerCopyDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2) { return 0; }
lw_ptr OSlwToolBPnnLayerSoftReplaceDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido) { return 0; }



void OSlwToolBPnnInit(OSlwToolBPnnSTU *pBPnn,lw_u16 max_mini_batch)
{
	OSLW_assert(!(pBPnn));

	memset(pBPnn, 0, sizeof(OSlwToolBPnnSTU));
	OSlwToolDListInitial(&pBPnn->Net, sizeof(OSlwToolNNLayerFullConSTU), NULL);
	pBPnn->_nl_factor = _ParaFrom(1.0);
	pBPnn->Train.mini_batch_max = max_mini_batch;
	pBPnn->Train._MemAllocCoff = 1;
	pBPnn->Train.Epsi = _ParaFrom(1e-6);
}


void OSlwToolBPnnLayerAppend
(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolDListNodeSTU *pnode,
	lw_u32 len,
	void *LayerList
)
{
	lw_u16 i;
	OSlwToolNNSubLayerBasicSTU **ppLIST1;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pnode));
	OSLW_assert(!(len));
	OSLW_assert(!(LayerList));

	pBPnn->Net.AppendFUN(&(pBPnn->Net), pnode);
	pnode->Data.uData = len;
	pnode->Data.pData = (void *)LayerList;

	ppLIST1 = LayerList;
	for (i = 0; i < len; i++)
	{
		ppLIST1[i]->pNN = pBPnn;
	}

	return;
}




void OSlwToolBPnnTrainInit(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	LossFunTYPE loss,
	ParaType nl
)
{

	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));


	memcpy(&pBPnn->ref, &pBPnn->y, sizeof(OSlwMat));

	if (pRef==NULL)
	{
		pRef = pmem->Malloc(pmem, PARA_MEM_CAL(pBPnn->ref.length));
	}

	pBPnn->ref.a = pRef;

	pBPnn->Train.LossFun = loss;
	pBPnn->Train.nl = nl;

}

void OSlwToolBPnnOptimInit(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
)
{
	OSLW_assert(!(pBPnn));

	pBPnn->Train.Epsi = _ParaFrom(1e-6);
	pBPnn->Train.Flag.Optim = optim;
	switch (optim)
	{
	case OSlwToolNNOptim_GradDesc:
		pBPnn->Train._MemAllocCoff = 1;
		break;

	case OSlwToolNNOptim_M:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train._MemAllocCoff = 2;
		break;

	case OSlwToolNNOptim_RMSp:
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 2;
		break;

	case OSlwToolNNOptim_Adam:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 3;
		break;

	case OSlwToolNNOptim_Nadam:
		pBPnn->Train.Beta1 = beta1;
		pBPnn->Train.Beta1T = beta1;
		pBPnn->Train.Beta2 = beta2;
		pBPnn->Train.Beta2T = beta2;
		pBPnn->Train._MemAllocCoff = 3;
		break;

	default:
		OSLW_assert(1);
		break;
	}

	return;

}


void OSlwToolBPnnAllDataInit(OSlwToolBPnnSTU *pBPnn,OSlwMemoryBasicSTU *pMem)
{

	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_u32 *pKind;
	register lw_u32 i, count = 0, j;
	lw_u32 flowdatamax = 0;
	void *pFlowDataAddr = NULL;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pMem));

	if (pBPnn->ParaGroupNum==0)
	{
		return;
	}

	pBPnn->pmem = pMem;

	if (pBPnn->Train.Flag.MemoryMethod== OSlwToolNNMemoryMethod_Chip)
	{
		node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				if (ppNNL[i]->FlowData.uData >= flowdatamax && ppNNL[i]->FlowData.pData==NULL)
				{
					flowdatamax = ppNNL[i]->FlowData.uData;
				}
			}
			
			node = (OSlwToolDListNodeSTU *)node->con.pNext;
		}
		//分配浮动内存
		if (flowdatamax)
		{
			pFlowDataAddr = pMem->Malloc(pMem, flowdatamax);
			node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
			while (node)
			{
				ppNNL = node->Data.pData;
				for (i = 0; i < node->Data.uData; i++)
				{
					ppNNL[i]->FlowData.pData = pFlowDataAddr;
				}
				node = (OSlwToolDListNodeSTU *)node->con.pNext;
			}
		}

		node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				ppNNL[i]->DataInit(ppNNL[i]);
			}

			node = (OSlwToolDListNodeSTU *)node->con.pNext;
		}



	}
	else
	{


		//block方式统一存储内存
		pKind = pMem->Malloc(pMem, sizeof(lw_u32)*(pBPnn->ParaGroupNum));
	

		//得到table的列
		count = 0;
		node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				pKind[count++] = ppNNL[i]->sizeofdata* (lw_u32)(pBPnn->Train._MemAllocCoff);
				if (ppNNL[i]->FlowData.uData >= flowdatamax && ppNNL[i]->FlowData.pData == NULL)
				{
					flowdatamax = ppNNL[i]->FlowData.uData;
				}
			}
			node = (OSlwToolDListNodeSTU *)node->con.pNext;
		}
		//分配浮动内存
		if (flowdatamax)
		{
			pFlowDataAddr = pMem->Malloc(pMem, flowdatamax);
		}

		//判断是否需要训练
		if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
		{
			OSlwToolTableInitial(&(pBPnn->Train.DataTable), 2, NULL, pBPnn->ParaGroupNum, pMem, NULL, pKind);//存储两行 分别是原始数值与delt
		}
		else
		{
			OSlwToolTableInitial(&(pBPnn->Train.DataTable), 1, NULL, pBPnn->ParaGroupNum, pMem, NULL, pKind);//存储一行 原始数值
		}

		//分配内存并且初始化
		count = 0;
		node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				//ppNNL[j]->NNmalloc();

				ppNNL[i]->FlowData.pData = pFlowDataAddr;

				if (ppNNL[i]->sizeofdata == 0)
				{
					continue;
				}

				if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 0, count, NULL),
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 1, count, NULL)
					);

					ppNNL[i]->DataInit(ppNNL[i]);
					
				}
				else
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPnn->Train.DataTable.ReadFun(&pBPnn->Train.DataTable, 0, count, NULL),
						NULL
					);

					ppNNL[i]->DataInit(ppNNL[i]);
				}
			}
		
			node = (OSlwToolDListNodeSTU *)node->con.pNext;
		}
	}

}

void* OSlwToolNNLayerSimpleRecover
(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	OSlwMemoryBasicSTU *pmem
)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));	
	switch((*pSimpleBak).NN_Kind)
	{
		case OSlwToolNNSubLayerKind_FullCon:
			pfc=OSlwToolBPnnFullConAppend(
			pBPnn,
			(*pSimpleBak).info[0], (*pSimpleBak).info[1],
			in, out,
			NULL, NULL,
			NULL, NULL, 0, 0,
			(*pSimpleBak).pTemplet, pmem
		);
		pfc->pRecover=(*pSimpleBak).pDataAddr;
		
		break;

		case OSlwToolNNSubLayerKind_Conv:
			pfc=OSlwToolBPnnConvAppend(
			pBPnn,
			(*pSimpleBak).info[0],(*pSimpleBak).info[1],(*pSimpleBak).info[2],
			(*pSimpleBak).info[3],(*pSimpleBak).info[4],(*pSimpleBak).info[5],
			in,out,
			NULL,NULL,
			NULL, NULL, 0, 0,
			(*pSimpleBak).pTemplet,pmem,NULL
		);
		pfc->pRecover=(*pSimpleBak).pDataAddr;
		break;
		
		case OSlwToolNNSubLayerKind_Pool:
			OSlwToolBPnnPoolAppend(
			pBPnn,
			(*pSimpleBak).info[0],(*pSimpleBak).info[1],(*pSimpleBak).info[2],
			(*pSimpleBak).info[3],(*pSimpleBak).info[4],
			in,out,
			(*pSimpleBak).info[5],
			pmem,NULL
		);
		break;
		
		case	OSlwToolNNSubLayerKind_ActFun:
		
		break;
		
	}		
	
	return pfc;
	
}


void* OSlwToolBPnnSimpleRecover
(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	lw_u16 simple_layer_len,
	OSlwMemoryBasicSTU *pmem
)
{
	
	lw_u16 i;
	ParaType *pin=in;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));	
	
		
	
	for(i=0;i<simple_layer_len-1;i++)
	{
		OSlwToolNNLayerSimpleRecover(pBPnn,pin,NULL,pSimpleBak,pmem);
		pin=NULL;
		pSimpleBak++;
	}
	
	OSlwToolNNLayerSimpleRecover(pBPnn,pin,out,pSimpleBak,pmem);
	
	return pBPnn;
	
}


void OSlwToolBPnnForward(OSlwToolBPnnSTU *pBPnn)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_ptr mmb;
	register lw_u32 i;
	OSLW_assert(!(pBPnn));

	mmb = pBPnn->Train.mini_batch_now;
	node = (OSlwToolDListNodeSTU *)pBPnn->Net.Head.con.pNext;
	while (node)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			mmb = ppNNL[i]->Forward(ppNNL[i], mmb);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pNext;
	}

	pBPnn->Train._batch_stream_count++;
	pBPnn->Train.AllBatchCount += pBPnn->Train.mini_batch_now;
	
	return;

}


void OSlwToolBPnnBackward(OSlwToolBPnnSTU *pBPnn)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_ptr mmb;
	register lw_u32 i;
	OSLW_assert(!(pBPnn));

	mmb = pBPnn->Train.mini_batch_now;
	node = (OSlwToolDListNodeSTU *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		i = node->Data.uData;
		while (i--)
		{
			mmb = ppNNL[i]->Backward(ppNNL[i], mmb);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}

	return;
}

void OSlwToolBPnnReview(OSlwToolBPnnSTU *pBPnn)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	register lw_u32 i;
	OSLW_assert(!(pBPnn));

	node = (OSlwToolDListNodeSTU *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->Update(ppNNL[i]);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}

	pBPnn->Train.AllBatchCount = 0;
	pBPnn->Train._batch_stream_count = 0;
	pBPnn->Train.mini_batch_now = 0;

	return;

}


void OSlwToolBPnnLoadX(OSlwToolBPnnSTU *pBPnn, OSlwMat *xs)
{
	OSlwMat _x;
	lw_32 res;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(xs));
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	if (xs->col != pBPnn->x.col)
	{
		OSLW_assert(1);
		return;
	}

	//计算剩余的批容量
	if (pBPnn->Train.mini_batch_max < (pBPnn->Train.mini_batch_now + xs->row))
	{
		OSLW_assert(1);
		return;
	}

	res = pBPnn->Train.mini_batch_max - pBPnn->Train.mini_batch_now;

	
	
	if (res < xs->row)
	{
		//输入batch超过了最大
		//调整输入矩阵
		xs->row=res;
		xs->length=xs->col*res;
	}
	else
	{
		res=xs->row;
	}
	
	

	//初始化
	OSlwMatInit(&_x,res,xs->col,OSlwMatGetR(pBPnn->x,pBPnn->Train.mini_batch_now));
	
	//复制矩阵
	LW_MAT_CPY(&(_x),xs);
	
	
#elif OSLW_TOOL_NN_DATA_FRAME OSLW_TOOL_NN_D_FRAME_F
		if (xs->col != pBPnn->x.row)
	{
		OSLW_assert(1);
		return;
	}

	//计算剩余的批容量
	if (pBPnn->Train.mini_batch_max < (pBPnn->Train.mini_batch_now + xs->row))
	{
		OSLW_assert(1);
		return;
	}

	res = pBPnn->Train.mini_batch_max - pBPnn->Train.mini_batch_now;

	if (res < xs->row)
	{	
		xs->row = res;
	}
	else
	{
		res=xs->row;
	}
	

	//初始化
	OSlwMatInit(&_x, pBPnn->x.row, res, pBPnn->x.a + pBPnn->Train.mini_batch_now);

	pOSlwToolMatrixTurn(&_x, xs);
	
#endif
	


	//更新批数量
	pBPnn->Train.mini_batch_now += res;
}



void OSlwToolBPnnLoadY(OSlwToolBPnnSTU *pBPnn, OSlwMat *ys)
{
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(ys));
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	
	if (ys->col != pBPnn->ref.col || pBPnn->Train.mini_batch_now != (ys->row+pBPnn->Train.y_offset) )
	{
		OSLW_assert(1);
		return;
	}

	//先拷贝y到ref 再拷贝ys到ref
	LW_PARA_JOIN(pBPnn->ref.a, pBPnn->y.a, pBPnn->Train.y_offset*ys->col, ys->a, ys->length);
	
#elif OSLW_TOOL_NN_DATA_FRAME OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#endif

}

void OSlwToolBPnnRun(OSlwToolBPnnSTU *pBPnn, OSlwMat *xs)
{

	OSLW_assert(!(pBPnn));

	if (!(xs==NULL))
	{
		OSlwToolBPnnLoadX(pBPnn, xs);
	}
	else
	{
		pBPnn->Train.mini_batch_now = pBPnn->Train.mini_batch_max;
	}
	
	
	OSlwToolBPnnForward(pBPnn);
	
	if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_NeedNot)
	{
		pBPnn->Train.mini_batch_now = 0;
		pBPnn->Train._batch_stream_count = 0;
		pBPnn->Train.AllBatchCount = 0;
	}

}


void OSlwToolBPnnCalErr(OSlwToolBPnnSTU *pBPnn)
{
	OSlwMat y, ref;

	OSLW_assert(!(pBPnn));

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	OSlwMatInit(&y, pBPnn->Train.mini_batch_now, pBPnn->y.col, pBPnn->y.a);
	OSlwMatInit(&ref, pBPnn->Train.mini_batch_now, pBPnn->ref.col, pBPnn->ref.a);

	if (pBPnn->Train.LossFun)
	{
		pBPnn->Error = pBPnn->Train.LossFun(&(y), &(ref), &(y));
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	
	OSlwMatInit(&y, pBPnn->y.row, pBPnn->Train.mini_batch_now, pBPnn->y.a);
	OSlwMatInit(&ref, pBPnn->ref.row, pBPnn->Train.mini_batch_now, pBPnn->ref.a);

	if (pBPnn->Train.LossFun)
	{
		pBPnn->Error = pBPnn->Train.LossFun(&(y), &(ref), &(y));
	}

#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	OSlwToolBPnnBackward(pBPnn);

	pBPnn->Train.mini_batch_now = 0;


}

void OSlwToolBPnnTrain(OSlwToolBPnnSTU *pBPnn,OSlwMat *ys)
{
	OSLW_assert(!(pBPnn));

	if (ys!=NULL)
	{
		OSlwToolBPnnLoadY(pBPnn, ys);
	}

	OSlwToolBPnnCalErr(pBPnn);
	OSlwToolBPnnReview(pBPnn);
}



void OSlwToolBPnnClear(OSlwToolBPnnSTU *pBPnn)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_u32 i;
	
	OSLW_assert(!(pBPnn));

	pBPnn->Train.mini_batch_now = 0;
	pBPnn->Train._batch_stream_count = 0;
	pBPnn->Train.AllBatchCount = 0;


	node = (OSlwToolDListNodeSTU *)pBPnn->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->Clear(ppNNL[i]);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}
}



void OSlwToolBPnnCopy(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2)
{
	OSlwToolDListNodeSTU *node1, *node2;
	OSlwToolNNSubLayerBasicSTU **ppNNL1, **ppNNL2;
	register lw_u32 i;
	OSLW_assert(!(pBPnn1));
	OSLW_assert(!(pBPnn2));


	node1 = (OSlwToolDListNodeSTU *)pBPnn1->Net.Head.con.pNext;
	node2 = (OSlwToolDListNodeSTU *)pBPnn2->Net.Head.con.pNext;
	while (node1 && node2)
	{
		ppNNL1 = node1->Data.pData;
		ppNNL2 = node2->Data.pData;
		for (i = 0; i < node1->Data.uData; i++)
		{
			ppNNL1[i]->Copy(ppNNL1[i], ppNNL2[i]);
		}

		node1 = (OSlwToolDListNodeSTU *)node1->con.pNext;
		node2 = (OSlwToolDListNodeSTU *)node2->con.pNext;
	}

	return;

}


void OSlwToolBPnnSoftReplace(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2,ParaType Tau)
{
	OSlwToolDListNodeSTU *node1, *node2;
	OSlwToolNNSubLayerBasicSTU **ppNNL1, **ppNNL2;
	register lw_u32 i;
	OSLW_assert(!(pBPnn1));
	OSLW_assert(!(pBPnn2));


	node1 = (OSlwToolDListNodeSTU *)pBPnn1->Net.Head.con.pNext;
	node2 = (OSlwToolDListNodeSTU *)pBPnn2->Net.Head.con.pNext;
	while (node1 && node2)
	{
		ppNNL1 = node1->Data.pData;
		ppNNL2 = node2->Data.pData;
		for (i = 0; i < node1->Data.uData; i++)
		{
			ppNNL1[i]->SoftReplace(ppNNL1[i], ppNNL2[i], Tau);
		}

		node1 = (OSlwToolDListNodeSTU *)node1->con.pNext;
		node2 = (OSlwToolDListNodeSTU *)node2->con.pNext;
	}

	return;
}

OSlwToolNNSubLayerBasicSTU *OSlwToolBPnnAt(OSlwToolBPnnSTU *pBPnn, lw_32 i, lw_32 j)
{

	OSlwToolDListNodeSTU *pln;
	OSlwToolNNSubLayerBasicSTU **ppNNL;

	i = i < 0 ? i + pBPnn->Net.NowLen : i;

	if (i < 0 )//彻底超出维度
	{
		return NULL;
	}

	pln = pBPnn->Net.IndexOfFUN(&(pBPnn->Net), OSlwToolDListCmpCount, i, NULL);
	
	j = j < 0 ? j + pln->Data.uData : j;

	if (j < 0)//彻底超出维度
	{
		return NULL;
	}

	ppNNL = pln->Data.pData;

	return ppNNL[j];
}


#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

