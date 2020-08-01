
/*(Ver.=0.97)
* OSLW_tool.h
*
*  Created on: 2017-7-25
*      Author: slw
*/

#ifndef OSLW_TOOL_VECTOR_MATH_H_
#define OSLW_TOOL_VECTOR_MATH_H_


#include "OSLW_define.h"
#include "OSLW_tool_define.h"


#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL


//设置向量为一个数字
#define LW_VEXTOR_SET_TYPE2(Y,K,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(K);\
	(Y)[1]=(K);\
	(Y)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)=(K);\
}while(0)

#define LW_VEXTOR_SET_TYPE4(Y,K,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(K);\
	(Y)[1]=(K);\
	(Y)[2]=(K);\
	(Y)[3]=(K);\
	(Y)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)=(K);\
}while(0)

#define LW_VEXTOR_SET_TYPE8(Y,K,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(K);\
	(Y)[1]=(K);\
	(Y)[2]=(K);\
	(Y)[3]=(K);\
	(Y)[4]=(K);\
	(Y)[5]=(K);\
	(Y)[6]=(K);\
	(Y)[7]=(K);\
	(Y)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)=(K);\
}while(0)


#define LW_VEXTOR_SET_TYPE16(Y,K,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(K);\
	(Y)[1]=(K);\
	(Y)[2]=(K);\
	(Y)[3]=(K);\
	(Y)[4]=(K);\
	(Y)[5]=(K);\
	(Y)[6]=(K);\
	(Y)[7]=(K);\
	(Y)[8]=(K);\
	(Y)[9]=(K);\
	(Y)[10]=(K);\
	(Y)[11]=(K);\
	(Y)[12]=(K);\
	(Y)[13]=(K);\
	(Y)[14]=(K);\
	(Y)[15]=(K);\
	(Y)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)=(K);\
}while(0)

#if CACHE_READ_NUM==2
#define LwVectSet(y,k,lenDiv,lenMod) LW_VEXTOR_SET_TYPE2(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectSet(y,k,lenDiv,lenMod) LW_VEXTOR_SET_TYPE4(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectSet(y,k,lenDiv,lenMod) LW_VEXTOR_SET_TYPE8(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectSet(y,k,lenDiv,lenMod) LW_VEXTOR_SET_TYPE16(y,k,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif





#define LW_VEXTOR_SUM_TYPE2(SUM,P,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=((P)[0]);\
	(SUM)+=((P)[1]);\
	(P)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=(*(P)++);\
}while(0)

#define LW_VEXTOR_SUM_TYPE4(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=((P)[0]);\
	(SUM)+=((P)[1]);\
	(SUM)+=((P)[2]);\
	(SUM)+=((P)[3]);\
	(P)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=(*(P)++);\
}while(0)


#define LW_VEXTOR_SUM_TYPE8(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=((P)[0]);\
	(SUM)+=((P)[1]);\
	(SUM)+=((P)[2]);\
	(SUM)+=((P)[3]);\
	(SUM)+=((P)[4]);\
	(SUM)+=((P)[5]);\
	(SUM)+=((P)[6]);\
	(SUM)+=((P)[7]);\
	(P)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=(*(P)++);\
}while(0)

#define LW_VEXTOR_SUM_TYPE16(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=((P)[0]);\
	(SUM)+=((P)[1]);\
	(SUM)+=((P)[2]);\
	(SUM)+=((P)[3]);\
	(SUM)+=((P)[4]);\
	(SUM)+=((P)[5]);\
	(SUM)+=((P)[6]);\
	(SUM)+=((P)[7]);\
	(SUM)+=((P)[8]);\
	(SUM)+=((P)[9]);\
	(SUM)+=((P)[10]);\
	(SUM)+=((P)[11]);\
	(SUM)+=((P)[12]);\
	(SUM)+=((P)[13]);\
	(SUM)+=((P)[14]);\
	(SUM)+=((P)[15]);\
	(P)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=(*(P)++);\
}while(0)

#if CACHE_READ_NUM==2
#define LwVectSum(out,v1,lenDiv,lenMod) LW_VEXTOR_SUM_TYPE2(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectSum(out,v1,lenDiv,lenMod) LW_VEXTOR_SUM_TYPE4(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectSum(out,v1,lenDiv,lenMod) LW_VEXTOR_SUM_TYPE8(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectSum(out,v1,lenDiv,lenMod) LW_VEXTOR_SUM_TYPE16(out,v1,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//Y=Y+P.*Q
#define LW_VEXTOR_DOTSUM_TYPE2(Y,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy((P)[0],(Q)[0]);\
	(Y)[1]+=_ParaMpy((P)[1],(Q)[1]);\
	(P)+=2;(Q)+=2;(Y)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)

#define LW_VEXTOR_DOTSUM_TYPE4(Y,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy((P)[0],(Q)[0]);\
	(Y)[1]+=_ParaMpy((P)[1],(Q)[1]);\
	(Y)[2]+=_ParaMpy((P)[2],(Q)[2]);\
	(Y)[3]+=_ParaMpy((P)[3],(Q)[3]);\
	(P)+=4;(Q)+=4;(Y)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)


#define LW_VEXTOR_DOTSUM_TYPE8(Y,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy((P)[0],(Q)[0]);\
	(Y)[1]+=_ParaMpy((P)[1],(Q)[1]);\
	(Y)[2]+=_ParaMpy((P)[2],(Q)[2]);\
	(Y)[3]+=_ParaMpy((P)[3],(Q)[3]);\
	(Y)[4]+=_ParaMpy((P)[4],(Q)[4]);\
	(Y)[5]+=_ParaMpy((P)[5],(Q)[5]);\
	(Y)[6]+=_ParaMpy((P)[6],(Q)[6]);\
	(Y)[7]+=_ParaMpy((P)[7],(Q)[7]);\
	(P)+=8;(Q)+=8;(Y)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)

