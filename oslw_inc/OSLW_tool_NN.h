/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.h
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#ifndef OSLW_TOOL_NN_H_
#define OSLW_TOOL_NN_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_basic_math.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)

#if 0
typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT{
	
	OSlwToolMatrixSTU *(*Forward)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, OSlwToolMatrixSTU *pOUTX, OSlwToolMatrixSTU *pOut);
	OSlwToolMatrixSTU *(*Backward)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, OSlwToolMatrixSTU *pOUTX,OSlwToolMatrixSTU *pOut);
	ParaType _p1;
}OSlwToolNNLayerActFunSTU;

OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNSigmoid_Forward,(OSlwToolNNLayerActFunSTU *pAF,OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNSigmoid_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolSigmoid;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolTanh;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNLinear_Forward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNLinear_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolLinear;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNReLU_Forward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*,OSlwToolNNReLU_Backward,(OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn,OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolReLU;

OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNLeakyReLU_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNLeakyReLU_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolLReLU;

OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNSwish_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNSwish_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolSwish;

/*
typedef struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT {
	struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT *pLayer;
	lw_u32 sizeofdata;
	lw_u16 TrainTableColId;
	
}OSlwToolNNSubLayerBasicSTU;


typedef struct OSLW_TOOL_NN_SUB_FULL_CONNECTION_LAYER_STRUCT
{
	OSlwToolNNSubLayerBasicSTU basic;
	OSlwToolMatrixSTU w;
	OSlwToolMatrixSTU in, out;
	OSlwToolMatrixSTU inErr, outErr;

}OSlwToolNNSubLayerFullConSTU;


typedef struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT {
	OSlwToolRandomBasicSTU *pRand;
	OSlwExternMemorySTU SubLayer;

	struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT(*ForwardFun)(struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB);
	struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT(*BackWardFun)(struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB, OSlwToolTableSTU *pTable, lw_u16 row);
	struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT(*UpdateFun)(struct OSLW_TOOL_NN_LAYER_BASIC_STRUCT *pNNLB, OSlwToolTableSTU *pTable, lw_u16 row);


}OSlwToolNNLayerBasicSTU;


typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	OSlwToolNNLayerBasicSTU basic;
	ParaType nl;
	lw_u16 xout_dim_z;
	OSlwToolMatrixSTU out_x, out_y;
	OSlwToolMatrixSTU delt, Bias;
	OSlwToolMatrixSTU in, inErr, outErr;
	OSlwToolNNLayerActFunSTU *pActFun;


}OSlwToolNNLayerFullConSTU;
*/
//typedef OSlwToolNNSubLayerFullConSTU OSlwToolNNLayerFullConSTU;




/*
//初版失败 不是对一个向量x[1:n]进行归一化 而是对整个mini-batch求 均值是mini-batch的均值
typedef struct OSLW_TOOL_NN_BATCH_NORMAL_STRUCT
{
	ParaType BN_gamma;
	ParaType BN_beta;
	ParaType BN_Epsi;

	ParaType RealMean;
	ParaType RealVar;

	ParaType NowMean;
	ParaType NowVar;

	ParaType *_p_org_x;

}OSlwToolNNBatchNormSTU;
*/

typedef struct OSLW_TOOL_NN_DROP_OUT_STRUCT
{
	OSlwToolMatrixSTU DropOutMat;
	ParaType PossDropOut;

}OSlwToolNNDropOutSTU;

//旧版本
typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT{
	OSlwToolNNLayerActFunSTU *pActFun;
	lw_u16 size_row,size_col;
	ParaType nl;
	OSlwToolRandomBasicSTU *pRand;
	OSlwToolMatrixSTU in, w, out, _out_x;
	OSlwToolMatrixSTU inErr,outErr;
	union 
	{
		OSlwToolMatrixSTU bias;
		OSlwToolMatrixSTU _x_out_N;
	}BiasUN;

	union 
	{
		OSlwToolMatrixSTU delta;
		OSlwToolMatrixSTU _d_x_N;
	}DeltaUN;
	
	//OSlwToolNNBatchNormSTU *pBN;
	//ParaType PDropOut;
	OSlwToolNNDropOutSTU *pDropOut;
}OSlwToolNNLayerFullConSTU;

void OSlwToolBPNNLayerInitial(
	OSlwToolNNLayerFullConSTU *pNNL,
	OSlwToolNNLayerActFunSTU *pAF,
	lw_u16 size_row,lw_u16 size_col,
	ParaType *pW,
	ParaType *pBias,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType rmax,ParaType rmin,
	ParaType nl);

void OSlwToolBPNNLayerForwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X);
void OSlwToolBPNNLayerBackwardInitial(OSlwToolNNLayerFullConSTU *pNNL,ParaType *pINerr,ParaType *pdelt,ParaType *pOuterr);
//void OSlwToolBPNNLayerReSize(OSlwToolNNLayerFullConSTU *pNNL,lw_u16 r,lw_u16 c);
//void  OSlwToolBPNNDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,ParaType P ,ParaType *pWBuf,ParaType *pBiasBuf,lw_u16 *pList1,lw_u16 *pList2);



typedef ParaType (*LossFunTYPE)(OSlwToolMatrixSTU *s,OSlwToolMatrixSTU *ref,OSlwToolMatrixSTU *pre);


typedef enum
{
	OSlwToolNNTrainSaveMethod_OnceSave = 0,//单条存储 会将新计算出的delt加上原来的（一般使用 占用内存较小）
	OSlwToolNNTrainSaveMethod_AllSave = 1//完全存储 每一条delt是独立存储（用于算法优化使用）
}OSlwToolNNTrainSaveMethodNum;

typedef enum
{
	OSlwToolNNTrainUpdateMethod_Auto = 0,//自动更新 当计数器==样本总长度时 自动执行复习程序
	OSlwToolNNTrainUpdateMethod_Manu = 1//手动更新 当计数器==样本总长度时 不在存储 等待手动更新
}OSlwToolNNTrainUpdateMethodNum;


typedef enum
{
	OSlwToolNNTrainStatus_Wait = 0,
	OSlwToolNNTrainStatus_Complete = 1,
	OSlwToolNNTrainStatus_Over = 2,
	OSlwToolNNTrainStatus_Using = 3
}OSlwToolNNTrainStatusNum;

typedef struct OSLW_TOOL_NN_TRAIN_STRUCT {
	OSlwToolTableSTU DeltTable;
	OSlwToolRandomBasicSTU *pRand;
	LossFunTYPE LossFun;
	
	lw_u32 BatchSetLength;
	lw_u32 SampleRandNum;
	lw_u32 SampleSetLength;
	lw_u32 count;

	lw_u16 BatchSampleDiv;
	lw_u16 SampleCount;

	lw_u32 _Count_for_NN;

	struct 
	{
		OSlwToolNNTrainSaveMethodNum SaveMethod : 4;
		OSlwToolNNTrainUpdateMethodNum UpdateMethod : 4;
		OSlwToolNNTrainStatusNum Status : 4;
		lw_u16 rst : 4;
	}Flag;


}OSlwToolNNTrainSTU;

typedef enum
{
	OSlwToolNNReguType_NAN = 0,
	OSlwToolNNReguType_L1 = 1,
	OSlwToolNNReguType_L2 = 2

}OSlwToolNNReguTypeNUM;


typedef struct OSLW_TOOL_NN_REGU_STRUCT {
	ParaType Lambda;
	ParaType WeSum;
	
	OSlwToolNNReguTypeNUM ReguType:8;
	lw_u16 ReguSumFlag : 8;

}OSlwToolNNReguSTU;

typedef struct OSLW_TOOL_BP_NN_STRUCT{
	OSlwToolDListSTU Net;

	OSlwToolNNTrainSTU Train;
	OSlwToolNNReguSTU Regu;

	//LossFunTYPE loss;
	
	OSlwToolMatrixSTU x, y, ref;
	ParaType Error;
	ParaType _nl_factor;

	
	lw_u16 ParaGroupNum;
	lw_u32 WeightParaNum;
	lw_u32 BiasParaNum;

}OSlwToolBPNNSTU;




void OSlwToolBPNNInitial(OSlwToolBPNNSTU *pBPNN);
void*
OSlwToolBPNNLayerAppend_Dynamic
(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolNNLayerActFunSTU *pAF,
	lw_u16 row,
	lw_u16 col,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType Rmax, ParaType Rmin,
	ParaType nl,
	ParaType *pin, ParaType *pout,
	OSlwMemoryBasicSTU *pMem,
	ParaType BN_epsi
);

//损失层
void OSlwToolBPNNLossInitial(
	OSlwToolBPNNSTU *pBPNN,//this
	ParaType *pRef,//ref 地址（可为NULL）
	OSlwToolRandomBasicSTU *pRand,//随机数指针
	OSlwMemoryBasicSTU *pMem,//内存分配指针
	lw_u32 set_len,//样本总长度
	lw_u32 sample_len,//采样长度 
	LossFunTYPE loss,//损失计算函数
	OSlwToolNNTrainSaveMethodNum savem,//存储模式
	OSlwToolNNTrainUpdateMethodNum updatem//更新模式
);
void OSlwToolBPNNReguInitial(
	OSlwToolBPNNSTU *pBPNN, 
	OSlwToolNNReguTypeNUM ReguType, 
	ParaType lambda
);

OSlwToolBPNNSTU *OSlwToolBPNNAppend(OSlwToolBPNNSTU *pBPNN,OSlwToolDListNodeSTU *pDLN,OSlwToolNNLayerFullConSTU *pNNL);


OSlwToolBPNNSTU *OSlwToolBPNNRunning(OSlwToolBPNNSTU *_pBPNN);
OSlwToolBPNNSTU *OSlwToolBPNNErrCalu(OSlwToolBPNNSTU *pBPNN);//误差计算
OSlwToolBPNNSTU *OSlwToolBPNNDeltCalu(OSlwToolBPNNSTU *_pBPNN, lw_u16 _batch_index);//增量计算
ParaType *OSlwToolBPNNGradForInput(OSlwToolBPNNSTU *_pBPNN);//对输入求导
OSlwToolBPNNSTU *_OSlwToolBPNNReviewOnce(OSlwToolBPNNSTU *_pBPNN, lw_u16 batch_num, OSlwToolMatrixSTU *pmat_k);
OSlwToolBPNNSTU *OSlwToolBPNNReview(OSlwToolBPNNSTU *_pBPNN);//复习
OSlwToolBPNNSTU *OSlwToolBPNNTrain(OSlwToolBPNNSTU *_pBPNN);//训练

OSlwToolBPNNSTU *OSlwToolBPNNCopy(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2);
OSlwToolBPNNSTU *OSlwToolBPNNSoftReplace(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2, ParaType tu);

#endif


typedef struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT {
	lw_ptr(*Forward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
	lw_ptr(*Backward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
	lw_ptr(*Update)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
	lw_ptr(*NNmalloc)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
	lw_ptr(*TrainCompleteCB)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
	lw_ptr(*DataInit)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
	lw_ptr(*Copy)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
	lw_ptr(*SoftReplace)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);

	lw_u32 sizeofdata;

	OSlwMat in, out;
	ParaType nl;

	struct OSLW_TOOL_BP_NN_STRUCT *pNN;

}OSlwToolNNSubLayerBasicSTU;

#define _OSLW_TOOL_NN_SUB_BASIC_DEFAULT OSlwToolBPNNLayerForwardDefault,OSlwToolBPNNLayerBackwardDefault,OSlwToolBPNNLayerUpdateDefault,OSlwToolBPNNLayerNNmallocDefault,OSlwToolBPNNLayerTrainCompleteCallBackDefault,\
0,\
0,0,0,NULL,\
0,0,0,NULL,\
_ParaFint(0),\
NULL


typedef ParaType (*OSlwNNinitFunType)(void *p, ParaType d1, ParaType d2);

typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	OSlwToolNNSubLayerBasicSTU basic;

	OSlwMat Weight, Bias;
	OSlwMat DeltW, DeltB;

	OSlwNNinitFunType pfun;
	void *pr;
	ParaType initd1, initd2;

}OSlwToolNNLayerFullConSTU;




typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT {

	OSlwToolNNSubLayerBasicSTU basic;
	lw_u32 _real_size;
	void(*_init)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *p,lw_u8 TrainFlag);
	

}OSlwToolNNLayerActFunSTU;

