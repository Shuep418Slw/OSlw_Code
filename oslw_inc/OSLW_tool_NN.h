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

typedef enum {
	OSlwToolNNSubLayerKind_FullCon='F',
	OSlwToolNNSubLayerKind_ActFun='A',
	OSlwToolNNSubLayerKind_Conv='C',
	OSlwToolNNSubLayerKind_Pool='P'
}OSlwToolNNSubLayerKind;

//神经网络层的基类
typedef struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT {

	lw_ptr(*Forward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//前向传递函数 输入:this 当前min_batch
	lw_ptr(*Backward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//反向传递函数 输入:this 当前min_batch
	lw_ptr(*Update)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//参数更新函数 输入:this
	lw_ptr(*NNmalloc)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);//内存分配函数 输入:this 前向传递参数指针 反向传递参数指针
	lw_ptr(*TrainCompleteCB)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//训练完成回调函数 一般用于清空delta 输入:this
	lw_ptr(*DataInit)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//数据初始化函数 用于初始化参数 输入:this
	lw_ptr(*Copy)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);//复制函数 输入:this1 this2
	lw_ptr(*SoftReplace)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);//软复制函数 输入:this1 this2
	
	OSlwToolNNSubLayerKind NN_Kind;
	
	lw_u32 sizeofdata;//sizeof(参数)
	
	OSlwExternMemorySTU FlowData;

	OSlwMat in, out;//输入矩阵与输出矩阵
	ParaType nl;//学习速率

	struct OSLW_TOOL_BP_NN_STRUCT *pNN;//指向本网络指针

}OSlwToolNNSubLayerBasicSTU;

#define _OSLW_TOOL_NN_SUB_BASIC_DEFAULT OSlwToolBPNNLayerForwardDefault,OSlwToolBPNNLayerBackwardDefault,OSlwToolBPNNLayerUpdateDefault,OSlwToolBPNNLayerNNmallocDefault,OSlwToolBPNNLayerTrainCompleteCallBackDefault,\
0,\
0,0,0,NULL,\
0,0,0,NULL,\
_ParaFint(0),\
NULL

//类型:初始化函数类型 可以适用于rand randi randn
typedef ParaType (*OSlwNNinitFunType)(void *p, ParaType d1, ParaType d2);


//神经网络全连接层
typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	OSlwToolNNSubLayerBasicSTU basic;//基类

	OSlwMat Weight, Bias;//权重与偏置
	OSlwMat DeltW, DeltB;//权重与偏置的修正量

	//权重初始化
	OSlwNNinitFunType pfun;//初始化函数 下面三个参数会原封不动输入内部
	void *pr;//
	ParaType initd1, initd2;//
	
	


	//偏置初始化（如果以下指针不为NULL 就调用权重初始化函数）
	//优先级为 _BiasInitFun>_BiasInit>pfun
	ParaType(*_BiasInitFun)();//偏置初始化函数 可以为NULL
	ParaType *_BiasInit;//偏置初始化数值 可以为NULL

	//数据恢复区指针
	ParaType *pRecover;
	
}OSlwToolNNLayerFullConSTU;



typedef struct OSLW_TOOL_NN_LAYER_CONVOLUTION_STRUCT{
	OSlwToolNNLayerFullConSTU databasic;//借用全连接类 作为基类
	lw_u16 in_x, in_y;//输入图像的大小
	lw_u16 out_x, out_y;//输出图像的大小
	lw_u16 conv_kernal_x, conv_kernal_y;//卷积核的大小
	lw_u16 conv_kernal_z;//卷积核的高度 == 输入图像的高度
	lw_u16 conv_kernal_num;//卷积核的数量 == 输出图像的高度
	lw_u16 move_delt;
	//ParaType *DataRes;//保留数据
}OSlwToolNNLayerConvSTU;


OSlwMat * _OSlwToolNNKernalConvFd
(
	OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *in_m, OSlwToolMatrixSTU *kern_m, OSlwToolMatrixSTU *bias,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y, lw_u16 out_z,
	lw_u16 move_delt,
	lw_u16 now_min_batch,
	lw_u8 move_method,
	ParaType *buf

);

OSlwToolMatrixSTU* _OSlwToolNNKernalConvBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr, OSlwToolMatrixSTU *kern,
	lw_u16 pic_x, lw_u16 pic_y, lw_u16 pic_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 now_min_batch,
	lw_u16 move_delt,
	ParaType *buf
);

ParaType _OSlwToolNNKernalConvCalDeltOnce
(
	ParaType *delt, ParaType *inerr, ParaType *outerr1,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 min_batch,
	lw_u16 move_delt
);


