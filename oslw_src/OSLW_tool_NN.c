/*(Ver.=0.9~)(Beg.=0.9)
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


#if OSLW_TOOL_IMPORT_NN_BPNN || OSLW_TOOL_IMPORT_ALL



#if 0



//网络层初始化函数
void OSlwToolBPNNLayerInitial(OSlwToolNNLayerFullConSTU *pNNL//网络层结构体
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
void OSlwToolBPNNLayerForwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X)
{
	OSLW_assert(!(pNNL));
	OSlwToolMatrixInitial(&pNNL->in, 1, pNNL->w.row, pIN);
	OSlwToolMatrixInitial(&pNNL->out, 1, pNNL->w.col, pOUT);
	OSlwToolMatrixInitial(&pNNL->_out_x, 1, pNNL->w.col, pOUT_X);
}


//后向传播初始化
void OSlwToolBPNNLayerBackwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pINerr, ParaType *pdelt, ParaType *pOuterr)
{

	OSLW_assert(!(pNNL));
	OSlwToolMatrixInitial(&pNNL->inErr, 1, pNNL->w.row, pINerr);
	OSlwToolMatrixInitial(&pNNL->outErr, 1, pNNL->w.col, pOuterr);
	OSlwToolMatrixInitial(&pNNL->DeltaUN.delta, 1, pNNL->w.col, pdelt);

}


//BN初始化
/*
void OSlwToolBPNNLayerBatchNormInitial(OSlwToolNNLayerFullConSTU *pNNL, OSlwToolNNBatchNormSTU *pBN,ParaType epsi)
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
void OSlwToolBPNNLayerReSize(OSlwToolNNLayerFullConSTU *pNNL,lw_u16 r,lw_u16 c)
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
void  OSlwToolBPNNDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,ParaType P,ParaType *pWBuf,ParaType *pBiasUN.biasBuf,lw_u16 *pList1,lw_u16 *pList2)
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

void  OSlwToolBPNNDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,OSlwToolNNDropOutSTU *pdrop, ParaType P, ParaType *pDropList)
{

	OSLW_assert(!(pNNL));
	OSLW_assert(!(pdrop));

	pNNL->pDropOut = pdrop;
	pdrop->PossDropOut = _ParaSub(_ParaFint(1), P);
	OSlwToolMatrixInitial(&(pdrop->DropOutMat), pNNL->in.row, pNNL->in.col, pDropList);

}

//神经网络层追加 使用的是tool中的双向链表
OSlwToolBPNNSTU *OSlwToolBPNNAppend(OSlwToolBPNNSTU *pBPNN, OSlwToolDListNodeSTU *pDLN, OSlwToolNNLayerFullConSTU *pNNL)
{
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pDLN));
	OSLW_assert(!(pNNL));

	pBPNN->Net.AppendFUN(&pBPNN->Net, pDLN);
	pDLN->Data.uData = sizeof(OSlwToolNNLayerFullConSTU);
	pDLN->Data.pData = (void *)pNNL;
	if (pBPNN->ParaGroupNum == 0)
	{

		OSlwToolMatrixInitial(&pBPNN->x, pNNL->in.row, pNNL->in.col, pNNL->in.a);

	}

	OSlwToolMatrixInitial(&pBPNN->y, pNNL->out.row, pNNL->out.col, pNNL->out.a);

	pBPNN->ParaGroupNum += 2;//同时加上偏置与权重

	pBPNN->WeightParaNum += pNNL->w.length;
	pBPNN->BiasParaNum += pNNL->BiasUN.bias.length;


	return pBPNN;
}


void*
OSlwToolBPNNLayerAppend_Dynamic
(
	OSlwToolBPNNSTU *pBPNN,
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

    OSLW_assert(!(pBPNN));
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

    OSlwToolBPNNLayerInitial(p1, pAF, row, col, p2, p3, pTRB, Rmax, Rmin, nl);
    if (!(pout))//如果没有自定义输出
    {
        p3 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个输出
    }
    else
    {
        p3 = pout;
    }

	p8 = pMem->Malloc(pMem, sizeof(ParaType)*col);//分配一个X输出

    if(pBPNN->ParaGroupNum == 0)//只有输入层
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
        pTNNL = pBPNN->Net.pTail->Data.pData;//得到上一层内存地址
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


    OSlwToolBPNNLayerForwardInitial(p1, p2, p3, p8);
    OSlwToolBPNNLayerBackwardInitial(p1, p4, p5, p6);
    OSlwToolBPNNAppend(pBPNN, p7, p1);

	
	if (DropOutEps > _ParaFint(0))
	{
	
		OSlwToolBPNNDropOutInitial(p1, pMem->Malloc(pMem, sizeof(OSlwToolNNDropOutSTU)), DropOutEps, pMem->Malloc(pMem, sizeof(ParaType) * 1 * col));

	}
	else
	{
		((OSlwToolNNLayerFullConSTU *)p1)->pDropOut = NULL;
	}
	

    return p1;

}



//神经网络初始化
void OSlwToolBPNNInitial(OSlwToolBPNNSTU *pBPNN)
{
    OSLW_assert(!(pBPNN));
    memset(pBPNN, 0, sizeof(OSlwToolBPNNSTU));
    OSlwToolDListInitial(&pBPNN->Net,sizeof(OSlwToolNNLayerFullConSTU),NULL);
    pBPNN->_nl_factor = _ParaFrom(1.0);

}


void OSlwToolBPNNLossInitial(
    OSlwToolBPNNSTU *pBPNN,
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

    OSLW_assert(!(pBPNN));
	OSLW_assert(!(pRef));
	//OSLW_assert(!(loss));
    OSLW_assert(!(pRand));
    OSLW_assert(!(pMem));

    pTNL=pBPNN->Net.pTail->Data.pData;




    OSlwToolMatrixInitial(&(pBPNN->ref),1,pTNL->out.col,pRef);
    pBPNN->Train.BatchSetLength = set_len - 1;
	pBPNN->Train.SampleSetLength = sample_len;
    pBPNN->Train.BatchSampleDiv = set_len / sample_len;
    pBPNN->Train.BatchSampleDiv = pBPNN->Train.BatchSampleDiv ? pBPNN->Train.BatchSampleDiv : 1;

    pBPNN->Train.LossFun = loss;
    pBPNN->Train.pRand = pRand;
    pBPNN->Train.Flag.SaveMethod = savem;
    pBPNN->Train.Flag.UpdateMethod = updatem;

	pKind = pMem->Malloc(pMem, sizeof(lw_u32)*(pBPNN->ParaGroupNum + 1));
    pKind[i++] = PARA_MEM_CAL(1);
    //得到每一列的大小
    pDLN = pBPNN->Net.pTail;
    do {
        pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
        pKind[i++] = PARA_MEM_CAL(pNNL->w.length);
		pKind[i++] = PARA_MEM_CAL(pNNL->BiasUN.bias.length);

        pDLN = (OSlwToolDListNodeSTU *)pDLN->con.pLast;
    } while (pDLN->Key.uData);


    if (pBPNN->Train.Flag.SaveMethod == OSlwToolNNTrainSaveMethod_AllSave)
    {
        OSlwToolTableInitial(&(pBPNN->Train.DeltTable), sample_len, NULL, pBPNN->ParaGroupNum + 1, pMem, NULL, pKind);//完全存储就会全部存下
    }
    else
    {
        OSlwToolTableInitial(&(pBPNN->Train.DeltTable), 1, NULL,  pBPNN->ParaGroupNum + 1, pMem, NULL, pKind);//节约存储只会存一条delt
    }

    //pMem->Free(pMem, pKind);

    //pBPNN->loss=loss;
}

void OSlwToolBPNNReguInitial(OSlwToolBPNNSTU *pBPNN, OSlwToolNNReguTypeNUM ReguType, ParaType lambda)
{

	OSLW_assert(!pBPNN);

	pBPNN->Regu.ReguType = ReguType;
	pBPNN->Regu.Lambda = lambda;

}

/*(Ver.=0.9~)(Beg.=0.9)
//对网络减肥
//dropout 效果不好
OSlwToolBPNNSTU* OSlwToolBPNNDropOutStart(OSlwToolBPNNSTU *_pBPNN)
{
    static OSlwToolBPNNSTU *pBPNN;
    static OSlwToolDListNodeSTU  *pDLN;
    static OSlwToolNNLayerFullConSTU *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    OSLW_assert(!(pBPNN));

    pBPNN=_pBPNN;

    if(!(pBPNN->LayerNumber))
        return NULL;

    pDLN=(OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;

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
        if(pDLN == (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext)
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
        else if(pDLN == pBPNN->Net.pTail)
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
        OSlwToolBPNNLayerReSize(pNNL,count_row,count_col);

        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pNext;

    } while(pDLN);

    return pBPNN;

}

//恢复网络
OSlwToolBPNNSTU* OSlwToolBPNNDropOutStop(OSlwToolBPNNSTU *_pBPNN)
{
    static OSlwToolBPNNSTU *pBPNN;
    static OSlwToolDListNodeSTU  *pDLN;
    static OSlwToolNNLayerFullConSTU *pNNL;
    static ParaType *pbuf;
    static lw_u16 i,j,count_row=0,count_col=0,*prow,*pcol;
    OSLW_assert(!(pBPNN));

    pBPNN=_pBPNN;

    if(!(pBPNN->LayerNumber))
        return NULL;

    pDLN=(OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;

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
        OSlwToolBPNNLayerReSize(pNNL,pNNL->size_row,pNNL->size_col);

        pDLN=(OSlwToolDListNodeSTU *)pDLN->con.pNext;
    } while(pDLN);

    return pBPNN;


}
*/

