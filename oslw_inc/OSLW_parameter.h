/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_parameter.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_PARAMETER_H_
#define OSLW_PARAMETER_H_

#include "OSLW_define.h"
/*(Ver.=0.9~)(Beg.=0.9)

核心理念：
参数: 数据
操作: 动作列表（I/O）
命令: 执行相应动作

*/



//<宏组说明>数据类型定义 定义参数所用的类型</宏组说明>
//#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
//	 typedef fixpoint ParaType;
//#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_FLOAT
//	 typedef float ParaType;
//#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_DOUBLE
//	 typedef double ParaType;
//#else
//	#error "MATH TYPE IS NOT DEFINED"
//#endif


//------------------------------------------
//<结构体名>OSlwParameterDataSTU</结构体名>
//<成员变量>
//Parameter:实际参数
//ParameterShallow:实际参数的影子寄存器
//delt:参数的增量
//Q:格式化因子 用于格式化
//</成员变量>
//------------------------------------------
typedef struct OSLW_PARAMETER_DATA_STRUCT{
	
	ParaType Parameter;
	ParaType ParameterShallow;
	ParaUnitStatusNUM ParaStatus;
	
	ParaType delt;
	ParaType F_b,F_k;
	ParaType ParaMax,ParaMin;
	
	lw_u8 Q;
	ParaType *pExtParameter;
		
}OSlwParameterDataSTU;


typedef struct OSLW_PARAMETER_ORDER_STRUCT{
	union{
		ParaType real_data;
		lw_u8 para_arr[5];
		lw_8 *pstr;
		OSlwExternMemorySTU ext;		
	}data;
	
	OSlwParaOrderNUM order;
	
}OSlwParameterOrderSTU;

typedef struct OSLW_PARAMETER_ORDER_LIST_STRUCT{

	lw_u8 RowId;
	lw_u8 OrderDir;
	OSlwParameterOrderSTU UOrder;
	OSlwParaListOrderNUM LOrder;
	OSlwExternMemorySTU ExtMem;
	
}OSlwParameterListOrderSTU;

#define OSLW_LIST_ORDER_GET 0

#define OSLW_LIST_ORDER_SET 1

typedef	union OSLW_PARAMETER_DATA_UNION{
		OSlwParameterDataSTU RealPara;
		OSlwExternMemorySTU ImagPara;
}OSlwParameterDataUN;

