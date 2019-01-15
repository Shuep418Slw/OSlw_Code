/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.h
 *
 *  Created on: 2017-7-25
 *      Author: slw
 */

#ifndef OSLW_TOOL_CONTROL_H_
#define OSLW_TOOL_CONTROL_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)




typedef struct OSLW_TOOL_PID_CTRL_STRUCT{
 
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

typedef struct OSLW_TOOL_SOGIPLL_STRUCT{
	OSlwExternMemorySTU ExtFun;	
	OSlwParameterSTU Para_PLLOutCorrect;
	
	ParaType K_alast,K_a,K_blast,K_b;
	ParaType K_mul,K_mullast;
	ParaType K_In1,K_In2,K_In1last;
	ParaType P_BaseFreq;
	lw_sf P_IrpFreq;
	
	ParaType I_input;
	
	ParaType O_BaseRad,O_RealRad;
		
	OSlwToolPidCtrlSTU PllPid;
		
}OSlwToolSogiPllSTU;


typedef struct OSLW_TOOL_PRCTRL_STRUCT{
	OSlwExternMemorySTU ExtFun;
	OSlwParameterSTU Para_Kp;	
	OSlwParameterSTU Para_num[2];
	OSlwParameterSTU Para_den[2];
	
	ParaType I_Ref,I_Fdb,O_Uout;

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


#endif // !(OSLW_SIMPLE_LEVEL >= 3)
#endif

