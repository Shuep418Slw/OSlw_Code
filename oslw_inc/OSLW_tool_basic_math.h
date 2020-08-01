/*(Ver.=0.97)
* OSLW_tool.h
*
*  Created on: 2017-7-25
*      Author: slw
*/

#ifndef OSLW_TOOL_BASIC_MATH_H_
#define OSLW_TOOL_BASIC_MATH_H_


#include "OSLW_define.h"
#include "OSLW_tool_define.h"
#include "OSLW_tool_vector_math.h"

#if !(OSLW_SIMPLE_LEVEL >= 4)

#if OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

typedef struct OSLW_TOOL_MATRIX_STRUCT {
	ParaType *a;
	LwMatLenType length;
	LwMatRowType row;
	LwMatColType col;

}OSlwToolMatrixSTU;

typedef OSlwToolMatrixSTU OSlwMat;
typedef OSlwToolMatrixSTU LwMat;

typedef enum
{
	OSlwMartixRes_Find = 0,
	OSlwMartixRes_NoFind,
	OSlwMartixRes_Over,
	OSlwMartixRes_Error

}OSlwMartixResNum;

//------------------------------------------
//<函数名>OSlwToolMatrixInitial</函数名>
//<功能说明>矩阵初始化</功能说明>
//<输入说明>m:矩阵结构体 row:行 col:列 a:内存空间</输入说明>
//<输出说明>m地址</输出说明>
//<备注>可使用LwMatInit调用，也可以采用LwMatDefault初始化</备注>
//------------------------------------------
OSlwToolMatrixSTU* OSlwToolMatrixInitial(OSlwToolMatrixSTU *m, LwMatRowType row, LwMatColType col, ParaType *a);

//判断长度
#define OSLW_TOOL_M_SIZE_EQU(A,B) ((A)->length == (B)->length)
//得到矩阵的一个数
#define OSLW_TOOL_M_GET(M,R,C) (*((M).a+(R)*(M).col+(C)))
//得到矩阵一行指针
#define OSLW_TOOL_M_GET_R(M,R) ((M).a+(R)*(M).col)
//得到矩阵一个元素的地址
#define OSlwMatGet(M,R,C) ((M).a+(R)*(M).col+(C))
//得到矩阵一个数
#define OSlwMatAt(M,R,C) (*((M).a+(R)*(M).col+(C)))
//得到行指针
#define OSlwMatGetR(M,R) ((M).a+(R)*(M).col)
//动态分配矩阵M的内存空间
#define OSLW_TOOL_M_DY_INIT(M,R,C,PMEM) do{OSlwToolMatrixInitial(&(M),(R),(C),(PMEM)->Malloc((PMEM) , PARA_MEM_CAL((R)*(C))));}while(0)

#define OSlwMatInit OSlwToolMatrixInitial
#define LwMatInit OSlwMatInit

#define OSlwMatNew OSLW_TOOL_M_DY_INIT
#define LwMatNew OSLW_TOOL_M_DY_INIT
//静态初始化
#define OSlwMatDef(R,C,A) {((ParaType *)(A)),(R)*(C),(R),(C)}
#define LwMatDefault(R,C,A) OSlwMatDef(R,C,A)
//静态new一个矩阵
#define OSlwMatStaticNew(M,R,C) static ParaType M##R##C [(R)*(C)];static OSlwMat M={((ParaType *)( M##R##C )),(R)*(C),(R),(C)};


//------------------------------------------
//<函数名>OSlwToolMatrixAdd</函数名>
//<功能说明>矩阵相加</功能说明>
//<输入说明>s=m1+m2</输入说明>
//<输出说明>s的地址</输出说明>
//<备注>采用动态调用pOSlwToolMatrixAdd，LwMatAdd调用 m1与m2有一个length为1就会采用标量+矩阵的方法</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixAdd,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2))

