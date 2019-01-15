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


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConAppend(
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


	//设置输入

	if (pin==NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * InCol));
	}

	OSlwToolMatrixInitial(&(node->basic.in), InCol, max_mini_batch, pin);

	//设置输出
	if (pout==NULL)
	{
		pout= pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch * OutCol));
	}

	OSlwToolMatrixInitial(&(node->basic.out), OutCol, max_mini_batch, pout);


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = OutCol;
	node->Bias.length = OutCol;

	node->DeltB.row = 1;
	node->DeltB.col = OutCol;
	node->DeltB.length = OutCol;

	node->Weight.row = OutCol;
	node->Weight.col = InCol;
	node->Weight.length = OutCol*InCol;

	node->DeltW.row = OutCol;
	node->DeltW.col = InCol;
	node->DeltW.length = OutCol*InCol;


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


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunAppend(
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

	//n_mb = pNNSLB->out.col;
	//n_ir = pNNSLB->in.row;
	//n_or = pNNSLB->out.row;

	//mb = pNNSLB->in.col;
	//ir = n_ir; 
	//or = n_or;
	//wcol = pfc->Weight.col;
	//
	//_mic = pNNSLB->in.a;
	//_mbiasb = pfc->Bias.a;
	//_moc = pNNSLB->out.a;


	//for(i=0;i<mini_b_num;i++)
	//{
	//	_mwr = pfc->Weight.a;
	//	_mbias = _mbiasb;
	//	_mout = _moc++;
	//	_ma1 = _mic++;
	//	for ( j = 0; j < n_or; j++)
	//	{
	//		_sum = _ParaFint(0);
	//		_ma2 = _mwr++;
	//		for (k = 0; k < n_ir; k++)
	//		{
	//			_sum = _ParaAdd(_sum , _ParaMpy(*(_ma1), *(_ma2)));
	//			_ma1 += mb;
	//			_ma2 += wcol;
	//		}
	//		_sum = _ParaAdd(_sum, *_mbias);
	//		*_mout = _sum;
	//		_mout += mb;
	//		_mbias++;
	//	}
	//}
	//原始程序 s=x*we+bi
	//现在 out=we*in+b
	//row = pNNSLB->out.row;
	//col = pNNSLB->in.col;
	//row1 = pNNSLB->in.row;
	//_s = pNNSLB->out.a;
	//_xbuf = pfc->Weight.a;
	//_bi = pfc->Bias.a;
	//for (i = 0; i<pNNSLB->out.row; i++)
	//{
	//	for (j = 0; j<pNNSLB->out.col; j++)
	//	{
	//		//_x=x->a+i*x->col;//x的指针归位 x归位于[n,1]
	//		_x = _xbuf;
	//		_we = pNNSLB->in.a + j;//we的指针归位 we归位于[1,n]
	//		sum = _ParaFrom(0);
	//		for (k = 0; k<row1; k++)
	//		{
	//			sum = _ParaAdd(sum, _ParaMpy(*_x, *_we));
	//			_x++;
	//			_we += col;
	//		}//行向量*列向量
	//		*_s++ = _ParaAdd(sum, *_bi);
	//	}
	//	_bi++;
	//	_xbuf += pfc->Weight.col;//x的指针归位 x归位于[n,1]
	//}


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

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	register lw_u16 buf1;
	register lw_u16 i, j, col;
	register ParaType _sum;
	register ParaType *_out, *_db, *_out_b;

	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	//n_mb = pNNSLB->out.col;
	//n_ir = pNNSLB->in.row;
	//n_or = pNNSLB->out.row;
	//mmb = pNNSLB->out.col;
	//
	//_mib = pNNSLB->in.a;
	//_mob = pNNSLB->out.a;
	//_dwb = pfc->DeltW.a;
	//_dbb = pfc->DeltB.a;
	//_mw = pfc->Weight.a;
	////dw(m*n)=x'(m*1)*delta(1*n)
	//_mi = _mib;
	//_dw = _dwb;
	//for (i = 0; i<pfc->Weight.row; i++)
	//{
	//	_mo = _mob;
	//	for (j = 0; j<pfc->Weight.col; j++)
	//	{
	//		
	//		//temp = _ParaMpy(pNNL->in.a[i], pNNL->DeltaUN.delta.a[j]);
	//		//OSLW_TOOL_M_GET(pNNL->w, i, j) = _ParaAdd(OSLW_TOOL_M_GET(pNNL->w, i, j), _ParaMpy(temp, pNNL->nl));//别忘记学习速度
	//		//OSLW_TOOL_M_GET(dw, i, j) = _ParaAdd(OSLW_TOOL_M_GET(dw, i, j), _ParaMpy(temp, nl));//别忘记学习速度
	//		
	//		temp = _ParaMpy(*_mi, *_mo);
	//		*_dw = (_ParaMpy(temp, nl));//别忘记学习速度 这样子不用清空 dw
	//		
	//		_dw++;
	//		_mo+= mmb;
	//	}
	//	_mi+= mmb;
	//}
	////得到bias的导数
	//_db = _dbb;
	//_mo = _mob;
	//for (j = 0; j<pfc->Weight.col; j++)
	//{
	//	*_db = (_ParaMpy(nl, *_mo));//这样子不用清空 db
	//	_db++;
	//	_mo += mmb;
	//}
	//

	////输入误差=delta(1*n)*w'(n*m)
	//_mo = _mob;
	//_mi = _mib;
	//for (i = 0; i<pfc->Weight.row; i++)
	//{
	//	temp = _ParaFint(0);
	//	for (j = 0; j<pfc->Weight.col; j++)
	//	{
	//		temp = _ParaAdd(temp, _ParaMpy(*_mo, *_mw));
	//		_mw++;
	//	}
	//	*_mi = temp;
	//	_mi += mmb;
	//}
	////计算之后的(不用清零)
	//for (k = 1; k < mini_b_num; k++)
	//{
	//	++_mib;
	//	++_mob;
	//	//dw(m*n)=x'(m*1)*delta(1*n)
	//	_mi = _mib;
	//	_dw = _dwb;
	//	for (i = 0; i<pfc->Weight.row; i++)
	//	{
	//		_mo = _mob;
	//		for (j = 0; j<pfc->Weight.col; j++)
	//		{
	//			//temp = _ParaMpy(pNNL->in.a[i], pNNL->DeltaUN.delta.a[j]);
	//			//OSLW_TOOL_M_GET(pNNL->w, i, j) = _ParaAdd(OSLW_TOOL_M_GET(pNNL->w, i, j), _ParaMpy(temp, pNNL->nl));//别忘记学习速度
	//			//OSLW_TOOL_M_GET(dw, i, j) = _ParaAdd(OSLW_TOOL_M_GET(dw, i, j), _ParaMpy(temp, nl));//别忘记学习速度
	//			temp = _ParaMpy(*_mi, *_mo);
	//			*_dw = _ParaAdd(*_dw,_ParaMpy(temp, nl));//别忘记学习速度
	//			_dw++;
	//			_mo += mmb;
	//		}
	//		_mi += mmb;
	//	}
	//	//得到bias的导数
	//	_db = _dbb;
	//	_mo = _mob;
	//	for (j = 0; j<pfc->Weight.col; j++)
	//	{
	//		*_db = _ParaAdd(*_db, _ParaMpy(nl, *_mo));
	//		_db++;
	//		_mo += mmb;
	//	}
	//	//输入误差=delta(1*n)*w'(n*m)
	//	_mo = _mob;
	//	_mi = _mib;
	//	for (i = 0; i<pfc->Weight.row; i++)
	//	{
	//		temp = _ParaFint(0);
	//		for (j = 0; j<pfc->Weight.col; j++)
	//		{
	//			temp = _ParaAdd(temp, _ParaMpy(*_mo, *_mw));
	//			_mw++;
	//		}
	//		*_mi = temp;
	//		_mi += mmb;
	//	}
	//}




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
		for (i = 0; i <	pNNSLB->out.row; i++)
		{
			_sum = _ParaFint(0);
			_out = _out_b;
			for ( j = 0; j < mini_b_num & 0xffff; j++)
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

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register lw_u32 i;
	register ParaType k;
	register ParaType *_mw, *_mwd;
	register ParaType *_mb, *_mbd;
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;


	k = _ParaDiv(_ParaMpy(pNNSLB->pNN->_nl_factor, pNNSLB->nl),_ParaFint(pNNSLB->pNN->Train.AllBatchCount));
	_mw = pfc->Weight.a;
	_mwd = pfc->DeltW.a;
	_mb = pfc->Bias.a;
	_mbd = pfc->DeltB.a;


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
	while(i--)
	{
		*_mw = _ParaAdd(*_mw, _ParaMpy(k, *_mwd));
		_mw++;
		_mwd++;
	}



	return 0;

}



lw_ptr OSlwToolBPNNLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward)
{
	register OSlwToolNNLayerFullConSTU *pfc;

	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	if (pmemForward)
	{
		OSlwToolMatrixInitial(&(pfc->Weight), pfc->Weight.row, pfc->Weight.col, pmemForward);
		OSlwToolMatrixInitial(&(pfc->Bias), pfc->Bias.row, pfc->Bias.col, (ParaType *)(pmemForward)+pfc->Weight.length);
	}
	if (pmemBackward)
	{
		OSlwToolMatrixInitial(&(pfc->DeltW), pfc->DeltW.row, pfc->DeltW.col, pmemBackward);
		OSlwToolMatrixInitial(&(pfc->DeltB), pfc->DeltB.row, pfc->DeltB.col, (ParaType *)(pmemBackward)+pfc->DeltW.length);
	}


	return 0;

}

