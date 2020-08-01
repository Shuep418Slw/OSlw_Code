/*(Ver.=0.97)
 * OSLW_tool.c
 *
 *  Created on: 2019-01-22
 *      Author: slw
 */

#include "OSLW_include.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)


#if OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL


#if OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType InCol,
	LwMatColType OutCol,
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
	node->basic.Forward = OSlwToolBPnnLayerFullConForward;
	node->basic.Backward = OSlwToolBPnnLayerFullConBackward;
	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Clear = OSlwToolBPnnLayerFullConClear;
	node->basic.DataInit = OSlwToolBPnnLayerFullConDataInit;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;

	

	return (OSlwToolNNSubLayerBasicSTU *)node;

}


lw_ptr OSlwToolBPnnLayerFullConForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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


lw_ptr OSlwToolBPnnLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	register lw_u16 buf1;
	//register lw_u16 i, j, col, row;
	//register ParaType _sum;
	//register ParaType *_out, *_db, *_out_b;

	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C
	//保存maxminibatch
	buf1 = pNNSLB->out.col;

	//记录当前minibatch
	pNNSLB->out.row = mini_b_num;
	pNNSLB->in.row = mini_b_num;


	////表示第一次反向传递 update函数自动清空梯度
	//if (pNNSLB->pNN->Train._batch_stream_count <= 1)
	//{
	//	//直接覆盖
	//	//dw=in'*out
	//	pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 2);
	//	//db = sum(out, 1);按列求和
	//	//_out_b = pNNSLB->out.a;
	//	//_db = pfc->DeltB.a;
	//	//row = pNNSLB->out.col;
	//	//for (i = 0; i < pNNSLB->out.col; i++)
	//	//{
	//	//	_sum = _ParaFint(0);
	//	//	_out = _out_b;
	//	//	for (j = 0; j < (mini_b_num & 0xffff); j++)
	//	//	{
	//	//		_sum = _ParaAdd(_sum, *_out);
	//	//		_out += row;
	//	//	}
	//	//	*_db++ = _sum;
	//	//	_out_b ++;
	//	//}


	//	//xd = out*w';
	//	pOSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), 1);
	//}
	//else
	//{
	//	//采用叠加方法
	//	//dw=in'*out
	//	pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 6);
	//	//db = sum(out, 1);按列求和
	//	_out_b = pNNSLB->out.a;
	//	_db = pfc->DeltB.a;
	//	row = pNNSLB->out.col;
	//	for (i = 0; i < pNNSLB->out.col; i++)
	//	{
	//		_sum = _ParaFint(0);
	//		_out = _out_b;
	//		for (j = 0; j < (mini_b_num & 0xffff); j++)
	//		{
	//			_sum = _ParaAdd(_sum, *_out);
	//			_out += row;
	//		}
	//		*_db = _ParaAdd(*_db, _sum);
	//		_db++;
	//		_out_b++;
	//	}
	//	//xd = out*w';
	//	pOSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), 1);
	//}

	//采用叠加方法
	if (pNNSLB->pNN->Train.Flag.NeedTrain== OSlwToolNNNeedTrain_Need)
	{
		//dw=in'*out
		pOSlwToolMatrixTurnMpy(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 0x12);
		//db = sum(out, 1);按列求和
		pOSlwToolMatrixSum(&(pfc->DeltB), &(pNNSLB->out), 0x12);
	}

	//xd = out*w';
	pOSlwToolMatrixTurnMpy(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), 0x01);


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

#define LIMIT_DELT(x,MX,MN) do{if((x)>(MX)) (x)=(MX);\
else if((x)<(MN)) (x)=(MN);}while(0)