//前向传播
OSlwToolBPNNSTU *OSlwToolBPNNRunning(OSlwToolBPNNSTU *_pBPNN)
{

    OSlwToolBPNNSTU *pBPNN;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    OSLW_assert(!(_pBPNN));
    pBPNN=_pBPNN;
    if(!(pBPNN->ParaGroupNum))
        return _pBPNN;
    pDLN=(OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
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

			if (pBPNN->Train.Flag.Status != OSlwToolNNTrainStatus_Using)//在训练状态
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

		if (pBPNN->Train.Flag.Status != OSlwToolNNTrainStatus_Using && pNNL->pDropOut)
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


    return pBPNN;
}



OSlwToolBPNNSTU *OSlwToolBPNNErrCalu(OSlwToolBPNNSTU *pBPNN)
{


    OSlwToolNNLayerFullConSTU *pTNL;
    OSLW_assert(!(pBPNN));

    pTNL=pBPNN->Net.pTail->Data.pData;

    if(pBPNN->Train.LossFun)
    {

        pBPNN->Error  = pBPNN->Train.LossFun(&(pTNL->outErr),&(pBPNN->ref),&(pTNL->out));

    }

    return pBPNN;

}


/*(Ver.=0.9~)(Beg.=0.9)
//旧版本
//后向传播
OSlwToolBPNNSTU *OSlwToolBPNNUpdate(OSlwToolBPNNSTU *_pBPNN)
{

    OSlwToolBPNNSTU *pBPNN;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    lw_u16 i,j;
    ParaType temp;
    OSLW_assert(!(_pBPNN));
    pBPNN=_pBPNN;

    if(!(pBPNN->LayerNumber))
        return _pBPNN;
    pDLN=pBPNN->Net.pTail;
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


    return pBPNN;
}
*/





OSlwToolBPNNSTU *OSlwToolBPNNDeltCalu(OSlwToolBPNNSTU *_pBPNN,lw_u16 _batch_index)
{

    OSlwToolBPNNSTU *pBPNN;
    OSlwToolDListNodeSTU  *pDLN;
    OSlwToolNNLayerFullConSTU *pNNL;
    lw_u16 i, j, count = 1, len;
	ParaType temp, nl, _regu_k_U, _regu_k_B, *_pdw, *_pw;
	OSlwToolMatrixSTU dw, dbias;
    OSlwToolTableSTU *ptable;

    OSLW_assert(!(_pBPNN));
    pBPNN = _pBPNN;

    if (!(pBPNN->ParaGroupNum))
        return _pBPNN;


    pDLN = pBPNN->Net.pTail;
    ptable = &(pBPNN->Train.DeltTable);
	
	_regu_k_B = _ParaMpy(pBPNN->Regu.Lambda, _ParaFrom(pBPNN->WeightParaNum));
	

    //假设w（m*n）
    do 
	{
        pNNL = (OSlwToolNNLayerFullConSTU *)pDLN->Data.pData;
		pNNL->pActFun->Backward(pNNL->pActFun, &pNNL->_out_x, &pNNL->out);

		//取出dw
		dw.col = pNNL->w.col;
		dw.a = ptable->ReadFun(ptable, _batch_index, count++, &len);

		nl = _ParaMpy(pNNL->nl, pBPNN->_nl_factor);
		
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
		switch (pBPNN->Regu.ReguType)
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


    return pBPNN;
}


ParaType *OSlwToolBPNNGradForInput(OSlwToolBPNNSTU *_pBPNN)
{

	OSlwToolBPNNSTU *pBPNN;
	OSlwToolDListNodeSTU  *pDLN;
	OSlwToolNNLayerFullConSTU *pNNL;
	lw_u16 i, j;
	ParaType temp, *pret_in_err;

	OSLW_assert(!(_pBPNN));
	pBPNN = _pBPNN;

	if (!(pBPNN->ParaGroupNum))
		return NULL;


	pNNL = pBPNN->Net.pTail->Data.pData;

	pOSlwToolMatrixSet(&(pNNL->outErr), _ParaFint(1), NULL);//输出误差为1 方便直接求导

	pDLN = pBPNN->Net.pTail;
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


OSlwToolBPNNSTU *_OSlwToolBPNNReviewOnce(OSlwToolBPNNSTU *_pBPNN,lw_u16 batch_num, OSlwToolMatrixSTU *pmat_k)
{
	OSlwToolBPNNSTU *pBPNN;
	OSlwToolDListNodeSTU  *pDLN;
	OSlwToolNNLayerFullConSTU *pNNL;
	OSlwToolTableSTU *ptable;
	lw_u16 count = 1, len;
	OSlwToolMatrixSTU dw, dbias;

	OSLW_assert(!(_pBPNN));
	pBPNN = _pBPNN;

	if (!(pBPNN->ParaGroupNum))
		return _pBPNN;
	ptable = &(pBPNN->Train.DeltTable);

	pDLN = pBPNN->Net.pTail;
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



	return pBPNN;
}


OSlwToolBPNNSTU *OSlwToolBPNNReview(OSlwToolBPNNSTU *_pBPNN)
{

    OSlwToolBPNNSTU *pBPNN;
    lw_u16 batch_num;
    ParaType temp;
	OSlwToolMatrixSTU mat_k;
    OSlwToolTableSTU *ptable;

    OSLW_assert(!(_pBPNN));
    pBPNN = _pBPNN;

    if (!(pBPNN->ParaGroupNum))
        return _pBPNN;
    ptable = &(pBPNN->Train.DeltTable);
	temp = _ParaDiv(_ParaFint(1), _ParaFrom(pBPNN->Train.SampleSetLength));
	mat_k.length = 1;
	mat_k.a = &temp;

    //更新 w 与 bias
    for ( batch_num = 0; batch_num < pBPNN->Train.SampleCount+1; batch_num++)
    {
		_OSlwToolBPNNReviewOnce(pBPNN, batch_num, &mat_k);

    }

    //清空dw与dbias
    memset(ptable->Row.pData, 0, ptable->Row.uData * ptable->ColSize);


    //状态重置
    pBPNN->Train.count = 0;
    pBPNN->Train.SampleCount = 0;
    pBPNN->Train.Flag.Status = OSlwToolNNTrainStatus_Wait;

    return pBPNN;
}



OSlwToolBPNNSTU *OSlwToolBPNNTrain(OSlwToolBPNNSTU *_pBPNN)
{
    OSlwToolBPNNSTU *pBPNN;
    OSlwToolNNTrainSTU *pTr;

    OSLW_assert(!(_pBPNN));
    pBPNN = _pBPNN;

    if (!(pBPNN->ParaGroupNum))
        return _pBPNN;

    pTr = &(pBPNN->Train);
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

            OSlwToolBPNNErrCalu(pBPNN);
            OSlwToolBPNNDeltCalu(pBPNN, pTr->SampleCount);
        }

        if (pTr->count == pTr->BatchSetLength)//采集满了
        {
            pTr->Flag.Status = OSlwToolNNTrainStatus_Complete;
            if (pTr->Flag.UpdateMethod == OSlwToolNNTrainUpdateMethod_Auto)//如果自动更新
            {
                OSlwToolBPNNReview(pBPNN);
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
        pBPNN->Train.Flag.Status = OSlwToolNNTrainStatus_Wait;
        break;
    }

    return _pBPNN;
}


//
OSlwToolBPNNSTU *OSlwToolBPNNCopy(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2)
{
    OSlwToolDListNodeSTU  *pDLN1, *pDLN2;
    OSlwToolNNLayerFullConSTU *pNNL1, *pNNL2;

    OSLW_assert(!(pBPNN1));
    OSLW_assert(!(pBPNN2));

    if ((pBPNN1->ParaGroupNum == 0 || pBPNN2->ParaGroupNum ==0) || pBPNN1->ParaGroupNum != pBPNN2->ParaGroupNum)
        return NULL;

    pDLN1 = (OSlwToolDListNodeSTU *)pBPNN1->Net.Head.con.pNext;
    pDLN2 = (OSlwToolDListNodeSTU *)pBPNN2->Net.Head.con.pNext;
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

    return pBPNN1;

}


//软替换函数 用于新老网络参数更新（DDPG）
OSlwToolBPNNSTU *OSlwToolBPNNSoftReplace(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2,ParaType tu)
{
	OSlwToolDListNodeSTU  *pDLN1, *pDLN2;
	OSlwToolNNLayerFullConSTU *pNNL1, *pNNL2;

	OSLW_assert(!(pBPNN1));
	OSLW_assert(!(pBPNN2));

	if ((pBPNN1->ParaGroupNum == 0 || pBPNN2->ParaGroupNum == 0) || pBPNN2->ParaGroupNum != pBPNN1->ParaGroupNum)
		return NULL;

	pDLN1 = (OSlwToolDListNodeSTU *)pBPNN1->Net.Head.con.pNext;
	pDLN2 = (OSlwToolDListNodeSTU *)pBPNN2->Net.Head.con.pNext;

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

	return pBPNN1;

}
#endif // 0

lw_ptr OSlwToolBPNNLayerForwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr OSlwToolBPNNLayerBackwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num) { return mini_b_num; }
lw_ptr OSlwToolBPNNLayerUpdateDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr OSlwToolBPNNLayerNNmallocDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward) { return 0; }
lw_ptr OSlwToolBPNNLayerTrainCompleteCallBackDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr OSlwToolBPNNLayerDataInitDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB) { return 0; }
lw_ptr OSlwToolBPNNLayerCopyDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2) { return 0; }
lw_ptr OSlwToolBPNNLayerSoftReplaceDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido) { return 0; }


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 InCol,
	lw_u16 OutCol,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
)
{
	
	OSlwToolNNLayerFullConSTU *node;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerFullConSTU));

	memset(node, 0, sizeof(OSlwToolNNLayerFullConSTU));

	node->basic.NN_Kind=OSlwToolNNSubLayerKind_FullCon;


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = OutCol;
	node->Bias.length = OutCol;

	node->DeltB.row = 1;
	node->DeltB.col = OutCol;
	node->DeltB.length = OutCol;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	node->Weight.row = InCol;
	node->Weight.col = OutCol;
	node->Weight.length = OutCol*InCol;

	node->DeltW.row = InCol;
	node->DeltW.col = OutCol;
	node->DeltW.length = OutCol*InCol;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * InCol));
	}

	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, InCol, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * OutCol));
	}

	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, OutCol, pout);
#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F

	node->Weight.row = OutCol;
	node->Weight.col = InCol;
	node->Weight.length = OutCol*InCol;

	node->DeltW.row = OutCol;
	node->DeltW.col = InCol;
	node->DeltW.length = OutCol*InCol;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * InCol));
	}

	OSlwToolMatrixInitial(&(node->basic.in), InCol, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * OutCol));
	}

	OSlwToolMatrixInitial(&(node->basic.out), OutCol, max_mini_batch, pout);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolBPNNLayerFullConForward;
	node->basic.Backward = OSlwToolBPNNLayerFullConBackward;
	node->basic.Update = OSlwToolBPNNLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPNNLayerFullConNNmalloc;
	node->basic.TrainCompleteCB = OSlwToolBPNNLayerFullConTrainCompleteCallBack;
	node->basic.DataInit = OSlwToolBPNNLayerFullConDataInit;
	node->basic.Copy = OSlwToolBPNNLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPNNLayerFullConSoftReplace;

	

	return (OSlwToolNNSubLayerBasicSTU *)node;

}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 Col,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	OSlwToolNNLayerActFunSTU *pTemplet,
	lw_u8 TrainFlag
)
{
	lw_u32 node_size;
	OSlwToolNNLayerActFunSTU *node;
	OSLW_assert(!pmem);
	OSLW_assert(!pTemplet);
	//分配节点内存

	if (pTemplet->_real_size < sizeof(OSlwToolNNLayerActFunSTU))
	{
		node_size = sizeof(OSlwToolNNLayerActFunSTU);
	}
	else
	{
		node_size = pTemplet->_real_size;
	}

	node = pmem->Malloc(pmem, node_size);

	memcpy(node, pTemplet, node_size);



#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, Col, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, Col, pout);


#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	
	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	OSlwToolMatrixInitial(&(node->basic.in), Col, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * Col));
	}
	OSlwToolMatrixInitial(&(node->basic.out), Col, max_mini_batch, pout);

#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	if (pTemplet->_init)
	{
		pTemplet->_init(pTemplet, TrainFlag);
	}

	return (OSlwToolNNSubLayerBasicSTU *)node;
}


lw_ptr OSlwToolBPNNLayerFullConForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 buf1;
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;