#define LW_VEXTOR_DOTSUM_TYPE16(Y,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy((P)[0],(Q)[0]);\
	(Y)[1]+=_ParaMpy((P)[1],(Q)[1]);\
	(Y)[2]+=_ParaMpy((P)[2],(Q)[2]);\
	(Y)[3]+=_ParaMpy((P)[3],(Q)[3]);\
	(Y)[4]+=_ParaMpy((P)[4],(Q)[4]);\
	(Y)[5]+=_ParaMpy((P)[5],(Q)[5]);\
	(Y)[6]+=_ParaMpy((P)[6],(Q)[6]);\
	(Y)[7]+=_ParaMpy((P)[7],(Q)[7]);\
	(Y)[8]+=_ParaMpy((P)[8],(Q)[8]);\
	(Y)[9]+=_ParaMpy((P)[9],(Q)[9]);\
	(Y)[10]+=_ParaMpy((P)[10],(Q)[10]);\
	(Y)[11]+=_ParaMpy((P)[11],(Q)[11]);\
	(Y)[12]+=_ParaMpy((P)[12],(Q)[12]);\
	(Y)[13]+=_ParaMpy((P)[13],(Q)[13]);\
	(Y)[14]+=_ParaMpy((P)[14],(Q)[14]);\
	(Y)[15]+=_ParaMpy((P)[15],(Q)[15]);\
	(P)+=16;(Q)+=16;(Y)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)

#if CACHE_READ_NUM==2
#define LwVectDotSum(y,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOTSUM_TYPE2(y,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectDotSum(y,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOTSUM_TYPE4(y,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectDotSum(y,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOTSUM_TYPE8(y,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectDotSum(y,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOTSUM_TYPE16(y,v1,v2,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//计算向量点积 与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8

#define LW_VEXTOR_DOT_TYPE2(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=(ParaType)cblas_ddot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE4(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=(ParaType)cblas_ddot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE8(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=(ParaType)cblas_ddot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE16(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=(ParaType)cblas_ddot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#else


#define LW_VEXTOR_DOT_TYPE2(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=(ParaType)cblas_sdot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE4(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=(ParaType)cblas_sdot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE8(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=(ParaType)cblas_sdot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_DOT_TYPE16(SUM,P,Q,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=(ParaType)cblas_sdot(_V_real_L1, (void *)(P), 1, (void *)(Q), 1);\
(SUM) = _res;\
(P) += _V_real_L1;(Q) += _V_real_L1;\
}while (0)

#endif//if PARA_LEN == 8


#else

#define LW_VEXTOR_DOT_TYPE2(SUM,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(Q)[0]);\
	(SUM)+=_ParaMpy((P)[1],(Q)[1]);\
	(P)+=2;(Q)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
(SUM)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)

#define LW_VEXTOR_DOT_TYPE4(SUM,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(Q)[0]);\
	(SUM)+=_ParaMpy((P)[1],(Q)[1]);\
	(SUM)+=_ParaMpy((P)[2],(Q)[2]);\
	(SUM)+=_ParaMpy((P)[3],(Q)[3]);\
	(P)+=4;(Q)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
(SUM)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)


#define LW_VEXTOR_DOT_TYPE8(SUM,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(Q)[0]);\
	(SUM)+=_ParaMpy((P)[1],(Q)[1]);\
	(SUM)+=_ParaMpy((P)[2],(Q)[2]);\
	(SUM)+=_ParaMpy((P)[3],(Q)[3]);\
	(SUM)+=_ParaMpy((P)[4],(Q)[4]);\
	(SUM)+=_ParaMpy((P)[5],(Q)[5]);\
	(SUM)+=_ParaMpy((P)[6],(Q)[6]);\
	(SUM)+=_ParaMpy((P)[7],(Q)[7]);\
	(P)+=8;(Q)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
(SUM)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)


#define LW_VEXTOR_DOT_TYPE16(SUM,P,Q,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(Q)[0]);\
	(SUM)+=_ParaMpy((P)[1],(Q)[1]);\
	(SUM)+=_ParaMpy((P)[2],(Q)[2]);\
	(SUM)+=_ParaMpy((P)[3],(Q)[3]);\
	(SUM)+=_ParaMpy((P)[4],(Q)[4]);\
	(SUM)+=_ParaMpy((P)[5],(Q)[5]);\
	(SUM)+=_ParaMpy((P)[6],(Q)[6]);\
	(SUM)+=_ParaMpy((P)[7],(Q)[7]);\
	(SUM)+=_ParaMpy((P)[8],(Q)[8]);\
	(SUM)+=_ParaMpy((P)[9],(Q)[9]);\
	(SUM)+=_ParaMpy((P)[10],(Q)[10]);\
	(SUM)+=_ParaMpy((P)[11],(Q)[11]);\
	(SUM)+=_ParaMpy((P)[12],(Q)[12]);\
	(SUM)+=_ParaMpy((P)[13],(Q)[13]);\
	(SUM)+=_ParaMpy((P)[14],(Q)[14]);\
	(SUM)+=_ParaMpy((P)[15],(Q)[15]);\
	(P)+=16;(Q)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
(SUM)+=_ParaMpy(*(P)++,*(Q)++);\
}while(0)