#define LwMatAdd(S,M1,M2) pOSlwToolMatrixAdd((S),(M1),(M2))


//------------------------------------------
//<函数名>OSlwToolMatrixSub</函数名>
//<功能说明>矩阵-</功能说明>
//<输入说明>s=m1-m2</输入说明>
//<输出说明>s的地址</输出说明>
//<备注>采用动态调用pOSlwToolMatrixSub，LwMatSub调用 m1与m2有一个length为1就会采用标量-矩阵的方法</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixSub,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2))

#define LwMatSub(S,M1,M2) pOSlwToolMatrixSub((S),(M1),(M2))

//------------------------------------------
//<函数名>OSlwToolMatrixDot</函数名>
//<功能说明>矩阵*</功能说明>
//<输入说明>s=m1.*m2</输入说明>
//<输出说明>s的地址</输出说明>
//<备注>采用动态调用pOSlwToolMatrixDot，LwMatDot调用 m1与m2有一个length为1就会采用标量*矩阵的方法</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixDot,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2))

#define LwMatDot(S,M1,M2) pOSlwToolMatrixDot((S),(M1),(M2))

//------------------------------------------
//<函数名>OSlwToolMatrixMpy</函数名>
//<功能说明>矩阵*</功能说明>
//<输入说明>s=m1*m2</输入说明>
//<输出说明>s的地址</输出说明>
//<备注>采用动态调用pOSlwToolMatrixMpy，LwMatMpy调用 m1与m2有一个length为1就会采用标量*矩阵的方法
//在引入cblas的时候会采用cblas优化
//</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixMpy,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2))

#define LwMatMpy(S,M1,M2) pOSlwToolMatrixMpy((S),(M1),(M2))

//------------------------------------------
//<函数名>OSlwToolMatrixTurn</函数名>
//<功能说明>矩阵转置</功能说明>
//<输入说明>s=buf'</输入说明>
//<输出说明>s的地址</输出说明>
//<备注>采用动态调用pOSlwToolMatrixTurn，LwMatTurn调用</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixTurn,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *buf))

#define LwMatTurn(S,M1) pOSlwToolMatrixTurn((S),(M1))

//------------------------------------------
//<函数名>OSlwToolMatrixResize</函数名>
//<功能说明>矩阵调整大小</功能说明>
//<输入说明></输入说明>
//<输出说明></输出说明>
//<备注>仅仅是修改row与col</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixReSize,
(OSlwToolMatrixSTU *s, LwMatRowType row, LwMatColType col))


//------------------------------------------
//<函数名>OSlwToolMatrixMaxMin</函数名>
//<功能说明>矩阵最值查询</功能说明>
//<输入说明>index指针可以为空</输入说明>
//<输出说明></输出说明>
//<备注>将矩阵作为向量查询</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixMaxMin,
(OSlwToolMatrixSTU *s, ParaType *pMax, lw_u16 *pMaxIndex, ParaType *pMin, lw_u16 *pMinIndex))


//------------------------------------------
//<函数名>OSlwToolMatrixIndex</函数名>
//<功能说明>矩阵单个数值查询最值</功能说明>
//<输入说明></输入说明>
//<输出说明></输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMartixResNum, OSlwToolMatrixIndex,
(OSlwToolMatrixSTU *s, ParaType data, lw_u16 *pindex, lw_u16 *pmaxlen))

//------------------------------------------
//<函数名>OSlwToolMatrixFind</函数名>
//<功能说明>子矩阵查询</功能说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMartixResNum, OSlwToolMatrixFind,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, lw_u16 SpacingLen, lw_u16 First, lw_u16 *pres))

//------------------------------------------
//<函数名>OSlwToolMatrixCmp</函数名>
//<功能说明>比较矩阵的数值</功能说明>
//<输入说明>pbuf</输入说明>
//<输出说明>pbuf</输出说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMartixResNum, OSlwToolMatrixCmp,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, lw_u8 *pbuf))