#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	//保存maxminibatch
	buf1 = pNNSLB->out.row;

	//记录当前minibatch
	pNNSLB->out.row = mini_b_num;
	pNNSLB->in.row = mini_b_num;

	pOSlwToolMatrixXWeBi(
		&(pNNSLB->out),
		&(pfc->Weight),
		&(pNNSLB->in),
		&(pfc->Bias)
	);

	//还原
	pNNSLB->out.row = buf1;
	pNNSLB->in.row = buf1;

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	//保存maxminibatch
	buf1 = pNNSLB->out.col;

	//记录当前minibatch
	pNNSLB->out.col = mini_b_num;
	pNNSLB->in.col = mini_b_num;

	pOSlwToolMatrixWeXBi(
		&(pNNSLB->out),
		&(pfc->Weight),
		&(pNNSLB->in),
		&(pfc->Bias)
	);

	//还原
	pNNSLB->out.col = buf1;
	pNNSLB->in.col = buf1;
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	register lw_u16 buf1;
	register lw_u16 i, j, col, row;
	register ParaType _sum;
	register ParaType *_out, *_db, *_out_b;

	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	//保存maxminibatch
	buf1 = pNNSLB->out.col;

	//记录当前minibatch
	pNNSLB->out.row = mini_b_num;
	pNNSLB->in.row = mini_b_num;


	//表示第一次反向传递
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		//直接覆盖
		//dw=in'*out
		pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 2);
		
		//db = sum(out, 1);按列求和
		_out_b = pNNSLB->out.a;
		_db = pfc->DeltB.a;
		row = pNNSLB->out.row;
		for (i = 0; i < pNNSLB->out.col; i++)
		{
			_sum = _ParaFint(0);
			_out = _out_b;
			for (j = 0; j < mini_b_num & 0xffff; j++)
			{
				_sum = _ParaAdd(_sum, *_out);
				_out += row;
			}
			*_db++ = _sum;
			_out_b ++;
		}

		//xd = out*w';
		OSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), 1);
	}
	else
	{
		//采用叠加方法
		//dw=in'*out
		pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 6);


		//db = sum(out, 1);按列求和
		_out_b = pNNSLB->out.a;
		_db = pfc->DeltB.a;
		row = pNNSLB->out.row;
		for (i = 0; i < pNNSLB->out.col; i++)
		{
			_sum = _ParaFint(0);
			_out = _out_b;
			for (j = 0; j < mini_b_num & 0xffff; j++)
			{
				_sum = _ParaAdd(_sum, *_out);
				_out += row;
			}
			*_db = _ParaAdd(*_db, _sum);
			_db++;

			_out_b++;
		}
		//xd = out*w';
		OSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), 5);
	}

	//还原
	pNNSLB->out.row = buf1;
	pNNSLB->in.row = buf1;

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	//保存maxminibatch
	buf1 = pNNSLB->out.col;

	//记录当前minibatch
	pNNSLB->out.col = mini_b_num;
	pNNSLB->in.col = mini_b_num;


	//表示第一次反向传递
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		//直接覆盖
		//dw=out*in'
		pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->out), &(pNNSLB->in), 0x01);

		//db = sum(out, 2);按行求和
		_out_b = pNNSLB->out.a;
		_db = pfc->DeltB.a;
		col = pNNSLB->out.col;
		for (i = 0; i < pNNSLB->out.row; i++)
		{
			_sum = _ParaFint(0);
			_out = _out_b;
			for (j = 0; j < mini_b_num & 0xffff; j++)
			{
				_sum = _ParaAdd(_sum, *_out);
				_out++;
			}
			*_db++ = _sum;
			_out_b += col;
		}

		//xd = out'*w;
		pOSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pfc->Weight), &(pNNSLB->out), 0x02);
	}
	else
	{
		//采用叠加方法
		//dw=out*in'
		pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->out), &(pNNSLB->in), 0x5);

		//db = sum(out, 2);按行求和
		_out_b = pNNSLB->out.a;
		_db = pfc->DeltB.a;
		col = pNNSLB->out.col;
		for (i = 0; i < pNNSLB->out.row; i++)
		{
			_sum = _ParaFint(0);
			_out = _out_b;
			for (j = 0; j < mini_b_num & 0xffff; j++)
			{
				_sum = _ParaAdd(_sum, *_out);
				_out++;
			}
			*_db = _ParaAdd(*_db, _sum);
			_db++;
			_out_b += col;
		}

		//xd = out'*w;
		pOSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pfc->Weight), &(pNNSLB->out), 0x2);
	}

	//还原
	pNNSLB->out.col = buf1;
	pNNSLB->in.col = buf1;
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register lw_u32 i, all_batch_count;
	register ParaType k, _div_m;
	register ParaType *_mw, *_mwd;
	register ParaType *_mb, *_mbd;
	register ParaType *_vw, *_vb;
	register ParaType *_uw, *_ub;
	register OSlwToolNNLayerFullConSTU *pfc;
	register OSlwToolBPNNSTU *pBPNN;
	ParaType b1, nb1, b2, nb2, e, b1t, b2t;
	ParaType vbbuf, ubbuf, vwbuf, uwbuf;
	ParaType bbuf, wbuf;

	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pBPNN = pNNSLB->pNN;

	all_batch_count = pBPNN->Train.AllBatchCount == 0 ? 1 : pBPNN->Train.AllBatchCount;
	
	_mw = pfc->Weight.a;
	_mwd = pfc->DeltW.a;
	_mb = pfc->Bias.a;
	_mbd = pfc->DeltB.a;


	switch (pBPNN->Train.Flag.Optim)
	{
	case OSlwToolNNOptim_GradDesc:
		k = _ParaDiv(_ParaMpy(pBPNN->_nl_factor, pNNSLB->nl), _ParaFint(all_batch_count));
		//单纯梯度下降法
		for (i = 0; i < pfc->Bias.length; i++)
		{
			*_mw = _ParaAdd(*_mw, _ParaMpy(k, *_mwd));
			_mw++;
			_mwd++;

			*_mb = _ParaAdd(*_mb, _ParaMpy(k, *_mbd));
			_mb++;
			_mbd++;

		}

		//少循环一点是一点
		i = pfc->Weight.length - pfc->Bias.length;
		while (i--)
		{
			*_mw = _ParaAdd(*_mw, _ParaMpy(k, *_mwd));
			_mw++;
			_mwd++;
		}


		break;

	case OSlwToolNNOptim_M:
		_vw = pfc->DeltW.a + pfc->DeltW.length;
		_vb = pfc->DeltB.a + pfc->DeltB.length;
		b1 = pBPNN->Train.Beta1T;
		nb1 = _ParaFrom(1) - b1;

		k = pBPNN->_nl_factor* pNNSLB->nl;
		_div_m = _ParaFrom(1) / _ParaFint(all_batch_count);

		//动量法
		for (i = 0; i < pfc->Bias.length; i++)
		{
			//计算出的梯度要先除以batch
			wbuf = *_mwd * _div_m;
			bbuf = *_mbd * _div_m;

			//动量法 v=0.9v+0.1d
			*_vw = *_vw * b1 + wbuf *nb1;
			*_vb = *_vb * b1 + bbuf *nb1;

			//w=w+v
			*_mw = _ParaAdd(*_mw, _ParaMpy(k, *_vw));
			_mw++;
			_mwd++;
			_vw++;

			*_mb = _ParaAdd(*_mb, _ParaMpy(k, *_vb));
			_mb++;
			_mbd++;
			_vb++;

		}

		i = pfc->Weight.length - pfc->Bias.length;
		while (i--)
		{
			wbuf = *_mwd * _div_m;
			*_vw = *_vw * b1 + wbuf *nb1;
			*_mw = _ParaAdd(*_mw, _ParaMpy(k, *_vw));
			_mw++;
			_mwd++;
			_vw++;
		}


		break;

	case OSlwToolNNOptim_RMSp:
		_uw = pfc->DeltW.a + pfc->DeltW.length;
		_ub = pfc->DeltB.a + pfc->DeltB.length;
		b2 = pBPNN->Train.Beta2T;
		nb2 = _ParaFrom(1) - b2;
		e = pBPNN->Train.Epsi;

		k = pBPNN->_nl_factor* pNNSLB->nl;
		_div_m = _ParaFrom(1) / _ParaFint(all_batch_count);

		//RMS
		for (i = 0; i < pfc->Bias.length; i++)
		{
			//计算出的梯度要先除以batch
			wbuf = *_mwd * _div_m;
			bbuf = *_mbd * _div_m;

			//RMS法u=0.9u+0.1d*d
			*_uw = *_uw * b2 + wbuf * wbuf * nb2;
			*_ub = *_ub * b2 + bbuf * bbuf * nb2;

			*_mw = _ParaAdd(*_mw, _ParaMpy(k, wbuf / (e + _ParaSqrt(*_uw))));
			_mw++;
			_mwd++;
			_uw++;

			*_mb = _ParaAdd(*_mb, _ParaMpy(k, bbuf / (e + _ParaSqrt(*_ub))));
			_mb++;
			_mbd++;
			_ub++;

		}

		//少循环一点是一点
		i = pfc->Weight.length - pfc->Bias.length;
		while (i--)
		{
			wbuf = *_mwd * _div_m;
			*_uw = *_uw * b2 + wbuf * wbuf * nb2;

			*_mw = _ParaAdd(*_mw, _ParaMpy(k, wbuf / (e + _ParaSqrt(*_uw))));
			_mw++;
			_mwd++;
			_uw++;
		}

		break;

	case OSlwToolNNOptim_Adam:
		_vw = pfc->DeltW.a + pfc->DeltW.length;
		_vb = pfc->DeltB.a + pfc->DeltB.length;
		b1 = pBPNN->Train.Beta1;
		nb1 = _ParaFrom(1) - b1;

		_uw = pfc->DeltW.a + (pfc->Weight.length<<1);
		_ub = pfc->DeltB.a + (pfc->DeltB.length<<1);
		b2 = pBPNN->Train.Beta2;
		nb2 = _ParaFrom(1) - b2;
		e = pBPNN->Train.Epsi;

		b1t = _ParaFrom(1)- pBPNN->Train.Beta1T;
		b2t = _ParaFrom(1)- pBPNN->Train.Beta2T;

		k = pBPNN->_nl_factor* pNNSLB->nl;
		_div_m = _ParaFrom(1) / _ParaFint(all_batch_count);

		pBPNN->Train.Beta1T *= b1;
		pBPNN->Train.Beta2T *= b2;

		//adam
		for (i = 0; i < pfc->Bias.length; i++)
		{
			//计算出的梯度要先除以batch
			wbuf = *_mwd * _div_m;
			bbuf = *_mbd * _div_m;

			*_vw = *_vw * b1 + wbuf *nb1;
			*_vb = *_vb * b1 + bbuf *nb1;

			*_uw = *_uw * b2 + wbuf * wbuf * nb2;
			*_ub = *_ub * b2 + bbuf * bbuf * nb2;

			vwbuf = *_vw / b1t;
			uwbuf = *_uw / b2t;
			vbbuf = *_vb / b1t;
			ubbuf = *_ub / b2t;

			*_mw = _ParaAdd(*_mw, _ParaMpy(k, vwbuf / (e + _ParaSqrt(uwbuf))));
			_mw++;
			_mwd++;
			_uw++;
			_vw++;

			*_mb = _ParaAdd(*_mb, _ParaMpy(k, vbbuf / (e + _ParaSqrt(ubbuf))));
			_mb++;
			_mbd++;
			_ub++;
			_vb++;

		}

		//少循环一点是一点
		i = pfc->Weight.length - pfc->Bias.length;
		while (i--)
		{

			wbuf = *_mwd * _div_m;

			*_vw = *_vw * b1 + wbuf *nb1;
			*_uw = *_uw * b2 + wbuf* wbuf *nb2;

			vwbuf = *_vw / b1t;
			uwbuf = *_uw / b2t;

			*_mw = _ParaAdd(*_mw, _ParaMpy(k, vwbuf / (e + _ParaSqrt(uwbuf))));
			_mw++;
			_mwd++;
			_uw++;
			_vw++;
		}
		break;

	case OSlwToolNNOptim_Nadam:
		OSLW_assert(1);
		break;

	default:
		OSLW_assert(1);
		break;
	}


	return 0;

}



lw_ptr OSlwToolBPNNLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward)
{
	register OSlwToolNNLayerFullConSTU *pfc;

	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	//前向传播内存空间 D+W
	if (pmemForward)
	{
		OSlwToolMatrixInitial(&(pfc->Weight), pfc->Weight.row, pfc->Weight.col, pmemForward);
		OSlwToolMatrixInitial(&(pfc->Bias), pfc->Bias.row, pfc->Bias.col, (ParaType *)(pmemForward)+pfc->Weight.length);
	}
	//后向传播内存空间 D*n+W*n n为多余空间
	if (pmemBackward)
	{
		OSlwToolMatrixInitial(&(pfc->DeltW), pfc->DeltW.row, pfc->DeltW.col, pmemBackward);
		OSlwToolMatrixInitial(&(pfc->DeltB), pfc->DeltB.row, pfc->DeltB.col, (ParaType *)(pmemBackward)+(pfc->DeltW.length) * (pNNSLB->pNN->Train._MemAllocCoff));
	}


	return 0;

}

lw_ptr OSlwToolBPNNLayerFullConTrainCompleteCallBack(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	LW_MAT_CLR(&(pfc->DeltW));
	LW_MAT_CLR(&(pfc->DeltB));
	//memset(pfc->DeltW.a, 0, PARA_MEM_CAL(pfc->DeltW.length));
	//memset(pfc->DeltB.a, 0, PARA_MEM_CAL(pfc->DeltB.length));
}



lw_ptr OSlwToolBPNNLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register lw_u32 i;
	register ParaType *w, *b, *dw, *db;
	register OSlwNNinitFunType pfun;
	register void *pr;
	register ParaType d1, d2;
	register OSlwToolNNLayerFullConSTU *pfc;
	register ParaType *pRe;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;


	//变量初值
	b = pfc->Bias.a;
	w = pfc->Weight.a;
	dw = pfc->DeltW.a;
	db = pfc->DeltB.a;
	pfun = pfc->pfun;
	pr = pfc->pr;
	d1 = pfc->initd1;
	d2 = pfc->initd2;
	pRe=pfc->pRecover;
	
	//采用全局学习速率
	if (pNNSLB->nl == _ParaFint(0))
	{
		pNNSLB->nl = pNNSLB->pNN->Train.nl;
	}
	
	if (pfc->_BiasInitFun)
	{
		for (i = 0; i < pfc->Bias.length; i++)
		{
			*b++ = pfc->_BiasInitFun();
		}
	}
	else if (pfc->_BiasInit)
	{
		d1 = *(pfc->_BiasInit);
		for (i = 0; i < pfc->Bias.length; i++)
		{
			*b++ = d1;
		}
	}

	if (pfc->pfun && pfc->pr)
	{


		if(pfc->_BiasInitFun == NULL && pfc->_BiasInit==NULL)
		{
			for (i = 0; i < pfc->Bias.length; i++)
			{
				*b++ = pfun(pr, d1, d2);
			}
		}


		for (i = 0; i < pfc->Bias.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
		{
			*db++ = _ParaFint(0);
		}

		for (i = 0; i < pfc->Weight.length; i++)
		{
			*w++ = pfun(pr, d1, d2);
		}

		for (i = 0; i < pfc->Weight.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
		{
			*dw++ = _ParaFint(0);
		}

	}
	else if(pfc->pRecover)
	{
		for (i = 0; i < pfc->Weight.length; i++)
		{
			*w++ = *pRe++;
		}
		for (i = 0; i < pfc->Bias.length; i++)
		{
			*b++ = *pRe++;
		}
		
		
		for (i = 0; i < pfc->Bias.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
		{
			*db++ = _ParaFint(0);
		}

		for (i = 0; i < pfc->Weight.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
		{
			*dw++ = _ParaFint(0);
		}

		
		
	}


	return 0;

}

lw_ptr OSlwToolBPNNLayerFullConCopy(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2)
{
	OSlwToolNNLayerFullConSTU *pfc1, *pfc2;
	OSLW_assert(!(pNNSLB1));
	OSLW_assert(!(pNNSLB2));
	
	pfc1 = (OSlwToolNNLayerFullConSTU *)pNNSLB1;
	pfc2 = (OSlwToolNNLayerFullConSTU *)pNNSLB2;

	memcpy(pfc1->Weight.a, pfc2->Weight.a, PARA_MEM_CAL(pfc1->Weight.length));
	memcpy(pfc1->Bias.a, pfc2->Bias.a, PARA_MEM_CAL(pfc1->Bias.length));

	return 0;
}


lw_ptr OSlwToolBPNNLayerFullConSoftReplace(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido)
{
	OSlwToolNNLayerFullConSTU *pfc1, *pfc2;
	OSLW_assert(!(pNNSLB1));
	OSLW_assert(!(pNNSLB2));

	pfc1 = (OSlwToolNNLayerFullConSTU *)pNNSLB1;
	pfc2 = (OSlwToolNNLayerFullConSTU *)pNNSLB2;

	pOSlwToolMatrix_RATIO_ADD(
		&(pfc1->Weight), 
		_ParaSub(_ParaFint(1), Raido), &(pfc1->Weight), 
		Raido, &(pfc2->Weight)
	);
	
	pOSlwToolMatrix_RATIO_ADD(
		&(pfc1->Bias),
		_ParaSub(_ParaFint(1), Raido), &(pfc1->Bias),
		Raido, &(pfc2->Bias)
	);

	return 0;


}



OSlwToolNNLayerActFunSTU _OSlwToolNNSigmoid = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(Sigmoid,OSlwToolNNLayerActFunSTU)};
OSlwToolNNLayerActFunSTU *LwSigmoid = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSigmoid;
lw_ptr OSlwToolBPNNLayerSigmoidForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out=_ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMpy(*_in, _ParaFint(-1)))));
		*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPNNLayerSigmoidBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaMpy(
			_ParaMpy(*_in, _ParaSub(_ParaFint(1), *_in))
			, *_out
		);
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

