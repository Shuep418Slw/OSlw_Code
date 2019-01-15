/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.c
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)

ParaType OSlwToolPidCtrlCal(OSlwToolPidCtrlSTU *p)
{
    p->K_e=_ParaSub(p->I_ref,p->I_fdb);

    p->K_up = _ParaMpy(p->K_e,p->Para_Kp.data.RealPara.Parameter);     //计算比例的输出
    p->K_upresat=_ParaAdd(_ParaAdd(p->K_up,p->K_ui),p->K_ud);   //限幅之前的输出

    if( p->K_upresat > p->P_max )         //限幅判断,同时将最终的控制量输出
        p->O_uout = p->P_max;
    else if( p->K_upresat < p->P_min )
        p->O_uout = p->P_min;
    else
        p->O_uout = p->K_upresat;

    p->K_ui+=_ParaAdd(_ParaMpy(p->Para_Ki.data.RealPara.Parameter,p->K_e), _ParaMpy(p->Para_Kc.data.RealPara.Parameter,_ParaSub(p->O_uout,p->K_upresat))); //计算积分项的输出
    p->K_ud = _ParaMpy(p->Para_Kd.data.RealPara.Parameter,_ParaSub(p->K_e,p->K_eup));   //计算微分相输出
    p->K_eup=	p->K_e;
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
    pSP->K_mul=_ParaMpy(_ParaMpy(_ParaFrom(2*PI),pSP->P_BaseFreq),_ParaMpy(_ParaFrom(1.414),pSP->I_input-pSP->K_alast)-pSP->K_blast);
    pSP->K_In1=pSP->K_In1last+_IQ14toIQ(_IQ14div(_IQtoIQ14(pSP->K_mul+pSP->K_mullast),_IQ14mpy(_IQ14(2),_IQ14(pSP->P_IrpFreq))));
    pSP->K_a=pSP->K_In1;
    pSP->K_In2=pSP->K_In2+(_IQ21mpy(_IQtoIQ21(pSP->K_In1+pSP->K_In1last),_IQ21(1.0f/(2*pSP->P_IrpFreq))));
    //pSP->K_b=_IQ21toIQ(_IQ21mpy((_IQ21mpy(pSP->K_In2,_IQ21(2*PI))),_IQtoIQ21(pSP->P_BaseFreq)));

//	temp=_IQ21toF(pSP->K_In2)+_IQtoF(pSP->K_In1+pSP->K_In1last)/(2.0f*_IQtoF(pSP->P_IrpFreq));
//	pSP->K_In2=_IQ21(temp);
    pSP->K_b=_IQ21toIQ(_IQ21mpy((_IQ21mpy(pSP->K_In2,_IQ21(2*PI))),_IQtoIQ21(pSP->P_BaseFreq)));
    //pSP->K_b=_IQ(temp*2*PI*_IQtoF((pSP->P_BaseFreq)));

#else

    pSP->K_mul=_ParaMpy(_ParaFrom(2*PI)*pSP->P_BaseFreq,_ParaMpy(_ParaFrom(1.414),pSP->I_input-pSP->K_alast)-pSP->K_blast);
    pSP->K_In1=pSP->K_In1last+_ParaDiv((pSP->K_mul+pSP->K_mullast),_ParaFrom(2*pSP->P_IrpFreq));
    pSP->K_a=pSP->K_In1;
    pSP->K_In2=pSP->K_In2+_ParaMpy((pSP->K_In1+pSP->K_In1last),_ParaFrom(1/(2*pSP->P_IrpFreq)));
    pSP->K_b=(_ParaMpy(pSP->K_In2,_ParaFrom(_ParaFrom(2*PI)*pSP->P_BaseFreq)));

#endif
    pSP->PllPid.I_fdb=_ParaMpy(_ParaSin(pSP->O_BaseRad),pSP->K_a)-_ParaMpy(_ParaCos(pSP->O_BaseRad),pSP->K_b);
    OSlwToolPidCtrlCal(&(pSP->PllPid));
    buf=pSP->O_BaseRad=pSP->O_BaseRad+pSP->PllPid.O_uout;


    if(buf>_ParaFrom(PI))buf=_ParaSub(buf,_ParaFrom(2*PI));
    else if(buf<_ParaFrom(-PI)) buf=_ParaAdd(buf,_ParaFrom(2*PI));
    else;

    pSP->O_BaseRad=buf;

    buf+=pSP->Para_PLLOutCorrect.data.RealPara.Parameter;

    if(buf>_ParaFrom(PI))buf=_ParaSub(buf,_ParaFrom(2*PI));
    else if(buf<_ParaFrom(-PI)) buf=_ParaAdd(buf,_ParaFrom(2*PI));
    else;

    pSP->O_RealRad=buf;

    pSP->K_alast=pSP->K_a;
    pSP->K_blast=pSP->K_b;
    pSP->K_mullast=pSP->K_mul;
    pSP->K_In1last=pSP->K_In1;

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
//    redata=sprintf('%sPara_num[0].Parameter=_ParaFrom(%f);//*(Ver.=0.9~)(Beg.=0.9)-1\n%sPara_num[1].Parameter=_ParaFrom(%f);//*(Ver.=0.9~)(Beg.=0.9)-1\n%sPara_den[0].Parameter=_ParaFrom(%f);\n%sPara_den[1].Parameter=_ParaFrom(%f);\n',STUname,-b(2),STUname,-b(3),STUname,a(2),STUname,a(3));
//    redata=[restr1,redata];
//elseif nargin==5 && strcmp(opt,'code')
//    restr1=sprintf('//kr is %f and wr is %f\n',kr,wr);
//    STUname='p->';
//    redata=sprintf('%sPara_num[0].Parameter=_ParaFrom(%f);//*(Ver.=0.9~)(Beg.=0.9)-1\n%sPara_num[1].Parameter=_ParaFrom(%f);//*(Ver.=0.9~)(Beg.=0.9)-1\n%sPara_den[0].Parameter=_ParaFrom(%f);\n%sPara_den[1].Parameter=_ParaFrom(%f);\n',STUname,-b(2),STUname,-b(3),STUname,a(2),STUname,a(3));
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
    p->K_e=_ParaSub(p->I_Ref,p->I_Fdb);

    p->K_Rout=_ParaAdd(
                  _ParaAdd(
                      _ParaAdd(
                          _ParaMpy(p->Para_num[0].data.RealPara.Parameter,p->K_yfia1),_ParaMpy(p->Para_num[1].data.RealPara.Parameter,p->K_yfia2))
                      ,_ParaMpy(p->Para_den[0].data.RealPara.Parameter,p->K_xfia1))
                  ,_ParaMpy(p->Para_den[1].data.RealPara.Parameter,p->K_xfia2));

    p->K_yfia2=p->K_yfia1;

    p->K_yfia1=p->K_Rout;

    p->K_xfia2=p->K_xfia1;

    p->K_xfia1=p->K_e;

    temp_out=_ParaAdd(_ParaMpy(p->Para_Kp.data.RealPara.Parameter,p->K_e),p->K_Rout);

    if(temp_out>p->P_max) temp_out=p->P_max;
    else if (temp_out<p->P_min)temp_out=p->P_min;
    else;

    p->O_Uout=temp_out;

    return p->O_Uout;
}
#endif // !(OSLW_SIMPLE_LEVEL >= 3)