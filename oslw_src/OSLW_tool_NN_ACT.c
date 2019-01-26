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



OSlwToolNNLayerActFunSTU _OSlwToolNNSigmoid = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(Sigmoid,OSlwToolNNLayerActFunSTU)};
OSlwToolNNLayerActFunSTU *LwSigmoid = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSigmoid;
lw_ptr OSlwToolBPnnLayerSigmoidForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out=_ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMpy(*_in, _ParaFint(-1)))));
		*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPnnLayerSigmoidBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerTanhForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerTanhBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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
lw_ptr OSlwToolBPnnLayerReLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
		*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPnnLayerReLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerReLU6Forward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_out = (*_in > _ParaInt(0)) ? *_in : _ParaInt(0);
	*_out = (*_in < _ParaInt(6)) ? *_in : _ParaInt(6);
	*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)

}

lw_ptr OSlwToolBPnnLayerReLU6Backward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerSwishForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerSwishBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerSeLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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


lw_ptr OSlwToolBPnnLayerSeLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerSoftMaxForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
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

lw_ptr OSlwToolBPnnLayerSoftMaxBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{

	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
	*_in = *_out;
	_OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB, mini_b_num)
}




#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