OSlwToolNNLayerActFunSTU _OSlwToolNNTanh = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(Tanh,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwTanh = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNTanh;

lw_ptr OSlwToolBPNNLayerTanhForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	ParaType _exp_x,_exp_nx;
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		_exp_x = _ParaExp(*_in);
		_exp_nx = _ParaExp(-(*_in));
		*_out = _ParaDiv(
			_ParaSub(_exp_x, _exp_nx),
			_ParaAdd(_exp_x, _exp_nx)
		);

		*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPNNLayerTanhBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaSub(
			_ParaFint(1),
			_ParaMpy(*_in, *_in)
		);
		*_in = _ParaMpy(*_in, *_out);

	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


OSlwToolNNLayerActFunSTU _OSlwToolNNReLU = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(ReLU,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwReLU = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNReLU;
lw_ptr OSlwToolBPNNLayerReLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
		*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPNNLayerReLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_in = _ParaMpy(
		((*_in > _ParaInt(0)) ? _ParaInt(1) : _ParaInt(0))
		, *_out
		);

	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

OSlwToolNNLayerActFunSTU _OSlwToolNNReLU6 = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(ReLU6,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwReLU6 = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNReLU6;

lw_ptr OSlwToolBPNNLayerReLU6Forward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
	*_out = (*_in < _ParaInt(6)) ? *_in : _ParaInt(6);
	*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPNNLayerReLU6Backward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register ParaType temp;
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	temp = (*_in > _ParaInt(0)) ? _ParaInt(1) : _ParaInt(0);
	temp = (*_in < _ParaInt(6)) ? _ParaInt(1) : _ParaInt(0);
	*_in = _ParaMpy(
		temp
		, *_out
	);

	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


OSlwToolNNLayerActFunSTU _OSlwToolNNSwish = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(Swish,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwSwish = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSwish;

lw_ptr OSlwToolBPNNLayerSwishForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out= _ParaMpy(*_in,
			_ParaDiv(
				_ParaFint(1),
				_ParaAdd(
					_ParaFint(1),
					_ParaExp(
						_ParaMpy(
							*_in, _ParaFint(-1)
						)
					)
				)
			)
		);
	//*_in = *_out;保留输入
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPNNLayerSwishBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register ParaType temp;
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	temp= _ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMpy(*_in, _ParaFint(-1)))));
	//x*s+s(1-x*s)=x*s+s-x*s^2=s*(x+1-x*s)
	*_in = _ParaMpy(
		temp, 
		_ParaAdd(
			*_in, 
			_ParaSub(
				_ParaFint(1), _ParaMpy(*_in, temp)
			)
		)
	);
	*_in = _ParaMpy(
		*_in
		, *_out
	);
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}

static ParaType _OSlwToolNNSeLU_Default[2] = { _ParaFrom(1.0507009873554804934193349852946),_ParaFrom(1.6732632423543772848170429916717) };
OSlwToolNNLayerActFunSeLUSTU _OSlwToolNNSeLU = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(SeLU,OSlwToolNNLayerActFunSeLUSTU),_OSlwToolNNSeLU_Default,NULL};
OSlwToolNNLayerActFunSTU *LwSeLU = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSeLU;

lw_ptr OSlwToolBPNNLayerSeLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	register ParaType Lamer, alpha;
	register ParaType temp;
	register lw_u32 i, _len;
	register ParaType *_in, *_out;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;
	_len = pNNSLB->out.col*(lw_u32)(mini_b_num);

	pthis = (OSlwToolNNLayerActFunSeLUSTU *)pNNSLB;
	Lamer = pthis->pForward[0];
	alpha = pthis->pForward[1];

	for (i = 0; i < _len; i++)
	{
		if (*_in > _ParaFrom(0))
		{
			temp = *_in;
		}
		else
		{
			temp = _ParaMpy(alpha, _ParaSub(_ParaExp(*_in), _ParaFrom(-1)));
		}

		*_out = _ParaMpy(Lamer, temp);
		*_in = *_out;

		_in++;
		_out++;
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	register ParaType Lamer, alpha;
	register ParaType temp;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;
	pthis = (OSlwToolNNLayerActFunSeLUSTU *)pNNSLB;
	Lamer = pthis->pForward[0];
	alpha = pthis->pForward[1];

	for (i = 0; i < pNNSLB->in.row; i++)
	{
		_in = _inb;
		_out = _outb;
		for (j = 0; j < (mini_b_num & 0xffff); j++)
		{
			if (*_in > _ParaFrom(0))
			{
				temp = *_in;
			}
			else
			{
				temp = _ParaMpy(alpha, _ParaSub(_ParaExp(*_in), _ParaFrom(-1)));
			}

			*_out = _ParaMpy(Lamer, temp);
			*_in = *_out;

			_in++;
			_out++;
		}
		_inb += mmb;
		_outb += mmb;
}
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerSeLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	register ParaType LamerMpyAlpha, Lamder;
	register ParaType temp;
	register lw_u32 i, _len;
	register ParaType *_in, *_out;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;
	_len = pNNSLB->out.col*(lw_u32)(mini_b_num);

	pthis = (OSlwToolNNLayerActFunSeLUSTU *)pNNSLB;
	Lamder = pthis->pForward[0];
	LamerMpyAlpha = _ParaMpy(pthis->pForward[0], pthis->pForward[1]);
	for (i = 0; i < _len; i++)
	{
		if (*_in > _ParaFrom(0))
		{
			temp = Lamder;
		}
		else
		{
			temp = _ParaAdd(*_in, LamerMpyAlpha);
		}

		*_in = _ParaMpy(
			temp
			, *_out
		);

		_in++;
		_out++;

	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	
	register ParaType LamerMpyAlpha, Lamder;
	register ParaType temp;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;
	pthis = (OSlwToolNNLayerActFunSeLUSTU *)pNNSLB;
	Lamder = pthis->pForward[0];
	LamerMpyAlpha = _ParaMpy(pthis->pForward[0], pthis->pForward[1]);
	for (i = 0; i < pNNSLB->in.row; i++)
	{
		_in = _inb;
		_out = _outb;
		for (j = 0; j < (mini_b_num & 0xffff); j++)
		{
			if (*_in > _ParaFrom(0))
			{
				temp = Lamder;
			}
			else
			{
				temp = _ParaAdd(*_in, LamerMpyAlpha);
			}

			*_in = _ParaMpy(
				temp
				, *_out
			);

			_in++;
			_out++;
		}
		_inb += mmb;
		_outb += mmb;
	}

#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	return mini_b_num;

}


OSlwToolNNLayerActFunSTU _OSlwToolNNSoftMax = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(SoftMax,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwSoftMax = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSoftMax;

lw_ptr OSlwToolBPNNLayerSoftMaxForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	register ParaType rmax, rsum;
	register lw_u16 i, j, col;
	register ParaType *_in, *_out, *_inb,*_outb;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	col = pNNSLB->out.col;
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;

	for (i = 0; i < (mini_b_num & 0xffff); i++)
	{

		//查询max
		_in = _inb;
		_out = _outb;

		rmax = *_in;
		for (j = 0; j < col; j++)
		{
			if (_in[j] > rmax)
			{
				rmax = _in[j];
			}
		}
		//计算差值 sum y与数值
		rsum = _ParaFint(0);
		for (j = 0; j < col; j++)
		{
			_out[j] = _ParaExp(_ParaSub(_in[j], rmax));
			rsum = _ParaAdd(rsum, _out[j]);
		}

		//归一化
		for (j = 0; j < col; j++)
		{
			_out[j] = _ParaDiv(_out[j], rsum);
		}

		_inb += col;
		_outb += col;

	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	register ParaType rmax, rsum;
	register lw_u16 i, j, mmb;
	register ParaType *_in, *_out, *_inb, *_outb;
	OSlwToolNNLayerActFunSeLUSTU *pthis;
	OSLW_assert(!(pNNSLB));
	_inb = pNNSLB->in.a;
	_outb = pNNSLB->out.a;
	mmb = pNNSLB->out.col;

	for (i = 0; i < (mini_b_num & 0xffff); i++)
	{

		//查询max
		_in = _inb;
		rmax = *_in;
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			if (*_in > rmax)
			{
				rmax = *_in;
			}
			_in += mmb;
		}

		//计算差值 sum y与数值
		_in = _inb;
		_out = _outb;
		rsum = _ParaFint(0);
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			*_out = _ParaExp(_ParaSub(*_in, rmax));
			rsum = _ParaAdd(rsum, *_out);
			_in += mmb;
			_out += mmb;
		}


		//归一化
		_in = _inb;
		_out = _outb;
		for (j = 0; j < pNNSLB->in.row; j++)
		{
			*_out = _ParaDiv(*_out, rsum);

			_in += mmb;
			_out += mmb;
		}

		_inb++;
		_outb++;

	}
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	return mini_b_num;

}

lw_ptr OSlwToolBPNNLayerSoftMaxBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	lw_u16 move_delt,
	lw_u8 move_method,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNLayerConvSTU *node;
	lw_u32 in_row, out_row;
	lw_u32 out_x, out_y, out_z;
	
	LwMatColType kern_len1, kern_len2, kern_res_len;

	OSLW_assert(!(pmem));

	//计算参数
	in_row = in_x*in_y*in_z;
	out_z = kern_num;
	kern_len1 = kern_x*kern_y*in_z;
	kern_len2 = kern_x*kern_y*kern_num;

	//分配节点内存
	node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerConvSTU));

	memset(node, 0, sizeof(OSlwToolNNLayerConvSTU));

	node->databasic.basic.NN_Kind=OSlwToolNNSubLayerKind_Conv;
	
	//node->DataRes = (ParaType *)(((lw_u8 *)node) + sizeof(OSlwToolNNLayerConvSTU));

	if (move_method == 's')
	{
		out_x = (in_x - kern_x) / (move_delt)+1;//横向移动次数
		out_y = (in_y - kern_y) / (move_delt)+1;//纵向移动次数

	}
	else if (move_method == 'f')
	{
		out_x = (in_x) / (move_delt)+1;//横向移动次数
		out_y = (in_y) / (move_delt)+1;//纵向移动次数

	}
	else
	{
		OSLW_assert(1);
	}

	out_row = out_x*out_y*out_z;

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	OSlwToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}

	
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	OSlwToolMatrixInitial(&(node->databasic.basic.out), max_mini_batch,out_row , pout);
	OSlwToolMatrixInitial(&(node->databasic.basic.in), max_mini_batch, in_row, pin);

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSlwToolMatrixInitial(&(node->databasic.basic.out), out_row, max_mini_batch, pout);
	OSlwToolMatrixInitial(&(node->databasic.basic.in), in_row, max_mini_batch, pin);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	//设置参数
	node->databasic.Bias.row = 1;
	node->databasic.Bias.col = out_z;
	node->databasic.Bias.length = out_z;

	node->databasic.DeltB.row = 1;
	node->databasic.DeltB.col = out_z;
	node->databasic.DeltB.length = out_z;

	node->databasic.Weight.row = kern_len1;//一个卷积核有三个维度
	node->databasic.Weight.col = kern_num;
	node->databasic.Weight.length = node->databasic.Weight.row*node->databasic.Weight.col;

	node->databasic.DeltW.row = kern_len1;;
	node->databasic.DeltW.col = kern_num;
	node->databasic.DeltW.length = node->databasic.Weight.row*node->databasic.Weight.col;

	kern_res_len = (kern_len1 > kern_len2) ? kern_len1:kern_len2;
	node->databasic.basic.FlowData.uData = PARA_MEM_CAL(kern_res_len);
	node->move_delt = move_delt;


	//计算要分配的内存大小
	node->databasic.basic.sizeofdata = PARA_MEM_CAL(node->databasic.Weight.length) + PARA_MEM_CAL(node->databasic.Bias.length);


	//成员函数(只有前向传递与反向传递采用独立函数)
	node->databasic.basic.Forward = OSlwToolBPNNLayerConvForward;
	node->databasic.basic.Backward = OSlwToolBPNNLayerConvBackward;

	node->databasic.basic.Update = OSlwToolBPNNLayerFullConUpdate;
	node->databasic.basic.NNmalloc = OSlwToolBPNNLayerFullConNNmalloc;
	node->databasic.basic.TrainCompleteCB = OSlwToolBPNNLayerFullConTrainCompleteCallBack;
	node->databasic.basic.DataInit = OSlwToolBPNNLayerFullConDataInit;
	node->databasic.basic.Copy = OSlwToolBPNNLayerFullConCopy;
	node->databasic.basic.SoftReplace = OSlwToolBPNNLayerFullConSoftReplace;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = out_z;
		info[3] = out_x*out_y*out_z;
	}

	return (OSlwToolNNSubLayerBasicSTU *)node;

}

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_F