#endif

#if CACHE_READ_NUM==2
#define LwVectDot(out,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOT_TYPE2(out,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectDot(out,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOT_TYPE4(out,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectDot(out,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOT_TYPE8(out,v1,v2,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectDot(out,v1,v2,lenDiv,lenMod) LW_VEXTOR_DOT_TYPE16(out,v1,v2,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//计算向量1范数 与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8


#define LW_VEXTOR_ASUM_TYPE2(SUM,P,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=cblas_dasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE4(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=cblas_dasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE8(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=cblas_dasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE16(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=cblas_dasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)


#else

#define LW_VEXTOR_ASUM_TYPE2(SUM,P,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=cblas_sasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE4(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=cblas_sasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE8(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=cblas_sasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ASUM_TYPE16(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=cblas_sasum (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0)

#endif // PARA_LEN==8


#else

#define LW_VEXTOR_ASUM_TYPE2(SUM,P,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaAbs((P)[0]);\
	(SUM)+=_ParaAbs((P)[1]);\
	(P)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=_ParaAbs(*(P));++(P);\
}while(0)

#define LW_VEXTOR_ASUM_TYPE4(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaAbs((P)[0]);\
	(SUM)+=_ParaAbs((P)[1]);\
	(SUM)+=_ParaAbs((P)[2]);\
	(SUM)+=_ParaAbs((P)[3]);\
	(P)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=_ParaAbs(*(P));++(P);\
}while(0)


#define LW_VEXTOR_ASUM_TYPE8(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaAbs((P)[0]);\
	(SUM)+=_ParaAbs((P)[1]);\
	(SUM)+=_ParaAbs((P)[2]);\
	(SUM)+=_ParaAbs((P)[3]);\
	(SUM)+=_ParaAbs((P)[4]);\
	(SUM)+=_ParaAbs((P)[5]);\
	(SUM)+=_ParaAbs((P)[6]);\
	(SUM)+=_ParaAbs((P)[7]);\
	(P)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=_ParaAbs(*(P));++(P);\
}while(0)


#define LW_VEXTOR_ASUM_TYPE16(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaAbs((P)[0]);\
	(SUM)+=_ParaAbs((P)[1]);\
	(SUM)+=_ParaAbs((P)[2]);\
	(SUM)+=_ParaAbs((P)[3]);\
	(SUM)+=_ParaAbs((P)[4]);\
	(SUM)+=_ParaAbs((P)[5]);\
	(SUM)+=_ParaAbs((P)[6]);\
	(SUM)+=_ParaAbs((P)[7]);\
	(SUM)+=_ParaAbs((P)[8]);\
	(SUM)+=_ParaAbs((P)[9]);\
	(SUM)+=_ParaAbs((P)[10]);\
	(SUM)+=_ParaAbs((P)[11]);\
	(SUM)+=_ParaAbs((P)[12]);\
	(SUM)+=_ParaAbs((P)[13]);\
	(SUM)+=_ParaAbs((P)[14]);\
	(SUM)+=_ParaAbs((P)[15]);\
	(P)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(SUM)+=_ParaAbs(*(P));++(P);\
}while(0)

#endif

#if CACHE_READ_NUM==2
#define LwVectAsum(out,v1,lenDiv,lenMod) LW_VEXTOR_ASUM_TYPE2(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectAsum(out,v1,lenDiv,lenMod) LW_VEXTOR_ASUM_TYPE4(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectAsum(out,v1,lenDiv,lenMod) LW_VEXTOR_ASUM_TYPE8(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectAsum(out,v1,lenDiv,lenMod) LW_VEXTOR_ASUM_TYPE16(out,v1,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif


//计算向量2范数 与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8

#define LW_VEXTOR_NORM_TYPE2(SUM,P,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=cblas_dnrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE4(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=cblas_dnrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE8(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=cblas_dnrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE16(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=cblas_dnrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#else

#define LW_VEXTOR_NORM_TYPE2(SUM,P,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
ParaType _res=cblas_snrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE4(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
ParaType _res=cblas_snrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE8(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
ParaType _res=cblas_snrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#define LW_VEXTOR_NORM_TYPE16(SUM,P,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
ParaType _res=cblas_snrm2 (_V_real_L1, (void *)(P), 1);\
(SUM) = _res;\
(P) += _V_real_L1;\
}while (0);

#endif // PARA_LEN==8



#else

#define LW_VEXTOR_NORM_TYPE2(SUM,P,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(P)[0]);\
	(SUM)+=_ParaMpy((P)[1],(P)[1]);\
	(P)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
{(SUM)+=_ParaMpy((P)[0],(P)[0]);(P)++;}\
(SUM)=_ParaSqrt((SUM));\
}while(0)

#define LW_VEXTOR_NORM_TYPE4(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(P)[0]);\
	(SUM)+=_ParaMpy((P)[1],(P)[1]);\
	(SUM)+=_ParaMpy((P)[2],(P)[2]);\
	(SUM)+=_ParaMpy((P)[3],(P)[3]);\
	(P)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
{(SUM)+=_ParaMpy((P)[0],(P)[0]);(P)++;}\
(SUM)=_ParaSqrt((SUM));\
}while(0)