lw_ptr OSlwToolBPNNLayerFullConTrainCompleteCallBack(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	memset(pfc->DeltW.a, 0, PARA_MEM_CAL(pfc->DeltW.length + pfc->DeltB.length));
}



lw_ptr OSlwToolBPNNLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register lw_u32 i;
	register ParaType *w, *b, *dw, *db;
	register OSlwNNinitFunType pfun;
	register void *pr;
	register ParaType d1, d2;
	register OSlwToolNNLayerFullConSTU *pfc;

	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;


	if (pfc->pfun && pfc->pr)
	{
		w = pfc->Weight.a;
		b = pfc->Bias.a;
		dw = pfc->DeltW.a;
		db = pfc->DeltB.a;
		pfun = pfc->pfun;
		pr = pfc->pr;
		d1 = pfc->initd1;
		d2 = pfc->initd2;

		for (i = 0; i < pfc->Bias.length; i++)
		{
			*w++ = pfun(pr, d1, d2);
			*b++ = pfun(pr, d1, d2);
			*dw++ = _ParaFint(0);
			*db++ = _ParaFint(0);
		}

		i = pfc->Weight.length - pfc->Bias.length;
		while (i--)
		{
			*w++ = pfun(pr, d1, d2);
			*dw++ = _ParaFint(0);
		}
	}

	//采用全局学习速率
	if (pNNSLB->nl == _ParaFint(0))
	{
		pNNSLB->nl = pNNSLB->pNN->Train.nl;
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

	return mini_b_num;

}


