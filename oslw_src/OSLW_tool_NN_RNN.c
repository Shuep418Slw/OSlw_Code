/*(Ver.=0.95)
 * OSLW_tool.c
 *
 *  Created on: 2019-01-22
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL


//void OSlwToolNNLayerRnnCellInit(
//	OSlwToolNNLayerRnnCellSTU *pCell,
//	lw_u16 in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
//	lw_u16 out_size,
//	ParaType *pin,//只需要输入 因为隐藏层与输出要输入时候才确定
//	ParaType *pwe,ParaType *pbi,
//	ParaType *pdwe,ParaType *pdbi,
//	OSlwToolNNSubLayerBasicSTU *pActTemplet
//	)
//{
//	
//	OSLW_assert(!(pin));
//	OSLW_assert(!(pwe));
//	OSLW_assert(!(pbi));
//	OSLW_assert(!(pActTemplet));
//
//	OSlwMatInit(&(pCell->in), 1, in_size, pin);
//	OSlwMatInit(&(pCell->ActFun.in), 1, out_size, NULL);
//	OSlwMatInit(&(pCell->ActFun.out), 1, out_size, NULL);
//	OSlwMatInit(&(pCell->we), in_size, out_size, pwe);
//	OSlwMatInit(&(pCell->bi), 1, out_size, pbi);
//	OSlwMatInit(&(pCell->dwe), in_size, out_size, pdwe);
//	OSlwMatInit(&(pCell->dbi), 1, out_size, pdbi);
//
//	pCell->ActFun.Backward = pActTemplet->Backward;
//	pCell->ActFun.Forward = pActTemplet->Forward;
//}
//
//void OSlwToolNNLayerRnnCellForword(
//	OSlwToolNNLayerRnnCellSTU *pCell
//)
//{
//	pOSlwToolMatrixXWeBi(
//		&(pCell->ActFun.in),
//		&(pCell->we),
//		&(pCell->in),
//		&(pCell->bi)
//	);
//
//	pCell->ActFun.Forward((void *)&(pCell->ActFun), 1);
//}
//
//void OSlwToolNNLayerRnnCellBackword(
//	OSlwToolNNLayerRnnCellSTU *pCell
//)
//{
//	register lw_u32 i;
//	register ParaType *p1, *p2;
//	pCell->ActFun.Backward((void *)&(pCell->ActFun), 1);
//
//	//采用叠加方法
//	//dw=in'*out
//	pOSlwToolMatrixTurnMpy(&(pCell->dwe), &(pCell->in), &(pCell->ActFun.in), 6);
//	//直接叠加
//	p1 = pCell->dbi.a;
//	p2 = pCell->ActFun.in.a;
//	i = pCell->ActFun.out.col;
//	while (i--)(*p1++) += *p2++;
//
//	//xd = out*w';
//	pOSlwToolMatrixTurnMpy(&(pCell->in), &(pCell->ActFun.in), &(pCell->we), 1);
//}

ParaType * OSlwToolNNLayerRnnCellForward(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag)
{
	OSlwToolNNLayerRnnCellSTU *pcell = (void *)pCELL;
	OSlwMat we = { pCELL->cell_we.pData,pCELL->cell_we.uData, pCELL->cell_in_now.col, pcell->ActFun.in.col };
	OSlwMat bi = { pCELL->cell_bi.pData,pCELL->cell_bi.uData,1, pcell->ActFun.in.col };


	pOSlwToolMatrixXWeBi(
		&(pcell->ActFun.in),
		&(we),
		&(pCELL->cell_in_now),
		&(bi)
	);
	
	pcell->ActFun.Forward((void *)&(pcell->ActFun), 1);

	return pcell->ActFun.out.a;
}

ParaType * OSlwToolNNLayerRnnCellBackward(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag)
{

	register lw_u32 i;
	register ParaType *p1, *p2;
	OSlwToolNNLayerRnnCellSTU *pcell = (void *)pCELL;
	OSlwMat we = { pCELL->cell_we.pData,pCELL->cell_we.uData, pCELL->cell_in_now.col, pcell->ActFun.out.col };
	OSlwMat dwe = { pCELL->p_cell_dwe,pCELL->cell_we.uData, pCELL->cell_in_now.col, pcell->ActFun.out.col };


	pcell->ActFun.Backward((void *)&(pcell->ActFun), 1);

	if (TrainFlag== OSlwToolNNNeedTrain_Need)
	{
		
		//采用叠加方法
		//dw=in'*out
		pOSlwToolMatrixTurnMpy(&(dwe), &(pCELL->cell_in_now), &(pcell->ActFun.in), 6);
		//直接叠加
		p1 = pCELL->p_cell_dbi;
		p2 = pcell->ActFun.in.a;
		i = pcell->ActFun.in.col;
		while (i--)(*p1++) += *p2++;
	}

	//xd = out*w';
	pOSlwToolMatrixTurnMpy(&(pCELL->cell_in_now), &(pcell->ActFun.in), &(we), 1);

	return pcell->ActFun.out.a;
}


ParaType * OSlwToolNNLayerRnnCellLoad(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, LwMatRowType index)
{
	OSlwToolNNLayerRnnCellSTU *pcell = (void *)pCELL;
	lw_u32 _offset_col = pcell->ActFun.out.col*index;
	
	pcell->ActFun.in.a = (ParaType *)(pCELL->cell_h_mem.pData) + _offset_col;
	pcell->ActFun.out.a = (ParaType *)(pCELL->cell_out_mem.pData) + _offset_col;

	return pcell->ActFun.out.a;
}

ParaType * OSlwToolNNLayerRnnCellParaInit(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL,ParaType *we, ParaType *bi, ParaType *dwe, ParaType *dbi, ParaType *stack)
{
	pCELL->cell_we.pData = we;
	pCELL->cell_bi.pData = bi;
	pCELL->p_cell_dwe = dwe;
	pCELL->p_cell_dbi = dbi;
	pCELL->cell_in_now.a = stack;

	return stack + pCELL->FlowDataLen;
}


void* OSlwToolNNLayerRnnCellNew(
	ParaType *pout,
	LwMatColType in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
	LwMatColType out_size,
	lw_u16 max_min_batch,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
)
{
	OSlwToolNNLayerRnnCellSTU *prnn;
	lw_u32  len;
	lw_u8 *pbase;
	OSLW_assert(!(pmem));

	len = out_size*max_min_batch;

	if (pout)
	{
		pbase = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerRnnCellSTU) + PARA_MEM_CAL(len));
	}
	else
	{
		pbase = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerRnnCellSTU) + PARA_MEM_CAL(len * 2));
		pout = (void *)(pbase + sizeof(OSlwToolNNLayerRnnCellSTU) + PARA_MEM_CAL(len));
	}


	prnn = (void *)pbase;

	OSlwMatInit(&(prnn->base.cell_in_now), 1, in_size, NULL);
	OSlwMatInit(&(prnn->ActFun.in), 1, out_size, NULL);
	OSlwMatInit(&(prnn->ActFun.out), 1, out_size, NULL);

	prnn->base.cell_out_now_handle = &(prnn->ActFun.out);
	
	prnn->base.cell_h_mem.uData = len;
	prnn->base.cell_h_mem.pData = pbase + sizeof(OSlwToolNNLayerRnnCellSTU);

	prnn->base.cell_out_mem.uData = len;
	prnn->base.cell_out_mem.pData = pout;

	prnn->base.cell_we.uData = in_size*out_size;
	prnn->base.cell_bi.uData = out_size;

	prnn->base.FlowDataLen = in_size;

	prnn->ActFun.Forward = ptem->basic.Forward;
	prnn->ActFun.Backward = ptem->basic.Backward;

	prnn->base.cell_forward = OSlwToolNNLayerRnnCellForward;
	prnn->base.cell_backward = OSlwToolNNLayerRnnCellBackward;
	prnn->base.cell_load = OSlwToolNNLayerRnnCellLoad;
	prnn->base.cell_parameter_init = OSlwToolNNLayerRnnCellParaInit;

	return prnn;
}

ParaType * OSlwToolNNLayerRnnCellLNormForward(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag)
{
	OSlwToolNNLayerRnnCellLNormSTU *pcell = (void *)pCELL;
	OSlwMat ht = { pcell->pHt, pcell->ActFun.out.col, 1, pcell->ActFun.out.col };
	OSlwMat we = { pCELL->cell_we.pData,pCELL->cell_in_now.col*pcell->ActFun.in.col, pCELL->cell_in_now.col, pcell->ActFun.in.col };
	OSlwMat bi = { pCELL->cell_bi.pData,pcell->ActFun.in.col,1, pcell->ActFun.in.col };


	pOSlwToolMatrixXWeBi(
		&(ht),
		&(we),
		&(pCELL->cell_in_now),
		&(bi)
	);

	pOSlwToolMatrixMoments(&(ht), pcell->pmean, pcell->pvar, 1);

	pcell->pvar[0] = _OSlwToolNNLayerLnForwordSub(ht.a, pcell->ActFun.in.a,
		pcell->pNormWe, pcell->pNormBi,
		pcell->pmean[0], pcell->pvar[0], bi.col
	);

	pcell->ActFun.Forward((void *)&(pcell->ActFun), 1);

	return pcell->ActFun.out.a;
}

ParaType * OSlwToolNNLayerRnnCellLNormBackward(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag)
{

	register lw_u32 i;
	register ParaType *p1, *p2;
	OSlwToolNNLayerRnnCellLNormSTU *pcell = (void *)pCELL;
	OSlwMat we = { pCELL->cell_we.pData,pCELL->cell_in_now.col*pcell->ActFun.out.col, pCELL->cell_in_now.col, pcell->ActFun.out.col };
	OSlwMat dwe1 = { pCELL->p_cell_dwe,we.length, pCELL->cell_in_now.col, pcell->ActFun.out.col };
	OSlwMat ht = { pcell->pHt, pcell->ActFun.out.col, 1, pcell->ActFun.out.col };
	OSlwMat bi = { pCELL->cell_bi.pData,pcell->ActFun.in.col,1, pcell->ActFun.in.col };

	pcell->ActFun.Backward((void *)&(pcell->ActFun), 1);

	if (TrainFlag == OSlwToolNNNeedTrain_Need)
	{
		//LN反向传递
		_OSlwToolNNLayerLnBackwordSub(
			ht.a, pcell->ActFun.in.a,
			pcell->pNormWe, pcell->pNormBi,
			dwe1.length + dwe1.a, pCELL->p_cell_dbi + bi.length,
			pcell->pmean[0], pcell->pvar[0],
			bi.col,
			pcell->pBuf
		);

		//采用叠加方法
		//dw=in'*out
		pOSlwToolMatrixTurnMpy(&(dwe1), &(pCELL->cell_in_now), &(ht), 6);
		//直接叠加
		p1 = pCELL->p_cell_dbi;
		p2 = pcell->ActFun.in.a;
		i = pcell->ActFun.in.col;
		while (i--)(*p1++) += *p2++;
	}
	else
	{
		//LN反向传递
		_OSlwToolNNLayerLnBackwordSub(
			ht.a, pcell->ActFun.in.a,
			pcell->pNormWe, pcell->pNormBi,
			NULL, NULL,
			pcell->pmean[0], pcell->pvar[0],
			bi.col,
			pcell->pBuf
		);
	}

	//xd = out*w';
	pOSlwToolMatrixTurnMpy(&(pCELL->cell_in_now), &(ht), &(we), 1);

	return pcell->ActFun.out.a;
}


ParaType * OSlwToolNNLayerRnnCellLNormLoad(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, LwMatRowType index)
{
	OSlwToolNNLayerRnnCellLNormSTU *pcell = (void *)pCELL;
	ParaType *p;
	lw_u32 _len_1 = pcell->ActFun.out.length;
	lw_u32 _len_2 = pcell->ActFun.out.row;
	lw_u32 _offset_col1 = pcell->ActFun.out.col*index;

	p = pCELL->cell_h_mem.pData;

	pcell->ActFun.in.a = p + _offset_col1;
	p += _len_1;
	pcell->pHt = p + _offset_col1;
	p += _len_1;
	pcell->pmean = p + index;
	pcell->pvar = p + _len_2 + index;

	pcell->ActFun.out.a = (ParaType *)(pCELL->cell_out_mem.pData) + _offset_col1;

	return pcell->ActFun.out.a;
}

ParaType * OSlwToolNNLayerRnnCellLNormParaInit(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL,ParaType *we, ParaType *bi, ParaType *dwe, ParaType *dbi, ParaType *stack)
{
	OSlwToolNNLayerRnnCellLNormSTU *pcell = (void *)pCELL;
	ParaType *norm_we, *norm_bi;
	LwMatColType out_len = pCELL->cell_out_now_handle->col;
	pCELL->cell_we.pData = we;
	pCELL->cell_bi.pData = bi;
	pCELL->p_cell_dwe = dwe;
	pCELL->p_cell_dbi = dbi;
	pCELL->cell_in_now.a = stack;
	pcell->pBuf = stack + pCELL->cell_in_now.col;
	pcell->pNormWe = (ParaType *)(pcell->base.cell_we.pData) + (pcell->base.cell_we.uData - out_len);
	pcell->pNormBi = (ParaType *)(pcell->base.cell_bi.pData) + (pcell->base.cell_bi.uData - out_len);

	return stack + pCELL->FlowDataLen;
}


void* OSlwToolNNLayerRnnCellLNormNew(
	ParaType *pout,
	LwMatColType in_size,//这个是CELL的输入大小 对应应该是RNN的输入+输出
	LwMatColType out_size,
	lw_u16 max_min_batch,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
)
{
	OSlwToolNNLayerRnnCellLNormSTU *prnn;
	lw_u32  len;
	lw_u8 *pbase;
	OSLW_assert(!(pmem));

	len = out_size*max_min_batch;

	if (pout)
	{
		pbase = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerRnnCellLNormSTU) + PARA_MEM_CAL(len * 2 + 2 * max_min_batch));
		/*
		排布：
		激活函数输入
		LN输入
		均值
		方差
		*/
	}
	else
	{
		pbase = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerRnnCellLNormSTU) + PARA_MEM_CAL(len * 3+ 2 * max_min_batch));
		pout = (void *)(pbase + sizeof(OSlwToolNNLayerRnnCellLNormSTU) + PARA_MEM_CAL(len * 2 + 2 * max_min_batch));
	}


	prnn = (void *)pbase;

	OSlwMatInit(&(prnn->base.cell_in_now), 1, in_size, NULL);
	OSlwMatInit(&(prnn->ActFun.in), 1, out_size, NULL);
	OSlwMatInit(&(prnn->ActFun.out), max_min_batch, out_size, NULL);

	prnn->base.cell_out_now_handle = &(prnn->ActFun.out);

	prnn->base.cell_h_mem.uData = len;
	prnn->base.cell_h_mem.pData = pbase + sizeof(OSlwToolNNLayerRnnCellLNormSTU);

	prnn->base.cell_out_mem.uData = len;
	prnn->base.cell_out_mem.pData = pout;

	prnn->base.cell_we.uData = in_size*out_size + out_size;
	prnn->base.cell_bi.uData = out_size + out_size;

	prnn->base.FlowDataLen = in_size + out_size;

	prnn->ActFun.Forward = ptem->basic.Forward;
	prnn->ActFun.Backward = ptem->basic.Backward;

	prnn->base.cell_forward = OSlwToolNNLayerRnnCellLNormForward;
	prnn->base.cell_backward = OSlwToolNNLayerRnnCellLNormBackward;
	prnn->base.cell_load = OSlwToolNNLayerRnnCellLNormLoad;
	prnn->base.cell_parameter_init = OSlwToolNNLayerRnnCellLNormParaInit;

	return prnn;
}


lw_ptr OSlwToolNNLayerBasicRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i;
	OSlwToolNNLayerBasicRnnSTU *pBRnn;
	OSlwToolNNLayerRnnCellBasicSTU *p_cell;
	lw_u8 nn_need_train_flag;

	OSLW_assert(!(pNNSLB));

	pBRnn = (void *)pNNSLB;
	p_cell = pBRnn->pRnnCell;
	nn_need_train_flag = pBRnn->databasic.basic.pNN->Train.Flag.NeedTrain;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	
	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		//加载数据
		//LW_PARA_JOIN(pBRnn->RnnCell.in.a, pNNSLB->out.a, pNNSLB->out.col, pNNSLB->in.a, pNNSLB->in.col);
		LW_PARA_JOIN(pBRnn->pRnnCell->cell_in_now.a, pNNSLB->out.a, pNNSLB->out.col, pNNSLB->in.a, pNNSLB->in.col);

		//前向传递
		p_cell->cell_forward(p_cell, nn_need_train_flag);
	}
	else
	{
		//加载数据
		LW_PARA_JOIN(p_cell->cell_in_now.a, NULL, pNNSLB->out.col, pNNSLB->in.a, pNNSLB->in.col);//第一次的时候 ht-1是全0
		p_cell->cell_load(p_cell, 0);

		//前向传递
		p_cell->cell_forward(p_cell, nn_need_train_flag);

		//保存数据
		LW_PARA_JOIN(pBRnn->out_t_1_mem.a, pNNSLB->out.a, pNNSLB->out.col, NULL, 0);

		for (i = 1; i < mini_b_num; i++)
		{
			//加载数据
			LW_PARA_JOIN(p_cell->cell_in_now.a, OSlwMatGetR(pBRnn->out_t_1_mem, i - 1), pNNSLB->out.col, OSlwMatGetR(pNNSLB->in, i), pNNSLB->in.col);
			p_cell->cell_load(p_cell, i);

			//前向传递
			p_cell->cell_forward(p_cell, nn_need_train_flag);

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
	OSlwToolNNLayerRnnCellBasicSTU *p_cell;
	lw_u8 nn_need_train_flag;


	pBRnn = (void *)pNNSLB;
	in_size = pNNSLB->in.col;
	out_size = pNNSLB->out.col;
	p_cell = pBRnn->pRnnCell;
	nn_need_train_flag = pBRnn->databasic.basic.pNN->Train.Flag.NeedTrain;

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
			LW_PARA_JOIN(p_cell->cell_in_now.a, OSlwMatGetR(pBRnn->out_t_1_mem, i-1), out_size, OSlwMatGetR(pNNSLB->in, i), in_size);
			p_cell->cell_load(p_cell, i);

			//反向传递
			p_cell->cell_backward(p_cell, nn_need_train_flag);

			//导出数据
			din = p_cell->cell_in_now.a;
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
		LW_PARA_JOIN(p_cell->cell_in_now.a, NULL, out_size, pNNSLB->in.a, in_size);

		p_cell->cell_load(p_cell, 0);

		//反向传递
		p_cell->cell_backward(p_cell, nn_need_train_flag);

		//导出数据
		din = p_cell->cell_in_now.a + out_size;
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
	OSlwToolNNLayerRnnCellBasicSTU *p_cell;

	OSLW_assert(!(pNNSLB));

	//先执行全连接初始化
	OSlwToolBPnnLayerFullConDataInit(pNNSLB);

	pbrnn = (void *)pNNSLB;


	p_cell = pbrnn->pRnnCell;
	p_cell->cell_parameter_init(
		p_cell,
		pbrnn->databasic.Weight.a,
		pbrnn->databasic.Bias.a,
		pbrnn->databasic.DeltW.a,
		pbrnn->databasic.DeltB.a,
		pNNSLB->FlowData.pData
	);
	

	//不需要训练
	if (pbrnn->NeedTrainFlag)
	{
		pbrnn->pRnnCell->cell_load(pbrnn->pRnnCell, 0);
	}
	return (lw_ptr)NULL;
}

lw_ptr OSlwToolBPnnLayerBasicRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerBasicRnnSTU *prnn;
	OSLW_assert(!(pNNSLB));
	prnn = (void *)pNNSLB;

	OSlwToolBPnnLayerFullConClear(pNNSLB);
	if(prnn->out_t_1_mem.a)
	{
		LW_MAT_CLR(&(prnn->out_t_1_mem));
	}
	return 0;
	
}


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerBasicRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pOut_t_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNLayerFullConSTU *node;
	OSlwToolNNLayerBasicRnnSTU *pbrnn;
	lw_u32 in_len, out_len;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerBasicRnnSTU));

	pbrnn = (void *)node;

	node->basic.NN_Kind = OSlwToolNNSubLayerKind_BasicRnn;

	in_len = max_mini_batch * InCol;
	out_len = max_mini_batch*OutCol;


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

	pbrnn->pRnnCell = new_cell_fun(pout, InCol + OutCol, OutCol, max_mini_batch, ptem, pmem);


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = pbrnn->pRnnCell->cell_bi.uData;
	node->Bias.length = node->Bias.col;

	node->DeltB.row = 1;
	node->DeltB.col = node->Bias.col;
	node->DeltB.length = node->Bias.col;
	
	node->Weight.col = OutCol;
	node->Weight.row = pbrnn->pRnnCell->cell_we.uData / OutCol;
	node->Weight.length = pbrnn->pRnnCell->cell_we.uData;

	node->DeltW.row = node->Weight.row;
	node->DeltW.col = node->Weight.col;
	node->DeltW.length = pbrnn->pRnnCell->cell_we.uData;

	if (NeedTrainFlag==0 && pOut_t_1==NULL)
	{
		pOut_t_1= pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(pbrnn->out_t_1_mem), max_mini_batch, OutCol, pOut_t_1);



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


	pbrnn->NeedTrainFlag = NeedTrainFlag;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}


void* OSlwToolBPnnBasicRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerBasicRnnSTU *pBRNN;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	_NN_GET_IN(pBPnn, pin);

	//初始化节点
	pnode1 = OSlwToolNNLayerBasicRnnNew(
		pin, pout,NULL,
		in_col, out_col,
		pBPnn->Train.mini_batch_max,
		pBPnn->Train.Flag.NeedTrain,
		new_cell_fun,
		pTemplet,
		pmem
	);
	pBRNN = (void *)pnode1;
	pfc = (void *)pnode1;

	pfc->basic.FlowData.uData = PARA_MEM_CAL(pBRNN->pRnnCell->FlowDataLen);

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
	ppLIST1[0] = pnode1;


	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	_NN_FULL_CON_CHIP_ALLAC();

	return ppLIST1;
}




lw_ptr OSlwToolNNLayerGruRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 i, j;
	register lw_u16 out_size, in_size;
	register ParaType *p1, *p2;
	register ParaType *pdst1,*pdst2, *psrc1, *psrc2, *psrc3;

	OSlwToolNNLayerGruRnnSTU *pBRnn;
	lw_u8 nn_need_train_flag;
	OSLW_assert(!(pNNSLB));

	pBRnn = (void *)pNNSLB;
	nn_need_train_flag = pBRnn->databasic.basic.pNN->Train.Flag.NeedTrain;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	out_size = pNNSLB->out.col;
	in_size = pNNSLB->in.col;
	//不需要训练
	if (pBRnn->NeedTrainFlag)
	{
		p1 = pNNSLB->out.a;
		p2 = pNNSLB->in.a;
		//运算重置门 r=sigmoid(x*wrx+ht_1*wrh+br)
		LW_PARA_JOIN(pBRnn->ResetGateHandle->cell_in_now.a, p1, out_size, p2, in_size);
		pBRnn->ResetGateHandle->cell_forward(pBRnn->ResetGateHandle, nn_need_train_flag);

		//运算更新门 u=sigmoid(x*wux+ht_1*wuh+bu)
		LW_PARA_JOIN(pBRnn->UpdateGateHandle->cell_in_now.a, p1, out_size, p2, in_size);
		pBRnn->UpdateGateHandle->cell_forward(pBRnn->UpdateGateHandle, nn_need_train_flag);

		//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
		for (
			j = 0,
			pdst1 = pBRnn->CoreCellHandle->cell_in_now.a,
			psrc1 = pBRnn->ResetGateHandle->cell_out_now_handle->a,
			psrc2 = p1
			; j < out_size; ++j, ++pdst1)
		{
			*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
		}

		for (j = 0, psrc3 = p2; j < in_size; j++)
		{
			pdst1[j] = psrc3[j];
		}
		pBRnn->CoreCellHandle->cell_forward(pBRnn->CoreCellHandle, nn_need_train_flag);

		//更新门到输出 o=(1-u)*c+u*ht_1 没有保留ht_1
		for (
			j = 0,
			pdst1 = pNNSLB->out.a,
			psrc1 = pBRnn->UpdateGateHandle->cell_out_now_handle->a,
			psrc2 = pBRnn->CoreCellHandle->cell_out_now_handle->a
			; j < out_size; ++j,++pdst1,++psrc1,++psrc2)
		{
			*pdst1 = _ParaMpy((_ParaFrom(1) - *psrc1), *psrc2) + _ParaMpy(*psrc1, *pdst1);
		}
	}
	else
	{
		//运算重置门 r=sigmoid(x*wrx+ht_1*wrh+br)
		pBRnn->ResetGateHandle->cell_load(pBRnn->ResetGateHandle, 0);
		LW_PARA_JOIN(pBRnn->ResetGateHandle->cell_in_now.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->ResetGateHandle->cell_forward(pBRnn->ResetGateHandle, nn_need_train_flag);

		//运算更新门 u=sigmoid(x*wux+ht_1*wuh+bu)
		pBRnn->UpdateGateHandle->cell_load(pBRnn->UpdateGateHandle, 0);
		LW_PARA_JOIN(pBRnn->UpdateGateHandle->cell_in_now.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->UpdateGateHandle->cell_forward(pBRnn->UpdateGateHandle, nn_need_train_flag);

		//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
		pBRnn->CoreCellHandle->cell_load(pBRnn->CoreCellHandle, 0);
		LW_PARA_JOIN(pBRnn->CoreCellHandle->cell_in_now.a, NULL, out_size, pNNSLB->in.a, in_size);
		pBRnn->CoreCellHandle->cell_forward(pBRnn->CoreCellHandle, nn_need_train_flag);

		//更新门到输出 o=(1-u)*c+u*ht_1 同时更新ht_1
		for (
			j = 0,
			pdst1=pNNSLB->out.a,
			pdst2=pBRnn->OutT_1.a,
			psrc1= pBRnn->UpdateGateHandle->cell_out_now_handle->a,
			psrc2= pBRnn->CoreCellHandle->cell_out_now_handle->a
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
			pBRnn->ResetGateHandle->cell_load(pBRnn->ResetGateHandle, i);
			LW_PARA_JOIN(pBRnn->ResetGateHandle->cell_in_now.a, p1, out_size, p2, in_size);
			pBRnn->ResetGateHandle->cell_forward(pBRnn->ResetGateHandle, nn_need_train_flag);

			//运算更新门
			pBRnn->UpdateGateHandle->cell_load(pBRnn->UpdateGateHandle, i);
			LW_PARA_JOIN(pBRnn->UpdateGateHandle->cell_in_now.a, p1, out_size, p2, in_size);
			pBRnn->UpdateGateHandle->cell_forward(pBRnn->UpdateGateHandle, nn_need_train_flag);

			//运算核心单元 c=F(x*wcx+ht_1*wch*r+bc)
			pBRnn->CoreCellHandle->cell_load(pBRnn->CoreCellHandle, i);
			for ( 
				j = 0,
				pdst1= pBRnn->CoreCellHandle->cell_in_now.a,
				psrc1= pBRnn->ResetGateHandle->cell_out_now_handle->a,
				psrc2=p1
				; j < out_size; ++j,++pdst1)
			{
				*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
			}

			for ( j = 0,psrc3=p2; j < in_size; j++)
			{
				pdst1[j] = psrc3[j];
			}
			pBRnn->CoreCellHandle->cell_forward(pBRnn->CoreCellHandle, nn_need_train_flag);

			//更新门到输出 o=(1-u)*c+u*ht_1 同时更新ht_1
			for (
				j = 0,
				pdst1 = OSlwMatGetR(pNNSLB->out,i),
				pdst2 = p1+out_size,//下一个ht_1
				psrc1 = pBRnn->UpdateGateHandle->cell_out_now_handle->a,
				psrc2 = pBRnn->CoreCellHandle->cell_out_now_handle->a,
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
	lw_u8 nn_need_train_flag;

	pBRnn = (void *)pNNSLB;
	in_size = pNNSLB->in.col;
	out_size = pNNSLB->out.col;
	nn_need_train_flag = pBRnn->databasic.basic.pNN->Train.Flag.NeedTrain;

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
			LW_PARA_JOIN(pBRnn->ResetGateHandle->cell_in_now.a, h_t_1, out_size, p2, in_size);
			pBRnn->ResetGateHandle->cell_load(pBRnn->ResetGateHandle, i);

			//更新门
			LW_PARA_JOIN(pBRnn->UpdateGateHandle->cell_in_now.a, h_t_1, out_size, p2, in_size);
			pBRnn->UpdateGateHandle->cell_load(pBRnn->UpdateGateHandle, i);

			//核心单元
			for (
				j = 0,
				pdst1 = pBRnn->CoreCellHandle->cell_in_now.a,
				psrc1 = pBRnn->ResetGateHandle->cell_out_now_handle->a,
				psrc2 = h_t_1
				; j < out_size; ++j, ++pdst1)
			{
				*pdst1 = _ParaMpy(psrc1[j], psrc2[j]);
			}

			for (j = 0, psrc3 = p2; j < in_size; j++)
			{
				pdst1[j] = psrc3[j];
			}
			pBRnn->CoreCellHandle->cell_load(pBRnn->CoreCellHandle, i);


			//	do/du= ht_1-c	do/dc=(1-u)		do/ht_1=u 第一部分保存于 输出误差上
			for (j = 0,
				pdst1 = pBRnn->CoreCellHandle->cell_out_now_handle->a,
				pdst2 = pBRnn->UpdateGateHandle->cell_out_now_handle->a,
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
			pBRnn->CoreCellHandle->cell_backward(pBRnn->CoreCellHandle, nn_need_train_flag);

			//更新门反向传递
			pBRnn->UpdateGateHandle->cell_backward(pBRnn->UpdateGateHandle, nn_need_train_flag);

			//do/dht_1=do/d(r*ht_1) * d(r*ht_1)/dht_1=do/d(r*ht_1) * r 第二部分保存于输出误差
			//do/dr=do/d(r*ht_1) * ht_1
			for (j = 0,
				psrc1 = pBRnn->CoreCellHandle->cell_in_now.a,//do/d(r*ht_1)
				psrc3 = pBRnn->ResetGateHandle->cell_out_now_handle->a,//r
				pdst3 = out_err_t_1,
				psrc2 = h_t_1
				; j < out_size; ++j, ++pdst3, ++psrc1, ++psrc2, ++psrc3)
			{
				temp1 = *psrc3;
				*psrc3 = _ParaMpy(*psrc2, *psrc1);//do/dr=do/d(r*ht_1) * ht_1
				*pdst3 += _ParaMpy(*psrc1, temp1);
			}

			//重置门反向传递
			pBRnn->ResetGateHandle->cell_backward(pBRnn->ResetGateHandle, nn_need_train_flag);

			//整理剩余的dht-1 分别是更新门与重置门的dht-1
			for (j = 0,
				pdst3 = out_err_t_1,
				psrc1 = pBRnn->UpdateGateHandle->cell_in_now.a,
				psrc2 = pBRnn->ResetGateHandle->cell_in_now.a,
				psrc3 = pBRnn->CoreCellHandle->cell_in_now.a
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
		pBRnn->UpdateGateHandle->cell_load(pBRnn->UpdateGateHandle, 0);
		LW_PARA_JOIN(pBRnn->UpdateGateHandle->cell_in_now.a, NULL, out_size, p2, in_size);
		
		//核心单元
		pBRnn->CoreCellHandle->cell_load(pBRnn->CoreCellHandle, 0);
		for (j = 0, pdst1 = pBRnn->CoreCellHandle->cell_in_now.a; j < out_size; ++j, ++pdst1)
		{
			*pdst1 = _ParaFrom(0);
		}

		for (j = 0, psrc3 = p2; j < in_size; ++j, ++pdst1, ++psrc3)
		{
			*pdst1 = *psrc3;
		}
		

		//	do/du= ht_1-c	do/dc=(1-u)		do/ht_1没有
		for (j = 0,
			pdst1 = pBRnn->CoreCellHandle->cell_out_now_handle->a,
			pdst2 = pBRnn->UpdateGateHandle->cell_out_now_handle->a,
			psrc1 =pNNSLB->out.a//dL/do
			; j < out_size; ++j, ++pdst1, ++pdst2, ++psrc1)
		{

			temp1 = *pdst2;
			*pdst2 = _ParaMpy(*psrc1, -(*pdst1));

			temp2 = _ParaMpy(temp1, *psrc1);//u*dL/do
			*pdst1 = *psrc1 - temp2;
		}
		//核心单元反向传递
		pBRnn->CoreCellHandle->cell_backward(pBRnn->CoreCellHandle, nn_need_train_flag);

		//更新门反向传递
		pBRnn->UpdateGateHandle->cell_backward(pBRnn->UpdateGateHandle, nn_need_train_flag);
		//do/dht_1没有
		//do/dr=do/d(r*ht_1) * ht_1=0
		//所以 重置门不需要计算

		//整理dxt
		for (
			j = 0,
			pdst3 = p2,
			psrc1= pBRnn->UpdateGateHandle->cell_in_now.a+out_size,
			psrc3 = pBRnn->CoreCellHandle->cell_in_now.a+ out_size
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
	OSlwToolNNLayerGruRnnSTU *pbrnn = (void *)pNNSLB;
	ParaType *pwe, *pbi, *pdw, *pdb;
	lw_u32 in_len, out_len;
	ParaType *pflow;

	//先执行全连接初始化
	OSlwToolBPnnLayerFullConDataInit(pNNSLB);

	if (pNNSLB->FlowData.pData==NULL)
	{
		OSLW_assert(1);
	}
	in_len = pNNSLB->in.col;
	out_len = pNNSLB->out.col;

	pwe = pbrnn->databasic.Weight.a;
	pbi = pbrnn->databasic.Bias.a;
	pdw = pbrnn->databasic.DeltW.a;
	pdb = pbrnn->databasic.DeltB.a;


	pbrnn = (void *)pNNSLB;
	pflow = pNNSLB->FlowData.pData;

#define INIT_RNN_CELL(CELL) \
pflow = (CELL)->cell_parameter_init((CELL), pwe, pbi, pdw, pdb, pflow);\
pwe += (CELL)->cell_we.uData;\
pbi += (CELL)->cell_bi.uData;\
pdw += (CELL)->cell_we.uData;\
pdb += (CELL)->cell_bi.uData;


	INIT_RNN_CELL(pbrnn->ResetGateHandle)
	INIT_RNN_CELL(pbrnn->UpdateGateHandle)
	INIT_RNN_CELL(pbrnn->CoreCellHandle)

	//不需要训练
	if (pbrnn->NeedTrainFlag)
	{
		pbrnn->ResetGateHandle->cell_load(pbrnn->ResetGateHandle, 0);
		pbrnn->UpdateGateHandle->cell_load(pbrnn->UpdateGateHandle, 0);
		pbrnn->CoreCellHandle->cell_load(pbrnn->CoreCellHandle, 0);

	}
	return (lw_ptr)NULL;
}

lw_ptr OSlwToolBPnnLayerGruRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerGruRnnSTU *prnn;
	OSLW_assert(!(pNNSLB));
	prnn = (void *)pNNSLB;

	OSlwToolBPnnLayerFullConClear(pNNSLB);
	if(prnn->OutT_1.a)
	{
		LW_MAT_CLR(&(prnn->OutT_1));
	}
	
	return 0;

}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerGruRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pResetGateOut,
	ParaType *pUpdateGateOut,
	ParaType *pCoreCellOut,
	ParaType *pOutT_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *ptem,
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
	in_len = max_mini_batch * InCol;
	out_len = max_mini_batch*OutCol;

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

	//设置ResetGate
	pResetGateOut = (pResetGateOut) ? pResetGateOut : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	pbrnn->ResetGateHandle = new_cell_fun(pResetGateOut, InCol + OutCol, OutCol, max_mini_batch, LwSigmoid, pmem);

	//设置UpdateGate
	pUpdateGateOut = (pUpdateGateOut) ? pUpdateGateOut : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	pbrnn->UpdateGateHandle = new_cell_fun(pUpdateGateOut, InCol + OutCol, OutCol, max_mini_batch, LwSigmoid, pmem);

	//设置CoreCell
	pCoreCellOut = (pCoreCellOut) ? pCoreCellOut : (pmem->Malloc(pmem, PARA_MEM_CAL(out_len)));
	pbrnn->CoreCellHandle = new_cell_fun(pCoreCellOut, InCol + OutCol, OutCol, max_mini_batch, ptem, pmem);

	if (NeedTrainFlag == 0 && pOutT_1 == NULL)
	{
		pOutT_1 = pmem->Malloc(pmem, PARA_MEM_CAL(out_len));
	}
	OSlwToolMatrixInitial(&(pbrnn->OutT_1), max_mini_batch, OutCol, pOutT_1);

	//设置参数
	node->Bias.row = 1;
	node->Bias.col = pbrnn->ResetGateHandle->cell_bi.uData +
		pbrnn->UpdateGateHandle->cell_bi.uData +
		pbrnn->CoreCellHandle->cell_bi.uData;
	node->Bias.length = node->Bias.col;

	node->DeltB.row = 1;
	node->DeltB.col = node->Bias.col;
	node->DeltB.length = node->Bias.col;

	node->Weight.length = pbrnn->ResetGateHandle->cell_we.uData +
		pbrnn->UpdateGateHandle->cell_we.uData +
		pbrnn->CoreCellHandle->cell_we.uData;
	node->Weight.col = OutCol;
	node->Weight.row = node->Weight.length / node->Weight.col;
	
	node->DeltW.row = node->Weight.row;
	node->DeltW.col = OutCol;
	node->DeltW.length = node->Weight.length;

	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);
	node->basic.FlowData.uData = PARA_MEM_CAL(pbrnn->ResetGateHandle->FlowDataLen +
		pbrnn->UpdateGateHandle->FlowDataLen +
		pbrnn->CoreCellHandle->FlowDataLen);

	//成员函数
	node->basic.Forward = OSlwToolNNLayerGruRnnForward;
	node->basic.Backward = OSlwToolNNLayerGruRnnBackward;
	node->basic.DataInit = OSlwToolBPnnLayerGruRnnDataInit;
	node->basic.Clear = OSlwToolBPnnLayerGruRnnClear;

	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	pbrnn->NeedTrainFlag = NeedTrainFlag;

	return (OSlwToolNNSubLayerBasicSTU *)node;

}




void* OSlwToolBPnnGruRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNSubLayerBasicSTU *pnode1;
	OSlwToolNNLayerGruRnnSTU *pBRNN;
	OSlwToolNNLayerFullConSTU *pfc;
	OSlwToolDListNodeSTU *pln1;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));

	_NN_GET_IN(pBPnn, pin);

	//初始化节点

	pnode1 = OSlwToolNNLayerGruRnnNew(
		pin, pout,
		NULL, NULL, NULL, NULL,
		in_col, out_col,
		pBPnn->Train.mini_batch_max,
		pBPnn->Train.Flag.NeedTrain,
		new_cell_fun, pTemplet, pmem
	);

	pBRNN = (void *)pnode1;
	pfc = (void *)pnode1;

	pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
	ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
	ppLIST1[0] = pnode1;


	OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);

	_NN_FULL_CON_CHIP_ALLAC();

	return ppLIST1;
}


#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