#define LW_VEXTOR_NORM_TYPE8(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(P)[0]);\
	(SUM)+=_ParaMpy((P)[1],(P)[1]);\
	(SUM)+=_ParaMpy((P)[2],(P)[2]);\
	(SUM)+=_ParaMpy((P)[3],(P)[3]);\
	(SUM)+=_ParaMpy((P)[4],(P)[4]);\
	(SUM)+=_ParaMpy((P)[5],(P)[5]);\
	(SUM)+=_ParaMpy((P)[6],(P)[6]);\
	(SUM)+=_ParaMpy((P)[7],(P)[7]);\
	(P)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
{(SUM)+=_ParaMpy((P)[0],(P)[0]);(P)++;}\
(SUM)=_ParaSqrt((SUM));\
}while(0)


#define LW_VEXTOR_NORM_TYPE16(SUM,P,DIV,MOD)  do{ \
LwMatLenType _v_i1=(DIV);\
(SUM)=_ParaFint(0);\
while (_v_i1--){\
	(SUM)+=_ParaMpy((P)[0],(P)[0]);\
	(SUM)+=_ParaMpy((P)[1],(P)[1]);\
	(SUM)+=_ParaMpy((P)[2],(P)[2]);\
	(SUM)+=_ParaMpy((P)[3],(P)[3]);\
	(SUM)+=_ParaMpy((P)[4],(P)[4]);\
	(SUM)+=_ParaMpy((P)[5],(P)[5]);\
	(SUM)+=_ParaMpy((P)[6],(P)[6]);\
	(SUM)+=_ParaMpy((P)[7],(P)[7]);\
	(SUM)+=_ParaMpy((P)[8],(P)[8]);\
	(SUM)+=_ParaMpy((P)[9],(P)[9]);\
	(SUM)+=_ParaMpy((P)[10],(P)[10]);\
	(SUM)+=_ParaMpy((P)[11],(P)[11]);\
	(SUM)+=_ParaMpy((P)[12],(P)[12]);\
	(SUM)+=_ParaMpy((P)[13],(P)[13]);\
	(SUM)+=_ParaMpy((P)[14],(P)[14]);\
	(SUM)+=_ParaMpy((P)[15],(P)[15]);\
	(P)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
{(SUM)+=_ParaMpy((P)[0],(P)[0]);(P)++;}\
(SUM)=_ParaSqrt((SUM));\
}while(0)

#endif


#if CACHE_READ_NUM==2
#define LwVectNorm2(out,v1,lenDiv,lenMod) LW_VEXTOR_NORM_TYPE2(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectNorm2(out,v1,lenDiv,lenMod) LW_VEXTOR_NORM_TYPE4(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectNorm2(out,v1,lenDiv,lenMod) LW_VEXTOR_NORM_TYPE8(out,v1,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectNorm2(out,v1,lenDiv,lenMod) LW_VEXTOR_NORM_TYPE16(out,v1,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//计算向量y=k*y 与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8

#define LW_VEXTOR_SCALE_TYPE2(Y,K,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_dscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE4(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_dscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE8(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_dscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE16(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_dscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)


#else

#define LW_VEXTOR_SCALE_TYPE2(Y,K,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_sscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE4(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_sscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE8(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_sscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SCALE_TYPE16(Y,K,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_sscal(_V_real_L1,(K),(void *)(Y),1);}\
(Y) += _V_real_L1;\
}while (0)

#endif // PARA_LEN==8


#else

#define LW_VEXTOR_SCALE_TYPE2(Y,K,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=_ParaMpy((K),(Y)[0]);\
	(Y)[1]=_ParaMpy((K),(Y)[1]);\
	(Y)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	{(*(Y))=_ParaMpy((K),(*(Y)));++(Y);}\
}else{LwVectSet(Y,_ParaFrom(0),DIV,MOD);}\
}while(0)

#define LW_VEXTOR_SCALE_TYPE4(Y,K,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=_ParaMpy((K),(Y)[0]);\
	(Y)[1]=_ParaMpy((K),(Y)[1]);\
	(Y)[2]=_ParaMpy((K),(Y)[2]);\
	(Y)[3]=_ParaMpy((K),(Y)[3]);\
	(Y)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	{(*(Y))=_ParaMpy((K),(*(Y)));++(Y);}\
}else{LwVectSet(Y,_ParaFrom(0),DIV,MOD);}\
}while(0)

#define LW_VEXTOR_SCALE_TYPE8(Y,K,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=_ParaMpy((K),(Y)[0]);\
	(Y)[1]=_ParaMpy((K),(Y)[1]);\
	(Y)[2]=_ParaMpy((K),(Y)[2]);\
	(Y)[3]=_ParaMpy((K),(Y)[3]);\
	(Y)[4]=_ParaMpy((K),(Y)[4]);\
	(Y)[5]=_ParaMpy((K),(Y)[5]);\
	(Y)[6]=_ParaMpy((K),(Y)[6]);\
	(Y)[7]=_ParaMpy((K),(Y)[7]);\
	(Y)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	{(*(Y))=_ParaMpy((K),(*(Y)));++(Y);}\
}else{LwVectSet(Y,_ParaFrom(0),DIV,MOD);}\
}while(0)