OSlwMat * _OSlwToolNNKernalConvFd
(
	OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *in_m, OSlwToolMatrixSTU *kern_m, OSlwToolMatrixSTU *bias,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x,lw_u16 out_y,lw_u16 out_z,
	lw_u16 move_delt,
	lw_u16 now_min_batch,
	lw_u8 move_method,
	ParaType *buf

)
{
	lw_u32 i, j, jmax, k, kmax;
	lw_u32 m, mmax, n, nmax, o, omax;
	lw_u32 _minb, _minb_mpy_col, _minb_mpy_picl, _minb_mpy_outl, _d;
	ParaType *p_pic_head, *p_out_head;
	ParaType *p_pic_pos, *p_pic_pos_row, *p_out_pos;
	ParaType *p_buf;
	ParaType *p_pic_move_pix, *p_out_move_pix;
	ParaType *p_pic_move_col;
	ParaType *p_pic_move_pic;
	ParaType *p_kern;
	ParaType *p_bias;
	ParaType _sum, _mpy_b;


	OSLW_assert(!(s));
	OSLW_assert(!(in_m));
	OSLW_assert(!(kern_m));
	OSLW_assert(!(bias));
	OSLW_assert(!(buf));

	_minb = in_m->col;
	_minb_mpy_col = _minb*pic_x;
	_minb_mpy_picl = _minb_mpy_col*pic_y;
	_d = move_delt*_minb_mpy_col;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_ROW(P) (P+=_minb_mpy_col)
#define _NEXT_ROW_M(P) (P+=_d)
#define _NEXT_PIC(P) (P+=_minb_mpy_picl)
#define _NEXT_O_PIC(P) (P+=_minb_mpy_outl)


	mmax = pic_z;
	nmax = kern_y;
	omax = kern_x;

	//计算不同模式，循环的次数
	jmax = out_y;
	kmax = out_x;
	move_delt *= _minb;

	//循环所有的min-batch
	for (i = 0; i < now_min_batch; ++i)
	{
		p_pic_head = (in_m->a) + i;
		p_out_head = (s->a) + i;

		_minb_mpy_outl = (jmax)*(kmax);

		p_pic_pos_row = p_pic_head;
		p_out_pos = p_out_head;

		//扫描图片
		for (j = 0; j < jmax; ++j)
		{
			p_pic_pos = p_pic_pos_row;

			for (k = 0; k < kmax; k++)
			{
				//抠图 得到小矩阵
				p_pic_move_pic = p_pic_pos;
				p_buf = buf;
				m = mmax;
				while (m--)
				{
					p_pic_move_col = p_pic_move_pic;
					n = nmax;
					while (n--)
					{
						o = omax;
						p_pic_move_pix = p_pic_move_col;
						while (o--)
						{
							*p_buf++ = *p_pic_move_pix;
							_NEXT_PIX(p_pic_move_pix);
						}//end o
						_NEXT_ROW(p_pic_move_col);
					}//end n
					_NEXT_PIC(p_pic_move_pic);
				}//end m


				 //与每一个卷积核运算 并且加上偏置
				p_kern = kern_m->a;
				p_out_move_pix = p_out_pos;
				//每一个卷积核
				for (m = 0; m < kern_m->row; ++m)
				{

					n = kern_m->col;
					_sum = _ParaFint(0);
					p_buf = buf;

					//相乘相加
					while (n--)
					{
						_mpy_b = _ParaMpy(*p_buf, *p_kern);
						_sum += _mpy_b;
						p_buf++;
						p_kern++;
					}//end while n


					*p_out_move_pix = _sum + bias->a[m];//加上偏置
					_NEXT_O_PIC(p_out_move_pix);
				}//end for m

				_NEXT_PIX(p_out_pos);//输出位移
				p_pic_pos += move_delt;//输入卷积核位移
			}//end k

			 //下一张小图像的行
			_NEXT_ROW_M(p_pic_pos_row);

		}//end j


	}//end i 
#undef _NEXT_PIX(P) 
#undef _NEXT_ROW(P) 
#undef _NEXT_ROW_M(P) 
#undef _NEXT_PIC(P)
#undef _NEXT_O_PIC(P)

	return s;
}


OSlwToolMatrixSTU* _OSlwToolNNKernalConvBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr, OSlwToolMatrixSTU *kern,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 in_x,lw_u16 in_y,lw_u16 in_z,
	lw_u16 now_min_batch,
	lw_u16 move_delt,
	ParaType *buf
)
{
	lw_u32 i, j, jmax;
	lw_u32 _minb, kern_len, _minb_mpy_in_len, _minb_mpy_out_len;

	lw_32 ix, iy, ixmax, ixmin, iymax, iymin;
	lw_32 m, n, mmax, nmax;
	ParaType *p_inerr_head, *p_outerr_head;
	ParaType *p_outerr_pos, *p_outerr_move, *p_inerr_pos, *p_inerr_move;
	ParaType *p_buf, *p_buf_bott;
	ParaType *p_kern,*p_kern_pos;
	ParaType databuf, datasum;

#define _OUTERR_PIX(D,X,Y) (D+(Y*pic_x+X)*_minb)
#define _NEXT_PIX(P) (P+=_minb)
#define _IN_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _OUT_NEXT_PIC(P) (P+=_minb_mpy_out_len)

	if (move_delt!=1)
	{
		//没看懂不是间隔1的情况下的求导方式 所以默认必须是移动1格
		OSLW_assert(1);
	}

	//清空输入矩阵
	LW_MAT_CLR(inerr);


	//变量初始化
	_minb = outerr->col;
	jmax = kern->row;
	p_buf_bott = buf + kern_x*kern_y - 1;
	kern_len = kern_x*kern_y;
	_minb_mpy_in_len = _minb*in_x*in_y;
	_minb_mpy_out_len = _minb*pic_x*pic_y;

	ixmin = 1 - (lw_32)kern_x;
	iymin = 1 - (lw_32)kern_y;

	ixmax = pic_x;
	iymax = pic_y;

	for (i = 0; i < now_min_batch; i++)
	{
		p_outerr_head = (outerr->a) + i;
		p_inerr_head = (inerr->a) + i;
		p_kern_pos = kern->a;
		
		p_outerr_pos = p_outerr_head;

		//先计算一张图像
		for (j = 0; j < jmax; ++j)
		{
			p_inerr_pos = p_inerr_head;
			//循环整张图像
			for (iy = iymin; iy < iymax; ++iy)
			{
				for (ix = ixmin; ix < ixmax; ++ix)
				{
					p_inerr_move = p_inerr_pos;

					//取出一小块
					nmax = kern_y + iy;
					p_buf = p_buf_bott;//指针倒过来取

					for (n= iy; n < nmax; ++n)
					{
#if 0
					
						for (m = ix; m < ix+kern_x; ++m)
						{
							if (n<0 || n>=pic_y || m<0 || m>=pic_x)
							{
								*p_buf-- = _ParaFint(0);
							}
							else
							{
								*p_buf-- = *(_OUTERR_PIX(p_outerr_pos, m, n));
							}
						}
						
						

#else		
						mmax = kern_x;

						if (n<0||n>=pic_y)
						{
							m = mmax;
							/*
 							超出范围 这一行数据全为0
							->  0  0
								0  X  X  X
								   X  X  X							
							*/
							while (m--)
							{
								*p_buf-- = _ParaFint(0);
							}
						}
						else
						{
							m = ix;

							if (m<0)
							{
								/*
								赋值为0
									0  0
								->	0  X  X  X
									X  X  X
								*/
								while (m)
								{
									*p_buf-- = _ParaFint(0);
									++m;
								}
								mmax += (ix);								
							}

							p_outerr_move = _OUTERR_PIX(p_outerr_pos, m, n);

							if (ix+ kern_x > pic_x)
							{
								/*
									X  X  X
								->  X  X  X  0
									      0  0
								先赋值再补零
								*/
								m = (ix + kern_x - pic_x);//最右边像素点数量
								mmax -= m;// 如果这一行数据原本是 000 XXXX 000 那这时mmax应该是4
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
								while (m--)
								{
									*p_buf-- = _ParaFint(0);
								}
							}
							else
							{
								/*
								    0  0
								->  X  X  X
									X  X  X
								直接赋值 不用补零
								*/
								while (mmax--)
								{
									*p_buf-- = *p_outerr_move;
									_NEXT_PIX(p_outerr_move);
								}
							}
						}// end if
#endif
					}//end n
					
					//现在已经取出一小块
					p_kern = p_kern_pos;
					m = in_z;
					while(m--)
					{
						n = kern_len;
						p_buf = buf;
						datasum = _ParaFint(0);
						
						while (n--)
						{
							databuf = _ParaMpy(*p_kern, *p_buf);
							datasum += databuf;
							++p_kern;
							++p_buf;
						}
						*p_inerr_move += datasum;
						_IN_NEXT_PIC(p_inerr_move);

					}

					_NEXT_PIX(p_inerr_pos);
					
				}// end iX
				
			}// end iY

			p_kern_pos += kern->col;
			_OUT_NEXT_PIC(p_outerr_pos);

		}//end j

	}//end i

#undef _OUTERR_PIX(D,X,Y) 
#undef _NEXT_PIX(P) 
#undef _IN_NEXT_PIC(P) 
#undef _OUT_NEXT_PIC(P) 

	return inerr;
}


ParaType _OSlwToolNNKernalConvCalDeltOnce
(
	ParaType *delt, ParaType *inerr, ParaType *outerr1,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 min_batch,
	lw_u16 move_delt
)
{
	lw_u32 i;
	lw_u32 j, k;
	lw_u32 m, n;
	lw_u32 _minb, _minb_mpy_in_x, _minb_mpy_in_x_move_delt, _minb_mpy_move_delt, _minb_mpy_in_len;
	ParaType sum, databuf;

	ParaType *p_in_pic, *p_in_row, *p_in_pix;

	ParaType *p_in_move_row, *p_in_move_pix;

	ParaType *p_out_move;

	ParaType *p_delt;

#define _NEXT_PIX(P) (P+=_minb)
#define _NEXT_I_ROW(P) (P+=_minb_mpy_in_x)
#define _NEXT_I_ROW_D(P) (P+=_minb_mpy_in_x_move_delt)
#define _NEXT_I_PIC(P) (P+=_minb_mpy_in_len)
#define _NEXT_I_PIX_D(P) (P+=_minb_mpy_move_delt)
	//变量初始化
	_minb = min_batch;

	_minb_mpy_in_x = _minb*in_x;
	_minb_mpy_in_x_move_delt = _minb_mpy_in_x*move_delt;
	_minb_mpy_move_delt = min_batch*move_delt;
	_minb_mpy_in_len = min_batch*in_x*in_y;

	p_delt = delt;
	//循环每一张图片
	
	for (i = 0, p_in_pic = inerr; i < in_z; ++i)
	{		
		for (j = 0, p_in_row= p_in_pic; j < kern_y; j++)
		{
			for (k = 0, p_in_pix= p_in_row; k < kern_x; k++)
			{

				//循环小图像并且相乘加
				m = out_y;
				p_in_move_row = p_in_pix;
				p_out_move = outerr1;
				sum = _ParaFint(0);

				while (m--)
				{
					n = out_x;
					p_in_move_pix = p_in_move_row;
					while (n--)
					{
						databuf = _ParaMpy(*p_in_move_pix, *p_out_move);
						sum += databuf;

						_NEXT_PIX(p_out_move);
						_NEXT_PIX(p_in_move_pix);

					}// end n

					_NEXT_I_ROW(p_in_move_row);
				}// end m

				//得到一个数sum
				*p_delt++ = sum;

				_NEXT_I_PIX_D(p_in_pix);
			}//end k
			_NEXT_I_ROW_D(p_in_row);
		}//end j
		_NEXT_I_PIC(p_in_pic);
	}//end i



	sum = _ParaFint(0);
	i = out_x*out_y;
	while (i--)
	{
		sum += *outerr1++;
	}

#undef _NEXT_PIX(P)
#undef _NEXT_I_ROW(P)
#undef _NEXT_I_ROW_D(P)
#undef _NEXT_I_PIC(P) 
#undef _NEXT_I_PIX_D(P)

	return sum;
}

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_F


