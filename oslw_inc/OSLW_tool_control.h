/*(Ver.=0.97)
 * OSLW_tool.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef OSLW_TOOL_CONTROL_H_
#define OSLW_TOOL_CONTROL_H_

#include "OSLW_define.h"

#if !(OSLW_SIMPLE_LEVEL >= 3)
#if 0
typedef struct OSLW_TOOL_PID_CTRL_STRUCT {
	OSlwExternMemorySTU ExtFun;
	OSlwParameterSTU Para_Kp;
	OSlwParameterSTU Para_Ki;
	OSlwParameterSTU Para_Kd;
	OSlwParameterSTU Para_Kc;
	ParaType K_up;
	ParaType K_ui;
	ParaType K_ud;
	ParaType I_ref;
	ParaType I_fdb;
	ParaType K_e;
	ParaType K_eup;
	ParaType K_upresat;
	ParaType O_uout;
	ParaType P_max;
	ParaType P_min;
	ParaType P_normal;
}OSlwToolPidCtrlSTU;
ParaType OSlwToolPidCtrlCal(OSlwToolPidCtrlSTU *p);
typedef struct OSLW_TOOL_SOGIPLL_STRUCT {
	OSlwExternMemorySTU ExtFun;
	OSlwParameterSTU Para_PLLOutCorrect;
	ParaType K_alast, K_a, K_blast, K_b;
	ParaType K_mul, K_mullast;
	ParaType K_In1, K_In2, K_In1last;
	ParaType P_BaseFreq;
	lw_sf P_IrpFreq;
	ParaType I_input;
	ParaType O_BaseRad, O_RealRad;
	OSlwToolPidCtrlSTU PllPid;
}OSlwToolSogiPllSTU;
typedef struct OSLW_TOOL_PRCTRL_STRUCT {
	OSlwExternMemorySTU ExtFun;
	OSlwParameterSTU Para_Kp;
	OSlwParameterSTU Para_num[2];
	OSlwParameterSTU Para_den[2];
	ParaType I_Ref, I_Fdb, O_Uout;
	ParaType P_max;
	ParaType P_min;
	ParaType K_Rout;
	ParaType K_e;
	ParaType K_yfia1;
	ParaType K_yfia2;
	ParaType K_xfia1;
	ParaType K_xfia2;
}OSlwToolPrCtrlSTU;
ParaType OSlwToolPrCtrlCal(OSlwToolPrCtrlSTU *p);
#endif


typedef struct OSLW_TOOL_CTRL_TF2_STRUCT {
	ParaType P_Num[3];
	ParaType P_Den[3];
	ParaType M_xin[3];
	ParaType M_yout[3];
}OSlwToolCtrlTf2STU;

ParaType OSlwToolCtrlTf2Run(OSlwToolCtrlTf2STU *tf, ParaType xin);
#define LwRunTf2(tf2,xin) OSlwToolCtrlTf2Run(tf2,xin);
void OSlwToolCtrlTf2_S2Z(lw_sf bs[3], lw_sf as[3], lw_sf bz[3], lw_sf az[3], lw_sf ts);

typedef OSlwToolCtrlTf2STU OSlwCtrlTf2;
typedef OSlwToolCtrlTf2STU LwTf2;

//Den2 应该为1
#define LwTf2_Default(Num2,Num1,Num0,Den2,Den1,Den0) {\
_ParaFrom(Num2), _ParaFrom(Num1), _ParaFrom(Num0),\
_ParaFrom(Den2), _ParaFrom(Den1), _ParaFrom(Den0),\
0,0,0,\
0,0,0\
}

typedef struct OSLW_TOOL_CTRL_TF3_STRUCT {
	ParaType P_Num[4];
	ParaType P_Den[4];
	ParaType M_xin[4];
	ParaType M_yout[4];
}OSlwToolCtrlTf3STU;

ParaType OSlwToolCtrlTf3Run(OSlwToolCtrlTf3STU *tf, ParaType xin);
#define LwRunTf3(tf3,xin) OSlwToolCtrlTf3Run(tf3,xin);

typedef OSlwToolCtrlTf3STU OSlwCtrlTf3;
typedef OSlwToolCtrlTf3STU LwTf3;

//Den3 应该为1
#define LwTf3_Default(Num3,Num2,Num1,Num0,Den3,Den2,Den1,Den0) {\
_ParaFrom(Num3),_ParaFrom(Num2), _ParaFrom(Num1), _ParaFrom(Num0),\
_ParaFrom(Den3),_ParaFrom(Den2), _ParaFrom(Den1), _ParaFrom(Den0),\
0,0,0,0,\
0,0,0,0\
}

typedef struct OSLW_TOOL_CTRL_TF4_STRUCT {
	ParaType P_Num[5];
	ParaType P_Den[5];
	ParaType M_xin[5];
	ParaType M_yout[5];
}OSlwToolCtrlTf4STU;

ParaType OSlwToolCtrlTf4Run(OSlwToolCtrlTf4STU *tf, ParaType xin);
#define LwRunTf4(tf4,xin) OSlwToolCtrlTf4Run(tf4,xin);

typedef OSlwToolCtrlTf4STU OSlwCtrlTf4;
typedef OSlwToolCtrlTf4STU LwTf4;

//Den4 应该为1
#define LwTf4_Default(Num4,Num3,Num2,Num1,Num0,Den4,Den3,Den2,Den1,Den0) {\
_ParaFrom(Num4),_ParaFrom(Num3),_ParaFrom(Num2), _ParaFrom(Num1), _ParaFrom(Num0),\
_ParaFrom(Den4),_ParaFrom(Den3),_ParaFrom(Den2), _ParaFrom(Den1), _ParaFrom(Den0),\
0,0,0,0,0\
0,0,0,0,0\
}

typedef struct OSLW_TOOL_CTRL_PID_STRUCT {

	ParaType P_Kp;
	ParaType P_Ki;
	ParaType P_Kd;

	ParaType P_min;
	ParaType P_max;

	ParaType P_b;
	ParaType P_c;
	ParaType P_N;

	ParaType I_ref;
	ParaType I_fdb;

	ParaType M_up;
	ParaType M_ui;
	ParaType M_ud;
	ParaType M_err_d_1;
	ParaType M_err_i_1;
	ParaType M_derr_filter;
	ParaType O_uout;

}OSlwToolCtrlPidSTU;


ParaType OSlwToolCtrlPidRun(OSlwToolCtrlPidSTU *p, ParaType ref, ParaType fdb);
#define LwRunPid(pid,ref,fdb) OSlwToolCtrlTf4Run(pid,ref,fdb);


typedef OSlwToolCtrlPidSTU OSlwCtrlPid;
typedef OSlwToolCtrlPidSTU LwPid;

#define LwPid_Default(KP,KI,KD,PMIN,PMAX,B,C,N){\
_ParaFrom(KP), _ParaFrom(KI), _ParaFrom(KD), _ParaFrom(PMIN), _ParaFrom(PMAX), _ParaFrom(B), _ParaFrom(C), _ParaFrom(N),\
0,0,\
0,0,0,0,0,0,0\
}

#define LwSimplePid_Default(KP,KI,KD,PMIN,PMAX) LwCtrlPid_Default(KP,KI,KD,PMIN,PMAX,1,1,0)


typedef struct OSLW_TOOL_CTRL_PR_STRUCT {
	lw_sf P_Kp;
	lw_sf P_Kr;
	lw_sf P_Wc;//带宽
	lw_sf P_F0;//基波频率
	lw_sf P_Fs;//运算频率

	OSlwToolCtrlTf2STU kr_ctrl;
	ParaType P_Pmax;//范围
	ParaType Uout;

	//s域系数
	lw_sf b_s[3];
	lw_sf a_s[3];

	//z域系数
	lw_sf xcoff[3];
	lw_sf ycoff[3];

}OSlwToolCtrlPrSTU;

typedef OSlwToolCtrlPrSTU OslwCtrlPr;
typedef OSlwToolCtrlPrSTU LwPr;


void OSlwToolCtrlPrInit(OSlwToolCtrlPrSTU *p, lw_sf kp, lw_sf kr, lw_sf wc, lw_sf f0, lw_sf fs, lw_sf Pmax);
ParaType OSlwToolCtrlPrRun(OSlwToolCtrlPrSTU *p, ParaType ref, ParaType fdb);

#define LwRunPr(pid,ref,fdb) OSlwToolCtrlPrRun(pid,ref,fdb);

typedef struct OSLW_TOOL_CTRL_SOGI_STRUCT {
	ParaType P_BaseFreq;//基波频率
	ParaType P_wc;//带宽因子
	ParaType P_IrpFreq;//运算频率

	ParaType I_Xin;

	ParaType M_alast, O_a, M_blast, O_b;//O_a与O_b分别为输出A与输出B
	ParaType M_mul, M_mullast;
	ParaType M_In1, M_In2, M_In1last;

}OSlwToolCtrlSogiSTU;

typedef OSlwToolCtrlSogiSTU OslwCtrlSogi;
typedef OSlwToolCtrlSogiSTU LwSogi;

void OSlwToolCtrlSogiRun(OSlwToolCtrlSogiSTU *pSP, ParaType xin);
#define LwRunSogi(p,x) OSlwToolCtrlSogiRun(p,x);

#define LwSogi_Default(f0,wc,fs) {\
_ParaFrom(f0), _ParaFrom(wc), _ParaFrom(fs),\
0,\
0,0,0,0\
0,0,\
0,0,0,\
}

#define LwRunPark22(A,B,D,Q,T) do{\
	(D)=_ParaMpy((A),_ParaCos(T))+_ParaMpy((B),_ParaSin(T));\
	(Q)=_ParaMpy((-(A)),_ParaSin(T))+_ParaMpy((B),_ParaCos(T));\
}while(0);

#define LwRunIPark22(A,B,D,Q,T) do{\
	(A)=_ParaMpy((D),_ParaCos(T))-_ParaMpy((Q),_ParaSin(T));\
	(B)=_ParaMpy((D),_ParaSin(T))+_ParaMpy((Q),_ParaCos(T));\
}while(0);

#endif // !(OSLW_SIMPLE_LEVEL >= 3)
#endif