//新建卷积层
OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//输入图像维度 长 宽 高
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,//卷积核维度 长 宽 高
	lw_u16 move_delt,//移动间隔 默认1
	lw_u8 move_method,//移动模式
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwMemoryBasicSTU *pmem,//内存分配指针
	lw_u32 info[4]//下一层信息 分别为 输出图像 长 宽 高 总长度(用于与全连接连接）
);

typedef enum {

	OSlwToolNNPoolingMethod_Max='M',
	OSlwToolNNPoolingMethod_Avg='A'
}
OSlwToolNNPoolingMethodNUM;


typedef struct OSLW_TOOL_NN_LAYER_POOLING_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	ParaType **MaxPool;
	lw_u16 in_x, in_y;//输入图像的大小
	lw_u16 pic_z;//图像高度 输入输出共用
	lw_u16 out_x, out_y;//输出图像的大小
	lw_u16 move_x, move_y;//移动间隔
	OSlwToolNNPoolingMethodNUM PoolMethod : 16;
}OSlwToolNNLayerPoolSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPoolNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//输入图像维度 长 宽 高
	lw_u16 pool_x, lw_u16 pool_y,//池化大小
	OSlwToolNNPoolingMethodNUM pool_method,//池化方法
	lw_u16 max_mini_batch,//最大 minibatch
	OSlwMemoryBasicSTU *pmem,//内存分配指针
	lw_u32 info[4]//下一层信息 分别为 输出图像 长 宽 高 总长度(用于与全连接连接)
);

void* _OSlwToolNNMaxPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
);

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingFD
(
	OSlwToolMatrixSTU *in, OSlwToolMatrixSTU *out,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 out_x, lw_u16 out_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
);

OSlwToolMatrixSTU* _OSlwToolNNMaxPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y,
	ParaType **data_mem
);

OSlwToolMatrixSTU* _OSlwToolNNAvgPoolingBK
(
	OSlwToolMatrixSTU *inerr, OSlwToolMatrixSTU *outerr,
	lw_u16 inerr_x, lw_u16 inerr_y, lw_u16 inerr_z,
	lw_u16 outerr_x, lw_u16 outerr_y,
	lw_u16 now_min_batch,
	lw_u16 move_x, lw_u16 move_y
);


typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT {

	OSlwToolNNSubLayerBasicSTU basic;
	lw_u32 _real_size;
	void(*_init)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *p,lw_u8 TrainFlag);
	

}OSlwToolNNLayerActFunSTU;

#define _OSLW_TOOL_NN_ACT_FUN_DEFAULT(NAME,KIND) OSlwToolBPNNLayer##NAME##Forward,OSlwToolBPNNLayer##NAME##Backward,OSlwToolBPNNLayerUpdateDefault,OSlwToolBPNNLayerNNmallocDefault,\
OSlwToolBPNNLayerTrainCompleteCallBackDefault,OSlwToolBPNNLayerDataInitDefault,OSlwToolBPNNLayerCopyDefault,OSlwToolBPNNLayerSoftReplaceDefault,\
OSlwToolNNSubLayerKind_ActFun,0,\
NULL,0,\
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


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 InCol,//权重的行 = 输入的行
	lw_u16 OutCol,//权重的列 = 输出的行
	lw_u16 max_mini_batch,//最大minibatch
	OSlwMemoryBasicSTU *pmem//内存分配结构体
);

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunNew(
	ParaType *pin,//输入指针 为NULL则分配 
	ParaType *pout,//输出指针 为NULL则分配 
	lw_u16 Col,//列数
	lw_u16 max_mini_batch,//最大minibatch
	OSlwMemoryBasicSTU *pmem,//内存分配结构体
	OSlwToolNNLayerActFunSTU *pTemplet,//激活函数模板
	lw_u8 TrainFlag//是否训练
);



//层缺省函数
lw_ptr OSlwToolBPNNLayerForwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerBackwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerUpdateDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerNNmallocDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPNNLayerTrainCompleteCallBackDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerDataInitDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerCopyDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPNNLayerSoftReplaceDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);