lw_ptr OSlwToolBPnnLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
    register lw_u32 i, all_batch_count;
	register ParaType k, _div_m;
	register ParaType *_mw, *_mwd;
	register ParaType *_mb, *_mbd;
	register ParaType *_vw, *_vb;
	register ParaType *_uw, *_ub;
	register OSlwToolNNLayerFullConSTU *pfc;
	register OSlwToolBPnnSTU *pBPnn;
	ParaType b1, nb1, b2, nb2, e, b1t, b2t;
	ParaType vbbuf, ubbuf, vwbuf, uwbuf;
	ParaType bbuf, wbuf;
	ParaType l2regu_k;
	ParaType temp;
	ParaType DeltMax, DeltMin;

	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;
	pBPnn = pNNSLB->pNN;

	all_batch_count = pBPnn->Train.AllBatchCount == 0 ? 1 : pBPnn->Train.AllBatchCount;
	
	_mw = pfc->Weight.a;
	_mwd = pfc->DeltW.a;
	_mb = pfc->Bias.a;
	_mbd = pfc->DeltB.a;

	l2regu_k = _ParaFint(1) - _ParaDiv(_ParaMpy(pNNSLB->nl, pNNSLB->LamdaL2), _ParaFint(all_batch_count));

	DeltMax = pBPnn->Train.DeltLimitMax;
	DeltMin = pBPnn->Train.DeltLimitMin;

	switch (pBPnn->Train.Flag.Optim)
	{
	case OSlwToolNNOptim_GradDesc:
		k = _ParaDiv(_ParaMpy(pBPnn->_nl_factor, pNNSLB->nl), _ParaFint(all_batch_count));

		if (!(pBPnn->Train.Flag.EnableDeltLimit))
		{
			//单纯梯度下降法
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _mb++, _mbd++)
			{
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, *_mwd));
				*_mb = _ParaAdd(*_mb, _ParaMpy(k, *_mbd));
				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}

			//少循环一点是一点
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, *_mwd));
				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;

			}
		}
		else
		{
			//单纯梯度下降法
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _mb++, _mbd++)
			{
				temp = _ParaMpy(k, *_mwd);
				LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);
				temp = _ParaMpy(k, *_mbd);
				LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mb = _ParaAdd(*_mb, temp);
				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}

			//少循环一点是一点
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				temp = _ParaMpy(k, *_mwd);
				LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);
				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;

			}
		}

		break;

	case OSlwToolNNOptim_M:
		_vw = pfc->DeltW.a + pfc->DeltW.length;
		_vb = pfc->DeltB.a + pfc->DeltB.length;
		b1 = pBPnn->Train.Beta1T;
		nb1 = _ParaFrom(1) - b1;

		k = _ParaMpy(pBPnn->_nl_factor, pNNSLB->nl);
		_div_m = _ParaDiv(_ParaFrom(1) , _ParaFint(all_batch_count));

		if (!(pBPnn->Train.Flag.EnableDeltLimit))
		{
			//动量法
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _vw++, _mb++, _mbd++, _vb++)
			{
				//计算出的梯度要先除以batch
				wbuf = _ParaMpy(*_mwd, _div_m);
				bbuf = _ParaMpy(*_mbd, _div_m);

				//动量法 v=0.9v+0.1d
				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_vb = _ParaMpy(*_vb, b1) + _ParaMpy(bbuf, nb1);

				//w=w+v
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, *_vw));
				*_mb = _ParaAdd(*_mb, _ParaMpy(k, *_vb));
				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				wbuf = _ParaMpy(*_mwd, _div_m);
				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, *_vw));
				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;
				_vw++;

			}
		}
		else
		{
			//动量法
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _vw++, _mb++, _mbd++, _vb++)
			{
				//计算出的梯度要先除以batch
				wbuf = _ParaMpy(*_mwd, _div_m);
				bbuf = _ParaMpy(*_mbd, _div_m);

				//动量法 v=0.9v+0.1d
				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_vb = _ParaMpy(*_vb, b1) + _ParaMpy(bbuf, nb1);

				//w=w+v
				temp = _ParaMpy(k, *_vw);LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k),temp);

				temp = _ParaMpy(k, *_vb);LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mb = _ParaAdd(*_mb, temp);
				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				wbuf = _ParaMpy(*_mwd, _div_m);
				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);

				temp = _ParaMpy(k, *_vw);LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, *_vw));
				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;
				_vw++;

			}
		}

		break;

	case OSlwToolNNOptim_RMSp:
		_uw = pfc->DeltW.a + pfc->DeltW.length;
		_ub = pfc->DeltB.a + pfc->DeltB.length;
		b2 = pBPnn->Train.Beta2T;
		nb2 = _ParaFrom(1) - b2;
		e = pBPnn->Train.Epsi;

		k = _ParaMpy(pBPnn->_nl_factor, pNNSLB->nl);
		_div_m = _ParaDiv(_ParaFrom(1) , _ParaFint(all_batch_count));


		if (!(pBPnn->Train.Flag.EnableDeltLimit))
		{
			//RMS
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _uw++, _mb++, _mbd++, _ub++)
			{
				//计算出的梯度要先除以batch
				wbuf = _ParaMpy(*_mwd, _div_m);
				bbuf = _ParaMpy(*_mbd, _div_m);

				//RMS法u=0.9u+0.1d*d
				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);
				*_ub = _ParaMpy(*_ub, b2) + _ParaMpy(_ParaMpy(bbuf, bbuf), nb2);

				temp = _ParaMpy(k, _ParaDiv(wbuf, (e + _ParaSqrt(*_uw))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);

				temp = _ParaMpy(k, _ParaDiv(bbuf, (e + _ParaSqrt(*_ub))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mb = _ParaAdd(*_mb, temp);

				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}

			//少循环一点是一点
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				wbuf = _ParaMpy(*_mwd, _div_m);
				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);
				temp = _ParaMpy(k, _ParaDiv(wbuf, (e + _ParaSqrt(*_uw))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);


				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;
				_uw++;

			}
		}
		else
		{

		}


		break;

	case OSlwToolNNOptim_Adam:
		_vw = pfc->DeltW.a + pfc->DeltW.length;
		_vb = pfc->DeltB.a + pfc->DeltB.length;
		b1 = pBPnn->Train.Beta1;
		nb1 = _ParaFrom(1) - b1;

		_uw = pfc->DeltW.a + (pfc->Weight.length<<1);
		_ub = pfc->DeltB.a + (pfc->DeltB.length<<1);
		b2 = pBPnn->Train.Beta2;
		nb2 = _ParaFrom(1) - b2;
		e = pBPnn->Train.Epsi;

		b1t = _ParaFrom(1)- pBPnn->Train.Beta1T;
		b2t = _ParaFrom(1)- pBPnn->Train.Beta2T;

		k = _ParaMpy(pBPnn->_nl_factor, pNNSLB->nl);
		_div_m = _ParaDiv(_ParaFrom(1) , _ParaFint(all_batch_count));

		if (!(pBPnn->Train.Flag.EnableDeltLimit))
		{
			//adam
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _uw++, _vw++, _mb++, _mbd++, _ub++, _vb++)
			{
				//计算出的梯度要先除以batch
				wbuf = _ParaMpy(*_mwd, _div_m);
				bbuf = _ParaMpy(*_mbd, _div_m);

				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_vb = _ParaMpy(*_vb, b1) + _ParaMpy(bbuf, nb1);

				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);
				*_ub = _ParaMpy(*_ub, b2) + _ParaMpy(_ParaMpy(bbuf, bbuf), nb2);

				vwbuf = _ParaDiv(*_vw, b1t);
				uwbuf = _ParaDiv(*_uw, b2t);
				vbbuf = _ParaDiv(*_vb, b1t);
				ubbuf = _ParaDiv(*_ub, b2t);

				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, _ParaDiv(vwbuf, (e + _ParaSqrt(uwbuf)))));
				*_mb = _ParaAdd(*_mb, _ParaMpy(k, _ParaDiv(vbbuf, (e + _ParaSqrt(ubbuf)))));

				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}

			//少循环一点是一点
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				wbuf = _ParaMpy(*_mwd, _div_m);

				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);

				vwbuf = _ParaDiv(*_vw, b1t);
				uwbuf = _ParaDiv(*_uw, b2t);

				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), _ParaMpy(k, _ParaDiv(vwbuf, (e + _ParaSqrt(uwbuf)))));
				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;
				_uw++;
				_vw++;
			}

		}
		else
		{
			//adam
			for (i = 0; i < pfc->Bias.length; i++, _mw++, _mwd++, _uw++, _vw++, _mb++, _mbd++, _ub++, _vb++)
			{
				//计算出的梯度要先除以batch
				wbuf = _ParaMpy(*_mwd, _div_m);
				bbuf = _ParaMpy(*_mbd, _div_m);

				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_vb = _ParaMpy(*_vb, b1) + _ParaMpy(bbuf, nb1);

				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);
				*_ub = _ParaMpy(*_ub, b2) + _ParaMpy(_ParaMpy(bbuf, bbuf), nb2);

				vwbuf = _ParaDiv(*_vw, b1t);
				uwbuf = _ParaDiv(*_uw, b2t);
				vbbuf = _ParaDiv(*_vb, b1t);
				ubbuf = _ParaDiv(*_ub, b2t);
				temp = _ParaMpy(k, _ParaDiv(vwbuf, (e + _ParaSqrt(uwbuf))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);

				temp = _ParaMpy(k, _ParaDiv(vbbuf, (e + _ParaSqrt(ubbuf))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mb = _ParaAdd(*_mb, temp);

				*_mwd = _ParaFint(0);
				*_mbd = _ParaFint(0);
			}

			//少循环一点是一点
			i = pfc->Weight.length - pfc->Bias.length;
			while (i--)
			{
				wbuf = _ParaMpy(*_mwd, _div_m);

				*_vw = _ParaMpy(*_vw, b1) + _ParaMpy(wbuf, nb1);
				*_uw = _ParaMpy(*_uw, b2) + _ParaMpy(_ParaMpy(wbuf, wbuf), nb2);

				vwbuf = _ParaDiv(*_vw, b1t);
				uwbuf = _ParaDiv(*_uw, b2t);

				temp = _ParaMpy(k, _ParaDiv(vwbuf, (e + _ParaSqrt(uwbuf))));LIMIT_DELT(temp, DeltMax, DeltMin);
				*_mw = _ParaAdd(_ParaMpy(*_mw, l2regu_k), temp);

				*_mwd = _ParaFint(0);
				_mw++;
				_mwd++;
				_uw++;
				_vw++;
			}

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



lw_ptr OSlwToolBPnnLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward)
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

lw_ptr OSlwToolBPnnLayerFullConClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
{
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	OSlwToolBPnnLayerClearDefault(pNNSLB);//调用基类函数
	if (pfc->DeltB.a && pfc->DeltW.a)
	{
		LW_MAT_CLR(&(pfc->DeltW));
		LW_MAT_CLR(&(pfc->DeltB));
	}

	return (lw_ptr)NULL;
}



lw_ptr OSlwToolBPnnLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB)
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

	//正则化参数
	if (pNNSLB->LamdaL2 == _ParaFint(0))
	{
		pNNSLB->LamdaL2 = pNNSLB->pNN->Train.L2Regu;
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

		if (db)
		{
			for (i = 0; i < pfc->Bias.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
			{
				*db++ = _ParaFint(0);
			}
		}

		for (i = 0; i < pfc->Weight.length; i++)
		{
			*w++ = pfun(pr, d1, d2);
		}
		if (dw)
		{
			for (i = 0; i < pfc->Weight.length*pNNSLB->pNN->Train._MemAllocCoff; i++)
			{
				*dw++ = _ParaFint(0);
			}
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

lw_ptr OSlwToolBPnnLayerFullConCopy(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2)
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


lw_ptr OSlwToolBPnnLayerFullConSoftReplace(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido)
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

void* OSlwToolBPnnFullConAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
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
	OSlwToolDListNodeSTU *pln1;
	//ParaType *pWreal, *pBreal;
	OSlwToolNNSubLayerBasicSTU **ppLIST1, **pptail;
	OSLW_assert(!(pBPnn));
	OSLW_assert(!(pmem));
	
	if (pTemplet)
	{
		_NN_GET_IN(pBPnn, pin);

		pnode1 = OSlwToolNNLayerFullConNew
		(
			pin, NULL,
			in_col, out_col,
			pBPnn->Train.mini_batch_max,
			pmem
		);

		pnode2 = OSlwToolNNLayerActFunNew(
			pnode1->out.a, pout, 
			out_col, pBPnn->Train.mini_batch_max, 
			pmem, pTemplet, 0
		);  

		pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));
		
		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 2);

		ppLIST1[0] = pnode1;
		ppLIST1[1] = pnode2;

		OSlwToolBPnnLayerAppend(pBPnn, pln1, 2, ppLIST1);

		//每次都要复制输出层
		memcpy(&(pBPnn->y), &(pnode2->out), sizeof(OSlwMat));

		pfc = (OSlwToolNNLayerFullConSTU *)pnode1;
	}
	else//采用不带激活函数的方式
	{
		_NN_GET_IN(pBPnn, pin);

		pnode1 = OSlwToolNNLayerFullConNew(
			pin, pout,
			in_col, out_col,
			pBPnn->Train.mini_batch_max,
			pmem
		);

		pln1 = pmem->Calloc(pmem, sizeof(OSlwToolDListNodeSTU));


		ppLIST1 = pmem->Malloc(pmem, sizeof(OSlwToolNNSubLayerBasicSTU *) * 1);
		ppLIST1[0] = pnode1;

		OSlwToolBPnnLayerAppend(pBPnn, pln1, 1, ppLIST1);
		
		//每次都要复制输出层
		memcpy(&(pBPnn->y), &(pnode1->out), sizeof(OSlwMat));

		pfc = (OSlwToolNNLayerFullConSTU *)pnode1;
	}


	pfc->pfun = pfun;
	pfc->pr = pr;
	pfc->initd1 = d1;
	pfc->initd2 = d2;

	_NN_FULL_CON_CHIP_ALLAC_1(pBPnn, pfc);

	//pBPnn->ParaGroupNum++;
	return ppLIST1;
}



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerShiftNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType Col,
	LwMatColType weight_len,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
)
{

	OSlwToolNNLayerFullConSTU *node;
	OSLW_assert(!(pmem));

	//分配节点内存
	node = pmem->Calloc(pmem, sizeof(OSlwToolNNLayerFullConSTU));
	node->basic.NN_Kind = OSlwToolNNSubLayerKind_Shift;


	//设置参数
	node->Bias.row = 1;
	node->Bias.col = weight_len;
	node->Bias.length = weight_len;

	node->DeltB.row = 1;
	node->DeltB.col = weight_len;
	node->DeltB.length = weight_len;

	node->Weight.row = 1;
	node->Weight.col = weight_len;
	node->Weight.length = weight_len;

	node->DeltW.row = 1;
	node->DeltW.col = weight_len;
	node->DeltW.length = weight_len;

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

	//计算要分配的内存大小
	node->basic.sizeofdata = PARA_MEM_CAL(node->Weight.length) + PARA_MEM_CAL(node->Bias.length);


	//成员函数
	node->basic.Forward = OSlwToolBPnnLayerShiftForward;
	node->basic.Backward = OSlwToolBPnnLayerShiftBackward;


	node->basic.Update = OSlwToolBPnnLayerFullConUpdate;
	node->basic.NNmalloc = OSlwToolBPnnLayerFullConNNmalloc;
	node->basic.Clear = OSlwToolBPnnLayerFullConClear;
	node->basic.DataInit = OSlwToolBPnnLayerFullConDataInit;
	node->basic.Copy = OSlwToolBPnnLayerFullConCopy;
	node->basic.SoftReplace = OSlwToolBPnnLayerFullConSoftReplace;



	return (OSlwToolNNSubLayerBasicSTU *)node;

}


