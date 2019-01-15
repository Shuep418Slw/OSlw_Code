/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_parameter.c
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */
#include "OSLW_include.h"


const pOSlwExeFun OSlwRealGetFunList[]= {OSlwParameter_GetNormal,OSlwParameter_GetPrecision,OSlwParameter_GetData};
const pOSlwExeFun OSlwRealSetFunList[]= {OSlwParameter_SetNormal,OSlwParameter_SetPrecision,OSlwParameter_SetData,OSlwParameter_SelfUp,OSlwParameter_SelfDown,OSlwParameter_Lock,OSlwParameter_Unlock};

/*(Ver.=0.9~)(Beg.=0.9)
//------------------------------------------
//<函数名>OSlwParameter_GetPrecision</函数名>
//<功能说明>将parameter转换成高精度格式 采用定点数进行存储 一共5字节 实际数值=(arr[1]*2^24+arr[2]*2^16+arr[3]*2^8+arr[4])/(2^arr[0])</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标实际寄存器或者矫正寄存器</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

OSlwParameterSTU *OSlwParameter_GetPrecision(OSlwParameterSTU *p,lw_u8 arr[5],lw_u8 aim)
{

	ParaType data;
	fixpoint buf;
	if(p==NULL || arr== NULL)
	{
		return NULL;
	}
	OSLW_PAPA_GET_DATA(data);


	buf=_ParaInt(_ParaMpy(_ParaAdd(data,p->F_b),p->F_k));


#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_DOUBLE
	buf=(fixpoint)(data*(double)(1<<p->Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
	buf=(fixpoint)(data*(float)(1<<p->Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_Q
	buf=FP_A_TO_FP_B(OSLW_GLOBAL_MATH_Q_FORM,(p->Q),(data));
#else
	buf=0;
#endif
	*arr++=p->Q;
	*arr++=((buf&0xff000000)>>24);
	*arr++=((buf&0x00ff0000)>>16);
	*arr++=((buf&0x0000ff00)>>8);
	*arr++=((buf&0x000000ff)>>0);
	return p;
}


//------------------------------------------
//<函数名>OSlwParameter_GetNormal</函数名>
//<功能说明>将parameter转换成普通格式 数值=(参数+F_b)*F_k 再由OSLW_PARA_NORMAL_LENGTH确定格式化之后的长度</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标实际寄存器或者影子寄存器</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

OSlwParameterSTU *OSlwParameter_GetNormal(OSlwParameterSTU *p,lw_u8 *arr,lw_u8 aim)
{
	ParaType data;
	lw_32 buf;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}
	OSLW_PAPA_GET_DATA(data);
	buf=_ParaInt(_ParaMpy(_ParaAdd(data,p->F_b),p->F_k));

#if OSLW_PARA_NORMAL_LENGTH >= 4
	*arr++ = (buf&0xff000000)>>24;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 3
	*arr++ = (buf&0x00ff0000)>>16;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 2
	*arr++ = (buf&0x0000ff00)>>8;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 1
	*arr++ = (buf&0x000000ff);
#endif
#if  OSLW_PARA_NORMAL_LENGTH<=0
	#error "OSLW PARA NORMAL LENGTH is too short"
#endif
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_SetPrecision</函数名>
//<功能说明>OSlwParameter_GetPrecision相反</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_SetPrecision(OSlwParameterSTU *p,lw_u8 arr[5],lw_u8 aim)
{
	ParaType *pdata;
	lw_64 buf=0;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}

	OSLW_PAPA_GET_PDATA(pdata);

	buf|=(((lw_64)arr[1]<<24)|((lw_64)arr[2]<<16)|((lw_64)arr[3]<<8)|((lw_64)arr[4]));
	*pdata=_ParaDiv(_ParaFint(buf) , (_ParaFint(((lw_32)1<<arr[0]))));

	return p;
}


//------------------------------------------
//<函数名>OSlwParameter_SetNormal</函数名>
//<功能说明>OSlwParameter_GetNormal相反</功能说明>
//<输入说明>p:this  arr[]:目标地址 aim:目标</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_SetNormal(OSlwParameterSTU *p,lw_u8 *arr,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;
	lw_64 buf=0;

	if(p==NULL || arr== NULL)
	{
		return NULL;
	}

	OSLW_PAPA_GET_PDATA(pdata);

#if OSLW_PARA_NORMAL_LENGTH >= 4
	buf|=((lw_32)(*arr++))<<24;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 3
	buf|=((lw_32)(*arr++))<<16;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 2
	buf|=((lw_32)(*arr++))<<8;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 1
	buf|=((lw_32)(*arr++));
#endif
	data=_ParaSub(_ParaDiv(_ParaFint(buf),p->F_k),p->F_b);

	OSLW_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;

	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_set</函数名>
//<功能说明>将参数影子寄存器存入实际参数寄存器</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_SelfSL(OSlwParameterSTU *p,lw_u8 cmd)
{
	ParaType *pdata;
	OSLW_assert(!p);
	pdata=(p->pExtParameter)?p->pExtParameter:&(p->Parameter);

	if(cmd)
		*pdata=p->ParameterShallow;
	else
		p->ParameterShallow=*pdata;
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_SelfUp</函数名>
//<功能说明>参数自增 parameter=parameter+delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------

OSlwParameterSTU *OSlwParameter_SelfUp(OSlwParameterSTU *p,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;



	if(p==NULL)
	{
		return NULL;
	}

	OSLW_PAPA_GET_PDATA(pdata);

	data=_ParaAdd((*pdata),p->delt);

	OSLW_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;
	return p;
}

//------------------------------------------
//<函数名>OSlwParameter_SelfDown</函数名>
//<功能说明>参数自增 parameter=parameter-delt</功能说明>
//<输入说明>p:this</输入说明>
//<输出说明>p:this</输出说明>
//------------------------------------------
OSlwParameterSTU *OSlwParameter_SelfDown(OSlwParameterSTU *p,lw_u8 aim)
{
	ParaType *pdata;
	ParaType data;

	if(p==NULL)
	{
		return NULL;
	}

	OSLW_PAPA_GET_PDATA(pdata);

	data=_ParaSub((*pdata),p->delt);

	OSLW_PARA_LIMIT(data,p->ParaMax,p->ParaMin);
	*pdata=data;

	return p;
}
*/


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_GetPrecision)
{

    fixpoint buf;
    lw_u8 *arr;
    OSLW_PARA_GET_FUN_READY();
    arr=pPO->data.para_arr;
    buf=_ParaInt(_ParaMpy(_ParaAdd(data,pPD->RealPara.F_b),pPD->RealPara.F_k));


#if OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_DOUBLE
    buf=(fixpoint)(data*(double)(1<<pPD->RealPara.Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_FLOAT
    buf=(fixpoint)(data*(float)(1<<pPD->RealPara.Q));
#elif OSLW_GLOBAL_MATH_TYPE == OSLW_GLOBAL_MATH_Q
    buf=FP_A_TO_FP_B(OSLW_GLOBAL_MATH_Q_FORM,(p->Q),(data));
#else
    buf=0;
#endif
    *arr++=pPD->RealPara.Q;
    *arr++=((buf&0xff000000)>>24);
    *arr++=((buf&0x00ff0000)>>16);
    *arr++=((buf&0x0000ff00)>>8);
    *arr++=((buf&0x000000ff)>>0);

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_GetNormal)
{

    lw_32 buf;
    lw_u8 *arr;
    OSLW_PARA_GET_FUN_READY();
    arr=pPO->data.para_arr;
    buf=_ParaInt(_ParaMpy(_ParaAdd(data,pPD->RealPara.F_b),pPD->RealPara.F_k));

#if OSLW_PARA_NORMAL_LENGTH >= 4
    *arr++ = (buf&0xff000000)>>24;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 3
    *arr++ = (buf&0x00ff0000)>>16;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 2
    *arr++ = (buf&0x0000ff00)>>8;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 1
    *arr++ = (buf&0x000000ff);
#endif
#if  OSLW_PARA_NORMAL_LENGTH<=0
#error "OSLW PARA NORMAL LENGTH is too short"
#endif
    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_SetPrecision)
{
    lw_64 buf=0;
    lw_u8 *arr;
    OSLW_PARA_SET_FUN_READY();
    arr=pPO->data.para_arr;

    buf|=(((lw_64)arr[1]<<24)|((lw_64)arr[2]<<16)|((lw_64)arr[3]<<8)|((lw_64)arr[4]));
    *pdata=_ParaDiv(_ParaFint(buf), (_ParaFint(((lw_32)1<<arr[0]))));

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_SetNormal)
{

    lw_64 buf=0;
    lw_u8 *arr;
    OSLW_PARA_SET_FUN_READY();
    arr=pPO->data.para_arr;


#if OSLW_PARA_NORMAL_LENGTH >= 4
    buf|=((lw_32)(*arr++))<<24;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 3
    buf|=((lw_32)(*arr++))<<16;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 2
    buf|=((lw_32)(*arr++))<<8;
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 1
    buf|=((lw_32)(*arr++));
#endif

    data=_ParaSub(_ParaDiv(_ParaFint(buf),pPD->RealPara.F_k),pPD->RealPara.F_b);

    OSLW_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_GetData)
{
    OSLW_PARA_GET_FUN_READY();

    pPO->data.real_data=data;

    return pPD;
}


OLSW_PARA_EXE_FUN_NAME(OSlwParameter_SetData)
{
    OSLW_PARA_SET_FUN_READY();

    data=pPO->data.real_data;

    OSLW_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(OSlwParameter_SelfUp)
{
    OSLW_PARA_SET_FUN_READY();

    data=_ParaAdd(*pdata,pPD->RealPara.delt);

    OSLW_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(OSlwParameter_SelfDown)
{
    OSLW_PARA_SET_FUN_READY();

    data=_ParaSub(*pdata,pPD->RealPara.delt);

    OSLW_PARA_LIMIT(data,pPD->RealPara.ParaMax,pPD->RealPara.ParaMin);

    *pdata=data;

    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(OSlwParameter_Lock)
{
    ParaType data;
    if(pPD==NULL || pPO== NULL)
    {
        return NULL;
    }
    if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect || pPD->RealPara.ParaStatus == ParaUnitStatus_Lock)
    {
        return NULL;
    }

    data=(pPD->RealPara.pExtParameter)?(*(pPD->RealPara.pExtParameter)):pPD->RealPara.Parameter;
    pPD->RealPara.ParaStatus=ParaUnitStatus_Lock;
    pPD->RealPara.ParameterShallow=data;
    return pPD;
}

OLSW_PARA_EXE_FUN_NAME(OSlwParameter_Unlock)
{
    ParaType *pdata;
    if(pPD==NULL || pPO== NULL)
    {
        return NULL;
    }
    if(pPD->RealPara.ParaStatus == ParaUnitStatus_Protect || pPD->RealPara.ParaStatus == ParaUnitStatus_Free)
    {
        return NULL;
    }

    pdata=(pPD->RealPara.pExtParameter)?((pPD->RealPara.pExtParameter)):(&(pPD->RealPara.Parameter));

    pPD->RealPara.ParaStatus=ParaUnitStatus_Free;
    if(pPO->order == ParaOrder_I_UnlockSave)
        *pdata=pPD->RealPara.ParameterShallow;

    return pPD;


}


void* OSlwParaListCtrlFun(OSlwParaListCtrlSTU *pPLC,OSlwParameterListOrderSTU *pPLO)
{
    OSlwParameterSTU *pP;
    OSlwParameterDataUN *pPD;
    OSlwParameterOrderSTU *pPO;
    OSlwParameterExeSTU *pPE;
    lw_8 i=0,n;
    lw_u8 *arr;

    OSLW_assert(!(pPLC));
    OSLW_assert(!(pPLO));

    if(pPLO->RowId > pPLC->ParaLength)
    {
        return NULL;
    }
    else
    {
        pP=pPLC->pParaList[pPLO->RowId];
    }

    if(pPLO->OrderDir == OSLW_LIST_ORDER_GET)//判断指令方向
    {
        pPE=&pPLC->ParaGetF;
    }
    else
    {
        pPE=&pPLC->ParaSetF;
    }


    switch(pPLO->LOrder)
    {
    case ParaLOrder_Once:

        pPD=&(pP->data);
        pPO=&(pPLO->UOrder);

        if(pPLO->UOrder.order < pPE->len )
        {
            pPE->ExeFunList[pPLO->UOrder.order](pPD,pPO,NULL);
        }

        break;

    case ParaLOrder_More:
        if(pPLO->ExtMem.pData && (pPLO->UOrder.order) < pPE->len && (pPLO->RowId + pPLO->ExtMem.uData) <= pPLC->ParaLength)
        {
            arr=pPLO->ExtMem.pData;
            n=pPLO->RowId+pPLO->ExtMem.uData/OSLW_PARA_NORMAL_LENGTH;
            pPO=&(pPLO->UOrder);
            for(i=pPLO->RowId; i<n; i++)
            {
                pPE->ExeFunList[pPLO->UOrder.order](&(pPLC->pParaList[i]->data),pPO,NULL);
#if OSLW_PARA_NORMAL_LENGTH >= 4
                *arr++ = pPO->data.para_arr[3];
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 3
                *arr++ = pPO->data.para_arr[2];
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 2
                *arr++ = pPO->data.para_arr[1];
#endif
#if OSLW_PARA_NORMAL_LENGTH >= 1
                *arr++ = pPO->data.para_arr[0];
#endif
            }

        }

        break;

    default:

        break;


    }
	return pPLC;
}



lw_u8 OSlwRealParaInitial(OSlwParameterSTU *pP,lw_u8 *str,lw_df data,ParaType *pExt,lw_u8 num,...)
{

    va_list ap;
    ParaType *pp;
    OSLW_assert(!(pP));

    memset(pP,0,sizeof(OSlwParameterSTU));

    pP->data.RealPara.ParaStatus=ParaUnitStatus_Protect;

    pp=&pP->data.RealPara.delt;

    pP->data.RealPara.ParameterShallow = pP->data.RealPara.Parameter=_ParaFrom(data);
    pP->data.RealPara.pExtParameter=pExt;



    va_start(ap, num);

    if(num>=0 && num<=5)
    {
        while(num--)
        {
            *pp++=_ParaFrom(va_arg(ap,lw_df));
        }
    }
    pP->data.RealPara.Q=15;


    va_end(ap);

    pP->data.RealPara.ParaStatus=ParaUnitStatus_Free;

    return 1;

}



lw_u8 OSlwRealParaListInitial(OSlwParaListCtrlSTU *pPLC,lw_8 *str)
{

    OSLW_assert(!pPLC);
    memset(pPLC,0,sizeof(OSlwParaListCtrlSTU));
    //名字copy
    strncpy(pPLC->Name,str,sizeof(lw_8)*OSLW_PAPA_CTRL_NAME_LENGTH);

    //函数列表初始化
    pPLC->ParaGetF.ExeFunList=(void *)OSlwRealGetFunList;
    pPLC->ParaGetF.len=sizeof(OSlwRealGetFunList)/sizeof(pOSlwExeFun);
    pPLC->ParaGetF.ExeFunList=(void *)OSlwRealSetFunList;
    pPLC->ParaGetF.len=sizeof(OSlwRealSetFunList)/sizeof(pOSlwExeFun);


    return 1;
}

