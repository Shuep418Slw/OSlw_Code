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


static const ParaType _LwMathTanhTable[32][4] = { _ParaFrom(-0.323768137), _ParaFrom(0.000000000), _ParaFrom(0.999829378), _ParaFrom(0.000000012),
_ParaFrom(-0.224577786),_ParaFrom(-0.060942552), _ParaFrom(1.012310413),_ParaFrom(-0.000852027),
_ParaFrom(-0.085294811),_ParaFrom(-0.232093472), _ParaFrom(1.082413830),_ParaFrom(-0.010423480),
_ParaFrom(0.030060432),_ParaFrom(-0.444716255), _ParaFrom(1.213049268),_ParaFrom(-0.037177618),
_ParaFrom(0.093805209),_ParaFrom(-0.601375420), _ParaFrom(1.341384456),_ParaFrom(-0.072221680),
_ParaFrom(0.112109067),_ParaFrom(-0.657604872), _ParaFrom(1.398963414),_ParaFrom(-0.091875298),
_ParaFrom(0.103550306),_ParaFrom(-0.626053856), _ParaFrom(1.360193527),_ParaFrom(-0.075995152),
_ParaFrom(0.084237250),_ParaFrom(-0.542992262), _ParaFrom(1.241116425),_ParaFrom(-0.019092174),
_ParaFrom(0.063665066),_ParaFrom(-0.441875863), _ParaFrom(1.075447316), _ParaFrom(0.071385249),
_ParaFrom(0.045976471),_ParaFrom(-0.344065011), _ParaFrom(0.895162355), _ParaFrom(0.182152329),
_ParaFrom(0.032252527),_ParaFrom(-0.259745097), _ParaFrom(0.722475170), _ParaFrom(0.300040114),
_ParaFrom(0.022203363),_ParaFrom(-0.191828827), _ParaFrom(0.569473397), _ParaFrom(0.414934245),
_ParaFrom(0.015098239),_ParaFrom(-0.139444167), _ParaFrom(0.440732858), _ParaFrom(0.520398495),
_ParaFrom(0.010183947),_ParaFrom(-0.100192739), _ParaFrom(0.336229856), _ParaFrom(0.613141426),
_ParaFrom(0.006832574),_ParaFrom(-0.071365567), _ParaFrom(0.253576587), _ParaFrom(0.692135910),
_ParaFrom(0.004567905),_ParaFrom(-0.050494378), _ParaFrom(0.189460295), _ParaFrom(0.757790993),
_ParaFrom(0.003046723),_ParaFrom(-0.035540547), _ParaFrom(0.140459581), _ParaFrom(0.811312839),
_ParaFrom(0.002028966),_ParaFrom(-0.024910282), _ParaFrom(0.103449253), _ParaFrom(0.854264559),
_ParaFrom(0.001349801),_ParaFrom(-0.017399257), _ParaFrom(0.075760608), _ParaFrom(0.888288365),
_ParaFrom(0.000897363),_ParaFrom(-0.012117675), _ParaFrom(0.055208919), _ParaFrom(0.914945277),
_ParaFrom(0.000596306),_ParaFrom(-0.008418297), _ParaFrom(0.040056265), _ParaFrom(0.935633700),
_ParaFrom(0.000396132),_ParaFrom(-0.005835573), _ParaFrom(0.028948485), _ParaFrom(0.951557814),
_ParaFrom(0.000263102),_ParaFrom(-0.004037429), _ParaFrom(0.020846768), _ParaFrom(0.963725513),
_ParaFrom(0.000174724),_ParaFrom(-0.002788538), _ParaFrom(0.014963994), _ParaFrom(0.972962252),
_ParaFrom(0.000116020),_ParaFrom(-0.001922917), _ParaFrom(0.010709291), _ParaFrom(0.979933157),
_ParaFrom(0.000077044),_ParaFrom(-0.001324238), _ParaFrom(0.007644057), _ParaFrom(0.985164489),
_ParaFrom(0.000051128),_ParaFrom(-0.000910244), _ParaFrom(0.005439617), _ParaFrom(0.989077223),
_ParaFrom(0.000034046),_ParaFrom(-0.000626889), _ParaFrom(0.003872778), _ParaFrom(0.991965222),
_ParaFrom(0.000022230),_ParaFrom(-0.000423612), _ParaFrom(0.002707107), _ParaFrom(0.994193362),
_ParaFrom(0.000016157),_ParaFrom(-0.000315402), _ParaFrom(0.002064429), _ParaFrom(0.995465695),
_ParaFrom(0.000005511),_ParaFrom(-0.000119165), _ParaFrom(0.000858748), _ParaFrom(0.997934929),
_ParaFrom(0.000023128),_ParaFrom(-0.000454708), _ParaFrom(0.002989045), _ParaFrom(0.993426652) };



 ParaType OSlwToolMathTanh(ParaType _x)
{
	register ParaType y;
	register lw_u32 _i;

	if (_x>0)
	{

		if (_x>_ParaFrom(6.5536))
		{
			return _ParaFrom(1);
		}

		_i = (_ParaInt(_ParaMpy(_x, _ParaFrom(10000)))) >> 11;
		y = _ParaAdd(_LwMathTanhTable[_i][1], _ParaMpy(_x, _LwMathTanhTable[_i][0]));
		y = _ParaAdd(_LwMathTanhTable[_i][2], _ParaMpy(y, _x));
		y = _ParaAdd(_LwMathTanhTable[_i][3], _ParaMpy(y, _x));

		return y;

	}
	else if (_x<0)
	{
		if (_x<_ParaFrom(-6.5536))
		{
			return _ParaFrom(-1);
		}

		_x = -_x;

		_i = (_ParaInt(_ParaMpy(_x, _ParaFrom(10000)))) >> 11;
		y = _x;
		y = _ParaAdd(_LwMathTanhTable[_i][1], _ParaMpy(_x, _LwMathTanhTable[_i][0]));
		y = _ParaAdd(_LwMathTanhTable[_i][2], _ParaMpy(y, _x));
		y = _ParaAdd(_LwMathTanhTable[_i][3], _ParaMpy(y, _x));

		return -y;

	}

	return _ParaFrom(0);

}


 ParaType OSlwToolMathSigmoid(ParaType _x)
{

#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	register ParaType y;
	y = OSlwToolMathTanh(_x >> 1);
	return (y + _ParaFrom(1)) >> 1;
#else
	register ParaType y;
	y = OSlwToolMathTanh(_ParaMpy(_x, _ParaFrom(0.5)));
	return _ParaMpy((y + _ParaFrom(1)), _ParaFrom(0.5));
#endif

}