#define LW_VEXTOR_SCALE_TYPE16(Y,K,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=_ParaMpy((K),(Y)[0]);\
	(Y)[1]=_ParaMpy((K),(Y)[1]);\
	(Y)[2]=_ParaMpy((K),(Y)[2]);\
	(Y)[3]=_ParaMpy((K),(Y)[3]);\
	(Y)[4]=_ParaMpy((K),(Y)[4]);\
	(Y)[5]=_ParaMpy((K),(Y)[5]);\
	(Y)[6]=_ParaMpy((K),(Y)[6]);\
	(Y)[7]=_ParaMpy((K),(Y)[7]);\
	(Y)[8]=_ParaMpy((K),(Y)[8]);\
	(Y)[9]=_ParaMpy((K),(Y)[9]);\
	(Y)[10]=_ParaMpy((K),(Y)[10]);\
	(Y)[11]=_ParaMpy((K),(Y)[11]);\
	(Y)[12]=_ParaMpy((K),(Y)[12]);\
	(Y)[13]=_ParaMpy((K),(Y)[13]);\
	(Y)[14]=_ParaMpy((K),(Y)[14]);\
	(Y)[15]=_ParaMpy((K),(Y)[15]);\
	(Y)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	{(*(Y))=_ParaMpy((K),(*(Y)));++(Y);}\
}else{LwVectSet(Y,_ParaFrom(0),DIV,MOD);}\
}while(0)

#endif

#if CACHE_READ_NUM==2
#define LwVectScale(y,k,lenDiv,lenMod) LW_VEXTOR_SCALE_TYPE2(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectScale(y,k,lenDiv,lenMod) LW_VEXTOR_SCALE_TYPE4(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectScale(y,k,lenDiv,lenMod) LW_VEXTOR_SCALE_TYPE8(y,k,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectScale(y,k,lenDiv,lenMod) LW_VEXTOR_SCALE_TYPE16(y,k,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//计算向量y+=k*x 与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8


#define LW_VEXTOR_ADD_K_X_TYPE2(Y,K,X,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_daxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE4(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_daxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE8(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_daxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE16(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_daxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#else

#define LW_VEXTOR_ADD_K_X_TYPE2(Y,K,X,DIV,MOD)do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_saxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE4(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_saxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE8(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_saxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_ADD_K_X_TYPE16(Y,K,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_saxpy(_V_real_L1,(K),(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#endif // PARA_LEN==8

#else

#define LW_VEXTOR_ADD_K_X_TYPE2(Y,K,X,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1 = (DIV);\
while (_v_i1--) {\
		(Y)[0] += _ParaMpy((K), (X)[0]);\
		(Y)[1] += _ParaMpy((K), (X)[1]);\
		(Y) += 2;(X) += 2;\
}\
_v_i1 = (MOD);\
while (_v_i1--)\
(*(Y)++) += _ParaMpy((K), (*(X)++));\
}else{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}\
}while(0)

#define LW_VEXTOR_ADD_K_X_TYPE4(Y,K,X,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy((K),(X)[0]);\
	(Y)[1]+=_ParaMpy((K),(X)[1]);\
	(Y)[2]+=_ParaMpy((K),(X)[2]);\
	(Y)[3]+=_ParaMpy((K),(X)[3]);\
	(Y)+=4;(X)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy((K),(*(X)++));\
}else{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}\
}while(0)

#define LW_VEXTOR_ADD_K_X_TYPE8(Y,K,X,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
ParaType _temp_K=(K);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy(_temp_K,(X)[0]);\
	(Y)[1]+=_ParaMpy(_temp_K,(X)[1]);\
	(Y)[2]+=_ParaMpy(_temp_K,(X)[2]);\
	(Y)[3]+=_ParaMpy(_temp_K,(X)[3]);\
	(Y)[4]+=_ParaMpy(_temp_K,(X)[4]);\
	(Y)[5]+=_ParaMpy(_temp_K,(X)[5]);\
	(Y)[6]+=_ParaMpy(_temp_K,(X)[6]);\
	(Y)[7]+=_ParaMpy(_temp_K,(X)[7]);\
	(Y)+=8;(X)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy((K),(*(X)++));\
}else{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}\
}while(0)


#define LW_VEXTOR_ADD_K_X_TYPE16(Y,K,X,DIV,MOD) do{ \
if((K)!=_ParaFrom(0)){\
LwMatLenType _v_i1=(DIV);\
ParaType _temp_K=(K);\
while (_v_i1--){\
	(Y)[0]+=_ParaMpy(_temp_K,(X)[0]);\
	(Y)[1]+=_ParaMpy(_temp_K,(X)[1]);\
	(Y)[2]+=_ParaMpy(_temp_K,(X)[2]);\
	(Y)[3]+=_ParaMpy(_temp_K,(X)[3]);\
	(Y)[4]+=_ParaMpy(_temp_K,(X)[4]);\
	(Y)[5]+=_ParaMpy(_temp_K,(X)[5]);\
	(Y)[6]+=_ParaMpy(_temp_K,(X)[6]);\
	(Y)[7]+=_ParaMpy(_temp_K,(X)[7]);\
	(Y)[8]+=_ParaMpy(_temp_K,(X)[8]);\
	(Y)[9]+=_ParaMpy(_temp_K,(X)[9]);\
	(Y)[10]+=_ParaMpy(_temp_K,(X)[10]);\
	(Y)[11]+=_ParaMpy(_temp_K,(X)[11]);\
	(Y)[12]+=_ParaMpy(_temp_K,(X)[12]);\
	(Y)[13]+=_ParaMpy(_temp_K,(X)[13]);\
	(Y)[14]+=_ParaMpy(_temp_K,(X)[14]);\
	(Y)[15]+=_ParaMpy(_temp_K,(X)[15]);\
	(Y)+=16;(X)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	(*(Y)++)+=_ParaMpy((K),(*(X)++));\
}else{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}\
}while(0)

#endif


