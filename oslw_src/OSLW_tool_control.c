/*(Ver.=0.97)
 * OSLW_tool.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)
#if 0
ParaType OSlwToolPidCtrlCal(OSlwToolPidCtrlSTU *p)
{
	p->K_e = _ParaSub(p->I_ref, p->I_fdb);
	p->K_up = _ParaMpy(p->K_e, p->Para_Kp.data.RealPara.Parameter);     //计算比例的输出
	p->K_upresat = _ParaAdd(_ParaAdd(p->K_up, p->K_ui), p->K_ud);   //限幅之前的输出
	if (p->K_upresat > p->P_max)         //限幅判断,同时将最终的控制量输出
		p->O_uout = p->P_max;
	else if (p->K_upresat < p->P_min)
		p->O_uout = p->P_min;
	else
		p->O_uout = p->K_upresat;
	p->K_ui += _ParaAdd(_ParaMpy(p->Para_Ki.data.RealPara.Parameter, p->K_e), _ParaMpy(p->Para_Kc.data.RealPara.Parameter, _ParaSub(p->O_uout, p->K_upresat))); //计算积分项的输出
	p->K_ud = _ParaMpy(p->Para_Kd.data.RealPara.Parameter, _ParaSub(p->K_e, p->K_eup));   //计算微分相输出
	p->K_eup = p->K_e;
	return p->O_uout;
}
//pSP->mul=_IQmpy(_IQ(2*PI*SOGI_BASE_FREQ),_IQmpy(_IQ(1.4142),pSP->InputData-pSP->alast)-pSP->blast);
//pSP->In1=pSP->In1last+_IQ14toIQ(_IQ14div(_IQtoIQ14(pSP->mul+pSP->mullast),_IQ14(2*SOGI_FREQ)));
//pSP->a=pSP->In1;
//pSP->In2=pSP->In2+_IQ21mpy(_IQtoIQ21(pSP->In1+pSP->In1last),_IQ21(1/(2*SOGI_FREQ)));
//pSP->b=_IQ21toIQ(_IQ21mpy(pSP->In2,_IQ21(2*PI*SOGI_BASE_FREQ)));
ParaType OSlwToolSogiPllCal(OSlwToolSogiPllSTU *pSP)
{
	ParaType buf;
#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	//	float temp;
	pSP->K_mul = _ParaMpy(_ParaMpy(_ParaFrom(2 * PI), pSP->P_BaseFreq), _ParaMpy(_ParaFrom(1.414), pSP->I_input - pSP->K_alast) - pSP->K_blast);
	pSP->K_In1 = pSP->K_In1last + _IQ14toIQ(_IQ14div(_IQtoIQ14(pSP->K_mul + pSP->K_mullast), _IQ14mpy(_IQ14(2), _IQ14(pSP->P_IrpFreq))));
	pSP->K_a = pSP->K_In1;
	pSP->K_In2 = pSP->K_In2 + (_IQ21mpy(_IQtoIQ21(pSP->K_In1 + pSP->K_In1last), _IQ21(1.0f / (2 * pSP->P_IrpFreq))));
	//pSP->K_b=_IQ21toIQ(_IQ21mpy((_IQ21mpy(pSP->K_In2,_IQ21(2*PI))),_IQtoIQ21(pSP->P_BaseFreq)));
	//	temp=_IQ21toF(pSP->K_In2)+_IQtoF(pSP->K_In1+pSP->K_In1last)/(2.0f*_IQtoF(pSP->P_IrpFreq));
	//	pSP->K_In2=_IQ21(temp);
	pSP->K_b = _IQ21toIQ(_IQ21mpy((_IQ21mpy(pSP->K_In2, _IQ21(2 * PI))), _IQtoIQ21(pSP->P_BaseFreq)));
	//pSP->K_b=_IQ(temp*2*PI*_IQtoF((pSP->P_BaseFreq)));
#else
	pSP->K_mul = _ParaMpy(_ParaFrom(2 * PI)*pSP->P_BaseFreq, _ParaMpy(_ParaFrom(1.414), pSP->I_input - pSP->K_alast) - pSP->K_blast);
	pSP->K_In1 = pSP->K_In1last + _ParaDiv((pSP->K_mul + pSP->K_mullast), _ParaFrom(2 * pSP->P_IrpFreq));
	pSP->K_a = pSP->K_In1;
	pSP->K_In2 = pSP->K_In2 + _ParaMpy((pSP->K_In1 + pSP->K_In1last), _ParaFrom(1 / (2 * pSP->P_IrpFreq)));
	pSP->K_b = (_ParaMpy(pSP->K_In2, _ParaFrom(_ParaFrom(2 * PI)*pSP->P_BaseFreq)));
#endif
	pSP->PllPid.I_fdb = _ParaMpy(_ParaSin(pSP->O_BaseRad), pSP->K_a) - _ParaMpy(_ParaCos(pSP->O_BaseRad), pSP->K_b);
	OSlwToolPidCtrlCal(&(pSP->PllPid));
	buf = pSP->O_BaseRad = pSP->O_BaseRad + pSP->PllPid.O_uout;
	if (buf>_ParaFrom(PI))buf = _ParaSub(buf, _ParaFrom(2 * PI));
	else if (buf<_ParaFrom(-PI)) buf = _ParaAdd(buf, _ParaFrom(2 * PI));
	else;
	pSP->O_BaseRad = buf;
	buf += pSP->Para_PLLOutCorrect.data.RealPara.Parameter;
	if (buf>_ParaFrom(PI))buf = _ParaSub(buf, _ParaFrom(2 * PI));
	else if (buf<_ParaFrom(-PI)) buf = _ParaAdd(buf, _ParaFrom(2 * PI));
	else;
	pSP->O_RealRad = buf;
	pSP->K_alast = pSP->K_a;
	pSP->K_blast = pSP->K_b;
	pSP->K_mullast = pSP->K_mul;
	pSP->K_In1last = pSP->K_In1;
	return pSP->O_RealRad;
}
//matlab SlwPRctrl_getPara.m
//function [redata] = SlwPRctrl_getPara(kr,wc,f0,fs,opt,STUname)
//%SLWPRCTRL_GETPARA
//%kr:100(recommend out range is -100 ~ +100)
//%wc:4.4(recommend out range is -100 ~ +100)
//%f0:50/60
//%fs:inerrupt freq
//%opt:code/data
//%STUname:pr ctrl struct's name default is p->
//ts=1/fs;
//wc=2*wc;
//num=[wc,0]*kr*10;
//den=[1,wc,(2*pi*f0)^2];
//[a,b]=c2dm(num,den,ts);
//if nargin==6
//    restr1=sprintf('//kr is %f and wc is %f\n',kr,wc);
//    redata=sprintf('%sPara_num[0].Parameter=_ParaFrom(%f);//*(Ver.=0.97)-1\n%sPara_num[1].Parameter=_ParaFrom(%f);//*(Ver.=0.97)-1\n%sPara_den[0].Parameter=_ParaFrom(%f);\n%sPara_den[1].Parameter=_ParaFrom(%f);\n',STUname,-b(2),STUname,-b(3),STUname,a(2),STUname,a(3));
//    redata=[restr1,redata];
//elseif nargin==5 && strcmp(opt,'code')
//    restr1=sprintf('//kr is %f and wr is %f\n',kr,wr);
//    STUname='p->';
//    redata=sprintf('%sPara_num[0].Parameter=_ParaFrom(%f);//*(Ver.=0.97)-1\n%sPara_num[1].Parameter=_ParaFrom(%f);//*(Ver.=0.97)-1\n%sPara_den[0].Parameter=_ParaFrom(%f);\n%sPara_den[1].Parameter=_ParaFrom(%f);\n',STUname,-b(2),STUname,-b(3),STUname,a(2),STUname,a(3));
//    redata=[restr1,redata];
//elseif nargin==5 && strcmp(opt,'data')
//    redata=[a;b];
//else
//    redata=tf(a,b,ts);
//end
//end
ParaType OSlwToolPrCtrlCal(OSlwToolPrCtrlSTU *p)
{
	ParaType temp_out;
	p->K_e = _ParaSub(p->I_Ref, p->I_Fdb);
	p->K_Rout = _ParaAdd(
		_ParaAdd(
			_ParaAdd(_ParaMpy(p->Para_num[0].data.RealPara.Parameter, p->K_yfia1), _ParaMpy(p->Para_num[1].data.RealPara.Parameter, p->K_yfia2))
			, _ParaMpy(p->Para_den[0].data.RealPara.Parameter, p->K_xfia1))
		, _ParaMpy(p->Para_den[1].data.RealPara.Parameter, p->K_xfia2));
	p->K_yfia2 = p->K_yfia1;
	p->K_yfia1 = p->K_Rout;
	p->K_xfia2 = p->K_xfia1;
	p->K_xfia1 = p->K_e;
	temp_out = _ParaAdd(_ParaMpy(p->Para_Kp.data.RealPara.Parameter, p->K_e), p->K_Rout);
	if (temp_out>p->P_max) temp_out = p->P_max;
	else if (temp_out<p->P_min)temp_out = p->P_min;
	else;
	p->O_Uout = temp_out;
	return p->O_Uout;
}
#endif


ParaType OSlwToolCtrlTf2Run(OSlwToolCtrlTf2STU *tf, ParaType xin)
{
	ParaType sum1;

	//延时
	tf->M_xin[2] = tf->M_xin[1];
	tf->M_xin[1] = tf->M_xin[0];
	tf->M_xin[0] = xin;

	tf->M_yout[2] = tf->M_yout[1];
	tf->M_yout[1] = tf->M_yout[0];

	//前向
	sum1 = _ParaMpy(tf->P_Num[0], tf->M_xin[0]);
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[1], tf->M_xin[1]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[2], tf->M_xin[2]));

	//后向
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[1], tf->M_yout[1]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[2], tf->M_yout[2]));

	tf->M_yout[0] = sum1;
	return sum1;

}

void OSlwToolCtrlTf2_S2Z(lw_sf bs[3], lw_sf as[3], lw_sf bz[3], lw_sf az[3], lw_sf ts)
{
	lw_sf ts2 = ts*ts;

	bz[2] = 4 * bs[0] - 2 * ts*bs[1] + ts2*bs[2];
	az[2] = 4 * as[0] - 2 * ts*as[1] + ts2*as[2];

	bz[1] = -8 * bs[0] + 2 * bs[2] * ts2;
	az[1] = -8 * as[0] + 2 * as[2] * ts2;

	bz[0] = 4 * bs[0] + 2 * bs[1] * ts + bs[2] * ts2;
	az[0] = 4 * as[0] + 2 * as[1] * ts + as[2] * ts2;

	bz[2] /= az[0];
	az[2] /= az[0];
	bz[1] /= az[0];
	az[1] /= az[0];
	bz[0] /= az[0];
	az[0] /= az[0];
	return;
}

ParaType OSlwToolCtrlTf3Run(OSlwToolCtrlTf3STU *tf, ParaType xin)
{
	ParaType sum1;

	//延时
	tf->M_xin[3] = tf->M_xin[2];
	tf->M_xin[2] = tf->M_xin[1];
	tf->M_xin[1] = tf->M_xin[0];
	tf->M_xin[0] = xin;

	tf->M_yout[3] = tf->M_yout[2];
	tf->M_yout[2] = tf->M_yout[1];
	tf->M_yout[1] = tf->M_yout[0];

	//前向
	sum1 = _ParaMpy(tf->P_Num[0], tf->M_xin[0]);
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[1], tf->M_xin[1]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[2], tf->M_xin[2]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[3], tf->M_xin[3]));

	//后向
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[1], tf->M_yout[1]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[2], tf->M_yout[2]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[3], tf->M_yout[3]));

	tf->M_yout[0] = sum1;
	return sum1;
}


ParaType OSlwToolCtrlTf4Run(OSlwToolCtrlTf4STU *tf, ParaType xin)
{
	ParaType sum1;

	//延时
	tf->M_xin[4] = tf->M_xin[3];
	tf->M_xin[3] = tf->M_xin[2];
	tf->M_xin[2] = tf->M_xin[1];
	tf->M_xin[1] = tf->M_xin[0];
	tf->M_xin[0] = xin;

	tf->M_yout[4] = tf->M_yout[3];
	tf->M_yout[3] = tf->M_yout[2];
	tf->M_yout[2] = tf->M_yout[1];
	tf->M_yout[1] = tf->M_yout[0];

	//前向
	sum1 = _ParaMpy(tf->P_Num[0], tf->M_xin[0]);
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[1], tf->M_xin[1]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[2], tf->M_xin[2]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[3], tf->M_xin[3]));
	sum1 = _ParaAdd(sum1, _ParaMpy(tf->P_Num[4], tf->M_xin[4]));

	//后向
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[1], tf->M_yout[1]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[2], tf->M_yout[2]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[3], tf->M_yout[3]));
	sum1 = _ParaSub(sum1, _ParaMpy(tf->P_Den[4], tf->M_yout[4]));

	tf->M_yout[0] = sum1;
	return sum1;
}


ParaType OSlwToolCtrlPidRun(OSlwToolCtrlPidSTU *p, ParaType ref, ParaType fdb)
{
	ParaType err_kp;
	ParaType err_ki, err_i_sum; 
	ParaType err_kd, derr;

	p->I_ref = ref;
	p->I_fdb = fdb;

	err_kp = _ParaSub(_ParaMpy(p->I_ref, p->P_b), p->I_fdb);
	err_ki = _ParaSub(p->I_ref, p->I_fdb);
	err_kd = _ParaSub(_ParaMpy(p->I_ref, p->P_c), p->I_fdb);

	p->M_up= _ParaMpy(err_kp, p->P_Kp);     //计算比例的输出

	//梯形公式 计算积分
	err_i_sum = _ParaAdd(err_ki, p->M_err_i_1);
	err_i_sum = _ParaMpy(_ParaDiv(err_i_sum, 2), p->P_Ki);
	p->M_ui= _ParaAdd(p->M_ui, err_i_sum);

	p->M_err_i_1 = err_ki;//保存梯形公式上一轮

	//积分器限幅
	if (p->M_ui > p->P_max)
		p->M_ui = p->P_max;
	else if (p->M_ui < p->P_min)
		p->M_ui = p->P_min;
	else;

	//KD计算
	derr = _ParaSub(err_kd, p->M_err_d_1);//微分
	p->M_derr_filter = _ParaAdd(_ParaMpy(p->P_N, p->M_derr_filter), 
		_ParaMpy(_ParaSub(_ParaFrom(1),p->P_N), derr));//惯性滤波

	p->M_ud = _ParaMpy(p->P_Kd, p->M_derr_filter);//计算微分输出
	p->M_err_d_1 = err_kd;//保存上一轮

	p->O_uout = _ParaAdd(p->M_up, _ParaAdd(p->M_ud, p->M_ui));

	//输出限幅
	if (p->O_uout > p->P_max)
		p->O_uout = p->P_max;
	else if (p->O_uout < p->P_min)
		p->O_uout = p->P_min;
	else;

	return p->O_uout;
}

void OSlwToolCtrlPrInit(OSlwToolCtrlPrSTU *p, lw_sf kp, lw_sf kr, lw_sf wc, lw_sf f0, lw_sf fs, lw_sf Pmax)
{
	lw_sf temp = (lw_sf)(_ParaMpy(_ParaFrom(2 * PI),f0));
	lw_sf ts;
	p->P_Kp = kp;
	p->P_Kr = kr;
	p->P_Wc = wc;
	p->P_F0 = f0;
	p->P_Fs = fs;
	p->P_Pmax = _ParaFrom(Pmax);

	p->a_s[0] = 1;
	p->a_s[1] = 2 * wc;
	p->a_s[2] = temp*temp;

	p->b_s[0] = kp*p->a_s[0];
	p->b_s[1] = kp*p->a_s[1] + wc*kr * 2;
	p->b_s[2] = kp*p->a_s[2];

	ts = 1 / fs;
	OSlwToolCtrlTf2_S2Z(p->b_s, p->a_s, p->xcoff, p->ycoff, ts);
	memset(&(p->kr_ctrl), 0, sizeof(p->kr_ctrl));

	p->kr_ctrl.P_Num[2] = _ParaFrom(p->xcoff[2]);
	p->kr_ctrl.P_Num[1] = _ParaFrom(p->xcoff[1]);
	p->kr_ctrl.P_Num[0] = _ParaFrom(p->xcoff[0]);

	p->kr_ctrl.P_Den[2] = _ParaFrom(p->ycoff[2]);
	p->kr_ctrl.P_Den[1] = _ParaFrom(p->ycoff[1]);
	p->kr_ctrl.P_Den[0] = _ParaFrom(p->ycoff[0]);

}


ParaType OSlwToolCtrlPrRun(OSlwToolCtrlPrSTU *p, ParaType ref, ParaType fdb)
{
	p->Uout = OSlwToolCtrlTf2Run((void *)&(p->kr_ctrl), _ParaSub(ref, fdb));
	if (p->Uout>p->P_Pmax)
	{
		p->Uout = p->P_Pmax;
	}
	else if (p->Uout<-p->P_Pmax)
	{
		p->Uout = -p->P_Pmax;
	}

	return p->Uout;
}


void OSlwToolCtrlSogiRun(OSlwToolCtrlSogiSTU *pSP, ParaType xin)
{
	ParaType err;
	ParaType temp1;
	ParaType deltT;

	pSP->I_Xin = xin;
	temp1 = _ParaMpy(pSP->P_wc, _ParaFrom(SQRT_2));
	deltT = _ParaDiv(_ParaFrom(1), _ParaMpy(_ParaFint(2), pSP->P_IrpFreq));

	err = _ParaMpy(temp1, (xin - pSP->M_alast)) - pSP->M_blast;

	pSP->M_mul = _ParaMpy(_ParaMpy(_ParaFrom(2 * PI), pSP->P_BaseFreq), err);
	pSP->M_In1 = pSP->M_In1last + _ParaMpy((pSP->M_mul + pSP->M_mullast), deltT);

	pSP->O_a = pSP->M_In1;
	pSP->M_In2 = pSP->M_In2 + _ParaMpy((pSP->M_In1 + pSP->M_In1last), deltT);
	pSP->O_b = _ParaMpy(_ParaMpy(pSP->M_In2,_ParaFrom(2 * PI)),pSP->P_BaseFreq);

	pSP->M_alast = pSP->O_a;
	pSP->M_blast = pSP->O_b;
	pSP->M_mullast = pSP->M_mul;
	pSP->M_In1last = pSP->M_In1;

	return;
}

#endif // !(OSLW_SIMPLE_LEVEL >= 3)