//全连接层函数
lw_ptr OSlwToolBPNNLayerFullConForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPNNLayerFullConTrainCompleteCallBack(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPNNLayerFullConCopy(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPNNLayerFullConSoftReplace(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);

//卷积层函数
lw_ptr OSlwToolBPNNLayerConvForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerConvBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//池化层函数
lw_ptr OSlwToolBPNNLayerPoolForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPNNLayerPoolBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);



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

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

#define _OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB) 	register lw_u32 i, _len;\
register ParaType *_in, *_out;\
OSLW_assert(!(pNNSLB));\
_in = pNNSLB->in.a;\
_out = pNNSLB->out.a;\
_len = pNNSLB->out.col*(lw_u32)(mini_b_num);\
for (i = 0; i < _len;i++){
	


#define _OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB,mini_b_num) 			_in++;_out++;}\
return mini_b_num;\

#elif OSLW_TOOL_NN_DATA_FRAME == OSLW_TOOL_NN_D_FRAME_F

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

#else

#error"NN data frame is unknown"

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C




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



/*
typedef enum
{
	OSlwToolNNTrainUpdateMethod_Auto = 0,//自动更新 当计数器==样本总长度时 自动执行复习程序
	OSlwToolNNTrainUpdateMethod_Manu = 1//手动更新 当计数器==样本总长度时 不在存储 等待手动更新
}OSlwToolNNTrainUpdateMethodNum;
*/

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


typedef enum
{
	OSlwToolNNOptim_GradDesc = 0,
	OSlwToolNNOptim_M = 1,
	OSlwToolNNOptim_RMSp = 2,
	OSlwToolNNOptim_Adam = 3,
	OSlwToolNNOptim_Nadam = 4

}OSlwToolNNOptimNum;

typedef struct OSLW_TOOL_NN_TRAIN_STRUCT {
	
	OSlwToolTableSTU DataTable;
	LossFunTYPE LossFun;
	ParaType nl;

	ParaType Beta1, Beta1T;
	ParaType Beta2, Beta2T;
	ParaType Epsi;

	lw_u32 AllBatchCount;
	lw_u32 AllBatchMax;

	lw_u16 _batch_stream_count;

	lw_u16 mini_batch_now;
	lw_u16 mini_batch_max;

	lw_u16 _MemAllocCoff;

	struct
	{
		OSlwToolNNNeedTrainNum NeedTrain : 2;
		//OSlwToolNNTrainUpdateMethodNum UpdateMethod : 2;
		OSlwToolNNTrainStatusNum Status : 4;
		OSlwToolNNMemoryMethodNum MemoryMethod : 4;
		OSlwToolNNOptimNum Optim : 4;
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


#define NN_LAYER_S_BAK_ARR_LEN 6


typedef struct OSLW_TOOL_NN_LAYER_SIMPLE_BACK_UP_STRUCT{

	OSlwToolNNSubLayerKind NN_Kind;
	lw_u32 info[NN_LAYER_S_BAK_ARR_LEN];
	
	OSlwToolNNLayerActFunSTU *pTemplet;
	
	ParaType *pDataAddr;
	
}OSlwToolNNLayerSimpleBakSTU;



void OSlwToolBPNNInit(OSlwToolBPNNSTU *pBPNN, lw_u16 max_mini_batch);

void OSlwToolBPNNLayerAppend
(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolDListNodeSTU *pnode,
	lw_u32 len,
	void *LayerList
);


void* OSlwToolBPNNFullConAppend
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

void* OSlwToolBPNNConvAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);

void* OSlwToolBPNNPoolAppend
(
	OSlwToolBPNNSTU *pBPNN,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	OSlwToolNNPoolingMethodNUM pool_method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);


void OSlwToolBPNNTrainInit(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	LossFunTYPE loss,
	ParaType nl
);

void OSlwToolBPNNOptimInit(
	OSlwToolBPNNSTU *pBPNN,
	OSlwToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
);

void* OSlwToolNNLayerSimpleRecover
(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPNNSimpleRecover
(
	OSlwToolBPNNSTU *pBPNN,
	ParaType *in,ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	lw_u16 simple_layer_len,
	OSlwMemoryBasicSTU *pmem
);



void OSlwToolBPNNAllDataInit(OSlwToolBPNNSTU *pBPNN, OSlwMemoryBasicSTU *pMem);
void OSlwToolBPNNForward(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNBackward(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNLoadX(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs);
void OSlwToolBPNNLoadY(OSlwToolBPNNSTU *pBPNN, OSlwMat *ys);
void OSlwToolBPNNRun(OSlwToolBPNNSTU *pBPNN, OSlwMat *xs);
void OSlwToolBPNNCalErr(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNTrain(OSlwToolBPNNSTU *pBPNN, OSlwMat *ys);
void OSlwToolBPNNReview(OSlwToolBPNNSTU *pBPNN);
void OSlwToolBPNNCopy(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2);
void OSlwToolBPNNSoftReplace(OSlwToolBPNNSTU *pBPNN1, OSlwToolBPNNSTU *pBPNN2, ParaType Tau);
OSlwToolNNSubLayerBasicSTU *OSlwToolBPNNAt(OSlwToolBPNNSTU *pBPNN, lw_32 i, lw_32 j);

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

#endif
