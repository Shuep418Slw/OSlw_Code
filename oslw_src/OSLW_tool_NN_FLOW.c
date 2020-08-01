/*(Ver.=0.97)
 * OSLW_tool.c
 *
 *  Created on: 2019-03-16
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

void *_OSlwNNLayerSplitOrMix(
	ParaType *src,//
	ParaType *dst,//
	lw_u16 InDimLen,//1 ~ 4 
	LwMatColType *InputDim,//block page row col
	LwMatColType *SplitList,//
	lw_u8 split_1_or_mix_0,
	lw_u8 equal_1_or_add_0
)
{
	register ParaType *p, *q, *p1, *p2, *p3;
	register LwMatLenType i, j, k, l;
	register LwMatLenType block_len, page_len, row_len;
	register LwMatLenType page_init, row_init;

	OSLW_assert(!(src));
	OSLW_assert(!(dst));
	OSLW_assert(!(InputDim));
	OSLW_assert(!(SplitList));

	switch (InDimLen)
	{
	case 1:
		if (split_1_or_mix_0 && equal_1_or_add_0)
		{
			for (i = SplitList[0], p= src+ SplitList[0],q=dst; i < SplitList[1]; ++i)
			{
				*q++ = *p++;
			}
		}
		else if (split_1_or_mix_0 && (!equal_1_or_add_0))
		{
			for (i = SplitList[0], p = src + SplitList[0], q = dst; i < SplitList[1]; ++i)
			{
				*q = _ParaAdd(*q, *p++);q++;
			}
		}
		else if (!(split_1_or_mix_0) && (equal_1_or_add_0))
		{
			for (i = SplitList[0], p = src + SplitList[0], q = dst; i < SplitList[1]; ++i)
			{
				*p++ = *q++;
			}
		}
		else
		{
			for (i = SplitList[0], p = src + SplitList[0], q = dst; i < SplitList[1]; ++i)
			{
				*p = _ParaAdd(*p, *q++);p++;
			}
		}
		break;

	case 2:
#ifdef for_loop_begin
#undef for_loop_begin
#endif // for_loop_begin

#define for_loop_begin 	row_len = InputDim[1];\
		for (i = SplitList[0], p1 = src + SplitList[0] * row_len, q = dst; i < SplitList[1]; ++i, p1 += row_len)\
		{for (j = SplitList[2], p = p1 + SplitList[2]; j < SplitList[3]; ++j){


#define for_loop_end  }\
		}

		if (split_1_or_mix_0 && equal_1_or_add_0)
		{
			for_loop_begin
				*q++ = *p++;
			for_loop_end
		}
		else if (split_1_or_mix_0 && (!equal_1_or_add_0))
		{
			for_loop_begin
				*q = _ParaAdd(*q, *p++);q++;
			for_loop_end
		}
		else if (!(split_1_or_mix_0) && (equal_1_or_add_0))
		{
			for_loop_begin
				*p++ = *q++;
			for_loop_end
		}
		else
		{
			for_loop_begin
				*p = _ParaAdd(*p, *q++);p++;
			for_loop_end
		}

#undef for_loop_start
#undef for_loop_end

		break;


	case 3:
#ifdef for_loop_begin
#undef for_loop_begin
#endif // for_loop_begin
#define for_loop_begin 		row_len = InputDim[2];\
		page_len = row_len*InputDim[1];\
		row_init=SplitList[2] * row_len;\
		for (i = SplitList[0], p2 = src + SplitList[0] * page_len, q = dst; i < SplitList[1]; ++i, p2 += page_len){\
			for (j = SplitList[2], p1 = p2 + row_init; j < SplitList[3]; ++j, p1 += row_len){\
				for (k = SplitList[4], p = p1 + SplitList[4]; k < SplitList[5]; ++k){


#define for_loop_end  }\
		}\
}\

		if (split_1_or_mix_0 && equal_1_or_add_0)
		{
			for_loop_begin
				*q++ = *p++;
			for_loop_end
		}
		else if (split_1_or_mix_0 && (!equal_1_or_add_0))
		{
			for_loop_begin
				*q = _ParaAdd(*q, *p++);q++;
			for_loop_end
		}
		else if (!(split_1_or_mix_0) && (equal_1_or_add_0))
		{
			for_loop_begin
				*p++ = *q++;
			for_loop_end
		}
		else
		{
			for_loop_begin
				*p = _ParaAdd(*p, *q++);p++;
			for_loop_end
		}


#undef for_loop_start
#undef for_loop_end


		break;
		

	case 4:
#ifdef for_loop_begin
#undef for_loop_begin
#endif // for_loop_begin
#define for_loop_begin 		row_len = InputDim[3];\
		page_len = row_len*InputDim[2];\
		block_len=page_len*InputDim[1];\
		row_init=SplitList[4] * row_len;\
		page_init=SplitList[2] * page_len;\
		for (i = SplitList[0], p3 = src + SplitList[0] * block_len, q = dst; i < SplitList[1]; ++i, p3 += block_len){\
			for (j = SplitList[2], p2 = p3 + page_init; j < SplitList[3]; ++j, p2 += page_len){\
				for (k = SplitList[4], p1 = p2 + row_init; k < SplitList[5]; ++k, p1 += row_len){\
					for (l = SplitList[6], p = p1 + SplitList[6]; l < SplitList[7]; ++l){


#define for_loop_end  }}}}

		if (split_1_or_mix_0 && equal_1_or_add_0)
		{
			//row_len = InputDim[3];
			//	page_len = row_len*InputDim[2];
			//	block_len = page_len*InputDim[1];
			//	row_init = SplitList[4] * row_len;
			//	page_init = SplitList[2] * page_len;
			//	for (i = SplitList[0], p3 = src + SplitList[0] * block_len, q = dst; i < SplitList[1]; ++i, p3 += block_len) {
			//		
			//			for (j = SplitList[2], p2 = p3 + page_init; j < SplitList[3]; ++j, p2 += page_len) {
			//				
			//					for (k = SplitList[4], p1 = p2 + row_init; k < SplitList[5]; ++k, p1 += row_len) {
			//						
			//							for (l = SplitList[6], p = p1 + SplitList[6]; l < SplitList[7]; ++l) {
			for_loop_begin
				*q++ = *p++;
			for_loop_end
		}
		else if (split_1_or_mix_0 && (!equal_1_or_add_0))
		{
			for_loop_begin
				*q = _ParaAdd(*q, *p++);q++;
			for_loop_end
		}
		else if (!(split_1_or_mix_0) && (equal_1_or_add_0))
		{
			for_loop_begin
				*p++ = *q++;
			for_loop_end
		}
		else
		{
			for_loop_begin
				*p = _ParaAdd(*p, *q++);p++;
			for_loop_end
		}

#undef for_loop_start
#undef for_loop_end


		break;

	default:
		OSLW_assert(1);
		break;
	}

	return q;
}



  void *_OSlwNNLayerPad_1D_Constant_Forword(ParaType *in,ParaType *out,LwMatColType in_col,LwMatColType ext[2])
{
	register LwMatColType i;
	i = ext[0];
	LW_WHILE_MEMSET(out, i, _ParaFint(0));
	i = in_col;
	LW_WHILE_MEMCPY(out, in, i);
	i = ext[1];
	LW_WHILE_MEMSET(out, i, _ParaFint(0));

	return out;
}

  void *_OSlwNNLayerPad_1D_Constant_Backword(ParaType *in, ParaType *out, LwMatColType in_col, LwMatColType ext[2])
{
	register LwMatColType i;
	i = in_col;
	out += ext[0];
	LW_WHILE_MEMCPY(in, out, i);
	return out + ext[1];
}


  void *_OSlwNNLayerPad_2D_Constant_Forword(ParaType *in, ParaType *out, LwMatColType in_col[2], LwMatColType ext[4])
{
	register LwMatColType i, j;
	//先清空
	i = (in_col[1] + ext[2] + ext[3]) * ext[0];
	LW_WHILE_MEMSET(out, i, _ParaFint(0));

	for ( j = 0; j < in_col[0]; ++j)
	{
		i = ext[2];
		LW_WHILE_MEMSET(out, i, _ParaFint(0));

		i = in_col[1];
		LW_WHILE_MEMCPY(out, in, i);

		i = ext[3];
		LW_WHILE_MEMSET(out, i, _ParaFint(0));
	}

	i = (in_col[1] + ext[2] + ext[3])* ext[1];
	LW_WHILE_MEMSET(out, i, _ParaFint(0));

	return out;
}

  void *_OSlwNNLayerPad_2D_Constant_Backword(ParaType *in, ParaType *out, LwMatColType in_col[2], LwMatColType ext[4])
{
	register LwMatColType i, j, delt1;
	delt1 = ext[2] + ext[3];
	out += (((in_col[1] + delt1) * ext[0]) + ext[2]);
	

	for (i = 0; i < in_col[0]; ++i,out+=delt1)
	{
		j = in_col[1];
		LW_WHILE_MEMCPY(in, out, j);
	}

	return out + (((in_col[1] + delt1) * ext[1]) + ext[3]) - delt1;

}


 void *_OSlwNNLayerExtend_1D_Nearest_Forword(ParaType *in, ParaType *out, LwMatColType in_col, LwMatColType ext)
{
	register LwMatColType i, j;
	ParaType temp;
	for ( i = 0; i < in_col; i++)
	{
		temp = *in++;
		j = ext;
		LW_WHILE_MEMSET(out, j, temp);
	}

	return out;
}


  void *_OSlwNNLayerExtend_1D_Nearest_Backword(ParaType *in, ParaType *out, LwMatColType in_col, LwMatColType ext)
{
	register LwMatColType i, j;
	ParaType temp;
	for (i = 0; i < in_col; i++)
	{
		temp = _ParaFint(0);
		for (j = 0; j < ext; j++)
		{
			temp = _ParaAdd(temp, *out++);
		}
		*in++ = temp;
	}

	return out;
}


  void *_OSlwNNLayerExtend_2D_Nearest_Forword(ParaType *in, ParaType *out, LwMatColType in_col[2], LwMatColType ext[2])
{
	register LwMatColType i, j;

	for (i = 0; i < in_col[0]; ++i, in += in_col[1])
	{
		for (j = 0; j < ext[0]; ++j)
		{
			out = _OSlwNNLayerExtend_1D_Nearest_Forword(in, out, in_col[1], ext[1]);
		}
	}
	return out;
}

  void *_OSlwNNLayerExtend_2D_Nearest_Backword(ParaType *in, ParaType *out, LwMatColType in_col[2], LwMatColType ext[2])
{
	register LwMatColType i, j, k, l;
	register ParaType *_in = in;
	ParaType temp;

	i = in_col[0] * in_col[1];
	LW_WHILE_MEMSET(_in, i, _ParaFint(0));

	_in = in;
	for (i = 0; i < in_col[0]; ++i,in+=in_col[1])
	{
		/*
		->	0 0 0 1 1 1
		->	0 0 0 1 1 1
		->	0 0 0 1 1 1
		*/
		for (j = 0; j < ext[0]; ++j)
		{
			/*
			* .-> *
			0 0 0 1 1 1
			0 0 0 1 1 1
			0 0 0 1 1 1
			*/
			//循环小矩形
			for (k = 0,_in=in; k < in_col[1]; k++)
			{
				temp = *_in;
				/*
				# # # 
				0 0 0 1 1 1
				0 0 0 1 1 1
				0 0 0 1 1 1
				*/
				for ( l = 0; l < ext[1]; l++)
				{
					temp = _ParaAdd(temp, *out++);
				}
				*_in++ = temp;
			}
		}
	}

	return out;
}