OSlwToolNNLayerActFunSTU _OSlwToolNNSigmoid = { _OSLW_TOOL_NN_ACT_FUN_DEFAULT(Sigmoid,OSlwToolNNLayerActFunSTU)};
OSlwToolNNLayerActFunSTU *LwSigmoid = (OSlwToolNNLayerActFunSTU *)&_OSlwToolNNSigmoid;
lw_ptr OSlwToolBPnnLayerSigmoidForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num)
{
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		//*_out=_ParaDiv(_ParaFint(1), _ParaAdd(_ParaFint(1), _ParaExp(_ParaMpy(*_in, _ParaFint(-1)))));
		*_out = OSlwToolMathSigmoid(*_in);
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
	//ParaType _exp_x,_exp_nx;
	_OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB)
		//_exp_x = _ParaExp(*_in);
		//_exp_nx = _ParaExp(-(*_in));
		//*_out = _ParaDiv(
		//	_ParaSub(_exp_x, _exp_nx),
		//	_ParaAdd(_exp_x, _exp_nx)
		//);
		*_out = OSlwToolMathTanh(*_in);
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
	temp = (*_in > _ParaFrom(0)) ? _ParaFrom(1) : _ParaFrom(0);
	temp = (*_in < _ParaFrom(6)) ? _ParaFrom(1) : _ParaFrom(0);
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
	//OSlwToolNNLayerActFunSeLUSTU *pthis;
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


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType Col,
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




#endif // OSLW_TOOL_IMPORT_NN_BPnn || OSLW_TOOL_IMPORT_ALL

#endif // OSLW_TOOL_IMPORT_NN || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