//------------------------------------------
//<函数名>OSlwToolMatrixSet</函数名>
//<功能说明>设置数值</功能说明>
//<输入说明>s=data或者矩阵a</输入说明>
//<输出说明></输出说明>
//<备注>如果a为空，就用data置数</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixSet,
(OSlwToolMatrixSTU *s, ParaType data, OSlwToolMatrixSTU *a))

#define LwMatCopy(S,M1) pOSlwToolMatrixSet((S),_ParaFrom(0),(M1))
#define LwMatSet(S,D) pOSlwToolMatrixSet((S),(D),NULL)

//------------------------------------------
//<函数名>OSlwToolMatrixJoin</函数名>
//<功能说明>矩阵拼接</功能说明>
//<输入说明>s={m1,m2} dim为0表示向量拼接 为1表示行拼接 2表示列拼接</输入说明>
//<输出说明>void</输出说明>
//<备注>dim=0 要求s的长度>=m1+m2 dim=1 要求s>=m1+m2且m1与m2行数相同 dim=2 要求row>m1+m2 col>m1 col>m2</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixJoin,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2, lw_u8 dim))

#define LwMatJoin(S,M1,M2,dim) pOSlwToolMatrixJoin((S),(M1),(M2),(dim))


//------------------------------------------
//<函数名>OSlwToolMatrixLossSquare</函数名>
//<功能说明>用于回归问题的损失函数</功能说明>
//<输入说明></输入说明>
//<输出说明></输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(ParaType, OSlwToolMatrixLossSquare,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre))

//------------------------------------------
//<函数名>OSlwToolMatrixMaxMin</函数名>
//<功能说明>用于softmax分类问题的损失函数</功能说明>
//<输入说明></输入说明>
//<输出说明></输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(ParaType, OSlwToolMatrixLossCrossEntropyForSoftMax,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre))

//------------------------------------------
//<函数名>OSlwToolMatrixSum</函数名>
//<功能说明>矩阵求和</功能说明>
//<输入说明>DST输出矩阵 SRV输入矩阵 dim维度</输入说明>
//<输出说明></输出说明>
//<备注>dim 8bit 低4位表示按照哪一个维度求和 0表示按照向量求和 1表示按照一行一行求和 2表示按照一列一列求和 bit4为1将结果加上去</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(ParaType, OSlwToolMatrixSum,
(OSlwToolMatrixSTU *DST, OSlwToolMatrixSTU *SRC, lw_u8 dim))

OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixMPYA,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *bi))

OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixWeXBi,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *bi))


//------------------------------------------
//<函数名>OSlwToolMatrixXWeBi</函数名>
//<功能说明>全连接层前向传递</功能说明>
//<输入说明>s=x*we+bi</输入说明>
//<输出说明></输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixXWeBi,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *we, OSlwToolMatrixSTU *x, OSlwToolMatrixSTU *bi))


//------------------------------------------
//<函数名>OSlwToolMatrixTurnMpy</函数名>
//<功能说明>矩阵转置相乘</功能说明>
//<输入说明>s=m1*m2 flag控制谁转置</输入说明>
//<输出说明></输出说明>
//<备注>flag为8bit bit0表示m2要转置 bit1代表m1要转置 bit4为1代表累加模式运算结果会加到s上</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwToolMatrixSTU*, OSlwToolMatrixTurnMpy,
(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, OSlwToolMatrixSTU *m2, lw_u8 flag))

#define LwMatTurnMpy(S,M1,M2,Flag) OSlwToolMatrixTurnMpy((S),(M1),(M2),Flag)