lw_ptr OSlwToolBPNNLayerSeLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
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

			*_in= _ParaMpy(
				temp
				, *_out
			);

			_in++;
			_out++;
		}
		_inb += mmb;
		_outb += mmb;
	}

	return mini_b_num;

}


OSlwToolNNLayerActFunSTU _OSlwToolNNSoftMax = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(SoftMax,OSlwToolNNLayerActFunSTU) };
OSlwToolNNLayerActFunSTU *LwSoftMax = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSoftMax;

lw_ptr OSlwToolBPNNLayerSoftMaxForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{


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
		for ( j = 0; j < pNNSLB->in.row; j++)
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
		for ( j = 0; j < pNNSLB->in.row; j++)
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




	return mini_b_num;

}

lw_ptr OSlwToolBPNNLayerSoftMaxBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}


void OSlwToolBPNNInit(OSlwToolBPNNSTU *pBPNN,lw_u16 max_mini_batch)
{
	OSLW_assert(!(pBPNN));

	memset(pBPNN, 0, sizeof(OSlwToolBPNNSTU));
	OSlwToolDListInitial(&pBPNN->Net, sizeof(OSlwToolNNLayerFullConSTU), NULL);
	pBPNN->_nl_factor = _ParaFrom(1.0);
	pBPNN->Train.mini_batch_max = max_mini_batch;

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


void OSlwToolBPNNFullConAppend
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

		pnode1 = OSlwToolNNLayerFullConAppend(
			pin, NULL,
			in_col, out_col,
			pBPNN->Train.mini_batch_max,
			pmem
		);

		pnode2 = OSlwToolNNLayerActFunAppend(
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

		pnode1 = OSlwToolNNLayerFullConAppend(
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
			pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata));
		}
		

	}


	pBPNN->ParaGroupNum++;
	
	return;
}