#if CACHE_READ_NUM==2
#define LwVectAddKX(y,k,x,lenDiv,lenMod) LW_VEXTOR_ADD_K_X_TYPE2(y,k,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectAddKX(y,k,x,lenDiv,lenMod) LW_VEXTOR_ADD_K_X_TYPE4(y,k,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectAddKX(y,k,x,lenDiv,lenMod) LW_VEXTOR_ADD_K_X_TYPE8(y,k,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectAddKX(y,k,x,lenDiv,lenMod) LW_VEXTOR_ADD_K_X_TYPE16(y,k,x,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif


//计算向量y=x  与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8

#define LW_VEXTOR_COPY_TYPE2(Y,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_dcopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE4(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_dcopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE8(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_dcopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE16(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_dcopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#else

#define LW_VEXTOR_COPY_TYPE2(Y,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_scopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE4(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_scopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE8(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_scopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_COPY_TYPE16(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_scopy (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#endif // PARA_LEN==8


#else

#define LW_VEXTOR_COPY_TYPE2(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(X)[0];\
	(Y)[1]=(X)[1];\
	(Y)+=2;(X)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	*(Y)++=*(X)++;\
}while(0)

#define LW_VEXTOR_COPY_TYPE4(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(X)[0];\
	(Y)[1]=(X)[1];\
	(Y)[2]=(X)[2];\
	(Y)[3]=(X)[3];\
	(Y)+=4;(X)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	*(Y)++=*(X)++;\
}while(0)

#define LW_VEXTOR_COPY_TYPE8(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(X)[0];\
	(Y)[1]=(X)[1];\
	(Y)[2]=(X)[2];\
	(Y)[3]=(X)[3];\
	(Y)[4]=(X)[4];\
	(Y)[5]=(X)[5];\
	(Y)[6]=(X)[6];\
	(Y)[7]=(X)[7];\
	(Y)+=8;(X)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	*(Y)++=*(X)++;\
}while(0)

#define LW_VEXTOR_COPY_TYPE16(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(Y)[0]=(X)[0];\
	(Y)[1]=(X)[1];\
	(Y)[2]=(X)[2];\
	(Y)[3]=(X)[3];\
	(Y)[4]=(X)[4];\
	(Y)[5]=(X)[5];\
	(Y)[6]=(X)[6];\
	(Y)[7]=(X)[7];\
	(Y)[8]=(X)[8];\
	(Y)[9]=(X)[9];\
	(Y)[10]=(X)[10];\
	(Y)[11]=(X)[11];\
	(Y)[12]=(X)[12];\
	(Y)[13]=(X)[13];\
	(Y)[14]=(X)[14];\
	(Y)[15]=(X)[15];\
	(Y)+=16;(X)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
	*(Y)++=*(X)++;\
}while(0)

#endif

#if CACHE_READ_NUM==2
#define LwVectCopy(y,x,lenDiv,lenMod) LW_VEXTOR_COPY_TYPE2(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectCopy(y,x,lenDiv,lenMod) LW_VEXTOR_COPY_TYPE4(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectCopy(y,x,lenDiv,lenMod) LW_VEXTOR_COPY_TYPE8(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectCopy(y,x,lenDiv,lenMod) LW_VEXTOR_COPY_TYPE16(y,x,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



//计算向量y与向量x交换  与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if PARA_LEN==8

#define LW_VEXTOR_SWAY_TYPE2(Y,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_dswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE4(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_dswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE8(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_dswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE16(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_dswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#else

#define LW_VEXTOR_SWAY_TYPE2(Y,X,DIV,MOD) do{\
LwMatLenType _V_real_L1 = ((DIV) << 1) + (MOD);\
{cblas_sswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE4(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 2) + (MOD);\
{cblas_sswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE8(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 3) + (MOD);\
{cblas_sswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)

#define LW_VEXTOR_SWAY_TYPE16(Y,X,DIV,MOD)  do{\
LwMatLenType _V_real_L1 = ((DIV) << 4) + (MOD);\
{cblas_sswap  (_V_real_L1,(void *)(X),1,(void *)(Y),1);}\
(Y) += _V_real_L1;(X) += _V_real_L1;\
}while (0)


#endif // PARA_LEN==8


#else

#define LW_VEXTOR_SWAY_TYPE2(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
ParaType _tEMP_bUF_rEG[2];\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	_tEMP_bUF_rEG[1]=(X)[1];\
	(X)[0]=(Y)[0];\
	(X)[1]=(Y)[1];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	(Y)[1]= _tEMP_bUF_rEG[1];\
	(Y)+=2;(X)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	(X)[0]=(Y)[0];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	++(Y),++(X);\
}\
}while(0)

#define LW_VEXTOR_SWAY_TYPE4(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
ParaType _tEMP_bUF_rEG[4];\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	_tEMP_bUF_rEG[1]=(X)[1];\
	_tEMP_bUF_rEG[2]=(X)[2];\
	_tEMP_bUF_rEG[3]=(X)[3];\
	(X)[0]=(Y)[0];\
	(X)[1]=(Y)[1];\
	(X)[2]=(Y)[2];\
	(X)[3]=(Y)[3];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	(Y)[1]= _tEMP_bUF_rEG[1];\
	(Y)[2]= _tEMP_bUF_rEG[2];\
	(Y)[3]= _tEMP_bUF_rEG[3];\
	(Y)+=4;(X)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	(X)[0]=(Y)[0];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	++(Y),++(X);\
}\
}while(0)