//------------------------------------------
//<函数名>OSlwToolMatrixConv2</函数名>
//<功能说明>二维卷积运算</功能说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(void*, OSlwToolMatrixConv2,
(
	OSlwToolMatrixSTU *s, //目标的
	OSlwToolMatrixSTU *m_kernal, //卷积核
	OSlwToolMatrixSTU *m2,//被卷积 
	lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
	lw_u8 EqualModel, //赋值模式 1:直接复制 0:相加
	OSlwToolMatrixConvMethodNUM MoveModel, //移动模式 'v'/'f'
	lw_u8 KernalModel, //核模式 0/180 180+‘f’=数学二维卷积
	ParaType *fast_buf//快速卷积内存区
	)
)

//只计算一个batch
//------------------------------------------
//<函数名>OSlwToolMatrixConvFastMultCh</函数名>
//<功能说明>CNN快速卷积运算</功能说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(void*, OSlwToolMatrixConvFastMultCh,
(
	OSlwToolMatrixSTU *m_out, //输出 row-col 代表一个通道 length代表真正大小
	OSlwToolMatrixSTU *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
	OSlwToolMatrixSTU *m_in,//被卷积 row-col 代表一个通道
	OSlwToolMatrixSTU *bias,//偏置 row-col-length 无所谓
	lw_u16 in_high,//输入高度 
	lw_u16 out_high,//输出高度
	lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
	OSlwToolMatrixConvMethodNUM conv_method,
	lw_u8 FD_1_or_BK_0,//前向传递或者反向传递
	ParaType *fast_buf//核 区域
	)
)

//------------------------------------------
//<函数名>OSlwToolMatrixConvFastMultChIm2ColFd</函数名>
//<功能说明>采用了Im2Col算法的CNN快速卷积运算</功能说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(
	void*, OSlwToolMatrixConvFastMultChIm2ColFd,
	(
		OSlwToolMatrixSTU *m_out, //输出 row-col 代表一个通道 length代表真正大小
		OSlwToolMatrixSTU *m_kernal, //卷积核 row-col 代表一个通道 length 代表一个核真正大小 [2,2,4] row:2 col:2 length:16
		OSlwToolMatrixSTU *m_in,//被卷积 row-col 代表一个通道
		OSlwToolMatrixSTU *bias,//偏置 row-col-length 无所谓
		lw_u16 in_high,//输入高度 
		lw_u16 out_high,//输出高度
		lw_u16 move_x, lw_u16 move_y,//横向纵向移动距离
		OSlwToolMatrixConvMethodNUM conv_method,
		lw_u32 now_flow_len,//现实缓冲区的大小
		ParaType *fast_buf//缓冲区
		)
)
//------------------------------------------
//<函数名>OSlwToolMatrixMoments</函数名>
//<功能说明>矩阵求均值与方差</功能说明>
//<输入说明>mean输出均值，var输出方差 src输入矩阵 dim维度</输入说明>
//<输出说明></输出说明>
//<备注>dim 同上</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(void*, OSlwToolMatrixMoments,
(
	OSlwToolMatrixSTU *src,
	ParaType *mean,
	ParaType *var,
	lw_u8 dim
	)
)

//------------------------------------------
//<函数名>OSlwToolMatrixVectShift</函数名>
//<功能说明>残差网络前向传递</功能说明>
//------------------------------------------

OSLW_TOOL_FUN_STATEMENT(void*, OSlwToolMatrixVectShift,
(
	OSlwToolMatrixSTU *y,
	OSlwToolMatrixSTU *we,
	OSlwToolMatrixSTU *x,
	OSlwToolMatrixSTU *bi
	)
)
//------------------------------------------
//<函数名>OSlwToolMatrixDotSum</函数名>
//<功能说明>矩阵点积</功能说明>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(ParaType, OSlwToolMatrixDotSum,
(
	OSlwToolMatrixSTU *y,
	OSlwToolMatrixSTU *x1,
	OSlwToolMatrixSTU *x2,
	lw_u8 dim
	)
)