void OSlwToolBPNNTrainInit(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 all_batch_max,
	LossFunTYPE loss,
	ParaType nl,
	OSlwToolNNTrainUpdateMethodNum updatem
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
	pBPNN->Train.AllBatchMax = all_batch_max;
	pBPNN->Train.Flag.UpdateMethod = updatem;

}


void OSlwToolBPNNAllDataInit(OSlwToolBPNNSTU *pBPNN,OSlwMemoryBasicSTU *pMem)
{

	OSlwToolDListNodeSTU *node;
	OSlwToolNNSubLayerBasicSTU **ppNNL;
	lw_u32 *pKind;
	register lw_u32 i, count = 0, j;
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
			}
			
			node = (OSlwToolDListNodeSTU *)node->con.pNext;
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
				pKind[count++] = ppNNL[i]->sizeofdata;
			}
			node = (OSlwToolDListNodeSTU *)node->con.pNext;
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


void OSlwToolBPNNLoad(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs, OSlwMat *ys)
{
	OSlwMat _x, _y;
	lw_32 res;
	OSLW_assert(!(pBPNN));
	OSLW_assert(!(xs));
	OSLW_assert(!(ys));

	if (xs->row != ys->row || xs->col != pBPNN->x.row || ys->col != pBPNN->ref.row)
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
		ys->row = res;
	}

	//初始化
	OSlwToolMatrixInitial(&_x, pBPNN->x.row, res, pBPNN->x.a + pBPNN->Train.mini_batch_now);
	OSlwToolMatrixInitial(&_y, pBPNN->ref.row, res, pBPNN->ref.a + pBPNN->Train.mini_batch_now);

	pOSlwToolMatrixTurn(&_x, xs);
	pOSlwToolMatrixTurn(&_y, ys);

	//更新批数量
	pBPNN->Train.mini_batch_now += res;
}


void OSlwToolBPNNRun(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs, OSlwMat *ys)
{
	OSlwToolBPNNLoad(pBPNN, xs, ys);
	
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


	OSlwToolMatrixInitial(&y, pBPNN->y.row, pBPNN->Train.mini_batch_now, pBPNN->y.a);
	OSlwToolMatrixInitial(&ref, pBPNN->ref.row, pBPNN->Train.mini_batch_now, pBPNN->ref.a);

	if (pBPNN->Train.LossFun)
	{
		pBPNN->Error = pBPNN->Train.LossFun(&(y), &(ref), &(y));
	}

	OSlwToolBPNNBackward(pBPNN);

	pBPNN->Train.mini_batch_now = 0;


}

void OSlwToolBPNNTrain(OSlwToolBPNNSTU *pBPNN)
{
	OSLW_assert(!(pBPNN));

	OSlwToolBPNNCalErr(pBPNN);
	
	if (pBPNN->Train.Flag.UpdateMethod == OSlwToolNNTrainUpdateMethod_Auto)
	{
		if (pBPNN->Train.AllBatchCount >= pBPNN->Train.AllBatchMax)
		{
			OSlwToolBPNNReview(pBPNN);
		}
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