typedef struct OSLW_PARAMETER_EXE_STRUCT{
	lw_u8 len;
	void* (*(*ExeFunList))(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
}OSlwParameterExeSTU;

typedef struct OSLW_PARAMETER_STRUCT{
	
	lw_u16 ParaId;
	lw_u8 *pParaName;	

	OSlwParameterDataUN data;
	
}OSlwParameterSTU;


typedef struct OSLW_PARA_CONTROL_STRUCT{
	lw_8 Name[OSLW_PAPA_CTRL_NAME_LENGTH];
	lw_u8 PageId;
	lw_u8 ParaLength;
	OSlwParameterSTU *pParaList[OSLW_PARA_LIST_LENGRH];
	struct OSLW_CORE_STRUCT *pOS;
	
	OSlwParameterExeSTU ParaGetF;
	OSlwParameterExeSTU ParaSetF;
	
}OSlwParaCtrlSTU;

typedef OSlwParaCtrlSTU OSlwParaListCtrlSTU;
typedef void* (*pOSlwExeFun)(OSlwParameterDataUN*,OSlwParameterOrderSTU*,void*);

extern const pOSlwExeFun OSlwRealGetFunList[];
extern const pOSlwExeFun OSlwRealSetFunList[];

//函数声明
/*(Ver.=0.9~)(Beg.=0.9)
OSlwParameterSTU *OSlwParameter_GetPrecision(OSlwParameterSTU *p,lw_u8 arr[5],lw_u8 aim);
OSlwParameterSTU *OSlwParameter_GetNormal(OSlwParameterSTU *p,lw_u8 *arr,lw_u8 aim);

OSlwParameterSTU *OSlwParameter_SetPrecision(OSlwParameterSTU *p,lw_u8 arr[5],lw_u8 aim);
OSlwParameterSTU *OSlwParameter_SetNormal(OSlwParameterSTU *p,lw_u8 *arr,lw_u8 aim);

OSlwParameterSTU *OSlwParameter_SelfSL(OSlwParameterSTU *p,lw_u8 cmd);
*/
#define OSLW_PARA_SELF_CMD_SAVE 0
#define OSLW_PARA_SELF_CMD_LOAD 1
/*(Ver.=0.9~)(Beg.=0.9)
OSlwParameterSTU *OSlwParameter_SelfUp(OSlwParameterSTU *p,lw_u8 aim);
OSlwParameterSTU *OSlwParameter_SelfDown(OSlwParameterSTU *p,lw_u8 aim);
*/

void *OSlwParameter_GetPrecision(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_GetNormal(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_GetData(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);


void *OSlwParameter_SetPrecision(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_SetNormal(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_SetData(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_SelfUp(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_SelfDown(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_Lock(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);
void *OSlwParameter_Unlock(OSlwParameterDataUN *pPD,OSlwParameterOrderSTU *pPO,void *pBuf);



lw_u8 OSlwRealParaListInitial(OSlwParaListCtrlSTU *pPLC,lw_8 *str);



OSlwParaCtrlSTU *OSlwParaCtrlCmdExe(OSlwParaCtrlSTU *pPC,void *pGU);




#define OSLW_PARA_LIMIT(D,MA,MI) 	do{if((MA) != (MI))\
{\
	(D)=(D)>(MA)?(MA):(D);\
	(D)=(D)<(MI)?(MI):(D);	\
}}while(0)

#define OSLW_PARA_DEFAULT {0,0,0,0,0,0,0,0,0,NULL,NULL\
}
#define OSLW_PARA_INIT(PARAM,DELT,B,K,M,N,EXTP) {0,_ParaFrom(PARAM),0,_ParaFrom(DELT),_ParaFrom(B),_ParaFrom(K),_ParaFrom(M),_ParaFrom(N),15,NULL,EXTP\
}

#define OSLW_PAPA_GET_DATA(DATA) 	if(p->pExtParameter)\
		DATA=aim?(*(p->pExtParameter)):(p->ParameterShallow);\
	else\
		DATA=aim?(p->Parameter):(p->ParameterShallow)

#define OSLW_PAPA_GET_PDATA(PDATA) 	if(p->pExtParameter)\
		PDATA=aim?(p->pExtParameter):(&(p->ParameterShallow));\
	else\
		PDATA=aim?(&(p->Parameter)):(&(p->ParameterShallow))


#define OSLW_PAPA_JUDGE_DATA(DATA) 		if(pPD->RealPara.pExtParameter)\
		DATA=pPD->RealPara.ParaStatus?(pPD->RealPara.ParameterShallow):(*(pPD->RealPara.pExtParameter));\
	else\
		DATA=pPD->RealPara.ParaStatus?(pPD->RealPara.ParameterShallow):(pPD->RealPara.Parameter);


#define OSLW_PAPA_JUDGE_PDATA(PDATA) 	if(pPD->RealPara.pExtParameter)\
		PDATA=pPD->RealPara.ParaStatus?(&(pPD->RealPara.ParameterShallow)):(pPD->RealPara.pExtParameter);\
	else\
		PDATA=pPD->RealPara.ParaStatus?(&(pPD->RealPara.ParameterShallow)):(&(pPD->RealPara.Parameter));


#define OSLW_PARA_GET_FUN_READY() 	ParaType data;\
	if(pPD==NULL || pPO== NULL)\
	{\
		return NULL;\
	}\
	if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect)\
	{\
		return NULL;\
	}\
	OSLW_PAPA_JUDGE_DATA(data);

	
#define OSLW_PARA_SET_FUN_READY() 		\
	ParaType *pdata;\
	ParaType data;\
	if(pPD==NULL || pPO== NULL) return NULL;\
	if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect)return NULL;\
	OSLW_PAPA_JUDGE_PDATA(pdata);

	
#define OLSW_PARA_EXE_FUN_NAME(NAME) void*NAME(OSlwParameterDataUN*pPD,OSlwParameterOrderSTU*pPO,void*pBuf)	
	
#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_PARAMETER_H*/