OSLW_TOOL_FUN_STATEMENT(
	OSlwToolMatrixSTU*,
	OSlwToolMatrix_RATIO_ADD,
	(OSlwToolMatrixSTU *s, ParaType a, OSlwToolMatrixSTU *m1, ParaType b, OSlwToolMatrixSTU *m2)
)

//------------------------------------------
//<函数名>OSlwToolMatrix_SVD</函数名>
//<功能说明>q=奇异值分解</功能说明>
//<输入说明>U(M*M),S(1*N),V(N*N),为输出 xin为输入（M*N） temp_run为计算中间变量至少2*N 缓存中间变量与误差</输入说明>
//<输出说明>分解状态</输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(lw_16, OSlwToolMatrix_SVD,
(
	OSlwMat *U,
	OSlwMat *S,
	OSlwMat *V,
	OSlwMat *xin,
	ParaType *temp_run
	)
)
#define LwMatSVD(U,S,XIN,temp) pOSlwToolMatrix_SVD((U),(S),NULL,(XIN),(void *)(temp))

typedef enum OSLW_TOOL_MATRIX_SOLVE_STATUS_NUM { OSlwMatSolveSucces = 0, OSlwMatSolveShapeError = 1, OSlwMatSolveZeroError = 2 }OSlwMatSolveStatusNUM;

//------------------------------------------
//<函数名>OSlwToolMatrixSolve</函数名>
//<功能说明>高斯消元法 解低阶线性方程</功能说明>
//<输入说明>A*x=y 输入为{A|y}</输入说明>
//<输出说明>消元结果</输出说明>
//<备注>只支持列>行的矩阵</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMatSolveStatusNUM, OSlwToolMatrixSolve, (OSlwToolMatrixSTU *s))

#define LwMatSolve(S) pOSlwToolMatrixSolve(S)



//------------------------------------------
//<函数名>OSlwToolMatrixInv</函数名>
//<功能说明>矩阵求逆</功能说明>
//<输入说明>m1 必须是方阵 temp至少2倍m1->length</输入说明>
//<输出说明>s 可以和m1同地址 返回值同上</输出说明>
//<备注></备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMatSolveStatusNUM, OSlwToolMatrixInv, (OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m1, ParaType *temp_work))

#define LwMatInv(S,M1,temp) pOSlwToolMatrixInv((S),(M1),(void *)(temp))



//------------------------------------------
//<函数名>OSlwToolMatrixSolveCrout</函数名>
//<功能说明>追赶法解方程</功能说明>
//<输入说明>X长度=D长度=Y长度=N U长度=N-1 注:L的长度必须是N 要多一个作为运算空间</输入说明>
//<输出说明>求解状态</输出说明>
//<备注> N=4
// | D0 U0  0  0 | |X0| |Y0|
// | L0 D1 U1  0 |*|X1|=|Y1|
// |  0 L1 D2 U2 | |X2| |Y2|
// |  0  0 L2 D3 | |X3| |Y3| 
//</备注>
//------------------------------------------
OSLW_TOOL_FUN_STATEMENT(OSlwMatSolveStatusNUM, OSlwToolMatrixSolveCrout,
(OSlwToolMatrixSTU *X, OSlwToolMatrixSTU *L, OSlwToolMatrixSTU *D, OSlwToolMatrixSTU *U, OSlwToolMatrixSTU *Y)
)




#if PARA_LEN==8
#define LW_MAT_CLR(A) do{\
	register lw_u32 *_pCLR_D=(void *)((A)->a);\
	register lw_u32 _CLR_len=((A)->length<<1);\
	while (_CLR_len--) *_pCLR_D++=0;\
}while(0)

#define LW_MAT_CPY(A,B) do{\
	register lw_u32 *_pCLR_D1=(void *)((A)->a);\
	register lw_u32 *_pCLR_D2=(void *)((B)->a);\
	register lw_u32 _CLR_len=(((A)->length)<<1);\
	while (_CLR_len--) *_pCLR_D1++=*_pCLR_D2++;\
}while(0)