lw_ptr OSlwToolBPNNLayerConvForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolNNLayerConvSTU *pcv;
	lw_ptr min_b = mini_b_num;
	OSlwMat in, out, we;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pcv = (void *)pfc;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	OSlwMatInit(&in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	OSlwMatInit(&out, pcv->out_x, pcv->out_y, pfc->basic.out.a);
	OSlwMatInit(&we, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->Weight.a);
	we.length*= pcv->conv_kernal_z;

	while (min_b--)
	{

		pOSlwToolMatrixConvFastMultCh(
			&out, &we, &in, &(pfc->Bias),
			pcv->conv_kernal_z, pcv->conv_kernal_num,
			pcv->move_delt, pcv->move_delt,
			1,
			pfc->basic.FlowData.pData
		);

		in.a += pfc->basic.in.col;
		out.a += pfc->basic.out.col;

	}



#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F

	_OSlwToolNNKernalConvFd(
		&(pfc->basic.out),
		&(pfc->basic.in),
		&(pfc->Weight),
		&(pfc->Bias),
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		1,//强制默认为1
		mini_b_num,
		's',//强制默认为非填充
		pcv->databasic.basic.FlowData.pData
	);

#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerConvBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolNNLayerConvSTU *pcv;
	lw_u16 i = 0, j, k;
	ParaType *db, *dw, *dw2, delt_bias, *dbm;
	lw_u32 move_d;
	OSlwMat m_dw, m_in, m_out;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pcv = (void *)pfc;
	move_d = pfc->basic.in.col*pcv->out_x*pcv->out_y;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	i = mini_b_num;
	db = pfc->DeltB.a;

	OSlwMatInit(&m_dw, pcv->conv_kernal_x, pcv->conv_kernal_y, pfc->DeltW.a);
	OSlwMatInit(&m_in, pcv->in_x, pcv->in_y, pfc->basic.in.a);
	OSlwMatInit(&m_out, pcv->out_x, pcv->out_y, pfc->basic.out.a);

	//如果是第一次反向传播 需要覆盖原先的梯度
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		//对于每一个卷积核
		for ( j = 0, m_out.a= pfc->basic.out.a; j < pcv->conv_kernal_num; j++,m_out.a+=m_out.length)
		{
			//计算db
			for ( k = 0, delt_bias=_ParaFint(0), dbm=m_out.a; k < m_out.length; k++, dbm++)
			{
				delt_bias += *dbm;
			}
			*db++ = delt_bias;

			//对于每一个维度
			for ( k = 0, m_in.a= pfc->basic.in.a; k < pcv->conv_kernal_z; k++,m_in.a+=m_in.length)
			{
				pOSlwToolMatrixConv2(
					&m_dw, &m_out, &m_in,
					pcv->move_delt, pcv->move_delt,
					1,
					's',
					0,
					NULL
				);
				m_dw.a += m_dw.length;
			}
		}
		i--;
	}



	while (i--)
	{

		m_dw.a = pfc->DeltW.a;
		db = pfc->DeltB.a;

		for (j = 0, m_out.a = pfc->basic.out.a; j < pcv->conv_kernal_num; j++, m_out.a += m_out.length,db++)
		{

			//计算db
			for (k = 0, delt_bias = _ParaFint(0), dbm = m_out.a; k < m_out.length; k++, dbm++)
			{
				delt_bias += *dbm;
			}
			*db += delt_bias;

			for (k = 0, m_in.a = pfc->basic.in.a; k < pcv->conv_kernal_z; k++, m_in.a += m_in.length)
			{
				pOSlwToolMatrixConv2(
					&m_dw, &m_out, &m_in,
					pcv->move_delt, pcv->move_delt,
					0,
					's',
					0,
					NULL
				);
				m_dw.a += m_dw.length;
			}
		}
	}


	//求输入误差
	i = mini_b_num;
	m_dw.a = pfc->Weight.a;
	m_dw.length *= pcv->conv_kernal_z;
	m_out.a = pfc->basic.out.a;
	m_in.a = pfc->basic.in.a;

	while (i--)
	{
		pOSlwToolMatrixConvFastMultCh(
			&m_in, &m_dw, &m_out, NULL,
			pcv->conv_kernal_num, pcv->conv_kernal_z,
			pcv->move_delt, pcv->move_delt,
			0,
			pfc->basic.FlowData.pData
		);

		m_in.a += m_in.length;
		m_out.a += m_out.length;
	}


#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	//如果是第一次反向传播 需要覆盖原先的梯度
	if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a;
		dw = pfc->DeltW.a;
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//先计算delt
			delt_bias = _OSlwToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//由于是第一次反向传播，要进行覆盖
			k = move_d;
			while (k--)
			{
				*dw++ = *dw2++;
			}
			*db++ = delt_bias;

		}

		i++;
	}

	for (; i < mini_b_num; i++)
	{
		j = pcv->conv_kernal_num;
		db = pfc->DeltB.a + i;
		dw = pfc->DeltW.a + i;
		//循环每一个卷积核
		while (j--)
		{
			dw2 = pcv->databasic.basic.FlowData.pData;

			//先计算delt
			delt_bias = _OSlwToolNNKernalConvCalDeltOnce(
				dw2,
				pfc->basic.in.a,
				pfc->basic.out.a,
				pcv->in_x,
				pcv->in_y,
				pcv->conv_kernal_z,
				pcv->conv_kernal_x,
				pcv->conv_kernal_y,
				pcv->out_x, pcv->out_y,
				pfc->basic.in.col,
				1
			);

			//不能覆盖 相加
			k = move_d;
			while (k--)
			{
				*dw += *dw2++;
				dw++;
			}
			*db += delt_bias;
			db++;
		}
	}

	//求输入误差
	_OSlwToolNNKernalConvBK(
		&(pfc->basic.in),
		&(pfc->basic.out),
		&(pfc->Weight),
		pcv->out_x,
		pcv->out_y,
		pcv->conv_kernal_num,
		pcv->conv_kernal_x,
		pcv->conv_kernal_y,
		pcv->in_x,
		pcv->in_y,
		pcv->conv_kernal_z,
		mini_b_num,
		1,
		pcv->databasic.basic.FlowData.pData
	);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C





	return mini_b_num;
}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPoolNew(
	ParaType *pin,
	ParaType *pout,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	OSlwToolNNPoolingMethodNUM pool_method,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNLayerPoolSTU *node;
	lw_u32 max_data_len;
	lw_u32 out_x, out_y;
	LwMatRowType in_row, out_row;

	out_x = in_x / pool_x;
	out_y = in_y / pool_y;

	in_row = in_x*in_y*in_z;;
	out_row = out_x*out_y*in_z;

	if (pool_method == 'M')
	{
		node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerPoolSTU) + sizeof(lw_ptr)*out_row);
		node->MaxPool = (void *)(((lw_u8 *)(node)) + sizeof(OSlwToolNNLayerPoolSTU));
	}
	else if(pool_method == 'A')
	{
		node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerPoolSTU));
	}
	else
	{
		OSLW_assert(1);
	}
	
	node->basic.NN_Kind=OSlwToolNNSubLayerKind_Pool;

	node->in_x = in_x;
	node->in_y = in_y;
	node->move_x = pool_x;
	node->move_y = pool_y;
	node->out_x = out_x;
	node->out_y = out_y;
	node->pic_z = in_z;
	node->PoolMethod = pool_method;

	//设置输入
	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * in_row));
	}

	OSlwToolMatrixInitial(&(node->basic.in), in_row, max_mini_batch, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * out_row));
	}

	OSlwToolMatrixInitial(&(node->basic.out), out_row, max_mini_batch, pout);


	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolBPNNLayerPoolForward;
	node->basic.Backward = OSlwToolBPNNLayerPoolBackward;

	node->basic.Update = OSlwToolBPNNLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPNNLayerNNmallocDefault;
	node->basic.TrainCompleteCB = OSlwToolBPNNLayerTrainCompleteCallBackDefault;
	node->basic.DataInit = OSlwToolBPNNLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPNNLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPNNLayerSoftReplaceDefault;

	if (info)
	{
		info[0] = out_x;
		info[1] = out_y;
		info[2] = in_z;
		info[3] = out_row;
	}

	return (OSlwToolNNSubLayerBasicSTU *)node;
}





#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

void* _OSlwToolNNMaxPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	ParaType *in_a = in->a, *out_a = out->a,*in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_max, *pMax=NULL;
	lw_32  jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0,in_r=in_c; jx < out_x; jx++,in_r+=in_r_move)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, pMax = in_r, now_max = *in_r; k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						if (*in_p_r > now_max)
						{
							pMax = in_p_r;
							now_max = *in_p_r;
						}
					}
				}
				*data_mem++ = pMax;
				*out_a++ = now_max;
			}
		}
		

		in_a += in_pic_2d_move;
	}
	return (void *)data_mem;
}

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = in->a, *out_a = out->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = in_x*in_y;
	in_p_c_move = in_x;
	in_r_move = move_x;
	in_c_move = in_x*move_y;

	now_min_batch *= in_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < out_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < out_x; jx++, in_r += in_r_move)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, now_sum =_ParaFint(0); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						now_sum += *in_p_r;
					}
				}

				*out_a++ = _ParaMpy(data_div, now_sum);
			}
		}


		in_a += in_pic_2d_move;
	}
	return (void *)out;
}



OSlwToolMatrixSTU* _OSlwToolNNMaxPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	lw_u32 i = now_min_batch*outerr_x*outerr_y*inerr_z;
	ParaType *out_a = outerr->a;

	LW_MAT_CLR(inerr);
	while (i--)
	{
		**data_mem = *out_a++;
		data_mem++;
	}

	return inerr;
}


OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	ParaType *in_a = inerr->a, *out_a = outerr->a, *in_p;
	ParaType *in_c, *in_r, *in_p_c, *in_p_r;
	lw_u32 in_pic_2d_move, in_c_move, in_r_move, in_p_c_move;
	ParaType now_sum, data_div = _ParaDiv(_ParaFint(1), _ParaFint(move_x* move_y));
	lw_32 jx, jy, k, l, m;


	in_pic_2d_move = inerr_x*inerr_y;
	in_p_c_move = inerr_x;
	in_r_move = move_x;
	in_c_move = inerr_x*move_y;

	now_min_batch *= inerr_z;
	while (now_min_batch--)
	{

		//大图像循环
		for (jy = 0, in_c = in_a; jy < outerr_y; jy++, in_c += in_c_move)
		{
			for (jx = 0, in_r = in_c; jx < outerr_x; jx++, in_r += in_r_move, out_a++)
			{

				//小图像循环
				for (k = 0, in_p_c = in_r, now_sum = _ParaMpy(*out_a, data_div); k < move_y; k++, in_p_c += in_p_c_move)
				{
					for (l = 0, in_p_r = in_p_c; l < move_x; l++, in_p_r++)
					{
						*in_p_r = now_sum;
					}
				}
			}
		}


		in_a += in_pic_2d_move;
	}
	


	return inerr;
}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
void* _OSlwToolNNMaxPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType maxdata;
	ParaType *pMaxIndex;

#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	p_in_head = in->a;
	p_out_head = out->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					maxdata = *p_in_move_row;
					pMaxIndex = p_in_move_row;
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							if (maxdata < *p_in_move_pix)
							{
								maxdata = *p_in_move_pix;
								pMaxIndex = p_in_move_pix;
							}

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//更新输出
					*data_mem++ = pMaxIndex;
					*p_out_pix = maxdata;


					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return (void *)data_mem;
}

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;

	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType sumdata, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = in->col;
	_minb_mpy_in_x = _minb * in_x;
	_minb_mpy_in_len = _minb_mpy_in_x * in_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = in->a;
	p_out_head = out->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (in_z--)
		{
			i = out_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = out_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					sumdata = _ParaFint(0);
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{

							sumdata += *p_in_move_pix;

							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}

					//更新输出

					*p_out_pix = _ParaMpy(sumdata, NumData);

					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}

				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return out;
}



OSlwToolMatrixSTU* _OSlwToolNNMaxPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
)
{

	LwMatColType _minb;
	ParaType *p_out_head, *p_out_pix;
	LwMatRowType i;

#define _NEXT_PIX(P) (P+=_minb)

	_minb = inerr->col;

	LW_MAT_CLR(inerr);

	p_out_head = outerr->a;
	while (now_min_batch--)
	{
		p_out_pix = p_out_head;
		i = outerr->row;

		while (i--)
		{
			**data_mem = *p_out_pix;
			++data_mem;
			_NEXT_PIX(p_out_pix);
		}

		p_out_head++;
	}


#undef _NEXT_PIX(P)

	return inerr;
}


OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
)
{
	LwMatColType _minb;
	lw_u16 _minb_mpy_in_x, _minb_mpy_in_len;
	lw_u32 m_x, m_y;
	lw_u32 i, j, m, n;

	ParaType *p_in_head, *p_in_pic, *p_in_row, *p_in_pix;
	ParaType *p_in_move_row, *p_in_move_pix;
	ParaType *p_out_head, *p_out_pix;

	ParaType data, NumData;


#define _NEXT_PIX(P) (P+=_minb)
#define _I_NEXT_ROW(P) (P+=_minb_mpy_in_x)
#define _I_NEXT_PIC(P) (P+=_minb_mpy_in_len)
#define _I_MOVE_COL(P) (P+=m_x)
#define _I_MOVE_ROW(P) (P+=m_y)

	_minb = inerr->col;;
	_minb_mpy_in_x = _minb * inerr_x;
	_minb_mpy_in_len = _minb_mpy_in_x * inerr_y;
	m_x = _minb*move_x;
	m_y = _minb_mpy_in_x*move_y;

	NumData = _ParaDiv(_ParaFint(1), _ParaFint(move_x*move_y));

	p_in_head = inerr->a;
	p_out_head = outerr->a;
	//循环每一个minbatch
	while (now_min_batch--)
	{
		p_in_pic = p_in_head;
		p_out_pix = p_out_head;
		//循环每一张图像
		while (inerr_z--)
		{
			i = outerr_y;
			p_in_row = p_in_pic;
			while (i--)
			{

				j = outerr_x;
				p_in_pix = p_in_row;
				while (j--)
				{

					//循环小图像
					p_in_move_row = p_in_pix;
					m = move_y;
					data = _ParaMpy(*p_out_pix, NumData);//得到平均值
					while (m--)
					{
						p_in_move_pix = p_in_move_row;
						n = move_x;
						while (n--)
						{
							*p_in_move_pix = data;//赋值
							_NEXT_PIX(p_in_move_pix);
						}
						_I_NEXT_ROW(p_in_move_row);
					}
					_NEXT_PIX(p_out_pix);
					_I_MOVE_COL(p_in_pix);
				}
				_I_MOVE_ROW(p_in_row);
			}
			_I_NEXT_PIC(p_in_pic);
		}

		p_in_head++;
		p_out_head++;
	}


#undef _NEXT_PIX(P)
#undef _I_NEXT_ROW(P)
#undef _I_NEXT_PIC(P)
#undef _I_MOVE_COL(P)
#undef _I_MOVE_ROW(P)

	return inerr;
}
#else
#error"NN data frame is unknown"
#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


lw_ptr OSlwToolBPNNLayerPoolForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPoolSTU *pPL;
	OSLW_assert(!(pNNSLB));
	pPL = (OSlwToolNNLayerPoolSTU *)pNNSLB;


	if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Max)
	{
		_OSlwToolNNMaxPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Avg)
	{
		_OSlwToolNNAvgPoolingFD(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		OSLW_assert(1);
	}


	return mini_b_num;
}


lw_ptr OSlwToolBPNNLayerPoolBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPoolSTU *pPL;
	OSLW_assert(!(pNNSLB));
	pPL = (OSlwToolNNLayerPoolSTU *)pNNSLB;


	if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Max)
	{
		_OSlwToolNNMaxPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y,
			pPL->MaxPool
		);
	}
	else if (pPL->PoolMethod == OSlwToolNNPoolingMethod_Avg)
	{
		_OSlwToolNNAvgPoolingBK(
			&(pNNSLB->in),
			&(pNNSLB->out),
			pPL->in_x, pPL->in_y, pPL->pic_z,
			pPL->out_x, pPL->out_y,
			mini_b_num,
			pPL->move_x, pPL->move_y
		);
	}
	else
	{
		OSLW_assert(1);
	}


	return mini_b_num;
}


void OSlwToolBPNNInit(OSlwToolBPNNSTU *pBPNN,lw_u16 max_mini_batch)
{
	OSLW_assert(!(pBPNN));

	memset(pBPNN, 0, sizeof(OSlwToolBPNNSTU));
	OSlwToolDListInitial(&pBPNN->Net, sizeof(OSlwToolNNLayerFullConSTU), NULL);
	pBPNN->_nl_factor = _ParaFrom(1.0);
	pBPNN->Train.mini_batch_max = max_mini_batch;
	pBPNN->Train._MemAllocCoff = 1;
	pBPNN->Train.Epsi = _ParaFrom(1e-8);
}


void OSlwToolBPNNLayerAppend
(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolDListNodeSTU *pnode,
	lw_u32 len,
	void *LayerList
)
{
	lw_u16 i;
	OSlwToolNNSubLayerBasicSTU **ppLIST1;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pnode));
	OSLW_assert(!(len));
	OSLW_assert(!(LayerList));

	pBPNN->Net.AppendFUN(&(pBPNN->Net), pnode);
	pnode->Data.uData = len;
	pnode->Data.pData = (void *)LayerList;

	ppLIST1 = LayerList;
	for (i = 0; i < len; i++)
	{
		ppLIST1[i]->pNN = pBPNN;
	}

	return;
}


void* OSlwToolBPNNFullConAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_col, lw_u16 out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
)
{
	
	OSlwToolNNSubLayerBasicSTU *pnode1, *pnode2;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1, *pln2;
	ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **ppLIST2, **pptail;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));
	
	if (pTemplet)
	{
		
		//只有输入层
		if (pBPNN->Net.NowLen == 1)
		{
			if (pin == NULL)
			{
				pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*pBPNN->Train.mini_batch_max));			
			}
			OSlwToolMatrixInitial(&(pBPNN->x), in_col, pBPNN->Train.mini_batch_max, pin);
		}
		else
		{
			pptail = pBPNN->Net.pTail->Data.pData;
			pin = pptail[0]->out.a;
		}

		pnode1 = OSlwToolNNLayerFullConNew(
			pin, NULL,
			in_col, out_col,
			pBPNN->Train.mini_batch_max,
			pmem
		);

		pnode2 = OSlwToolNNLayerActFunNew(
			pnode1->out.a, pout, 
			out_col, pBPNN->Train.mini_batch_max, 
			pmem, pTemplet, 0
		);  

		pln1 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));
		pln2 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));
		

		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST2 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

		ppLIST1[0] = pnode1;
		ppLIST2[0] = pnode2;

		OSlwToolBPNNLayerAppend(pBPNN, pln1, 1, ppLIST1);
		OSlwToolBPNNLayerAppend(pBPNN, pln2, 1, ppLIST2);

		//每次都要复制输出层
		memcpy(&(pBPNN->y), &(pnode2->out), sizeof(OSlwMat));

		pfc = (OSlwToolNNLayerFullConSTU *)pnode1;
	}
	else//采用不带激活函数的方式
	{

		//只有输入层
		if (pBPNN->Net.NowLen == 1)
		{
			if (pin == NULL)
			{
				pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*pBPNN->Train.mini_batch_max));		
			}
			OSlwToolMatrixInitial(&(pBPNN->x), in_col, pBPNN->Train.mini_batch_max, pin);
		}
		else
		{
			pptail = pBPNN->Net.pTail->Data.pData;
			pin = pptail[0]->out.a;
		}

		pnode1 = OSlwToolNNLayerFullConNew(
			pin, pout,
			in_col, out_col,
			pBPNN->Train.mini_batch_max,
			pmem
		);

		pln1 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));


		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST1[0] = pnode1;

		OSlwToolBPNNLayerAppend(pBPNN, pln1, 1, ppLIST1);
		
		//每次都要复制输出层
		memcpy(&(pBPNN->y), &(pnode1->out), sizeof(OSlwMat));

		pfc = (OSlwToolNNLayerFullConSTU *)pnode1;
	}


	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;




	//碎片化存储 直接进行内存分配
	if (pBPNN->Train.Flag.MemoryMethod== OSlwToolNNMemoryMethod_Chip)
	{
		if (pWe==NULL)
		{
			pWreal = pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Weight.length));
		}
		else
		{
			pWreal = pWe;
		}

		if (pBi==NULL)
		{
			pBreal = pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Bias.length));
		}
		else
		{
			pBreal = pBi;
		}

		pfc->Weight.a = pWreal;
		pfc->Bias.a = pBreal;
		if (pBPNN->Train.Flag.NeedTrain==OSlwToolNNNeedTrain_Need)
		{
			pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (lw_u32)(pBPNN->Train._MemAllocCoff)));
		}
		

	}


	pBPNN->ParaGroupNum++;
	
	return pnode1;
}


void* OSlwToolBPNNConvAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1, *pnode2;
	OSlwToolNNLayerConvSTU *pcv;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1, *pln2;
	ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **ppLIST2, **pptail;
	LwMatRowType mat_in_col, mat_out_col;
	lw_u32 out_x, out_y;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));

	mat_in_col = in_x*in_y*in_z;
	out_x = (in_x - kern_x) + 1;//横向移动次数
	out_y = (in_y - kern_y) + 1;//纵向移动次数
	mat_out_col = out_x*out_y*kern_num;

	if (pTemplet)
	{

		//只有输入层
		if (pBPNN->Net.NowLen == 1)
		{
			if (pin == NULL)
			{
				pin = pmem->Malloc(pmem, PARA_MEM_CAL(mat_in_col*pBPNN->Train.mini_batch_max));
			}
			OSlwToolMatrixInitial(&(pBPNN->x), mat_in_col, pBPNN->Train.mini_batch_max, pin);
		}
		else
		{
			pptail = pBPNN->Net.pTail->Data.pData;
			pin = pptail[0]->out.a;
		}

		pnode1 = OSlwToolNNLayerConvNew(
			pin,
			NULL,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			's',
			pBPNN->Train.mini_batch_max,
			pmem,
			info
		);

		pnode2 = OSlwToolNNLayerActFunNew(
			pnode1->out.a, pout,
			mat_out_col, pBPNN->Train.mini_batch_max,
			pmem, pTemplet, 0
		);

		pln1 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));
		pln2 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));


		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST2 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

		ppLIST1[0] = pnode1;
		ppLIST2[0] = pnode2;

		OSlwToolBPNNLayerAppend(pBPNN, pln1, 1, ppLIST1);
		OSlwToolBPNNLayerAppend(pBPNN, pln2, 1, ppLIST2);

		//每次都要复制输出层
		memcpy(&(pBPNN->y), &(pnode2->out), sizeof(OSlwMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}
	else//采用不带激活函数的方式
	{

		//只有输入层
		if (pBPNN->Net.NowLen == 1)
		{
			if (pin == NULL)
			{
				pin = pmem->Malloc(pmem, PARA_MEM_CAL(mat_in_col*pBPNN->Train.mini_batch_max));
			}
			OSlwToolMatrixInitial(&(pBPNN->x), mat_in_col, pBPNN->Train.mini_batch_max, pin);
		}
		else
		{
			pptail = pBPNN->Net.pTail->Data.pData;
			pin = pptail[0]->out.a;
		}

		pnode1 = OSlwToolNNLayerConvNew(
			pin,
			pout,
			in_x, in_y, in_z,
			kern_x, kern_y, kern_num,
			1,
			's',
			pBPNN->Train.mini_batch_max,
			pmem,
			info
		);

		pln1 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));


		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST1[0] = pnode1;

		OSlwToolBPNNLayerAppend(pBPNN, pln1, 1, ppLIST1);

		//每次都要复制输出层
		memcpy(&(pBPNN->y), &(pnode1->out), sizeof(OSlwMat));

		pcv = (void *)pnode1;
		pfc = (void *)pnode1;
	}


	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	pcv->in_x = in_x;
	pcv->in_y = in_y;
	pcv->out_x = out_x;
	pcv->out_y = out_y;

	pcv->conv_kernal_x = kern_x;
	pcv->conv_kernal_y = kern_y;
	pcv->conv_kernal_z = in_z;
	pcv->conv_kernal_num = kern_num;


	//碎片化存储 直接进行内存分配
	if (pBPNN->Train.Flag.MemoryMethod == OSlwToolNNMemoryMethod_Chip)
	{
		if (pWe == NULL)
		{
			pWreal = pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Weight.length));
		}
		else
		{
			pWreal = pWe;
		}

		if (pBi == NULL)
		{
			pBreal = pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Bias.length));
		}
		else
		{
			pBreal = pBi;
		}

		pfc->Weight.a = pWreal;
		pfc->Bias.a = pBreal;
		if (pBPNN->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
		{
			pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (lw_u32)(pBPNN->Train._MemAllocCoff)));
		}
	}


	pBPNN->ParaGroupNum++;

	return pnode1;
}


void* OSlwToolBPNNPoolAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	OSlwToolNNPoolingMethodNUM pool_method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerPoolSTU *pPL;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	LwMatRowType mat_in_col;


	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));

	mat_in_col = in_x*in_y*in_z;


	//只有输入层
	if (pBPNN->Net.NowLen == 1)
	{
		if (pin == NULL)
		{
			pin = pmem->Malloc(pmem, PARA_MEM_CAL(mat_in_col*pBPNN->Train.mini_batch_max));
		}
		OSlwToolMatrixInitial(&(pBPNN->x), mat_in_col, pBPNN->Train.mini_batch_max, pin);
	}
	else
	{
		pptail = pBPNN->Net.pTail->Data.pData;
		pin = pptail[0]->out.a;
	}

	pnode1 = OSlwToolNNLayerPoolNew(
		pin,
		pout,
		in_x, in_y, in_z,
		pool_x, pool_x,
		pool_method,
		pBPNN->Train.mini_batch_max,
		pmem,
		info
	);

	pln1 = pmem->Malloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

	ppLIST1[0] = pnode1;

	OSlwToolBPNNLayerAppend(pBPNN, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPNN->y), &(pnode1->out), sizeof(OSlwMat));

	return pnode1;
}