static ParaType * _LwSplitOrMixNewSrc(ParaType *p,lw_u16 dim, LwMatColType *ShapeList, OSlwMemoryBasicSTU *pmem)
{
	LwMatColType len = 1, i;
	if (p)
	{
		return p;
	}

	
	for ( i = 0; i < dim; i++)
	{
		len *= ShapeList[i];
	}

	return pmem->Malloc(pmem, PARA_MEM_CAL(len));

}


static void * _LwSplitOrMixNewDst(void *p, lw_u16 dim, lw_u16 SplitNum, LwMatColType *SplitList,lw_u16 max_min_batch, OSlwMemoryBasicSTU *pmem)
{
	LwMatColType j, i, len = 1;
	ParaType **data;
	if (p)
	{
		return p;
	}

	data = pmem->Calloc(pmem, sizeof(ParaType *) * SplitNum);

	for (i = 0; i < SplitNum; i++)
	{
		len = max_min_batch;
		SplitList[0] = 0;
		SplitList[1] = max_min_batch;
		SplitList += 2;
		for ( j = 1; j < dim; j++)
		{
			len *= (SplitList[1] - SplitList[0]);
			SplitList += 2;
		}

		data[i] = (ParaType *)(pmem->Malloc(pmem, PARA_MEM_CAL(len)));
	}

	return data;
}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerSplitNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType **poutList,//输出指针列表 为NULL则分配 
	lw_u16 Dim, // 输入量的维度
	LwMatColType *ShapeList,//输入量的形状
	lw_u16 SplitNum,//要切成的块数量
	LwMatColType *SplitList,//具体的切法
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwMemoryBasicSTU *pmem//内存分配指针
)
{
	OSlwToolNNLayerSplitSTU *node;
	LwMatLenType i;
	LwMatColType _len = 1;

	OSLW_assert(Dim>4);
	OSLW_assert(Dim<2);
	OSLW_assert(!(ShapeList));
	OSLW_assert(!(SplitList));
	OSLW_assert(!(pmem));


	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerSplitSTU));


	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Split;

	ShapeList[0] = max_mini_batch;

	for ( i = 1; i < Dim; i++)
	{
		_len *= ShapeList[i];
	}

	//设置输入
	pin = _LwSplitOrMixNewSrc(pin, Dim, ShapeList, pmem);
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, _len, pin);

	//设置输出

	poutList = _LwSplitOrMixNewDst(poutList, Dim, SplitNum, SplitList, max_mini_batch, pmem);
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch,1, poutList[0]);

	node->Indim = Dim;
	node->OutList = poutList;
	node->OutNum = SplitNum;
	node->SplitList = SplitList;

	for (i = 0; i < Dim; i++)
	{
		node->ShapeList[i] = ShapeList[i];
	}

	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolNNLayerSplitForward;
	node->basic.Backward = OSlwToolNNLayerSplitBackward;

	node->basic.Update = OSlwToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPnnLayerNNmallocDefault;
	node->basic.Clear = OSlwToolBPnnLayerClearDefault;
	node->basic.DataInit = OSlwToolBPnnLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPnnLayerSoftReplaceDefault;



	return (OSlwToolNNSubLayerBasicSTU *)node;

}



lw_ptr OSlwToolNNLayerSplitForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	LwMatColType temp, *split_l, split_delt;
	OSlwToolNNLayerSplitSTU *pspl;
	lw_u16 i;
	ParaType *_in;
	
	OSLW_assert(!(pNNSLB));
	pspl = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	split_delt = (pspl->Indim << 1);
	split_l = pspl->SplitList;

	temp = pspl->ShapeList[0];
	pspl->ShapeList[0] = mini_b_num;
	for (i = 0; i < pspl->OutNum; ++i,split_l+=split_delt)
	{
		split_l[0] = 0;
		split_l[1] = mini_b_num;
		_OSlwNNLayerSplitOrMix(_in, pspl->OutList[i], pspl->Indim, pspl->ShapeList, split_l, 1, 1);
	}


	pspl->ShapeList[0] = temp;

	return mini_b_num;

}


lw_ptr OSlwToolNNLayerSplitBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	LwMatColType temp, *split_l, split_delt;
	OSlwToolNNLayerSplitSTU *pspl;
	lw_u16 i;
	ParaType *_in;

	OSLW_assert(!(pNNSLB));
	pspl = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	split_delt = (pspl->Indim << 1);
	split_l = pspl->SplitList;

	temp = pspl->ShapeList[0];
	pspl->ShapeList[0] = mini_b_num;

	LW_MAT_CLR(&(pNNSLB->in));

	for (i = 0; i < pspl->OutNum; ++i, split_l += split_delt)
	{
		split_l[0] = 0;
		split_l[1] = mini_b_num;
		_OSlwNNLayerSplitOrMix(_in, pspl->OutList[i], pspl->Indim, pspl->ShapeList, split_l, 0, 0);
	}


	pspl->ShapeList[0] = temp;

	return mini_b_num;
}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerMixNew(
	ParaType **pinList,//输入指针列表 为NULL则分配 
	ParaType *pout,//输出指针列表 为NULL则分配 
	lw_u16 Dim, // 输出量的维度
	LwMatColType *ShapeList,//输出量的形状
	lw_u16 MixNum,//块数量
	LwMatColType *MixList,//具体的切法
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwMemoryBasicSTU *pmem//内存分配指针
)
{
	OSlwToolNNLayerMixSTU *node;
	LwMatLenType i;
	LwMatColType _len = 1;
	OSLW_assert(Dim>4);
	OSLW_assert(Dim<2);
	OSLW_assert(!(ShapeList));
	OSLW_assert(!(MixList));
	OSLW_assert(!(pmem));


	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerMixSTU));


	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Mix;

	ShapeList[0] = max_mini_batch;

	//设置输入
	pinList = _LwSplitOrMixNewDst(pinList, Dim, MixNum, MixList, max_mini_batch, pmem);
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, 1, pinList[0]);

	//设置输出

	for ( i = 1; i < Dim; i++)
	{
		_len *= ShapeList[i];
	}

	pout = _LwSplitOrMixNewSrc(pout,Dim,ShapeList,pmem);
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, _len, pout);

	node->Outdim = Dim;
	node->InList = pinList;
	node->InNum = MixNum;
	node->MixList = MixList;

	for (i = 0; i < Dim; i++)
	{
		node->ShapeList[i] = ShapeList[i];
	}

	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolNNLayerMixForward;
	node->basic.Backward = OSlwToolNNLayerMixBackward;

	node->basic.Update = OSlwToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPnnLayerNNmallocDefault;
	node->basic.Clear = OSlwToolBPnnLayerClearDefault;
	node->basic.DataInit = OSlwToolBPnnLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPnnLayerSoftReplaceDefault;



	return (OSlwToolNNSubLayerBasicSTU *)node;

}


lw_ptr OSlwToolNNLayerMixForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	LwMatColType temp, *out_split_l, out_split_delt;
	OSlwToolNNLayerMixSTU *pmix;
	lw_u16 i;
	ParaType *_out;

	OSLW_assert(!(pNNSLB));
	pmix = (void *)pNNSLB;
	_out = pNNSLB->out.a;
	out_split_delt = (pmix->Outdim << 1);
	out_split_l = pmix->MixList;

	//保存minbatch
	temp = pmix->ShapeList[0];
	pmix->ShapeList[0] = mini_b_num;
	//清空输出矩阵
	LW_MAT_CLR(&(pNNSLB->out));

	for (i = 0; i < pmix->InNum; ++i, out_split_l += out_split_delt)
	{
		out_split_l[0] = 0;
		out_split_l[1] = mini_b_num;
		_OSlwNNLayerSplitOrMix(_out,pmix->InList[i], pmix->Outdim, pmix->ShapeList, out_split_l, 0, 0);
	}


	pmix->ShapeList[0] = temp;

	return mini_b_num;

}


lw_ptr OSlwToolNNLayerMixBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	LwMatColType temp, *out_split_l, out_split_delt;
	OSlwToolNNLayerMixSTU *pmix;
	lw_u16 i;
	ParaType *_out;

	OSLW_assert(!(pNNSLB));
	pmix = (void *)pNNSLB;
	_out = pNNSLB->out.a;
	out_split_delt = (pmix->Outdim << 1);
	out_split_l = pmix->MixList;

	//保存minbatch
	temp = pmix->ShapeList[0];
	pmix->ShapeList[0] = mini_b_num;

	for (i = 0; i < pmix->InNum; ++i, out_split_l += out_split_delt)
	{
		out_split_l[0] = 0;
		out_split_l[1] = mini_b_num;
		_OSlwNNLayerSplitOrMix(_out, pmix->InList[i], pmix->Outdim, pmix->ShapeList, out_split_l, 1, 1);
	}


	pmix->ShapeList[0] = temp;

	return mini_b_num;
}




OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPadNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *PadList,//Pad的方法 1维或者2维
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwToolNNPadNUM method,
	OSlwMemoryBasicSTU *pmem,//内存分配指针
	lw_u32 info[4]
)
{
	OSlwToolNNLayerPadSTU *node;
	LwMatLenType i;
	LwMatColType _len = 1;

	OSLW_assert(Dim>4);
	OSLW_assert(Dim<2);
	OSLW_assert(!(ShapeList));
	OSLW_assert(!(PadList));
	OSLW_assert(!(pmem));


	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerPadSTU));


	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Pad;

	ShapeList[0] = max_mini_batch;

	//设置输入
	for ( i = 1; i < Dim; i++)
	{
		_len *= ShapeList[i];
	}

	pin = _LwSplitOrMixNewSrc(pin, Dim, ShapeList, pmem);
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, _len, pin);
	
	for (i = 0; i < Dim; i++)
	{
		node->ShapeList[i] = ShapeList[i];

	}

	if (Dim==2)
	{
		ShapeList[1] += (PadList[0] + PadList[1]);
		node->Padlist[0] = PadList[0];
		node->Padlist[1] = PadList[1];

		_len = ShapeList[1];

	}
	else if (Dim == 3)
	{
		ShapeList[Dim - 2] += (PadList[0] + PadList[1]);
		ShapeList[Dim - 1] += (PadList[2] + PadList[3]);
		node->Padlist[0] = PadList[0];
		node->Padlist[1] = PadList[1];
		node->Padlist[2] = PadList[2];
		node->Padlist[3] = PadList[3];

		_len = ShapeList[1] * ShapeList[2];
	}
	else
	{
		ShapeList[Dim - 2] += (PadList[0] + PadList[1]);
		ShapeList[Dim - 1] += (PadList[2] + PadList[3]);
		node->Padlist[0] = PadList[0];
		node->Padlist[1] = PadList[1];
		node->Padlist[2] = PadList[2];
		node->Padlist[3] = PadList[3];

		_len = ShapeList[1] * ShapeList[2] * ShapeList[3];
	}


	//设置输出
	pout = _LwSplitOrMixNewSrc(pout, Dim, ShapeList, pmem);
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, _len, pout);

	node->IOdim = Dim;
	node->PadMethod = method;


	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolNNLayerPadForward;
	node->basic.Backward = OSlwToolNNLayerPadBackward;

	node->basic.Update = OSlwToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPnnLayerNNmallocDefault;
	node->basic.Clear = OSlwToolBPnnLayerClearDefault;
	node->basic.DataInit = OSlwToolBPnnLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPnnLayerSoftReplaceDefault;


	if (info)
	{
		if (Dim == 2)
		{
			info[0] = ShapeList[1];
			info[1] = ShapeList[1];

		}
		else if (Dim==3)
		{
			info[0] = ShapeList[1];
			info[1] = ShapeList[2];
			info[2] = info[0] * info[1];
		}
		else
		{
			info[0] = ShapeList[2];
			info[1] = ShapeList[3];
			info[2] = ShapeList[1];
			info[3] = info[0] * info[1] * info[2];
		}

	}

	return (OSlwToolNNSubLayerBasicSTU *)node;
}



lw_ptr OSlwToolNNLayerPadForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPadSTU *ppad;
	LwMatLenType i, len, delt_in;
	ParaType *_in, *_out;
	OSLW_assert(!(pNNSLB));

	ppad = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;

	//判断模式
	switch (ppad->PadMethod)
	{
		 
	case OSlwToolNNPad_Constant:
		//判断维度
		switch (ppad->IOdim)
		{
		case 2:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for ( i = 0; i < len; i++)
			{
				_out=_OSlwNNLayerPad_1D_Constant_Forword(_in, _out, ppad->ShapeList[1], ppad->Padlist);
				_in += delt_in;
			}
			break;

		case 3:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out=_OSlwNNLayerPad_2D_Constant_Forword(_in, _out, ppad->ShapeList + 1, ppad->Padlist);
				_in += delt_in;
			}

			break;


		case 4:

			len = mini_b_num*ppad->ShapeList[1];
			delt_in = ppad->ShapeList[2] * ppad->ShapeList[3];

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerPad_2D_Constant_Forword(_in, _out, ppad->ShapeList + 2, ppad->Padlist);
				_in += delt_in;
			}


			break;

		default:
			OSLW_assert(1);
			break;
		}

		break;

	case OSlwToolNNPad_Reflect:
		OSLW_assert(1);
		break;

	default:
		OSLW_assert(1);
		break;
	}
	

	return mini_b_num;
}


lw_ptr OSlwToolNNLayerPadBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerPadSTU *ppad;
	LwMatLenType i, len, delt_in;
	ParaType *_in, *_out;
	OSLW_assert(!(pNNSLB));

	ppad = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;

	//判断模式
	switch (ppad->PadMethod)
	{

	case OSlwToolNNPad_Constant:
		//判断维度
		switch (ppad->IOdim)
		{
		case 2:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerPad_1D_Constant_Backword(_in, _out, ppad->ShapeList[1], ppad->Padlist);
				_in += delt_in;
			}
			break;

		case 3:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerPad_2D_Constant_Backword(_in, _out, ppad->ShapeList + 1, ppad->Padlist);
				_in += delt_in;
			}

			break;


		case 4:

			len = mini_b_num*ppad->ShapeList[1];
			delt_in = ppad->ShapeList[2] * ppad->ShapeList[3];

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerPad_2D_Constant_Backword(_in, _out, ppad->ShapeList + 2, ppad->Padlist);
				_in += delt_in;
			}

			break;

		default:
			OSLW_assert(1);
			break;
		}

		break;

	case OSlwToolNNPad_Reflect:
		OSLW_assert(1);
		break;

	default:
		OSLW_assert(1);
		break;
	}


	return mini_b_num;
}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerExtendNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *ExtList,//扩展的方法 1维或者2维
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwToolNNExtendNUM method,
	OSlwMemoryBasicSTU *pmem,//内存分配指针
	lw_u32 info[4]
)
{
	OSlwToolNNLayerExtendSTU *node;
	LwMatLenType i;
	LwMatColType _len = 1;

	OSLW_assert(Dim>4);
	OSLW_assert(Dim<2);
	OSLW_assert(!(ShapeList));
	OSLW_assert(!(ExtList));
	OSLW_assert(!(pmem));


	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerExtendSTU));


	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Extend;

	ShapeList[0] = max_mini_batch;

	//设置输入
	for (i = 1; i < Dim; i++)
	{
		_len *= ShapeList[i];
	}
	pin = _LwSplitOrMixNewSrc(pin, Dim, ShapeList, pmem);
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, _len, pin);

	for (i = 0; i < Dim; i++)
	{
		node->ShapeList[i] = ShapeList[i];
	}

	if (Dim == 2)
	{
		ShapeList[1] *= ExtList[0];
		node->Extlist[0] = ExtList[0];
		_len = ShapeList[1];
	}
	else if (Dim == 3)
	{
		ShapeList[Dim - 2] *= ExtList[0];
		ShapeList[Dim - 1] *= ExtList[1];
		node->Extlist[0] = ExtList[0];
		node->Extlist[1] = ExtList[1];
		_len = ShapeList[1] * ShapeList[2];
	}
	else
	{
		ShapeList[Dim - 2] *= ExtList[0];
		ShapeList[Dim - 1] *= ExtList[1];
		node->Extlist[0] = ExtList[0];
		node->Extlist[1] = ExtList[1];
		_len = ShapeList[1] * ShapeList[2] * ShapeList[3];
	}

	//设置输出
	pout = _LwSplitOrMixNewSrc(pout, Dim, ShapeList, pmem);
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, _len, pout);

	node->IOdim = Dim;
	node->ExtendMethod = method;


	//成员函数 只用前向传递采用采用函数其他采用默认
	node->basic.Forward = OSlwToolNNLayerExtendForward;
	node->basic.Backward = OSlwToolNNLayerExtendBackward;

	node->basic.Update = OSlwToolBPnnLayerUpdateDefault;
	node->basic.NNmalloc = OSlwToolBPnnLayerNNmallocDefault;
	node->basic.Clear = OSlwToolBPnnLayerClearDefault;
	node->basic.DataInit = OSlwToolBPnnLayerDataInitDefault;
	node->basic.Copy = OSlwToolBPnnLayerCopyDefault;
	node->basic.SoftReplace = OSlwToolBPnnLayerSoftReplaceDefault;


	if (info)
	{
		if (Dim == 2)
		{
			info[0] = ShapeList[1];
			info[1] = ShapeList[1];

		}
		else if (Dim == 3)
		{
			info[0] = ShapeList[1];
			info[1] = ShapeList[2];
			info[2] = info[0] * info[1];
		}
		else
		{
			info[0] = ShapeList[2];
			info[1] = ShapeList[3];
			info[2] = ShapeList[1];
			info[3] = info[0] * info[1] * info[2];
		}

	}

	return (OSlwToolNNSubLayerBasicSTU *)node;
}






lw_ptr OSlwToolNNLayerExtendForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerExtendSTU *pEXT;
	LwMatLenType i, len, delt_in;
	ParaType *_in, *_out;
	OSLW_assert(!(pNNSLB));

	pEXT = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;

	//判断模式
	switch (pEXT->ExtendMethod)
	{

	case OSlwToolNNExtend_Nearest:
		//判断维度
		switch (pEXT->IOdim)
		{
		case 2:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_1D_Nearest_Forword(_in, _out, pEXT->ShapeList[1], pEXT->Extlist[0]);
				_in += delt_in;
			}
			break;

		case 3:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_2D_Nearest_Forword(_in, _out, pEXT->ShapeList+1, pEXT->Extlist);
				_in += delt_in;
			}

			break;


		case 4:

			len = mini_b_num*pEXT->ShapeList[1];
			delt_in = pEXT->ShapeList[2] * pEXT->ShapeList[3];

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_2D_Nearest_Forword(_in, _out, pEXT->ShapeList + 2, pEXT->Extlist);
				_in += delt_in;
			}


			break;

		default:
			OSLW_assert(1);
			break;
		}

		break;

	case OSlwToolNNExtend_Linear:
		OSLW_assert(1);
		break;

	default:
		OSLW_assert(1);
		break;
	}


	return mini_b_num;
}


lw_ptr OSlwToolNNLayerExtendBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	OSlwToolNNLayerExtendSTU *pEXT;
	LwMatLenType i, len, delt_in;
	ParaType *_in, *_out;
	OSLW_assert(!(pNNSLB));

	pEXT = (void *)pNNSLB;
	_in = pNNSLB->in.a;
	_out = pNNSLB->out.a;

	//判断模式
	switch (pEXT->ExtendMethod)
	{

	case OSlwToolNNPad_Constant:
		//判断维度
		switch (pEXT->IOdim)
		{
		case 2:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_1D_Nearest_Backword(_in, _out, pEXT->ShapeList[1], pEXT->Extlist[0]);
				_in += delt_in;
			}
			break;

		case 3:
			len = mini_b_num;
			delt_in = pNNSLB->in.col;

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_2D_Nearest_Backword(_in, _out, pEXT->ShapeList + 1, pEXT->Extlist);
				_in += delt_in;
			}

			break;


		case 4:

			len = mini_b_num*pEXT->ShapeList[1];
			delt_in = pEXT->ShapeList[2] * pEXT->ShapeList[3];

			for (i = 0; i < len; i++)
			{
				_out = _OSlwNNLayerExtend_2D_Nearest_Backword(_in, _out, pEXT->ShapeList + 2, pEXT->Extlist);
				_in += delt_in;
			}


			break;

		default:
			OSLW_assert(1);
			break;
		}

		break;

	case OSlwToolNNPad_Reflect:
		OSLW_assert(1);
		break;

	default:
		OSLW_assert(1);
		break;
	}


	return mini_b_num;
}


void* OSlwToolBPnnPadAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *PadList,//Pad的方法 1维或者2维
	ParaType *pin, ParaType *pout,
	OSlwToolNNPadNUM method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNSubLayerBasicSTU *pnode1;
	//OSlwToolNNLayerPadSTU *pPad;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	LwMatColType in_col = 0;

	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	_NN_GET_IN(pBPnn, pin);


	pnode1 = OSlwToolNNLayerPadNew(pin, pout, Dim, ShapeList, PadList, pBPnn->Train.mini_batch_max, method, pmem, info);

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

	ppLIST1[0] = pnode1;

	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

	return ppLIST1;

}



void* OSlwToolBPnnExtendAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u8 Dim, // IO的维度
	LwMatColType *ShapeList,//输出量的形状
	LwMatColType *ExtList,//Ext的方法
	ParaType *pin, ParaType *pout,
	OSlwToolNNExtendNUM method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
)
{
	OSlwToolNNSubLayerBasicSTU *pnode1;
	//OSlwToolNNLayerPadSTU *pPad;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	LwMatColType in_col = 0;

	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	_NN_GET_IN(pBPnn, pin);


	pnode1 = OSlwToolNNLayerExtendNew(pin, pout, Dim, ShapeList, ExtList, pBPnn->Train.mini_batch_max, method, pmem, info);

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);

	ppLIST1[0] = pnode1;

	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

	return ppLIST1;

}


#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)