#define _OSLW_TOOL_NN_ACT_FUN_DEFAULT(NAME,KIND) OSlwToolBPNNLayer##NAME##Forward,OSlwToolBPNNLayer##NAME##Backward,OSlwToolBPNNLayerUpdateDefault,OSlwToolBPNNLayerNNmallocDefault,\
OSlwToolBPNNLayerTrainCompleteCallBackDefault,OSlwToolBPNNLayerDataInitDefault,OSlwToolBPNNLayerCopyDefault,OSlwToolBPNNLayerSoftReplaceDefault,\
0,\
0,0,0,NULL,\
0,0,0,NULL,\
_ParaFint(0),\
NULL,\
sizeof(KIND),NULL


typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_SELU_STRUCT
{
	OSlwToolNNLayerActFunSTU basic;
	ParaType *pForward, *pBackword;
}OSlwToolNNLayerActFunSeLUSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConAppend(
	ParaType *pin,
	ParaType *pout,
	lw_u16 InCol,
	lw_u16 OutCol,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
);

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunAppend(
	ParaType *pin,
	ParaType *pout,
	lw_u16 Col,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem,
	OSlwToolNNLayerActFunSTU *pTemplet,
	lw_u8 TrainFlag
);


lw_ptr OSlwToolBPNNLayerForwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerBackwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerUpdateDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerNNmallocDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPNNLayerTrainCompleteCallBackDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerDataInitDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerCopyDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPNNLayerSoftReplaceDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);