#define LW_VEXTOR_SWAY_TYPE8(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
ParaType _tEMP_bUF_rEG[8];\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	_tEMP_bUF_rEG[1]=(X)[1];\
	_tEMP_bUF_rEG[2]=(X)[2];\
	_tEMP_bUF_rEG[3]=(X)[3];\
	_tEMP_bUF_rEG[4]=(X)[4];\
	_tEMP_bUF_rEG[5]=(X)[5];\
	_tEMP_bUF_rEG[6]=(X)[6];\
	_tEMP_bUF_rEG[7]=(X)[7];\
	(X)[0]=(Y)[0];\
	(X)[1]=(Y)[1];\
	(X)[2]=(Y)[2];\
	(X)[3]=(Y)[3];\
	(X)[4]=(Y)[4];\
	(X)[5]=(Y)[5];\
	(X)[6]=(Y)[6];\
	(X)[7]=(Y)[7];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	(Y)[1]= _tEMP_bUF_rEG[1];\
	(Y)[2]= _tEMP_bUF_rEG[2];\
	(Y)[3]= _tEMP_bUF_rEG[3];\
	(Y)[4]= _tEMP_bUF_rEG[4];\
	(Y)[5]= _tEMP_bUF_rEG[5];\
	(Y)[6]= _tEMP_bUF_rEG[6];\
	(Y)[7]= _tEMP_bUF_rEG[7];\
	(Y)+=8;(X)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	(X)[0]=(Y)[0];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	++(Y),++(X);\
}\
}while(0)

#define LW_VEXTOR_SWAY_TYPE16(Y,X,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
ParaType _tEMP_bUF_rEG[16];\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	_tEMP_bUF_rEG[1]=(X)[1];\
	_tEMP_bUF_rEG[2]=(X)[2];\
	_tEMP_bUF_rEG[3]=(X)[3];\
	_tEMP_bUF_rEG[4]=(X)[4];\
	_tEMP_bUF_rEG[5]=(X)[5];\
	_tEMP_bUF_rEG[6]=(X)[6];\
	_tEMP_bUF_rEG[7]=(X)[7];\
	_tEMP_bUF_rEG[8]=(X)[8];\
	_tEMP_bUF_rEG[9]=(X)[9];\
	_tEMP_bUF_rEG[10]=(X)[10];\
	_tEMP_bUF_rEG[11]=(X)[11];\
	_tEMP_bUF_rEG[12]=(X)[12];\
	_tEMP_bUF_rEG[13]=(X)[13];\
	_tEMP_bUF_rEG[14]=(X)[14];\
	_tEMP_bUF_rEG[15]=(X)[15];\
	(X)[0]=(Y)[0];\
	(X)[1]=(Y)[1];\
	(X)[2]=(Y)[2];\
	(X)[3]=(Y)[3];\
	(X)[4]=(Y)[4];\
	(X)[5]=(Y)[5];\
	(X)[6]=(Y)[6];\
	(X)[7]=(Y)[7];\
	(X)[8]=(Y)[8];\
	(X)[9]=(Y)[9];\
	(X)[10]=(Y)[10];\
	(X)[11]=(Y)[11];\
	(X)[12]=(Y)[12];\
	(X)[13]=(Y)[13];\
	(X)[14]=(Y)[14];\
	(X)[15]=(Y)[15];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	(Y)[1]= _tEMP_bUF_rEG[1];\
	(Y)[2]= _tEMP_bUF_rEG[2];\
	(Y)[3]= _tEMP_bUF_rEG[3];\
	(Y)[4]= _tEMP_bUF_rEG[4];\
	(Y)[5]= _tEMP_bUF_rEG[5];\
	(Y)[6]= _tEMP_bUF_rEG[6];\
	(Y)[7]= _tEMP_bUF_rEG[7];\
	(Y)[8]= _tEMP_bUF_rEG[8];\
	(Y)[9]= _tEMP_bUF_rEG[9];\
	(Y)[10]= _tEMP_bUF_rEG[10];\
	(Y)[11]= _tEMP_bUF_rEG[11];\
	(Y)[12]= _tEMP_bUF_rEG[12];\
	(Y)[13]= _tEMP_bUF_rEG[13];\
	(Y)[14]= _tEMP_bUF_rEG[14];\
	(Y)[15]= _tEMP_bUF_rEG[15];\
	(Y)+=16;(X)+=16;\
}\
_v_i1=(MOD);\
while (_v_i1--){\
	_tEMP_bUF_rEG[0]=(X)[0];\
	(X)[0]=(Y)[0];\
	(Y)[0]= _tEMP_bUF_rEG[0];\
	++(Y),++(X);\
}\
}while(0)

#endif

#if CACHE_READ_NUM==2
#define LwVectSwap(y,x,lenDiv,lenMod) LW_VEXTOR_SWAY_TYPE2(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==4
#define LwVectSwap(y,x,lenDiv,lenMod) LW_VEXTOR_SWAY_TYPE4(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==8
#define LwVectSwap(y,x,lenDiv,lenMod) LW_VEXTOR_SWAY_TYPE8(y,x,lenDiv,lenMod)
#elif CACHE_READ_NUM==16
#define LwVectSwap(y,x,lenDiv,lenMod) LW_VEXTOR_SWAY_TYPE16(y,x,lenDiv,lenMod)
#else
#error "CACHE_READ_NUM is 2 or 4 or 8 or 16"
#endif



#define LW_VDST_ADD_VS1_MPY_VS2_TYPE2(DST,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((DST)[0],_ParaMpy((S1)[0],(S2)[0]));\
	(DST)[1]=_ParaAdd((DST)[1],_ParaMpy((S1)[1],(S2)[1]));\
	(S1)+=2;(S2)+=2;(DST)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)


