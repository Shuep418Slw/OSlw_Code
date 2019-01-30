/*(Ver.=0.93)
 * OSLW_tool.c
 *
 *  Created on: 2019-01-22
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL


void OSlwToolNNLayerRnnCellInit(
	OSlwToolNNLayerRnnCellSTU *pCell,
	lw_u16 in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
	lw_u16 out_size,
	ParaType *pin,//只需要输入 因为隐藏层与输出要输入时候才确定
	ParaType *pwe,ParaType *pbi,
	ParaType *pdwe,ParaType *pdbi,
	OSlwToolNNSubLayerBasicSTU *pActTemplet
	)
{
	
	OSLW_assert(!(pin));
	OSLW_assert(!(pwe));
	OSLW_assert(!(pbi));
	OSLW_assert(!(pdwe));
	OSLW_assert(!(pdbi));
	OSLW_assert(!(pActTemplet));

	OSlwMatInit(&(pCell->in), 1, in_size, pin);
	OSlwMatInit(&(pCell->ActFun.in), 1, out_size, NULL);
	OSlwMatInit(&(pCell->ActFun.out), 1, out_size, NULL);
	OSlwMatInit(&(pCell->we), in_size, out_size, pwe);
	OSlwMatInit(&(pCell->bi), 1, out_size, pbi);
	OSlwMatInit(&(pCell->dwe), in_size, out_size, pdwe);
	OSlwMatInit(&(pCell->dbi), 1, out_size, pdbi);

	pCell->ActFun.Backward = pActTemplet->Backward;
	pCell->ActFun.Forward = pActTemplet->Forward;
}

void OSlwToolNNLayerRnnCellForword(
	OSlwToolNNLayerRnnCellSTU *pCell
)
{
	pOSlwToolMatrixXWeBi(
		&(pCell->ActFun.in),
		&(pCell->we),
		&(pCell->in),
		&(pCell->bi)
	);

	pCell->ActFun.Forward((void *)&(pCell->ActFun), 1);
}

void OSlwToolNNLayerRnnCellBackword(
	OSlwToolNNLayerRnnCellSTU *pCell
)
{
	register lw_u32 i;
	register ParaType *p1, *p2;
	pCell->ActFun.Backward((void *)&(pCell->ActFun), 1);

	//采用叠加方法
	//dw=in'*out
	pOSlwToolMatrixTurnMpy(&(pCell->dwe), &(pCell->in), &(pCell->ActFun.in), 6);
	//直接叠加
	p1 = pCell->dbi.a;
	p2 = pCell->ActFun.in.a;
	i = pCell->ActFun.out.col;
	while (i--)(*p1++) += *p2++;

	//xd = out*w';
	pOSlwToolMatrixTurnMpy(&(pCell->in), &(pCell->ActFun.in), &(pCell->we), 1);
}



lw_ptr OSlwToolNNLayerBasicRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i;
	OSlwToolNNLayerBasicRnnSTU *pBRnn;

	OSLW_assert(!(pNNSLB));

	pBRnn = (void *)pNNSLB;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	
	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		//加载数据
		LW_PARA_JOIN(pBRnn->RnnCell.in.a, pNNSLB->out.a, pNNSLB->out.col, pNNSLB->in.a, pNNSLB->in.col);

		//前向传递
		OSlwToolNNLayerRnnCellForword(&(pBRnn->RnnCell));
	}
	else
	{
		//加载数据
		LW_PARA_JOIN(pBRnn->RnnCell.in.a, NULL, pNNSLB->out.col, pNNSLB->in.a, pNNSLB->in.col);//第一次的时候 ht-1是全0
		pBRnn->RnnCell.ActFun.in.a = pBRnn->h_t_mem.a;
		pBRnn->RnnCell.ActFun.out.a = pNNSLB->out.a;

		//前向传递
		OSlwToolNNLayerRnnCellForword(&(pBRnn->RnnCell));

		//保存数据
		LW_PARA_JOIN(pBRnn->out_t_1_mem.a, pNNSLB->out.a, pNNSLB->out.col, NULL, 0);

		for (i = 1; i < mini_b_num; i++)
		{
			//加载数据
			LW_PARA_JOIN(pBRnn->RnnCell.in.a, OSlwMatGetR(pBRnn->out_t_1_mem, i - 1), pNNSLB->out.col, OSlwMatGetR(pNNSLB->in, i), pNNSLB->in.col);
			pBRnn->RnnCell.ActFun.in.a = OSlwMatGetR(pBRnn->h_t_mem, i);
			pBRnn->RnnCell.ActFun.out.a = OSlwMatGetR(pNNSLB->out, i);

			//前向传递
			OSlwToolNNLayerRnnCellForword(&(pBRnn->RnnCell));

			//保存数据
			LW_PARA_JOIN(OSlwMatGetR(pBRnn->out_t_1_mem, i), OSlwMatGetR(pNNSLB->out, i), pNNSLB->out.col, NULL, 0);
		}
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	return mini_b_num;
}

lw_ptr OSlwToolNNLayerBasicRnnBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i, j;
	register ParaType *din, *dbuf;
	register lw_u16 in_size, out_size;
	OSlwToolNNLayerBasicRnnSTU *pBRnn;

	pBRnn = (void *)pNNSLB;
	in_size = pNNSLB->in.col;
	out_size = pNNSLB->out.col;


#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		OSLW_assert(1);
	}
	else
	{
		for (i = mini_b_num - 1; i >0; --i)
		{
			//载入数据
			LW_PARA_JOIN(pBRnn->RnnCell.in.a, OSlwMatGetR(pBRnn->out_t_1_mem, i-1), out_size, OSlwMatGetR(pNNSLB->in, i), in_size);

			pBRnn->RnnCell.ActFun.in.a = OSlwMatGetR(pBRnn->h_t_mem, i);
			pBRnn->RnnCell.ActFun.out.a = OSlwMatGetR(pNNSLB->out, i);

			//反向传递
			OSlwToolNNLayerRnnCellBackword(&(pBRnn->RnnCell));

			//导出数据
			din = pBRnn->RnnCell.in.a;
			//dht-1加到输出误差上
			for (j = 0, dbuf = OSlwMatGetR(pNNSLB->out, i - 1); j < out_size; ++j, dbuf++, din++)
			{
				*dbuf += *din;
			}
			//dx传到下一层
			for (j = 0, dbuf = OSlwMatGetR(pNNSLB->in, i); j < in_size; ++j, dbuf++, din++)
			{
				*dbuf = *din;
			}

		}


		//载入数据
		LW_PARA_JOIN(pBRnn->RnnCell.in.a, NULL, out_size, pNNSLB->in.a, in_size);

		pBRnn->RnnCell.ActFun.in.a = pBRnn->h_t_mem.a;
		pBRnn->RnnCell.ActFun.out.a = pNNSLB->out.a;

		//反向传递
		OSlwToolNNLayerRnnCellBackword(&(pBRnn->RnnCell));

		//导出数据
		din = pBRnn->RnnCell.in.a + out_size;
		//dx传到下一层
		for (j = 0, dbuf = pNNSLB->in.a; j < in_size; ++j, dbuf++, din++)
		{
			*dbuf = *din;
		}

	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;
}

lw_ptr OSlwToolBPnnLayerBasicRnnDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	OSlwToolNNLayerBasicRnnSTU *pbrnn;

	//先执行全连接初始化
	OSlwToolBPnnLayerFullConDataInit(pNNSLB);

	pbrnn = (void *)pNNSLB;

	OSlwToolNNLayerRnnCellInit(
		&(pbrnn->RnnCell),
		pNNSLB->in.col + pNNSLB->out.col, pNNSLB->out.col,
		pbrnn->RnnCell.in.a,
		pbrnn->databasic.Weight.a, pbrnn->databasic.Bias.a,
		pbrnn->databasic.DeltW.a, pbrnn->databasic.DeltB.a,
		pbrnn->pActTemplet
	);

	//不需要训练
	if (pbrnn->NeedTrainFlag)
	{
		pbrnn->RnnCell.ActFun.in.a = pbrnn->h_t_mem.a;
		pbrnn->RnnCell.ActFun.out.a = pbrnn->databasic.basic.out.a;
	}
	return (lw_ptr)NULL;
}

lw_ptr OSlwToolBPnnLayerBasicRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerBasicRnnSTU *prnn;
	OSLW_assert(!(pNNSLB));
	prnn = (void *)pNNSLB;

	OSlwToolBPnnLayerFullConClear(pNNSLB);
	LW_MAT_CLR(&(prnn->out_t_1_mem));

	return 0;
	
}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerBasicRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *ph,
	ParaType *pOut_t_1,
	lw_u16 InCol,
	lw_u16 OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNLayerFullConSTU *node;
	OSlwToolNNLayerBasicRnnSTU *pbrnn;
	lw_u32 in_len, out_len;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerBasicRnnSTU));

	pbrnn = (void *)node;
	
	memset(node, 0, sizeof(OSlwToolNNLayerBasicRnnSTU));

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_BasicRnn;


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = OutCol;
	node->Bias.length = OutCol;

	node->DeltB.row = 1;
	node->DeltB.col = OutCol;
	node->DeltB.length = OutCol;

	in_len = max_mini_batch * InCol;
	out_len = max_mini_batch*OutCol;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	node->Weight.row = InCol+OutCol;
	node->Weight.col = OutCol;
	node->Weight.length = node->Weight.row*node->Weight.col;

	node->DeltW.row = node->Weight.row;
	node->DeltW.col = OutCol;
	node->DeltW.length = node->Weight.length;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_len));
	}
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, InCol, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, OutCol, pout);

	//设置CEll隐藏层
	if (ph == NULL)
	{
		ph = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(pbrnn->h_t_mem), max_mini_batch, OutCol, ph);

	if (NeedTrainFlag==0 && pOut_t_1==NULL)
	{
		pOut_t_1= pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(pbrnn->out_t_1_mem), max_mini_batch, OutCol, pOut_t_1);

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolNNLayerBasicRnnForward;
	node->basic.Backward = OSlwToolNNLayerBasicRnnBackward;
	node->basic.DataInit = OSlwToolBPnnLayerBasicRnnDataInit;
	node->basic.Clear = OSlwToolBPnnLayerBasicRnnClear;

	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}


void* OSlwToolBPnnBasicRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
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

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerBasicRnnSTU *pBRNN;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1;
	ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	//只有输入层
	if (pBPnn->Net.NowLen == 1)
	{
		if (pin == NULL)
		{
			pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*pBPnn->Train.mini_batch_max));
		}
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
		OSlwToolMatrixInitial(&(pBPnn->x), pBPnn->Train.mini_batch_max, in_col, pin);
#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
		OSlwToolMatrixInitial(&(pBPnn->x), in_col, pBPnn->Train.mini_batch_max, pin);
#else
#error"NN data frame is unknown"
		OSLW_assert(1);
#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

		
	}
	else
	{
		pptail = pBPnn->Net.pTail->Data.pData;
		pin = pptail[pBPnn->Net.pTail->Data.uData-1]->out.a;
	}

	//初始化节点

	pnode1 = OSlwToolNNLayerBasicRnnNew(
		pin, pout,
		NULL, NULL,
		in_col, out_col,
		pBPnn->Train.mini_batch_max,
		pBPnn->Train.Flag.NeedTrain,
		pmem
	);
	pBRNN = (void *)pnode1;
	pfc = (void *)pnode1;

	pBRNN->pActTemplet = pTemplet;
	pBRNN->RnnCell.in.a= pmem->Malloc(pmem, PARA_MEM_CAL(in_col+out_col));

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
	ppLIST1[0] = pnode1;


	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));
	
	

	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	//碎片化存储 直接进行内存分配
	if (pBPnn->Train.Flag.MemoryMethod == OSlwToolNNMemoryMethod_Chip)
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
		if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
		{
			pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (lw_u32)(pBPnn->Train._MemAllocCoff)));
		}
	}


	//pBPnn->ParaGroupNum++;

	return ppLIST1;
}







lw_ptr OSlwToolNNLayerGruRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i, j;
	register lw_u16 out_size, in_size;
	register ParaType *p1, *p2;
	register ParaType *pdst1,*pdst2, *psrc1, *psrc2, *psrc3;

	OSlwToolNNLayerGruRnnSTU *pBRnn;

	OSLW_assert(!(pNNSLB));

	pBRnn = (void *)pNNSLB;


#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	out_size = pNNSLB->out.col;
	in_size = pNNSLB->in.col;
	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		p1 = pNNSLB->out.a;
		p2 = pNNSLB->in.a;
		//运算重置门 r=sigmoid(x*wrx+ht_1*wrh+br)
		LW_PARA_JOIN(pBRnn->ResetGate.in.a, p1, out_size, p2, in_size);
		OSlwToolNNLayerRnnCellForword(&(pBRnn->ResetGate));

		//运算更新门 u=sigmoid(x*wux+ht_1*wuh+bu)
		LW_PARA_JOIN(pBRnn->UpdateGate.in.a, p1, out_size, p2, in_size);
		OSlwToolNNLayerRnnCellForword(&(pBRnn->UpdateGate));

		//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
		for (
			j = 0,
			pdst1 = pBRnn->CoreCell.in.a,
			psrc1 = pBRnn->ResetGate.ActFun.out.a,
			psrc2 = p1
			; j < out_size; ++j, ++pdst1)
		{
			*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
		}

		for (j = 0, psrc3 = p2; j < in_size; j++)
		{
			pdst1[j] = psrc3[j];
		}
		OSlwToolNNLayerRnnCellForword(&(pBRnn->CoreCell));

		//更新门到输出 o=(1-u)*c+u*ht_1 没有保留ht_1
		for (
			j = 0,
			pdst1 = pNNSLB->out.a,
			psrc1 = pBRnn->UpdateGate.ActFun.out.a,
			psrc2 = pBRnn->CoreCell.ActFun.out.a
			; j < out_size; ++j,++pdst1,++psrc1,++psrc2)
		{
			*pdst1 = _ParaMpy((_ParaFrom(1) - *psrc1), *psrc2) + _ParaMpy(*psrc1, *pdst1);
		}
	}
	else
	{
		//运算重置门 r=sigmoid(x*wrx+ht_1*wrh+br)
		LW_PARA_JOIN(pBRnn->ResetGate.in.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->ResetGate.ActFun.in.a = pBRnn->ResetGateHtMem.a;
		pBRnn->ResetGate.ActFun.out.a = pBRnn->ResetGateOutMem.a;
		OSlwToolNNLayerRnnCellForword(&(pBRnn->ResetGate));

		//运算更新门 u=sigmoid(x*wux+ht_1*wuh+bu)
		LW_PARA_JOIN(pBRnn->UpdateGate.in.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->UpdateGate.ActFun.in.a = pBRnn->UpdateGateHtMem.a;
		pBRnn->UpdateGate.ActFun.out.a = pBRnn->UpdateGateOutMem.a;
		OSlwToolNNLayerRnnCellForword(&(pBRnn->UpdateGate));

		//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
		LW_PARA_JOIN(pBRnn->CoreCell.in.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->CoreCell.ActFun.in.a = pBRnn->CoreCellHtMem.a;
		pBRnn->CoreCell.ActFun.out.a = pBRnn->CoreCellOutMem.a;
		OSlwToolNNLayerRnnCellForword(&(pBRnn->CoreCell));

		//更新门到输出 o=(1-u)*c+u*ht_1 同时更新ht_1
		for (
			j = 0,
			pdst1=pNNSLB->out.a,
			pdst2=pBRnn->OutT_1.a,
			psrc1= pBRnn->UpdateGate.ActFun.out.a,
			psrc2= pBRnn->CoreCell.ActFun.out.a
			; j < out_size; j++)
		{
			pdst1[j] = _ParaMpy((_ParaFrom(1)- psrc1[j]),psrc2[j]);
			pdst2[j] = pdst1[j];
		}



		p1 = pBRnn->OutT_1.a;
		for (i = 1; i < mini_b_num; i++)
		{
			
			p2 = OSlwMatGetR(pNNSLB->in, i);

			//运算重置门
			LW_PARA_JOIN(pBRnn->ResetGate.in.a, p1, out_size, p2, in_size);
			pBRnn->ResetGate.ActFun.in.a = OSlwMatGetR(pBRnn->ResetGateHtMem,i);
			pBRnn->ResetGate.ActFun.out.a = OSlwMatGetR(pBRnn->ResetGateOutMem,i);
			OSlwToolNNLayerRnnCellForword(&(pBRnn->ResetGate));

			//运算更新门
			LW_PARA_JOIN(pBRnn->UpdateGate.in.a, p1, out_size, p2, in_size);
			pBRnn->UpdateGate.ActFun.in.a = OSlwMatGetR(pBRnn->UpdateGateHtMem,i);
			pBRnn->UpdateGate.ActFun.out.a = OSlwMatGetR(pBRnn->UpdateGateOutMem,i);
			OSlwToolNNLayerRnnCellForword(&(pBRnn->UpdateGate));


			//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
			for ( 
				j = 0,
				pdst1= pBRnn->CoreCell.in.a,
				psrc1= pBRnn->ResetGate.ActFun.out.a,
				psrc2=p1
				; j < out_size; ++j,++pdst1)
			{
				*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
			}

			for ( j = 0,psrc3=p2; j < in_size; j++)
			{
				pdst1[j] = psrc3[j];
			}

			pBRnn->CoreCell.ActFun.in.a = OSlwMatGetR(pBRnn->CoreCellHtMem,i);
			pBRnn->CoreCell.ActFun.out.a = OSlwMatGetR(pBRnn->CoreCellOutMem,i);
			OSlwToolNNLayerRnnCellForword(&(pBRnn->CoreCell));

			//更新门到输出 o=(1-u)*c+u*ht_1 同时更新ht_1
			for (
				j = 0,
				pdst1 = OSlwMatGetR(pNNSLB->out,i),
				pdst2 = p1+out_size,//下一个ht_1
				psrc1 = pBRnn->UpdateGate.ActFun.out.a,
				psrc2 = pBRnn->CoreCell.ActFun.out.a,
				psrc3 = p1
				; j < out_size; j++)
			{
				pdst1[j] = _ParaMpy((_ParaFrom(1) - psrc1[j]), psrc2[j]) + _ParaMpy(psrc1[j], psrc3[j]);
				pdst2[j] = pdst1[j];
			}

			p1 = pdst2;
		}
	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C



	return mini_b_num;
}

lw_ptr OSlwToolNNLayerGruRnnBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i, j;
	register ParaType *dx;
	register ParaType *douterr_t_1;
	register ParaType *pdst1, *pdst2, *pdst3;
	register ParaType *psrc1, *psrc2, *psrc3;
	register ParaType *out_err_t_1, *h_t_1;
	register ParaType *p2;
	ParaType temp1, temp2;
	register lw_u16 in_size, out_size;
	OSlwToolNNLayerGruRnnSTU *pBRnn;

	pBRnn = (void *)pNNSLB;
	in_size = pNNSLB->in.col;
	out_size = pNNSLB->out.col;


#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		OSLW_assert(1);
	}
	else
	{
		for (i = mini_b_num - 1; i > 0; --i)
		{
			//准备数据
			out_err_t_1 = OSlwMatGetR(pNNSLB->out, i - 1);
			h_t_1 = OSlwMatGetR(pBRnn->OutT_1, i - 1);

			p2 = OSlwMatGetR(pNNSLB->in, i);
			//重置门
			LW_PARA_JOIN(pBRnn->ResetGate.in.a, h_t_1, out_size, p2, in_size);
			pBRnn->ResetGate.ActFun.in.a = OSlwMatGetR(pBRnn->ResetGateHtMem, i);
			pBRnn->ResetGate.ActFun.out.a = OSlwMatGetR(pBRnn->ResetGateOutMem, i);
			//更新门
			LW_PARA_JOIN(pBRnn->UpdateGate.in.a, h_t_1, out_size, p2, in_size);
			pBRnn->UpdateGate.ActFun.in.a = OSlwMatGetR(pBRnn->UpdateGateHtMem, i);
			pBRnn->UpdateGate.ActFun.out.a = OSlwMatGetR(pBRnn->UpdateGateOutMem, i);
			//核心单元
			for (
				j = 0,
				pdst1 = pBRnn->CoreCell.in.a,
				psrc1 = pBRnn->ResetGate.ActFun.out.a,
				psrc2 = h_t_1
				; j < out_size; ++j, ++pdst1)
			{
				*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
			}

			for (j = 0, psrc3 = p2; j < in_size; j++)
			{
				pdst1[j] = psrc3[j];
			}
			pBRnn->CoreCell.ActFun.in.a = OSlwMatGetR(pBRnn->CoreCellHtMem, i);
			pBRnn->CoreCell.ActFun.out.a = OSlwMatGetR(pBRnn->CoreCellOutMem, i);


			//	do/du= ht_1-c	do/dc=(1-u)		do/ht_1=u 第一部分保存于 输出误差上
			for (j = 0,
				pdst1 = pBRnn->CoreCell.ActFun.out.a,
				pdst2 = pBRnn->UpdateGate.ActFun.out.a,
				pdst3 = out_err_t_1,
				psrc1 = OSlwMatGetR(pNNSLB->out, i),//dL/do
				psrc2 = h_t_1//ht_1
				; j < out_size; ++j, ++pdst1, ++pdst2, ++pdst3, ++psrc1, ++psrc2)
			{

				temp1 = *pdst2;
				*pdst2 = _ParaMpy(*psrc1, *psrc2 - *pdst1);

				temp2 = _ParaMpy(temp1, *psrc1);//u*dL/do
				*pdst1 = *psrc1 - temp2;
				*pdst3 += temp2;
			}

			//核心单元反向传递
			OSlwToolNNLayerRnnCellBackword(&(pBRnn->CoreCell));

			//更新门反向传递
			OSlwToolNNLayerRnnCellBackword(&(pBRnn->UpdateGate));

			//do/dht_1=do/d(r*ht_1) * d(r*ht_1)/dht_1=do/d(r*ht_1) * r 第二部分保存于输出误差
			//do/dr=do/d(r*ht_1) * ht_1
			for (j = 0,
				psrc1 = pBRnn->CoreCell.in.a,//do/d(r*ht_1)
				psrc3 = pBRnn->ResetGate.ActFun.out.a,//r
				pdst3 = out_err_t_1,
				psrc2 = h_t_1
				; j < out_size; ++j, ++pdst3, ++psrc1, ++psrc2, ++psrc3)
			{
				temp1 = *psrc3;
				*psrc3 = _ParaMpy(*psrc2, *psrc1);//do/dr=do/d(r*ht_1) * ht_1
				*pdst3 += _ParaMpy(*psrc1, temp1);
			}

			//重置门反向传递
			OSlwToolNNLayerRnnCellBackword(&(pBRnn->ResetGate));

			//整理剩余的dht-1 分别是更新门与重置门的dht-1
			for (j = 0,
				pdst3 = out_err_t_1,
				psrc1 = pBRnn->UpdateGate.in.a,
				psrc2 = pBRnn->ResetGate.in.a,
				psrc3 = pBRnn->CoreCell.in.a
				; j < out_size; ++j, ++pdst3, ++psrc1, ++psrc2)
			{
				*pdst3 += *psrc1;
				*pdst3 += *psrc2;
			}

			//整理dxt
			for (
				j = 0,
				pdst3 = p2,
				psrc3 += out_size
				; j < in_size; ++j, ++pdst3, ++psrc1, ++psrc2, ++psrc3)
			{
				*pdst3 = *psrc1;
				*pdst3 += *psrc2;
				*pdst3 += *psrc3;
			}
		}


		h_t_1 = NULL;
		p2 = pNNSLB->in.a;
		//重置门
		//LW_PARA_JOIN(pBRnn->ResetGate.in.a, NULL, out_size, p2, in_size);
		//pBRnn->ResetGate.ActFun.in.a = pBRnn->ResetGateHtMem.a;
		//pBRnn->ResetGate.ActFun.out.a = pBRnn->ResetGateOutMem.a;
		//更新门
		LW_PARA_JOIN(pBRnn->UpdateGate.in.a, NULL, out_size, p2, in_size);
		pBRnn->UpdateGate.ActFun.in.a = pBRnn->UpdateGateHtMem.a;
		pBRnn->UpdateGate.ActFun.out.a = pBRnn->UpdateGateOutMem.a;
		//核心单元
		for (j = 0, pdst1 = pBRnn->CoreCell.in.a; j < out_size; ++j, ++pdst1)
		{
			*pdst1 = _ParaFrom(0);
		}

		for (j = 0, psrc3 = p2; j < in_size; ++j, ++pdst1, ++psrc3)
		{
			*pdst1 = *psrc3;
		}
		pBRnn->CoreCell.ActFun.in.a = pBRnn->CoreCellHtMem.a;
		pBRnn->CoreCell.ActFun.out.a = pBRnn->CoreCellOutMem.a;

		//	do/du= ht_1-c	do/dc=(1-u)		do/ht_1没有
		for (j = 0,
			pdst1 = pBRnn->CoreCell.ActFun.out.a,
			pdst2 = pBRnn->UpdateGate.ActFun.out.a,
			psrc1 =pNNSLB->out.a//dL/do
			; j < out_size; ++j, ++pdst1, ++pdst2, ++psrc1)
		{

			temp1 = *pdst2;
			*pdst2 = _ParaMpy(*psrc1, -(*pdst1));

			temp2 = _ParaMpy(temp1, *psrc1);//u*dL/do
			*pdst1 = *psrc1 - temp2;
		}
		//核心单元反向传递
		OSlwToolNNLayerRnnCellBackword(&(pBRnn->CoreCell));

		//更新门反向传递
		OSlwToolNNLayerRnnCellBackword(&(pBRnn->UpdateGate));
		//do/dht_1没有
		//do/dr=do/d(r*ht_1) * ht_1=0
		//所以 重置门不需要计算

		//整理dxt
		for (
			j = 0,
			pdst3 = p2,
			psrc1= pBRnn->UpdateGate.in.a+out_size,
			psrc3 = pBRnn->CoreCell.in.a+ out_size
			; j < in_size; ++j, ++pdst3, ++psrc1, ++psrc3)
		{
			*pdst3 += *psrc1;
			*pdst3 += *psrc3;
		}
	


	}

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	return mini_b_num;
}

lw_ptr OSlwToolBPnnLayerGruRnnDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	OSlwToolNNLayerGruRnnSTU *pbrnn;
	lw_u32 w_offset, b_offset, in_offset;
	lw_u32 in_len, out_len;
	//先执行全连接初始化
	OSlwToolBPnnLayerFullConDataInit(pNNSLB);

	if (pNNSLB->FlowData.pData==NULL)
	{
		OSLW_assert(1);
	}
	in_len = pNNSLB->in.col;
	out_len = pNNSLB->out.col;

	in_offset = in_len + out_len;
	w_offset = in_offset*out_len;
	b_offset = out_len;

	pbrnn = (void *)pNNSLB;

	OSlwToolNNLayerRnnCellInit(
		&(pbrnn->ResetGate),
		in_offset, out_len,
		pNNSLB->FlowData.pData,
		pbrnn->databasic.Weight.a, pbrnn->databasic.Bias.a,
		pbrnn->databasic.DeltW.a, pbrnn->databasic.DeltB.a,
		(void *)LwSigmoid
	);

	OSlwToolNNLayerRnnCellInit(
		&(pbrnn->UpdateGate),
		in_offset, out_len,
		(ParaType *)(pNNSLB->FlowData.pData) + in_offset,
		pbrnn->databasic.Weight.a + w_offset, pbrnn->databasic.Bias.a + b_offset,
		pbrnn->databasic.DeltW.a + w_offset, pbrnn->databasic.DeltB.a + b_offset,
		(void *)LwSigmoid
	);

	OSlwToolNNLayerRnnCellInit(
		&(pbrnn->CoreCell),
		in_offset, out_len,
		(ParaType *)(pNNSLB->FlowData.pData) + (in_offset<<1),
		pbrnn->databasic.Weight.a + (w_offset<<1), pbrnn->databasic.Bias.a + (b_offset<<1),
		pbrnn->databasic.DeltW.a + (w_offset<<1), pbrnn->databasic.DeltB.a + (b_offset<<1),
		pbrnn->pActTemplet
	);

	//不需要训练
	if (pbrnn->NeedTrainFlag)
	{
		pbrnn->ResetGate.ActFun.in.a = pbrnn->ResetGateHtMem.a;
		pbrnn->ResetGate.ActFun.out.a = pbrnn->ResetGateOutMem.a;

		pbrnn->UpdateGate.ActFun.in.a = pbrnn->UpdateGateHtMem.a;
		pbrnn->UpdateGate.ActFun.out.a = pbrnn->UpdateGateOutMem.a;

		pbrnn->CoreCell.ActFun.in.a = pbrnn->CoreCellHtMem.a;
		pbrnn->CoreCell.ActFun.out.a = pbrnn->CoreCellOutMem.a;

	}
	return (lw_ptr)NULL;
}

lw_ptr OSlwToolBPnnLayerGruRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerGruRnnSTU *prnn;
	OSLW_assert(!(pNNSLB));
	prnn = (void *)pNNSLB;

	OSlwToolBPnnLayerFullConClear(pNNSLB);
	LW_MAT_CLR(&(prnn->OutT_1));

	return 0;

}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerGruRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pCoreCellOut,
	ParaType *pCoreCellHt,
	ParaType *pResetGateOut,
	ParaType *pResetGateHt,
	ParaType *pUpdateGateOut,
	ParaType *pUpdateGateHt,
	ParaType *pOutT_1,
	lw_u16 InCol,
	lw_u16 OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNLayerFullConSTU *node;
	OSlwToolNNLayerGruRnnSTU *pbrnn;
	lw_u32 in_len, out_len;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Malloc(pmem, sizeof(OSlwToolNNLayerGruRnnSTU));

	pbrnn = (void *)node;
	
	memset(node, 0, sizeof(OSlwToolNNLayerGruRnnSTU));

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_GruRnn;


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = OutCol*3;
	node->Bias.length = OutCol*3;

	node->DeltB.row = 1;
	node->DeltB.col = OutCol*3;
	node->DeltB.length = OutCol*3;

	in_len = max_mini_batch * InCol;
	out_len = max_mini_batch*OutCol;
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

	node->Weight.row = 3*(InCol + OutCol);
	node->Weight.col = OutCol;
	node->Weight.length = node->Weight.row*node->Weight.col;

	node->DeltW.row = node->Weight.row;
	node->DeltW.col = OutCol;
	node->DeltW.length = node->Weight.length;

	//设置输入

	if (pin == NULL)
	{
		pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_len));
	}
	OSlwToolMatrixInitial(&(node->basic.in), max_mini_batch, InCol, pin);

	//设置输出
	if (pout == NULL)
	{
		pout = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(node->basic.out), max_mini_batch, OutCol, pout);

	//设置CoreCell输出
	pCoreCellOut = (pCoreCellOut)? pCoreCellOut :(pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->CoreCellOutMem), max_mini_batch, OutCol, pCoreCellOut);

	//设置CoreCell隐藏层
	pCoreCellHt = (pCoreCellHt) ? pCoreCellHt : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->CoreCellHtMem), max_mini_batch, OutCol, pCoreCellHt);

	//设置ResetGate输出
	pResetGateOut = (pResetGateOut) ? pResetGateOut : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->ResetGateOutMem), max_mini_batch, OutCol, pResetGateOut);

	//设置ResetGate隐藏层
	pResetGateHt = (pResetGateHt) ? pResetGateHt : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->ResetGateHtMem), max_mini_batch, OutCol, pResetGateHt);

	//设置UpdateGate输出
	pUpdateGateOut = (pUpdateGateOut) ? pUpdateGateOut : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->UpdateGateOutMem), max_mini_batch, OutCol, pUpdateGateOut);

	//设置UpdateGate隐藏层
	pUpdateGateHt = (pUpdateGateHt) ? pUpdateGateHt : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	OSlwToolMatrixInitial(&(pbrnn->UpdateGateHtMem), max_mini_batch, OutCol, pUpdateGateHt);

	if (NeedTrainFlag == 0 && pOutT_1 == NULL)
	{
		pOutT_1 = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(pbrnn->OutT_1), max_mini_batch, OutCol, pOutT_1);

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
	OSLW_assert(1);
#else

#error"NN data frame is unknown"
	OSLW_assert(1);

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolNNLayerGruRnnForward;
	node->basic.Backward = OSlwToolNNLayerGruRnnBackward;
	node->basic.DataInit = OSlwToolBPnnLayerGruRnnDataInit;
	node->basic.Clear = OSlwToolBPnnLayerGruRnnClear;

	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}




void* OSlwToolBPnnGruRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
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

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerGruRnnSTU *pBRNN;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1;
	ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	//只有输入层
	if (pBPnn->Net.NowLen == 1)
	{
		if (pin == NULL)
		{
			pin = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*pBPnn->Train.mini_batch_max));
		}
#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
		OSlwToolMatrixInitial(&(pBPnn->x), pBPnn->Train.mini_batch_max, in_col, pin);
#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F
		OSLW_assert(1);
#else
#error"NN data frame is unknown"
		OSLW_assert(1);
#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C


	}
	else
	{
		pptail = pBPnn->Net.pTail->Data.pData;
		pin = pptail[pBPnn->Net.pTail->Data.uData-1]->out.a;
	}

	//初始化节点

	pnode1 = OSlwToolNNLayerGruRnnNew(
		pin, pout,
		NULL, NULL,
		NULL, NULL,
		NULL, NULL,
		NULL,
		in_col, out_col,
		pBPnn->Train.mini_batch_max,
		pBPnn->Train.Flag.NeedTrain,
		pmem
	);

	pBRNN = (void *)pnode1;
	pfc = (void *)pnode1;

	pBRNN->pActTemplet = pTemplet;
	pfc->basic.FlowData.uData = 3 * PARA_MEM_CAL(in_col*out_col);

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
	ppLIST1[0] = pnode1;


	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	//每次都要复制输出层
	memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	//碎片化存储 直接进行内存分配
	if (pBPnn->Train.Flag.MemoryMethod == OSlwToolNNMemoryMethod_Chip)
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
		if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
		{
			pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (lw_u32)(pBPnn->Train._MemAllocCoff)));
		}
	}


	//pBPnn->ParaGroupNum++;

	return ppLIST1;
}


#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