#define LW_PARA_JOIN(PD,PS1,L1,PS2,L2) do{\
	register lw_u32 *_p_D=(void *)(PD);\
	register lw_u32 *_p_S1=(void *)(PS1);\
	register lw_u32 *_p_S2=(void *)(PS2);\
	register lw_u32 _L_1=(L1)<<1;\
	register lw_u32 _L_2=(L2)<<1;\
	if(_p_S1){while(_L_1--) *_p_D++=*_p_S1++;}\
	else {while (_L_1--) *_p_D++=0;}\
	if(_p_S2){while (_L_2--) *_p_D++=*_p_S2++;}\
	else {while (_L_2--) *_p_D++=0;}\
}while(0)

#define LW_WHILE_MEMSET(P,I,DATA) \
while ((I)--)\
	*(P)++ = DATA;

#define LW_WHILE_MEMCPY(P,Q,I) \
while ((I)--)\
	*(P)++ = *(Q)++;


#elif PARA_LEN==4
#define LW_MAT_CLR(A) do{\
	register lw_u32 *_pCLR_D=(void *)((A)->a);\
	register lw_u32 _CLR_len=(A)->length;\
	while (_CLR_len--) *_pCLR_D++=0;\
}while(0)

#define LW_MAT_CPY(A,B) do{\
	register lw_u32 *_pCLR_D1=(void *)((A)->a);\
	register lw_u32 *_pCLR_D2=(void *)((B)->a);\
	register lw_u32 _CLR_len=(A)->length;\
	while (_CLR_len--) *_pCLR_D1++=*_pCLR_D2++;\
}while(0)

#define LW_PARA_JOIN(PD,PS1,L1,PS2,L2) do{\
	register lw_u32 *_p_D=(void *)(PD);\
	register lw_u32 *_p_S1=(void *)(PS1);\
	register lw_u32 *_p_S2=(void *)(PS2);\
	register lw_u32 _L_1=(L1);\
	register lw_u32 _L_2=(L2);\
	if(_p_S1){while(_L_1--) *_p_D++=*_p_S1++;}\
	else {while (_L_1--) *_p_D++=0;}\
	if(_p_S2){while (_L_2--) *_p_D++=*_p_S2++;}\
	else {while (_L_2--) *_p_D++=0;}\
}while(0)

#define LW_WHILE_MEMSET(P,I,DATA) \
while ((I)--)\
	*(P)++ = DATA;

#define LW_WHILE_MEMCPY(P,Q,I) \
while ((I)--)\
	*(P)++ = *(Q)++;

#endif


static inline ParaType _OSlwToolMathExp256(ParaType _x)
{

#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	register ParaType x = _ParaFrom(1) + (_x >> 8);
#else
	register ParaType x = _ParaFrom(1) + _ParaDiv(_x, _ParaFint(256));
#endif

	x = _ParaMpy(x, x);x = _ParaMpy(x, x);x = _ParaMpy(x, x);
	x = _ParaMpy(x, x);x = _ParaMpy(x, x);x = _ParaMpy(x, x);
	x = _ParaMpy(x, x);x = _ParaMpy(x, x);

	return x;
}


static	inline ParaType _OSlwToolMathExp1024(ParaType _x)
{

#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	register ParaType x = _ParaFrom(1) + (_x >> 10);
#else
	register ParaType x = _ParaFrom(1) + _ParaDiv(_x, _ParaFint(1024));
#endif

	x = _ParaMpy(x, x);x = _ParaMpy(x, x);x = _ParaMpy(x, x);
	x = _ParaMpy(x, x);x = _ParaMpy(x, x);x = _ParaMpy(x, x);
	x = _ParaMpy(x, x);x = _ParaMpy(x, x);x = _ParaMpy(x, x);
	x = _ParaMpy(x, x);

	return x;
}


#endif //OSLW_TOOL_IMPORT_MATH || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 4)
#endif