lw_ptr OSlwToolBPnnLayerShiftForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	register lw_u16 buf1;
	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;

	//保存maxminibatch
	buf1 = pNNSLB->out.row;

	//记录当前minibatch
	pNNSLB->out.row = mini_b_num;
	pNNSLB->in.row = mini_b_num;


	pOSlwToolMatrixVectShift(
		&(pNNSLB->out),
		&(pfc->Weight),
		&(pNNSLB->in),
		&(pfc->Bias)
	);

	//还原
	pNNSLB->out.row = buf1;
	pNNSLB->in.row = buf1;


	return mini_b_num;

}


lw_ptr OSlwToolBPnnLayerShiftBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	register lw_u16 buf1;
	//register lw_u16 i, j, col, row;
	//register ParaType _sum;
	//register ParaType *_out, *_db, *_out_b;

	register OSlwToolNNLayerFullConSTU *pfc;
	OSLW_assert(!(pNNSLB));
	pfc = (OSlwToolNNLayerFullConSTU *)pNNSLB;


	//保存maxminibatch
	buf1 = pNNSLB->out.col;

	//记录当前minibatch
	pNNSLB->out.row = mini_b_num;
	pNNSLB->in.row = mini_b_num;

	//采用叠加方法
	if (pNNSLB->pNN->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)
	{
		//****只完成了权重为列向量的反向传递****
		//dw=sum(in.*out,2)
		pOSlwToolMatrixDotSum(&(pfc->DeltW), &(pNNSLB->in), &(pNNSLB->out), 0x12);
		//db = sum(out, 1);按列求和
		pOSlwToolMatrixSum(&(pfc->DeltB), &(pNNSLB->out), 0x12);
	}
	//xd = out.*remat(we);
	pOSlwToolMatrixVectShift(&(pNNSLB->in), &(pNNSLB->out), &(pfc->Weight), NULL);


	//还原
	pNNSLB->out.row = buf1;
	pNNSLB->in.row = buf1;


	return mini_b_num;

}



#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

