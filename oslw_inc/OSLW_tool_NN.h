/*(Ver.=0.97)
 * OSLW_tool.h
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#ifndef OSLW_TOOL_NN_H_
#define OSLW_TOOL_NN_H_

#include "OSLW_define.h"
 //#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_basic_math.h"

#if !(OSLW_SIMPLE_LEVEL >= 2)

#if 0
typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT {

	OSlwToolMatrixSTU *(*Forward)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, OSlwToolMatrixSTU *pOUTX, OSlwToolMatrixSTU *pOut);
	OSlwToolMatrixSTU *(*Backward)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *pAF, OSlwToolMatrixSTU *pOUTX, OSlwToolMatrixSTU *pOut);
	ParaType _p1;
}OSlwToolNNLayerActFunSTU;

OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNSigmoid_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNSigmoid_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolSigmoid;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNTanh_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolTanh;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNLinear_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNLinear_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
extern  OSlwToolNNLayerActFunSTU OSlwToolLinear;


OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNReLU_Forward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
OSLW_TOOL_FUN_STATEMENT
(OSlwToolMatrixSTU*, OSlwToolNNReLU_Backward, (OSlwToolNNLayerActFunSTU *pAF, OSlwToolMatrixSTU *pIn, OSlwToolMatrixSTU *pOut));
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
//����ʧ�� ���Ƕ�һ������x[1:n]���й�һ�� ���Ƕ�����mini-batch�� ��ֵ��mini-batch�ľ�ֵ
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

//�ɰ汾
typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT {
	OSlwToolNNLayerActFunSTU *pActFun;
	lw_u16 size_row, size_col;
	ParaType nl;
	OSlwToolRandomBasicSTU *pRand;
	OSlwToolMatrixSTU in, w, out, _out_x;
	OSlwToolMatrixSTU inErr, outErr;
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

void OSlwToolBPnnLayerInitial(
	OSlwToolNNLayerFullConSTU *pNNL,
	OSlwToolNNLayerActFunSTU *pAF,
	lw_u16 size_row, lw_u16 size_col,
	ParaType *pW,
	ParaType *pBias,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType rmax, ParaType rmin,
	ParaType nl);

void OSlwToolBPnnLayerForwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pIN, ParaType *pOUT, ParaType *pOUT_X);
void OSlwToolBPnnLayerBackwardInitial(OSlwToolNNLayerFullConSTU *pNNL, ParaType *pINerr, ParaType *pdelt, ParaType *pOuterr);
//void OSlwToolBPnnLayerReSize(OSlwToolNNLayerFullConSTU *pNNL,lw_u16 r,lw_u16 c);
//void  OSlwToolBPnnDropOutInitial(OSlwToolNNLayerFullConSTU *pNNL,ParaType P ,ParaType *pWBuf,ParaType *pBiasBuf,lw_u16 *pList1,lw_u16 *pList2);



typedef ParaType(*LossFunTYPE)(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre);


typedef enum
{
	OSlwToolNNTrainSaveMethod_OnceSave = 0,//�����洢 �Ὣ�¼������delt����ԭ���ģ�һ��ʹ�� ռ���ڴ��С��
	OSlwToolNNTrainSaveMethod_AllSave = 1//��ȫ�洢 ÿһ��delt�Ƕ����洢�������㷨�Ż�ʹ�ã�
}OSlwToolNNTrainSaveMethodNum;

typedef enum
{
	OSlwToolNNTrainUpdateMethod_Auto = 0,//�Զ����� ��������==�����ܳ���ʱ �Զ�ִ�и�ϰ����
	OSlwToolNNTrainUpdateMethod_Manu = 1//�ֶ����� ��������==�����ܳ���ʱ ���ڴ洢 �ȴ��ֶ�����
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

	OSlwToolNNReguTypeNUM ReguType : 8;
	lw_u16 ReguSumFlag : 8;

}OSlwToolNNReguSTU;

typedef struct OSLW_TOOL_BP_NN_STRUCT {
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

}OSlwToolBPnnSTU;




void OSlwToolBPnnInitial(OSlwToolBPnnSTU *pBPnn);
void*
OSlwToolBPnnLayerAppend_Dynamic
(
	OSlwToolBPnnSTU *pBPnn,
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

//��ʧ��
void OSlwToolBPnnLossInitial(
	OSlwToolBPnnSTU *pBPnn,//this
	ParaType *pRef,//ref ��ַ����ΪNULL��
	OSlwToolRandomBasicSTU *pRand,//�����ָ��
	OSlwMemoryBasicSTU *pMem,//�ڴ����ָ��
	lw_u32 set_len,//�����ܳ���
	lw_u32 sample_len,//�������� 
	LossFunTYPE loss,//��ʧ���㺯��
	OSlwToolNNTrainSaveMethodNum savem,//�洢ģʽ
	OSlwToolNNTrainUpdateMethodNum updatem//����ģʽ
);
void OSlwToolBPnnReguInitial(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolNNReguTypeNUM ReguType,
	ParaType lambda
);

OSlwToolBPnnSTU *OSlwToolBPnnAppend(OSlwToolBPnnSTU *pBPnn, OSlwToolDListNodeSTU *pDLN, OSlwToolNNLayerFullConSTU *pNNL);


OSlwToolBPnnSTU *OSlwToolBPnnRunning(OSlwToolBPnnSTU *_pBPnn);
OSlwToolBPnnSTU *OSlwToolBPnnErrCalu(OSlwToolBPnnSTU *pBPnn);//������
OSlwToolBPnnSTU *OSlwToolBPnnDeltCalu(OSlwToolBPnnSTU *_pBPnn, lw_u16 _batch_index);//��������
ParaType *OSlwToolBPnnGradForInput(OSlwToolBPnnSTU *_pBPnn);//��������
OSlwToolBPnnSTU *_OSlwToolBPnnReviewOnce(OSlwToolBPnnSTU *_pBPnn, lw_u16 batch_num, OSlwToolMatrixSTU *pmat_k);
OSlwToolBPnnSTU *OSlwToolBPnnReview(OSlwToolBPnnSTU *_pBPnn);//��ϰ
OSlwToolBPnnSTU *OSlwToolBPnnTrain(OSlwToolBPnnSTU *_pBPnn);//ѵ��

OSlwToolBPnnSTU *OSlwToolBPnnCopy(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2);
OSlwToolBPnnSTU *OSlwToolBPnnSoftReplace(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2, ParaType tu);

#endif

typedef enum {
	OSlwToolNNSubLayerKind_FullCon = 'F',
	OSlwToolNNSubLayerKind_ActFun = 'A',
	OSlwToolNNSubLayerKind_Conv = 'C',
	OSlwToolNNSubLayerKind_Pool = 'P',
	OSlwToolNNSubLayerKind_BasicRnn = 'R',
	OSlwToolNNSubLayerKind_GruRnn = 'G',
	OSlwToolNNSubLayerKind_LNorm = 'L',
	OSlwToolNNSubLayerKind_INorm = 'I',
	OSlwToolNNSubLayerKind_Shift = 's',
	OSlwToolNNSubLayerKind_Split = 'S',
	OSlwToolNNSubLayerKind_Mix = 'M',
	OSlwToolNNSubLayerKind_Extend = 'E',
	OSlwToolNNSubLayerKind_Pad = 'p'
}OSlwToolNNSubLayerKind;

//�������Ļ���
typedef struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT {
	OSlwMat in, out;//����������������
	lw_ptr(*Forward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//ǰ�򴫵ݺ��� ����:this ��ǰmin_batch
	lw_ptr(*Backward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//���򴫵ݺ��� ����:this ��ǰmin_batch
	lw_ptr(*Update)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//�������º��� ����:this
	lw_ptr(*NNmalloc)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);//�ڴ���亯�� ����:this ǰ�򴫵ݲ���ָ�� ���򴫵ݲ���ָ��
	lw_ptr(*Clear)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//��������������delta ����:this
	lw_ptr(*DataInit)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);//���ݳ�ʼ������ ���ڳ�ʼ������ ����:this
	lw_ptr(*Copy)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);//���ƺ��� ����:this1 this2
	lw_ptr(*SoftReplace)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);//���ƺ��� ����:this1 this2

	OSlwToolNNSubLayerKind NN_Kind : 8;
	lw_u16 RunningStopFlag : 8;

	lw_u32 sizeofdata;//sizeof(����)

	OSlwExternMemorySTU FlowData;


	ParaType nl;//ѧϰ����
	ParaType LamdaL2;//L2����ϵ��

	struct OSLW_TOOL_BP_NN_STRUCT *pNN;//ָ������ָ��

}OSlwToolNNSubLayerBasicSTU;

#define _OSLW_TOOL_NN_SUB_BASIC_DEFAULT OSlwToolBPnnLayerForwardDefault,OSlwToolBPnnLayerBackwardDefault,OSlwToolBPnnLayerUpdateDefault,OSlwToolBPnnLayerNNmallocDefault,OSlwToolBPnnLayerClearDefault,\
0,\
0,0,0,NULL,\
0,0,0,NULL,\
_ParaFint(0),\
NULL

//����:��ʼ���������� ����������rand randi randn
typedef ParaType(*OSlwNNinitFunType)(void *p, ParaType d1, ParaType d2);


//������ȫ���Ӳ�
typedef struct OSLW_TOOL_NN_LAYER_FULL_CONNECTION_STRUCT
{
	OSlwToolNNSubLayerBasicSTU basic;//����

	OSlwMat Weight, Bias;//Ȩ����ƫ��
	OSlwMat DeltW, DeltB;//Ȩ����ƫ�õ�������

	//Ȩ�س�ʼ��
	OSlwNNinitFunType pfun;//��ʼ������ ��������������ԭ�ⲻ�������ڲ�
	void *pr;//
	ParaType initd1, initd2;//

	//ƫ�ó�ʼ�����������ָ�벻ΪNULL �͵���Ȩ�س�ʼ��������
	//���ȼ�Ϊ _BiasInitFun>_BiasInit>pfun
	ParaType(*_BiasInitFun)();//ƫ�ó�ʼ������ ����ΪNULL
	ParaType *_BiasInit;//ƫ�ó�ʼ����ֵ ����ΪNULL

	//���ݻָ���ָ��
	ParaType *pRecover;


}OSlwToolNNLayerFullConSTU;

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerFullConNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	LwMatColType InCol,//Ȩ�ص��� = �������
	LwMatColType OutCol,//Ȩ�ص��� = �������
	lw_u16 max_mini_batch,//���minibatch
	OSlwMemoryBasicSTU *pmem//�ڴ����ṹ��
);

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerShiftNew(
	ParaType *pin,
	ParaType *pout,
	LwMatColType Col,
	LwMatColType weight_len,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
);

typedef struct OSLW_TOOL_NN_LAYER_CONVOLUTION_STRUCT {
	OSlwToolNNLayerFullConSTU databasic;//����ȫ������ ��Ϊ����
	lw_u16 in_x, in_y;//����ͼ��Ĵ�С
	lw_u16 out_x, out_y;//���ͼ��Ĵ�С
	lw_u16 conv_kernal_x, conv_kernal_y;//����˵Ĵ�С
	lw_u16 conv_kernal_z;//����˵ĸ߶� == ����ͼ��ĸ߶�
	lw_u16 conv_kernal_num;//����˵����� == ���ͼ��ĸ߶�
	lw_u16 move_delt;
	OSlwToolMatrixConvMethodNUM ConvMethod : 16;
	lw_u16 im2col_flag;
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


//�½������
OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//����ͼ��ά�� �� �� ��
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,//�����ά�� �� �� ��
	lw_u16 move_delt,//�ƶ���� Ĭ��1
	OSlwToolMatrixConvMethodNUM move_method,//�ƶ�ģʽ
	lw_u16 max_mini_batch,//��� minibatch
	OSlwMemoryBasicSTU *pmem,//�ڴ����ָ��
	lw_u32 info[4]//��һ����Ϣ �ֱ�Ϊ ���ͼ�� �� �� �� �ܳ���(������ȫ�������ӣ�
);

//���þ����Ϊim2colģʽ
OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerConvSetIm2Col(
	OSlwToolNNSubLayerBasicSTU *pbase,
	lw_u32 udata_sizeof_pdata,
	void *pdata
);



typedef enum {

	OSlwToolNNPoolingMethod_Max = 'M',
	OSlwToolNNPoolingMethod_Avg = 'A'
}
OSlwToolNNPoolingMethodNUM;


typedef struct OSLW_TOOL_NN_LAYER_POOLING_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	ParaType **MaxPool;
	lw_u16 in_x, in_y;//����ͼ��Ĵ�С
	lw_u16 pic_z;//ͼ��߶� �����������
	lw_u16 out_x, out_y;//���ͼ��Ĵ�С
	lw_u16 move_x, move_y;//�ƶ����
	OSlwToolNNPoolingMethodNUM PoolMethod : 16;
}OSlwToolNNLayerPoolSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPoolNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,//����ͼ��ά�� �� �� ��
	lw_u16 pool_x, lw_u16 pool_y,//�ػ���С
	OSlwToolNNPoolingMethodNUM pool_method,//�ػ�����
	lw_u16 max_mini_batch,//��� minibatch
	OSlwMemoryBasicSTU *pmem,//�ڴ����ָ��
	lw_u32 info[4]//��һ����Ϣ �ֱ�Ϊ ���ͼ�� �� �� �� �ܳ���(������ȫ��������)
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
	void(*_init)(struct OSLW_TOOL_NN_LAYER_ACT_FUN_STRUCT *p, lw_u8 TrainFlag);


}OSlwToolNNLayerActFunSTU;

#define _OSLW_TOOL_NN_ACT_FUN_DEFAULT(NAME,KIND) NULL,0,0,0,NULL,0,0,0,\
OSlwToolBPnnLayer##NAME##Forward,OSlwToolBPnnLayer##NAME##Backward,OSlwToolBPnnLayerUpdateDefault,OSlwToolBPnnLayerNNmallocDefault,\
OSlwToolBPnnLayerClearDefault,OSlwToolBPnnLayerDataInitDefault,OSlwToolBPnnLayerCopyDefault,OSlwToolBPnnLayerSoftReplaceDefault,\
OSlwToolNNSubLayerKind_ActFun,0,0,\
NULL,0,\
_ParaFint(0),_ParaFint(0),\
NULL,\
sizeof(KIND),NULL

typedef struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT {
	OSlwMat cell_in_now;
	OSlwMat *cell_out_now_handle;

	OSlwExternMemorySTU cell_h_mem, cell_out_mem;

	ParaType *(*cell_forward)(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag);
	ParaType *(*cell_backward)(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, lw_u8 TrainFlag);
	ParaType *(*cell_load)(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, LwMatRowType index);
	ParaType *(*cell_parameter_init)(struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_BASIC_STRUCT *pCELL, ParaType *we, ParaType *bi, ParaType *dwe, ParaType *dbi, ParaType *stack);

	OSlwExternMemorySTU cell_we, cell_bi;
	ParaType *p_cell_dwe, *p_cell_dbi;
	lw_u32 FlowDataLen;
}OSlwToolNNLayerRnnCellBasicSTU;

typedef void* (*OSlwToolNNLayerRnnCellNewFunType)(ParaType *, LwMatColType, LwMatColType, lw_u16, void *, void *);



void* OSlwToolNNLayerRnnCellNew(
	ParaType *pout,
	LwMatColType in_size,//�����CELL�������С ��ӦӦ����RNN������+���
	LwMatColType out_size,
	lw_u16 max_min_batch,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
);

#define LwRnnCellType1 OSlwToolNNLayerRnnCellNew


typedef struct OSLW_TOOL_NN_LAYER_RECURRENT_CELL_STRUCT {
	OSlwToolNNLayerRnnCellBasicSTU base;

	struct
	{
		OSlwMat in, out;
		lw_ptr(*Forward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//ǰ�򴫵ݺ��� ����:this ��ǰmin_batch
		lw_ptr(*Backward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//���򴫵ݺ��� ����:this ��ǰmin_batch

	}ActFun;

}OSlwToolNNLayerRnnCellSTU;



typedef struct OSLW_TOOL_NN_LAYER_RECURRENT_LNORM_CELL_STRUCT {
	OSlwToolNNLayerRnnCellBasicSTU base;

	struct
	{
		OSlwMat in, out;
		lw_ptr(*Forward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//ǰ�򴫵ݺ��� ����:this ��ǰmin_batch
		lw_ptr(*Backward)(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);//���򴫵ݺ��� ����:this ��ǰmin_batch
	}ActFun;

	ParaType *pNormWe, *pNormBi;
	ParaType *pmean, *pvar;
	ParaType *pHt, *pBuf;

}OSlwToolNNLayerRnnCellLNormSTU;

void* OSlwToolNNLayerRnnCellLNormNew(
	ParaType *pout,
	LwMatColType in_size,//�����CELL�������С ��ӦӦ����RNN������+���
	LwMatColType out_size,
	lw_u16 max_min_batch,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
);

#define LwRnnCellType2 OSlwToolNNLayerRnnCellLNormNew

typedef struct OSLW_TOOL_NN_LAYER_BASIC_RECURRENT_STRUCT {
	OSlwToolNNLayerFullConSTU databasic;//����ȫ������ ��Ϊ����
	OSlwToolNNLayerRnnCellBasicSTU *pRnnCell;
	OSlwMat out_t_1_mem;
	lw_u8 NeedTrainFlag;
}OSlwToolNNLayerBasicRnnSTU;



OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerBasicRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pOut_t_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
);

//void OSlwToolNNLayerRnnCellForword(
//	OSlwToolNNLayerRnnCellSTU *pCell
//);
//void OSlwToolNNLayerRnnCellBackword(
//	OSlwToolNNLayerRnnCellSTU *pCell
//);
//void OSlwToolNNLayerRnnCellInit(
//	OSlwToolNNLayerRnnCellSTU *pCell,
//	lw_u16 in_size,//�����CELL�������С ��ӦӦ����RNN������+���
//	lw_u16 out_size,
//	ParaType *pin,//ֻ��Ҫ���� ��Ϊ���ز������Ҫ����ʱ���ȷ��
//	ParaType *pwe, ParaType *pbi,
//	ParaType *pdwe, ParaType *pdbi,
//	OSlwToolNNSubLayerBasicSTU *pActTemplet
//);



typedef struct OSLW_TOOL_NN_LAYER_GRU_RECURRENT_STRUCT {
	OSlwToolNNLayerFullConSTU databasic;//����ȫ������ ��Ϊ����

	//RNN��Ԫ
	OSlwToolNNLayerRnnCellBasicSTU *CoreCellHandle;
	//������
	OSlwToolNNLayerRnnCellBasicSTU *ResetGateHandle;
	//������
	OSlwToolNNLayerRnnCellBasicSTU *UpdateGateHandle;
	//��һ�ּ���
	OSlwMat OutT_1;
	lw_u8 NeedTrainFlag;
}OSlwToolNNLayerGruRnnSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerGruRnnNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pResetGateOut,
	ParaType *pUpdateGateOut,
	ParaType *pCoreCellOut,
	ParaType *pOutT_1,
	LwMatColType InCol,
	LwMatColType OutCol,
	lw_u16 max_mini_batch,
	lw_u8 NeedTrainFlag,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *ptem,
	OSlwMemoryBasicSTU *pmem
);



typedef struct OSLW_TOOL_NN_LAYER_SPILT_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	LwMatColType ShapeList[4];
	ParaType **OutList;
	LwMatColType *SplitList;
	lw_u16 OutNum;//�����������65535
	lw_u16 Indim;//ά�Ȳ�̫���ܳ���65535
}OSlwToolNNLayerSplitSTU;

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerSplitNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType **poutList,//���ָ���б� ΪNULL����� 
	lw_u16 Dim, // ��������ά��
	LwMatColType *ShapeList,//����������״
	lw_u16 SplitNum,//Ҫ�гɵĿ�����
	LwMatColType *SplitList,//������з�
	lw_u16 max_mini_batch,//��� minibatch
	OSlwMemoryBasicSTU *pmem//�ڴ����ָ��
);

typedef struct OSLW_TOOL_NN_LAYER_MIX_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	LwMatColType ShapeList[4];
	ParaType **InList;
	LwMatColType *MixList;
	lw_u16 InNum;//����������ܳ���65535
	lw_u16 Outdim;//ά�Ȳ�̫���ܳ���65535
}OSlwToolNNLayerMixSTU;

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerMixNew(
	ParaType **pinList,//����ָ���б� ΪNULL����� 
	ParaType *pout,//���ָ���б� ΪNULL����� 
	lw_u16 Dim, // �������ά��
	LwMatColType *ShapeList,//���������״
	lw_u16 MixNum,//������
	LwMatColType *MixList,//������з�
	lw_u16 max_mini_batch,//��� minibatch
	OSlwMemoryBasicSTU *pmem//�ڴ����ָ��
);

typedef enum {
	OSlwToolNNPad_Constant = 0,
	OSlwToolNNPad_Reflect = 1
}OSlwToolNNPadNUM;

typedef struct OSLW_TOOL_NN_LAYER_PAD_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	LwMatColType ShapeList[4];
	LwMatColType Padlist[4];
	lw_u8 PadLen;
	lw_u8 IOdim : 4;
	OSlwToolNNPadNUM PadMethod : 4;
}OSlwToolNNLayerPadSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerPadNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	lw_u8 Dim, // IO��ά��
	LwMatColType *ShapeList,//���������״
	LwMatColType *PadList,//Pad�ķ��� 1ά����2ά
	lw_u16 max_mini_batch,//��� minibatch
	OSlwToolNNPadNUM method,
	OSlwMemoryBasicSTU *pmem,//�ڴ����ָ��
	lw_u32 info[4]
);


typedef enum {
	OSlwToolNNExtend_Nearest = 0,
	OSlwToolNNExtend_Linear = 1
}OSlwToolNNExtendNUM;

typedef struct OSLW_TOOL_NN_LAYER_EXTEND_STRUCT {
	OSlwToolNNSubLayerBasicSTU basic;
	LwMatColType ShapeList[4];
	LwMatColType Extlist[2];
	lw_u8 ExtLen;
	lw_u8 IOdim : 4;
	OSlwToolNNExtendNUM ExtendMethod : 4;
}OSlwToolNNLayerExtendSTU;

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerExtendNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	lw_u8 Dim, // IO��ά��
	LwMatColType *ShapeList,//���������״
	LwMatColType *ExtList,//��չ�ķ��� 1ά����2ά
	lw_u16 max_mini_batch,//��� minibatch
	OSlwToolNNExtendNUM method,
	OSlwMemoryBasicSTU *pmem,//�ڴ����ָ��
	lw_u32 info[4]
);


typedef struct OSLW_TOOL_NN_LAYER_ACT_FUN_SELU_STRUCT
{
	OSlwToolNNLayerActFunSTU basic;
	ParaType *pForward, *pBackword;
}OSlwToolNNLayerActFunSeLUSTU;


OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerActFunNew(
	ParaType *pin,//����ָ�� ΪNULL����� 
	ParaType *pout,//���ָ�� ΪNULL����� 
	LwMatColType Col,//����
	lw_u16 max_mini_batch,//���minibatch
	OSlwMemoryBasicSTU *pmem,//�ڴ����ṹ��
	OSlwToolNNLayerActFunSTU *pTemplet,//�����ģ��
	lw_u8 TrainFlag//�Ƿ�ѵ��
);



typedef struct OSLW_TOOL_NN_LAYER_NORM_STRUCT {
	OSlwToolNNLayerFullConSTU databasic;//����ȫ������ ��Ϊ����
	ParaType *pMean;
	ParaType *pVar;
}OSlwToolNNLayerLNormSTU;

ParaType _OSlwToolNNLayerLnForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType col
);


ParaType _OSlwToolNNLayerLnBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType col,
	ParaType *pBuf
);

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerLNormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType Col,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
);

typedef struct OSLW_TOOL_NN_INSTANCE_NORM_STRUCT {
	OSlwToolNNLayerFullConSTU databasic;//����ȫ������ ��Ϊ����
	ParaType *pMean;
	ParaType *pVar;
	LwMatColType in_x, in_y, in_z;
}OSlwToolNNLayerINormSTU;


ParaType _OSlwToolNNLayerInForwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType mean,
	ParaType var,
	LwMatColType x_mpy_y
);


ParaType _OSlwToolNNLayerInBackwordSub(
	ParaType *in,
	ParaType *out,
	ParaType *we,
	ParaType *bi,
	ParaType *dwe,
	ParaType *dbi,
	ParaType mean,
	ParaType div,
	LwMatColType x_mpy_y,
	ParaType *pBuf
);

OSlwToolNNSubLayerBasicSTU * OSlwToolNNLayerINormNew(
	ParaType *pin,
	ParaType *pout,
	ParaType *pmean,
	ParaType *pvar,
	LwMatColType in_x,
	LwMatColType in_y,
	LwMatColType in_z,
	lw_u16 max_mini_batch,
	OSlwMemoryBasicSTU *pmem
);


//��ȱʡ����
lw_ptr OSlwToolBPnnLayerForwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerBackwardDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerUpdateDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerNNmallocDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPnnLayerClearDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerDataInitDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerCopyDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPnnLayerSoftReplaceDefault(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);


//ȫ���Ӳ㺯��
lw_ptr OSlwToolBPnnLayerFullConForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerFullConBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerFullConUpdate(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerFullConNNmalloc(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, void *pmemForward, void *pmemBackward);
lw_ptr OSlwToolBPnnLayerFullConClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerFullConDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerFullConCopy(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2);
lw_ptr OSlwToolBPnnLayerFullConSoftReplace(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB1, struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB2, ParaType Raido);

lw_ptr OSlwToolBPnnLayerShiftForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerShiftBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//����㺯��
lw_ptr OSlwToolBPnnLayerConvForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerConvBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//�ػ��㺯��
lw_ptr OSlwToolBPnnLayerPoolForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerPoolBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//BasicRnn����
lw_ptr OSlwToolNNLayerBasicRnnBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerBasicRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerBasicRnnDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerBasicRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);


//GRURNN����
lw_ptr OSlwToolNNLayerGruRnnBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerGruRnnForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerGruRnnDataInit(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);
lw_ptr OSlwToolBPnnLayerGruRnnClear(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB);


//layer norm����
lw_ptr OSlwToolNNLayerLNormForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerLNormBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//Instance norm����
lw_ptr OSlwToolNNLayerINormForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerINormBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//�зֲ㺯��
lw_ptr OSlwToolNNLayerSplitForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerSplitBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//��ϲ㺯��
lw_ptr OSlwToolNNLayerMixForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerMixBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

//pad
lw_ptr OSlwToolNNLayerPadForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerPadBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


//��չ��
lw_ptr OSlwToolNNLayerExtendForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolNNLayerExtendBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


extern OSlwToolNNLayerActFunSTU _OSlwToolNNSigmoid;
extern OSlwToolNNLayerActFunSTU *LwSigmoid;
lw_ptr OSlwToolBPnnLayerSigmoidForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerSigmoidBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);


extern OSlwToolNNLayerActFunSTU _OSlwToolNNTanh;
extern OSlwToolNNLayerActFunSTU *LwTanh;
lw_ptr OSlwToolBPnnLayerTanhForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerTanhBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNReLU;
extern OSlwToolNNLayerActFunSTU *LwReLU;
lw_ptr OSlwToolBPnnLayerReLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerReLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNReLU6;
extern OSlwToolNNLayerActFunSTU *LwReLU6;
lw_ptr OSlwToolBPnnLayerReLU6Forward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerReLU6Backward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNSwish;
extern OSlwToolNNLayerActFunSTU *LwSwish;
lw_ptr OSlwToolBPnnLayerSwishForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerSwishBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSeLUSTU _OSlwToolNNSeLU;
extern OSlwToolNNLayerActFunSTU *LwSeLU;
lw_ptr OSlwToolBPnnLayerSeLUForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerSeLUBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

extern OSlwToolNNLayerActFunSTU _OSlwToolNNSoftMax;
extern OSlwToolNNLayerActFunSTU *LwSoftMax;
lw_ptr OSlwToolBPnnLayerSoftMaxForward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);
lw_ptr OSlwToolBPnnLayerSoftMaxBackward(struct OSLW_TOOL_NN_SUB_LAYER_BASIC_STRUCT *pNNSLB, lw_ptr mini_b_num);

#if OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C

#define _OSLW_TOOL_NN_ACTFUN_SEMI1(pNNSLB) 	register lw_u32 i, _len;\
register ParaType *_in, *_out;\
OSLW_assert(!(pNNSLB));\
_in = pNNSLB->in.a;\
_out = pNNSLB->out.a;\
_len = pNNSLB->out.col*(lw_u32)(mini_b_num);\
for (i = 0; i < _len;i++){



#define _OSLW_TOOL_NN_ACTFUN_SEMI2(pNNSLB,mini_b_num) 			_in++;_out++;}\
return mini_b_num;

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
return mini_b_num;

#else

#error"NN data frame is unknown"

#endif // OSLW_TOOL_NN_DATA_FRAME==OSLW_TOOL_NN_D_FRAME_C




typedef enum
{
	OSlwToolNNReguType_NAN = 0,
	OSlwToolNNReguType_L1 = 1,
	OSlwToolNNReguType_L2 = 2

}OSlwToolNNReguTypeNUM;


//typedef struct OSLW_TOOL_NN_REGU_STRUCT {
//	ParaType Lambda;
//	ParaType WeSum;
//
//	OSlwToolNNReguTypeNUM ReguType : 8;
//	lw_u16 ReguSumFlag : 8;
//
//}OSlwToolNNReguSTU;

typedef ParaType(*LossFunTYPE)(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *ref, OSlwToolMatrixSTU *pre);



/*
typedef enum
{
	OSlwToolNNTrainUpdateMethod_Auto = 0,//�Զ����� ��������==�����ܳ���ʱ �Զ�ִ�и�ϰ����
	OSlwToolNNTrainUpdateMethod_Manu = 1//�ֶ����� ��������==�����ܳ���ʱ ���ڴ洢 �ȴ��ֶ�����
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

	OSlwToolTableSTU DataTable;//�����ʽ��Ϊ�˱�������ݶ�
	LossFunTYPE LossFun;
	//ѧϰ����
	ParaType nl;

	//L2����
	ParaType L2Regu;

	//�ݶ��޷�
	ParaType DeltLimitMax;
	ParaType DeltLimitMin;

	ParaType Beta1, Beta1T;
	ParaType Beta2, Beta2T;
	ParaType Epsi;

	lw_u32 AllBatchCount;
	lw_u32 AllBatchMax;

	lw_u16 _batch_stream_count;

	lw_u16 mini_batch_now;
	lw_u16 mini_batch_max;

	lw_u16 _MemAllocCoff;

	lw_u16 y_offset;

	struct
	{
		OSlwToolNNNeedTrainNum NeedTrain : 2;
		lw_u16 EnableDeltLimit : 2;
		//OSlwToolNNTrainUpdateMethodNum UpdateMethod : 2;
		OSlwToolNNTrainStatusNum Status : 4;
		OSlwToolNNMemoryMethodNum MemoryMethod : 4;
		OSlwToolNNOptimNum Optim : 4;
	}Flag;

}OSlwToolNNTrainSTU;



typedef struct OSLW_TOOL_BP_NN_STRUCT {
	OSlwToolDListSTU Net;

	OSlwToolNNTrainSTU Train;
	OSlwMat x, y, ref;

	ParaType Error;
	ParaType _nl_factor;

	OSlwMemoryBasicSTU *pmem;

	lw_u16 ParaGroupNum;
}OSlwToolBPnnSTU;


#define NN_LAYER_S_BAK_ARR_LEN 6


typedef struct OSLW_TOOL_NN_LAYER_SIMPLE_BACK_UP_STRUCT {

	OSlwToolNNSubLayerKind NN_Kind;
	lw_u32 info[NN_LAYER_S_BAK_ARR_LEN];

	OSlwToolNNLayerActFunSTU *pTemplet;

	ParaType *pDataAddr;

}OSlwToolNNLayerSimpleBakSTU;



void OSlwToolBPnnInit(OSlwToolBPnnSTU *pBPnn, lw_u16 max_mini_batch);

void OSlwToolBPnnLayerAppend
(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolDListNodeSTU *pnode,
	lw_u32 len,
	void *LayerList
);

#define _NN_GET_IN(PBPNN,PIN) do{if ((PIN) == NULL)\
{\
	if ((PBPNN)->Net.NowLen == 1)\
	{\
		(PIN) = pmem->Malloc(pmem, PARA_MEM_CAL(in_col*(PBPNN)->Train.mini_batch_max));\
		OSlwMatInit(&((PBPNN)->x), (PBPNN)->Train.mini_batch_max, in_col, (PIN));\
	}\
	else\
	{\
		pptail = (PBPNN)->Net.pTail->Data.pData;\
		(PIN) = pptail[(PBPNN)->Net.pTail->Data.uData - 1]->out.a;\
	}\
}\
else\
{\
	if ((PBPNN)->Net.NowLen == 1)OSlwMatInit(&((PBPNN)->x), (PBPNN)->Train.mini_batch_max, in_col, (PIN));\
}\
}while (0)


#define _NN_FULL_CON_CHIP_ALLAC_1(P_BPNN,P_FC) do{\
if ((P_BPNN)->Train.Flag.MemoryMethod == OSlwToolNNMemoryMethod_Chip)\
{\
	(P_FC)->Weight.a = pWe == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL((P_FC)->Weight.length)) : pWe;\
	(P_FC)->Bias.a = pBi == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL((P_FC)->Bias.length)) : pBi;\
	if ((P_BPNN)->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)\
	{\
		(P_FC->basic).NNmalloc((void *)P_FC, NULL, pmem->Malloc(pmem, (P_FC->basic).sizeofdata * (lw_u32)((P_BPNN)->Train._MemAllocCoff)));\
	}\
}\
}while(0)

#define _NN_FULL_CON_CHIP_ALLAC() do{\
memcpy(&(pBPnn->y), &(pfc->basic.out), sizeof(OSlwMat));\
pfc->pfun = pfun;pfc->pr = pr;pfc->initd1 = d1;pfc->initd2 = d2;\
if (pBPnn->Train.Flag.MemoryMethod == OSlwToolNNMemoryMethod_Chip)\
{\
	pfc->Weight.a = pWe == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Weight.length)) : pWe;\
	pfc->Bias.a = pBi == NULL ? pmem->Malloc(pmem, PARA_MEM_CAL(pfc->Bias.length)) : pBi;\
	if (pBPnn->Train.Flag.NeedTrain == OSlwToolNNNeedTrain_Need)\
	{\
		pnode1->NNmalloc(pnode1, NULL, pmem->Malloc(pmem, pnode1->sizeofdata * (lw_u32)(pBPnn->Train._MemAllocCoff)));\
	}\
}\
}while(0)


void* OSlwToolBPnnFullConAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPnnConvAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 kern_x, lw_u16 kern_y, lw_u16 kern_num,
	OSlwToolMatrixConvMethodNUM conv_method,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);

void* OSlwToolBPnnPoolAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u16 in_x, lw_u16 in_y, lw_u16 in_z,
	lw_u16 pool_x, lw_u16 pool_y,
	ParaType *pin, ParaType *pout,
	OSlwToolNNPoolingMethodNUM pool_method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);

void* OSlwToolBPnnBasicRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPnnGruRnnAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col, LwMatColType out_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	OSlwNNinitFunType pfun,
	OSlwToolRandomBasicSTU *pr,
	ParaType d1, ParaType d2,
	OSlwToolNNLayerRnnCellNewFunType new_cell_fun,
	OSlwToolNNLayerActFunSTU *pTemplet,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPnnLNormAppend
(
	OSlwToolBPnnSTU *pBPnn,
	LwMatColType in_col,
	ParaType *pin, ParaType *pout,
	ParaType *pWe, ParaType *pBi,
	ParaType *pmean, ParaType *pvar,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPnnPadAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u8 Dim, // IO��ά��
	LwMatColType *ShapeList,//���������״
	LwMatColType *PadList,//Pad�ķ��� 1ά����2ά
	ParaType *pin, ParaType *pout,
	OSlwToolNNPadNUM method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);

void* OSlwToolBPnnExtendAppend
(
	OSlwToolBPnnSTU *pBPnn,
	lw_u8 Dim, // IO��ά��
	LwMatColType *ShapeList,//���������״
	LwMatColType *ExtList,//Ext�ķ���
	ParaType *pin, ParaType *pout,
	OSlwToolNNExtendNUM method,
	OSlwMemoryBasicSTU *pmem,
	lw_u32 info[4]
);


void OSlwToolBPnnTrainInit(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *pRef,
	OSlwMemoryBasicSTU *pmem,
	LossFunTYPE loss,
	ParaType nl
);

void OSlwToolBPnnOptimInit(
	OSlwToolBPnnSTU *pBPnn,
	OSlwToolNNOptimNum optim,
	ParaType beta1,
	ParaType beta2
);

void* OSlwToolNNLayerSimpleRecover
(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *in, ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	OSlwMemoryBasicSTU *pmem
);

void* OSlwToolBPnnSimpleRecover
(
	OSlwToolBPnnSTU *pBPnn,
	ParaType *in, ParaType *out,
	OSlwToolNNLayerSimpleBakSTU *pSimpleBak,
	lw_u16 simple_layer_len,
	OSlwMemoryBasicSTU *pmem
);



void OSlwToolBPnnAllDataInit(OSlwToolBPnnSTU *pBPnn, OSlwMemoryBasicSTU *pMem);
void OSlwToolBPnnForward(OSlwToolBPnnSTU *pBPnn);
void OSlwToolBPnnBackward(OSlwToolBPnnSTU *pBPnn);
void OSlwToolBPnnLoadX(OSlwToolBPnnSTU *pBPnn, OSlwMat *xs);
void OSlwToolBPnnLoadY(OSlwToolBPnnSTU *pBPnn, OSlwMat *ys);
void OSlwToolBPnnRun(OSlwToolBPnnSTU *pBPnn, OSlwMat *xs);
void OSlwToolBPnnCalErr(OSlwToolBPnnSTU *pBPnn);
void OSlwToolBPnnTrain(OSlwToolBPnnSTU *pBPnn, OSlwMat *ys);
void OSlwToolBPnnReview(OSlwToolBPnnSTU *pBPnn);
void OSlwToolBPnnClear(OSlwToolBPnnSTU *pBPnn);
void OSlwToolBPnnCopy(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2);
void OSlwToolBPnnSoftReplace(OSlwToolBPnnSTU *pBPnn1, OSlwToolBPnnSTU *pBPnn2, ParaType Tau);
OSlwToolNNSubLayerBasicSTU *OSlwToolBPnnAt(OSlwToolBPnnSTU *pBPnn, lw_32 i, lw_32 j);

#define OSlwToolBPnnL2ReguInit(A, B) do{(A)->Train.L2Regu=B;}while(0)

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

#endif