#define LW_VDST_ADD_VS1_MPY_VS2_TYPE4(DST,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((DST)[0],_ParaMpy((S1)[0],(S2)[0]));\
	(DST)[1]=_ParaAdd((DST)[1],_ParaMpy((S1)[1],(S2)[1]));\
	(DST)[2]=_ParaAdd((DST)[2],_ParaMpy((S1)[2],(S2)[2]));\
	(DST)[3]=_ParaAdd((DST)[3],_ParaMpy((S1)[3],(S2)[3]));\
	(S1)+=4;(S2)+=4;(DST)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)

#define LW_VDST_ADD_VS1_MPY_VS2_TYPE8(DST,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((DST)[0],_ParaMpy((S1)[0],(S2)[0]));\
	(DST)[1]=_ParaAdd((DST)[1],_ParaMpy((S1)[1],(S2)[1]));\
	(DST)[2]=_ParaAdd((DST)[2],_ParaMpy((S1)[2],(S2)[2]));\
	(DST)[3]=_ParaAdd((DST)[3],_ParaMpy((S1)[3],(S2)[3]));\
	(DST)[4]=_ParaAdd((DST)[4],_ParaMpy((S1)[4],(S2)[4]));\
	(DST)[5]=_ParaAdd((DST)[5],_ParaMpy((S1)[5],(S2)[5]));\
	(DST)[6]=_ParaAdd((DST)[6],_ParaMpy((S1)[6],(S2)[6]));\
	(DST)[7]=_ParaAdd((DST)[7],_ParaMpy((S1)[7],(S2)[7]));\
	(S1)+=8;(S2)+=8;(DST)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)




#define LW_VDST_EQU_D0_ADD_VS1_MPY_VS2_TYPE2(DST,D0,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((D0),_ParaMpy((S1)[0],(S2)[0]);\
	(DST)[1]=_ParaAdd((D0),_ParaMpy((S1)[1],(S2)[1]);\
	(S1)+=2;(S2)+=2;(DST)+=2;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)


#define LW_VDST_EQU_D0_ADD_VS1_MPY_VS2_TYPE4(DST,D0,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((D0),_ParaMpy((S1)[0],(S2)[0]);\
	(DST)[1]=_ParaAdd((D0),_ParaMpy((S1)[1],(S2)[1]);\
	(DST)[2]=_ParaAdd((D0),_ParaMpy((S1)[2],(S2)[2]);\
	(DST)[3]=_ParaAdd((D0),_ParaMpy((S1)[3],(S2)[3]);\
	(S1)+=4;(S2)+=4;(DST)+=4;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)


#define LW_VDST_EQU_D0_ADD_VS1_MPY_VS2_TYPE8(DST,D0,S1,S2,DIV,MOD) do{ \
LwMatLenType _v_i1=(DIV);\
while (_v_i1--){\
	(DST)[0]=_ParaAdd((D0),_ParaMpy((S1)[0],(S2)[0]);\
	(DST)[1]=_ParaAdd((D0),_ParaMpy((S1)[1],(S2)[1]);\
	(DST)[2]=_ParaAdd((D0),_ParaMpy((S1)[2],(S2)[2]);\
	(DST)[3]=_ParaAdd((D0),_ParaMpy((S1)[3],(S2)[3]);\
	(DST)[4]=_ParaAdd((D0),_ParaMpy((S1)[4],(S2)[4]);\
	(DST)[5]=_ParaAdd((D0),_ParaMpy((S1)[5],(S2)[5]);\
	(DST)[6]=_ParaAdd((D0),_ParaMpy((S1)[6],(S2)[6]);\
	(DST)[7]=_ParaAdd((D0),_ParaMpy((S1)[7],(S2)[7]);\
	(S1)+=8;(S2)+=8;(DST)+=8;\
}\
_v_i1=(MOD);\
while (_v_i1--)\
*(DST)++=_ParaAdd(*(DST)++,_ParaMpy(*(S1)++,*(S2)++));\
}while(0)



//计算向量a的绝对值最大  与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if (PARA_LEN == 8)
#define LwVectAmax(a,n,ndelt) cblas_idamax (n,a,ndelt)
#else
#define LwVectAmax(a,n,ndelt) cblas_isamax (n,a,ndelt)
#endif


#else

static LwMatLenType LwVectAmax(ParaType *a, LwMatLenType n, LwMatLenType ndelt)
{
	ParaType _temp = _ParaAbs(a[0]), _t2;
	LwMatLenType _index = 0, i;
	a += ndelt;
	for (i = 1; i < n; i++)
	{
		_t2 = _ParaAbs(*a);
		a += ndelt;
		if (_temp < _t2) {
			_temp = _t2;
			_index = i;
		}
	}
	return _index;
}

#endif



//计算向量a的绝对值最小  与CBLAS-L1兼容
#if defined(OSLW_USING_CBLAS) && OSLW_USING_CBLAS

#if (PARA_LEN == 8)
#define LwVectAmin(a,n,ndelt) cblas_idamin (n,a,ndelt)
#else
#define LwVectAmin(a,n,ndelt) cblas_isamin (n,a,ndelt)
#endif


#else

static LwMatLenType LwVectAmin(ParaType *a, LwMatLenType n, LwMatLenType ndelt)
{
	ParaType _temp = _ParaAbs(a[0]), _t2;
	LwMatLenType _index = 0, i;
	a += ndelt;
	for (i = 1; i < n; i++)
	{
		_t2 = _ParaAbs(*a);
		a += ndelt;
		if (_temp > _t2) {
			_temp = _t2;
			_index = i;
		}
	}
	return _index;
}

#endif



#endif

#endif

#endif


