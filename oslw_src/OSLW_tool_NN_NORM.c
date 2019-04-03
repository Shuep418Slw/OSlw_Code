/*(Ver.=0.95)
 * OSLW_tool.c
 *
 *  Created on: 2019-02-04
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

ParaType _OSlwToolNNLayerLnForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType col
)
{
	register ParaType temp;
	register ParaType _dim;
	OSLW_assert(!(out));
	OSLW_assert(!(in));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));

	_dim = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(var, _ParaFrom(1e-6))));

	while (col--)
	{
		temp = _ParaMpy(_ParaSub(*in++, mean), _dim);
		*out++ = _ParaAdd(*bi++, _ParaMpy(*we++, temp));
	}

	return _dim;
}

/*
-((0.166667 (x1 +
1/3 (-x1 - x2 - x3)) (4/3 (x1 + 1/3 (-x1 - x2 - x3)) -
2/3 (x2 + 1/3 (-x1 - x2 - x3)) -
2/3 (1/3 (-x1 - x2 - x3) + x3)))/(epl +
1/3 ((x1 + 1/3 (-x1 - x2 - x3))^2 + (x2 +
1/3 (-x1 - x2 - x3))^2 + (1/3 (-x1 - x2 - x3) +
x3)^2))^1.5) 


+ 2/(
3 (epl + 1/
3 ((x1 + 1/3 (-x1 - x2 - x3))^2 + (x2 +
1/3 (-x1 - x2 - x3))^2 + (1/3 (-x1 - x2 - x3) + x3)^2))^0.5)


*/
ParaType _OSlwToolNNLayerLnBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType col,
	ParaType *pBuf
)
{
	register ParaType temp, tmp1, tmp2, tmp3, _sum;
	register ParaType _div;
	register lw_u32 i, j;
	register ParaType *pin, *pout, *pw, *p, *q, *p2;
	register ParaType one_div_n, one_plus_one_div_n, one_sub_one_div_n, factor;

	OSLW_assert(!(out));
	OSLW_assert(!(in));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));

	_div = div;
	one_div_n = _ParaDiv(_ParaFint(1), _ParaFrom(col));
	one_plus_one_div_n = _ParaSub(_ParaFint(1), one_div_n);
	one_sub_one_div_n = _ParaMpy(one_div_n, _ParaFrom(-1));
	factor = _ParaMpy(_div, _ParaMpy(_ParaFrom(-1), one_div_n));//-1/n*_div


	//loop1:求dw db （x-m）/(var)^0.5
	if (dwe && dbi)
	{
		for (i = 0, pin = in, pout = out, pw = we; i < col; ++i, ++pout, ++pin, ++dwe, ++dbi,++pw)
		{
			temp = _ParaMpy(_ParaSub(*pin, mean), _div);
			*dwe = _ParaAdd(*dwe, _ParaMpy(temp, *pout));
			*dbi = _ParaAdd(*dbi, *pout);
			*pin = temp;//用xin保存（x-m）/(var)^0.5
			*pout = _ParaMpy(*pout, *pw);
		}
	}
	else
	{
		for (i = 0, pin = in, pout = out, pw = we; i < col; ++i, ++pout, ++pin,++pw)
		{
			temp = _ParaMpy(_ParaSub(*pin, mean), _div);
			*pin = temp;//用xin保存（x-m）/(var)^0.5
			*pout = _ParaMpy(*pout, *pw);
		}
	}

	
	
	
	for ( i = 0,q= pBuf; i < col; ++i,++q)
	{

		//求出sum
		for (j = 0, _sum = _ParaFint(0), p = in; j < col; ++j, ++p)
		{
			tmp2 = (i == j) ? one_plus_one_div_n : one_sub_one_div_n;
			_sum = _ParaAdd(_sum, _ParaMpy(tmp2, *p));
		}

		_sum = _ParaMpy(_sum, factor);
		for (j = 0, tmp3 = _ParaFint(0), p = out, p2= in; j < col; ++j, ++p,++p2)
		{
			tmp1= (i == j) ? one_plus_one_div_n : one_sub_one_div_n;
			tmp2 = _ParaMpy(_div, tmp1);
			tmp3 = 
				_ParaAdd(tmp3,
				_ParaMpy(
					*p,
					_ParaAdd(
						_ParaMpy(_sum, *p2)
						,
						tmp2
					)
				)
				);
		}

		*q = tmp3;//先保存于 out
	}

	while (col--)
	{
		*in++ = *pBuf++;
	}

	return 0;
}



lw_ptr OSlwToolNNLayerLNormForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register LwMatRowType i;
	OSlwToolNNLayerLNormSTU *pln;

	OSLW_assert(!(pNNSLB));

	pln = (void *)pNNSLB;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	pOSlwToolMatrixMoments(&(pNNSLB->in), pln->pMean, pln->pVar,1);
	
	for ( i = 0; i < mini_b_num; i++)
	{
		pln->pVar[i]=_OSlwToolNNLayerLnForwordSub(OSlwMatGetR(pNNSLB->in, i), OSlwMatGetR(pNNSLB->out, i),
			pln->databasic.Weight.a, pln->databasic.Bias.a,
			pln->pMean[i], pln->pVar[i], pNNSLB->in.col
		);
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	return mini_b_num;
}

lw_ptr OSlwToolNNLayerLNormBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i;
	OSlwToolNNLayerLNormSTU *pln;

	OSLW_assert(!(pNNSLB));

	pln = (void *)pNNSLB;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	for (i = 0; i < mini_b_num; i++)
	{
		_OSlwToolNNLayerLnBackwordSub(
			OSlwMatGetR(pNNSLB->in, i), OSlwMatGetR(pNNSLB->out, i),
			pln->databasic.Weight.a, pln->databasic.Bias.a,
			pln->databasic.DeltW.a, pln->databasic.DeltB.a,
			pln->pMean[i], pln->pVar[i],
			pNNSLB->out.col,
			pNNSLB->FlowData.pData
		);
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	return mini_b_num;
}

static ParaType OSlwToolNNLayerLNormWeightInit(void *p, ParaType pmin, ParaType pmax) { return _ParaFrom(1); }
static ParaType OSlwToolNNLayerLNormBiasInit(void) { return _ParaFrom(0); }



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerLNormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType Col,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNLayerFullConSTU *node;
	OSlwToolNNLayerLNormSTU *pln;
	lw_u32 in_len, out_len;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerLNormSTU));
	pln = (void *)node;

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_LNorm;


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = Col;
	node->Bias.length = Col;

	node->DeltB.row = 1;
	node->DeltB.col = Col;
	node->DeltB.length = Col;

	in_len = max_mini_batch * Col;
	out_len = max_mini_batch*Col;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	node->Weight.row = 1;
	node->Weight.col = Col;
	node->Weight.length = node->Weight.row*node->Weight.col;

	node->DeltW.row = 1;
	node->DeltW.col = Col;
	node->DeltW.length = node->Weight.length;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Calloc(pmem, PARA_MEM_CAL(in_len));
	}
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, Col, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Calloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, Col, pout);

	pln->pMean = pmean == NULL ? pmem->Calloc(pmem, PARA_MEM_CAL(max_mini_batch)) : pmean;
	pln->pVar = pvar == NULL ? pmem->Calloc(pmem, PARA_MEM_CAL(max_mini_batch)) : pvar;
	node->basic.FlowData.uData = PARA_MEM_CAL(Col);

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolNNLayerLNormForward;
	node->basic.Backward = OSlwToolNNLayerLNormBackward;
	node->basic.DataInit = OSlwToolBPnnLayerFullConDataInit;

	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Clear = OSlwToolBPnnLayerFullConClear;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}


void* OSlwToolBPnnLNormAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	ParaType *pmean,ParaType *pvar,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	_NN_GET_IN(pBPnn, pin);
	//初始化节点

	pnode1 = OSlwToolNNLayerLNormNew(
		pin, pout,
		pmean, pvar,
		in_col, pBPnn->Train.mini_batch_max,
		pmem
	);
	pfc = (void *)pnode1;
	pfc->_BiasInitFun = OSlwToolNNLayerLNormBiasInit;
	pfc->pfun = OSlwToolNNLayerLNormWeightInit;
	pfc->pr= OSlwToolNNLayerLNormWeightInit;



	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerLNormSTU *) * 1);
	ppLIST1[0] = pnode1;


	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	_NN_FULL_CON_CHIP_ALLAC_1(pBPnn, pfc);

	return ppLIST1;
}




ParaType _OSlwToolNNLayerInForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType x_mpy_y
)
{

	ParaType _dim;
	ParaType k, b;

	OSLW_assert(!(out));
	OSLW_assert(!(in));
	OSLW_assert(!(we));
	OSLW_assert(!(bi));

	_dim = _ParaDiv(_ParaFint(1), _ParaSqrt(_ParaAdd(var, _ParaFrom(1e-6))));
	k = _ParaMpy(*we, _dim);
	b = _ParaSub(*bi, _ParaMpy(k, mean));


	while (x_mpy_y--)
	{
		*out++ = _ParaAdd(b, _ParaMpy(*in++, k));
	}

	return _dim;




}


ParaType _OSlwToolNNLayerInBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType x_mpy_y,
	ParaType *pBuf
)
{

	OSLW_assert(1);

}

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerINormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType in_x,
	LwMatColType in_y,
	LwMatColType in_z,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
)
{
	OSlwToolNNLayerFullConSTU *node;
	OSlwToolNNLayerINormSTU *pIn;
	lw_u32 in_len, out_len;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerINormSTU));
	pIn = (void *)node;

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_INorm;

	pIn->in_x = in_x;
	pIn->in_y = in_y;
	pIn->in_z = in_z;

	//设置参数
	node->Bias.row = 1;
	node->Bias.col = in_z;
	node->Bias.length = in_z;

	node->DeltB.row = 1;
	node->DeltB.col = in_z;
	node->DeltB.length = in_z;

	in_len = max_mini_batch * in_x*in_y*in_z;
	out_len = in_len;

	node->Weight.row = 1;
	node->Weight.col = in_z;
	node->Weight.length = in_z;

	node->DeltW.row = 1;
	node->DeltW.col = in_z;
	node->DeltW.length = node->Weight.length;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_len));
	}
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, in_x*in_y*in_z, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, in_x*in_y*in_z, pout);

	pIn->pMean = pmean == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch*in_z)) : pmean;
	pIn->pVar = pvar == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(max_mini_batch*in_z)) : pvar;
	
	node->basic.FlowData.uData = PARA_MEM_CAL(in_z);

	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolNNLayerINormForward;
	node->basic.Backward = OSlwToolNNLayerINormBackward;
	node->basic.DataInit = OSlwToolBPnnLayerFullConDataInit;

	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Clear = OSlwToolBPnnLayerFullConClear;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}

lw_ptr OSlwToolNNLayerINormForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	register lw_u32 i, j;
	OSlwToolNNLayerINormSTU *pIn;
	OSlwMat in_mat;
	register ParaType *pin, *pout;
	OSLW_assert(!(mini_b_num));
	OSLW_assert(!(pNNSLB));

	pIn = (void *)pNNSLB;
	OSlwMatInit(&in_mat, mini_b_num*pIn->in_z, pIn->in_x*pIn->in_y, pNNSLB->in.a);

	pin = pNNSLB->in.a;
	pout = pNNSLB->out.a;

	pOSlwToolMatrixMoments(&(in_mat), pIn->pMean, pIn->pVar, 1);

	for (i = 0; i < in_mat.row; i++)
	{
		j = i%in_mat.col;

		pIn->pVar[i] = _OSlwToolNNLayerInForwordSub(
			pin, pout,
			pIn->databasic.Weight.a + j, pIn->databasic.Bias.a + j,
			pIn->pMean[i], pIn->pVar[i],
			in_mat.col
		);

		pin += in_mat.col;
		pout += in_mat.col;
	}


	return mini_b_num;
}



lw_ptr OSlwToolNNLayerINormBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{


	OSLW_assert(1);
	return mini_b_num;
}

#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