void OSlwToolBPNNTrainInit(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	LossFunTYPE loss,
	ParaType nl
)
{

	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));


	memcpy(&pBPNN->ref, &pBPNN->y, sizeof(OSlwMat));

	if (pRef==NULL)
	{
		pRef = pmem->Malloc(pmem, PARA_MEM_CAL(pBPNN->ref.length));
	}

	pBPNN->ref.a = pRef;

	pBPNN->Train.LossFun = loss;
	pBPNN->Train.nl = nl;

}

void OSlwToolBPNNOptimInit(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
)
{
	OSLW_assert(!(pBPNN));

	pBPNN->Train.Epsi = _ParaFrom(5e-8);
	pBPNN->Train.Flag.Optim = optim;
	switch (optim)
	{
	case OSlwToolNNOptim_GradDesc:
		pBPNN->Train._MemAllocCoff = 1;
		break;

	case OSlwToolNNOptim_M:
		pBPNN->Train.Beta1 = beta1;
		pBPNN->Train.Beta1T = beta1;
		pBPNN->Train._MemAllocCoff = 2;
		break;

	case OSlwToolNNOptim_RMSp:
		pBPNN->Train.Beta2 = beta2;
		pBPNN->Train.Beta2T = beta2;
		pBPNN->Train._MemAllocCoff = 2;
		break;

	case OSlwToolNNOptim_Adam:
		pBPNN->Train.Beta1 = beta1;
		pBPNN->Train.Beta1T = beta1;
		pBPNN->Train.Beta2 = beta2;
		pBPNN->Train.Beta2T = beta2;
		pBPNN->Train._MemAllocCoff = 3;
		break;

	case OSlwToolNNOptim_Nadam:
		pBPNN->Train.Beta1 = beta1;
		pBPNN->Train.Beta1T = beta1;
		pBPNN->Train.Beta2 = beta2;
		pBPNN->Train.Beta2T = beta2;
		pBPNN->Train._MemAllocCoff = 3;
		break;

	default:
		OSLW_assert(1);
		break;
	}

	return;

}


void OSlwToolBPNNAllDataInit(OSlwToolBPNNSTU *pBPNN,OSlwMemoryBasicSTU *pMem)
{

	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_u32 *pKind;
	register lw_u32 i, count = 0, j;
	lw_u32 flowdatamax = 0;
	void *pFlowDataAddr = NULL;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pMem));

	if (pBPNN->ParaGroupNum==0)
	{
		return;
	}

	pBPNN->pmem = pMem;

	if (pBPNN->Train.Flag.MemoryMethod== OSlwToolNNMemoryMethod_Chip)
	{
		node = (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				ppNNL[i]->DataInit(ppNNL[i]);
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
			node = (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
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

	}
	else
	{


		//block方式统一存储内存
		pKind = pMem->Malloc(pMem, sizeof(lw_u32)*(pBPNN->ParaGroupNum));
	

		//得到table的列
		count = 0;
		node = (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
		while (node)
		{
			ppNNL = node->Data.pData;
			for (i = 0; i < node->Data.uData; i++)
			{
				pKind[count++] = ppNNL[i]->sizeofdata* (lw_u32)(pBPNN->Train._MemAllocCoff);
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
		if (pBPNN->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
		{
			OSlwToolTableInitial(&(pBPNN->Train.DataTable), 2, NULL, pBPNN->ParaGroupNum, pMem, NULL, pKind);//存储两行 分别是原始数值与delt
		}
		else
		{
			OSlwToolTableInitial(&(pBPNN->Train.DataTable), 1, NULL, pBPNN->ParaGroupNum, pMem, NULL, pKind);//存储一行 原始数值
		}

		//分配内存并且初始化
		count = 0;
		node = (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
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

				if (pBPNN->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPNN->Train.DataTable.ReadFun(&pBPNN->Train.DataTable, 0, count, NULL),
						pBPNN->Train.DataTable.ReadFun(&pBPNN->Train.DataTable, 1, count, NULL)
					);

					ppNNL[i]->DataInit(ppNNL[i]);
					
				}
				else
				{
					ppNNL[i]->NNmalloc(
						ppNNL[i],
						pBPNN->Train.DataTable.ReadFun(&pBPNN->Train.DataTable, 0, count, NULL),
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
	OSlwToolBPNNSTU *pBPNN,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	OSlwMemoryBasicSTU *pmem
)
{
	OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));	
	switch((*pSimpleBak).NN_Kind)
	{
		case OSlwToolNNSubLayerKind_FullCon:
			pfc=OSlwToolBPNNFullConAppend(
			pBPNN,
			(*pSimpleBak).info[0], (*pSimpleBak).info[1],
			in, out,
			NULL, NULL,
			NULL, NULL, 0, 0,
			(*pSimpleBak).pTemplet, pmem
		);
		pfc->pRecover=(*pSimpleBak).pDataAddr;
		
		break;

		case OSlwToolNNSubLayerKind_Conv:
			pfc=OSlwToolBPNNConvAppend(
			pBPNN,
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
			OSlwToolBPNNPoolAppend(
			pBPNN,
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


void* OSlwToolBPNNSimpleRecover
(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	lw_u16 simple_layer_len,
	OSlwMemoryBasicSTU *pmem
)
{
	
	lw_u16 i;
	ParaType *pin=in;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(pmem));	
	
		
	
	for(i=0;i<simple_layer_len-1;i++)
	{
		OSlwToolNNLayerSimpleRecover(pBPNN,pin,NULL,pSimpleBak,pmem);
		pin=NULL;
		pSimpleBak++;
	}
	
	OSlwToolNNLayerSimpleRecover(pBPNN,pin,out,pSimpleBak,pmem);
	
	return pBPNN;
	
}


void OSlwToolBPNNForward(OSlwToolBPNNSTU *pBPNN)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_ptr mmb;
	register lw_u32 i;
	OSLW_assert(!(pBPNN));

	mmb = pBPNN->Train.mini_batch_now;
	node = (OSlwToolDListNodeSTU *)pBPNN->Net.Head.con.pNext;
	while (node)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			mmb = ppNNL[i]->Forward(ppNNL[i], mmb);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pNext;
	}

	pBPNN->Train._batch_stream_count++;
	pBPNN->Train.AllBatchCount += pBPNN->Train.mini_batch_now;
	
	return;

}


void OSlwToolBPNNBackward(OSlwToolBPNNSTU *pBPNN)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_ptr mmb;
	register lw_u32 i;
	OSLW_assert(!(pBPNN));

	mmb = pBPNN->Train.mini_batch_now;
	node = (OSlwToolDListNodeSTU *)pBPNN->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			mmb = ppNNL[i]->Backward(ppNNL[i], mmb);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}

	return;
}

void OSlwToolBPNNReview(OSlwToolBPNNSTU *pBPNN)
{
	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	register lw_u32 i;
	OSLW_assert(!(pBPNN));

	node = (OSlwToolDListNodeSTU *)pBPNN->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->Update(ppNNL[i]);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}

	pBPNN->Train.AllBatchCount = 0;
	pBPNN->Train._batch_stream_count = 0;
	pBPNN->Train.mini_batch_now = 0;

	return;

}


void OSlwToolBPNNLoadX(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs)
{
	OSlwMat _x;
	lw_32 res;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(xs));

	if (xs->col != pBPNN->x.row)
	{
		OSLW_assert(1);
		return;
	}

	//计算剩余的批容量
	if (pBPNN->Train.mini_batch_max < (pBPNN->Train.mini_batch_now + xs->row))
	{
		OSLW_assert(1);
		return;
	}

	res = pBPNN->Train.mini_batch_max - pBPNN->Train.mini_batch_now;

	if (res < xs->row)
	{
		xs->row = res;
	}

	//初始化
	OSlwMatInit(&_x, pBPNN->x.row, res, pBPNN->x.a + pBPNN->Train.mini_batch_now);

	pOSlwToolMatrixTurn(&_x, xs);

	//更新批数量
	pBPNN->Train.mini_batch_now += res;
}



void OSlwToolBPNNLoadY(OSlwToolBPNNSTU *pBPNN, OSlwMat *ys)
{
	OSlwMat _y;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(ys));

	if (ys->col != pBPNN->ref.row || pBPNN->Train.mini_batch_now != ys->row )
	{
		OSLW_assert(1);
		return;
	}

	//初始化
	OSlwMatInit(&_y, pBPNN->ref.row, pBPNN->ref.col, pBPNN->ref.a);

	pOSlwToolMatrixTurn(&_y, ys);


}

void OSlwToolBPNNRun(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs)
{

	OSLW_assert(!(pBPNN));

	if (!(xs==NULL))
	{
		OSlwToolBPNNLoadX(pBPNN, xs);
	}
	else
	{
		pBPNN->Train.mini_batch_now = pBPNN->Train.mini_batch_max;
	}
	
	
	OSlwToolBPNNForward(pBPNN);
	
	if (pBPNN->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_NeedNot)
	{
		pBPNN->Train.mini_batch_now = 0;
		pBPNN->Train._batch_stream_count = 0;
		pBPNN->Train.AllBatchCount = 0;
	}

}


void OSlwToolBPNNCalErr(OSlwToolBPNNSTU *pBPNN)
{
	OSlwMat y, ref;

	OSLW_assert(!(pBPNN));


	OSlwMatInit(&y, pBPNN->y.row, pBPNN->Train.mini_batch_now, pBPNN->y.a);
	OSlwMatInit(&ref, pBPNN->ref.row, pBPNN->Train.mini_batch_now, pBPNN->ref.a);

	if (pBPNN->Train.LossFun)
	{
		pBPNN->Error = pBPNN->Train.LossFun(&(y), &(ref), &(y));
	}

	OSlwToolBPNNBackward(pBPNN);

	pBPNN->Train.mini_batch_now = 0;


}

void OSlwToolBPNNTrain(OSlwToolBPNNSTU *pBPNN,OSlwMat *ys)
{

	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_u32 i;
	OSLW_assert(!(pBPNN));

	if (ys!=NULL)
	{
		OSlwToolBPNNLoadY(pBPNN, ys);
	}

	OSlwToolBPNNCalErr(pBPNN);
	OSlwToolBPNNReview(pBPNN);

	node = (OSlwToolDListNodeSTU *)pBPNN->Net.pTail;
	while (node->Key.uData)
	{
		ppNNL = node->Data.pData;
		for (i = 0; i < node->Data.uData; i++)
		{
			ppNNL[i]->TrainCompleteCB(ppNNL[i]);
		}

		node = (OSlwToolDListNodeSTU *)node->con.pLast;
	}



}

void OSlwToolBPNNCopy(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2)
{
	OSlwToolDListNodeSTU *node1, *node2;
	OSlwToolNNSubLayerBasicSTU **ppNNL1, **ppNNL2;
	register lw_u32 i;
	OSLW_assert(!(pBPNN1));
	OSLW_assert(!(pBPNN2));


	node1 = (OSlwToolDListNodeSTU *)pBPNN1->Net.Head.con.pNext;
	node2 = (OSlwToolDListNodeSTU *)pBPNN2->Net.Head.con.pNext;
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


void OSlwToolBPNNSoftReplace(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2,ParaType Tau)
{
	OSlwToolDListNodeSTU *node1, *node2;
	OSlwToolNNSubLayerBasicSTU **ppNNL1, **ppNNL2;
	register lw_u32 i;
	OSLW_assert(!(pBPNN1));
	OSLW_assert(!(pBPNN2));


	node1 = (OSlwToolDListNodeSTU *)pBPNN1->Net.Head.con.pNext;
	node2 = (OSlwToolDListNodeSTU *)pBPNN2->Net.Head.con.pNext;
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

OSlwToolNNSubLayerBasicSTU *OSlwToolBPNNAt(OSlwToolBPNNSTU *pBPNN, lw_32 i, lw_32 j)
{

	OSlwToolDListNodeSTU *pln;
	OSlwToolNNSubLayerBasicSTU **ppNNL;

	i = i < 0 ? i + pBPNN->Net.NowLen : i;

	if (i < 0 )//彻底超出维度
	{
		return NULL;
	}

	pln = pBPNN->Net.IndexOfFUN(&(pBPNN->Net), OSlwToolDListCmpCount, i, NULL);
	
	j = j < 0 ? j + pln->Data.uData : j;

	if (j < 0)//彻底超出维度
	{
		return NULL;
	}

	ppNNL = pln->Data.pData;

	return ppNNL[j];
}



#endif // OSLW_TOOL_IMPORT_NN_BPNN || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