lw_ptr OSlwToolBPNNLayerFullConForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPNNLayerFullConTrainCompleteCallBack(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConCopy(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPNNLayerFullConSoftReplace(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);


extern OSlwToolNNLayerActFunSTU _OSlwToolNNSigmoid;
extern OSlwToolNNLayerActFunSTU *LwSigmoid;
lw_ptr OSlwToolBPNNLayerSigmoidForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerSigmoidBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


extern OSlwToolNNLayerActFunSTU _OSlwToolNNTanh;
extern OSlwToolNNLayerActFunSTU *LwTanh;
lw_ptr OSlwToolBPNNLayerTanhForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerTanhBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNReLU;
extern OSlwToolNNLayerActFunSTU *LwReLU;
lw_ptr OSlwToolBPNNLayerReLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerReLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNReLU6;
extern OSlwToolNNLayerActFunSTU *LwReLU6;
lw_ptr OSlwToolBPNNLayerReLU6Forward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerReLU6Backward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNSwish;
extern OSlwToolNNLayerActFunSTU *LwSwish;
lw_ptr OSlwToolBPNNLayerSwishForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerSwishBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSeLUSTU _OSlwToolNNSeLU;
extern OSlwToolNNLayerActFunSTU *LwSeLU;
lw_ptr OSlwToolBPNNLayerSeLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerSeLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNSoftMax;
extern OSlwToolNNLayerActFunSTU *LwSoftMax;
lw_ptr OSlwToolBPNNLayerSoftMaxForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerSoftMaxBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

#define _OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB) 	register lw_u16 i, j, mmb;\
register ParaType *_in, *_out, *_inb, *_outb;\
OSLW_assert(!(pNNSLB));\
_inb = pNNSLB->in.a;\
_outb = pNNSLB->out.a;\
mmb = pNNSLB->out.col;\
for (i = 0; i < pNNSLB->in.row; i++)\
{\
	_in = _inb;\
	_out = _outb;\
	for (j = 0; j <(mini_b_num& 0xffff); j++)\
	{\


#define _OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB,mini_b_num) 			_in++;\
_out++;\
	}\
_inb += mmb;\
_outb += mmb;}\
return mini_b_num;\


typedef enum
{
	OSlwToolNNReguType_NAN = 0,
	OSlwToolNNReguType_L1 = 1,
	OSlwToolNNReguType_L2 = 2

}OSlwToolNNReguTypeNUM;


typedef struct OSLW_TOOL_NN_REGU_STRUCT {
	ParaType Lambda;
	ParaType WeSum;

	OSlwToolNNReguTypeNUM ReguType : 8;
	lw_u16 ReguSumFlag : 8;

}OSlwToolNNReguSTU;

typedef ParaType(*LossFunTYPE)(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre);




typedef enum
{
	OSlwToolNNTrainUpdateMethod_Auto = 0,//自动更新 当计数器==样本总长度时 自动执行复习程序
	OSlwToolNNTrainUpdateMethod_Manu = 1//手动更新 当计数器==样本总长度时 不在存储 等待手动更新
}OSlwToolNNTrainUpdateMethodNum;


typedef enum
{
	OSlwToolNNTrainStatus_Wait = 0,
	OSlwToolNNTrainStatus_Complete = 1,
	OSlwToolNNTrainStatus_Over = 2,
	OSlwToolNNTrainStatus_Using = 3
}OSlwToolNNTrainStatusNum;


typedef enum
{
	OSlwToolNNNeedTrain_Need = 0,
	OSlwToolNNNeedTrain_NeedNot = 1
}OSlwToolNNNeedTrainNum;

typedef enum
{
	OSlwToolNNMemoryMethod_Chip = 0,
	OSlwToolNNMemoryMethod_Block = 1

}OSlwToolNNMemoryMethodNum;

typedef struct OSLW_TOOL_NN_TRAIN_STRUCT {
	
	OSlwToolTableSTU DataTable;
	LossFunTYPE LossFun;
	ParaType nl;

	lw_u32 AllBatchCount;
	lw_u32 AllBatchMax;
	lw_u16 _batch_stream_count;

	lw_u16 mini_batch_now;
	lw_u16 mini_batch_max;


	struct
	{
		OSlwToolNNNeedTrainNum NeedTrain : 4;
		OSlwToolNNTrainUpdateMethodNum UpdateMethod : 4;
		OSlwToolNNTrainStatusNum Status : 4;
		OSlwToolNNMemoryMethodNum MemoryMethod : 4;
	}Flag;


}OSlwToolNNTrainSTU;



typedef struct OSLW_TOOL_BP_NN_STRUCT {
	OSlwToolDListSTU Net;

	OSlwToolNNTrainSTU Train;
	OSlwToolNNReguSTU Regu;
	

	OSlwMat x, y, ref;

	ParaType Error;
	ParaType _nl_factor;

	OSlwMemoryBasicSTU *pmem;

	lw_u16 ParaGroupNum;
}OSlwToolBPNNSTU;


void OSlwToolBPNNInit(OSlwToolBPNNSTU *pBPNN, lw_u16 max_mini_batch);
void OSlwToolBPNNLayerAppend
(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolDListNodeSTU *pnode,
	lw_u32 len,
	void *LayerList
);
void OSlwToolBPNNFullConAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_col, lw_u16 out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
);
void OSlwToolBPNNTrainInit(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 all_batch_max,
	LossFunTYPE loss,
	ParaType nl,
	OSlwToolNNTrainUpdateMethodNum updatem
);


void OSlwToolBPNNAllDataInit(OSlwToolBPNNSTU *pBPNN, OSlwMemoryBasicSTU *pMem);
void OSlwToolBPNNForward(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNBackward(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNLoad(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs, OSlwMat *ys);
void OSlwToolBPNNRun(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs, OSlwMat *ys);
void OSlwToolBPNNCalErr(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNTrain(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNReview(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNCopy(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2);
void OSlwToolBPNNSoftReplace(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2, ParaType Tau);
OSlwToolNNSubLayerBasicSTU *OSlwToolBPNNAt(OSlwToolBPNNSTU *pBPNN, lw_32 i, lw_32 j);

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

#endif
