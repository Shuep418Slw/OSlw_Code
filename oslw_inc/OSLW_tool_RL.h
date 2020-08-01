/*(Ver.=0.97)
 * OSLW_tool.h
 *
 *  Created on: 2017-11-27
 *      Author: slw
 */

#ifndef OSLW_TOOL_RL_H_
#define OSLW_TOOL_RL_H_

#include "OSLW_define.h"
 //#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_NN.h"
#include "OSLW_memory.h"
#if !(OSLW_SIMPLE_LEVEL >= 2)

/*(Ver.=0.97)
���ݽṹ:
����	״̬ 3*2��һ��3��״̬ ÿһ��״̬2����������
		���� 2*4 ��һ��4�ֶ��� ÿһ������2�ֲ�����---����C�����ǰ����������еķ�ʽ�洢 ���Զ�Ӧ����ת�� �ȶ�������ʵ����4*2
		!!!!!!!!!!!!!!�д����м�������
		qtable 3*4
			/									\T
			|	|a0.0|	|a1.0|	|a2.0|	|a3.0|	|
			|	|a0.1|	|a1.1|	|a2.1|	|a3.1|	|
			\									/
			  ********************************
| s0.0 s0.0 | *	|q0 0|	|q0 1|	|q0 2|	|q0 3|
| s1.0 s1.0 | *	|q1 0|	|q1 1|	|q1 2|	|q1 3|
| s2.0 s2.0 | *	|q2 0|	|q2 1|	|q2 2|	|q2 3|

*/
typedef struct OSLW_TOOL_Q_RL_DATA_STATIC_BASIC_STRUCT {
	OSlwToolMatrixSTU StateTable;//״̬
	OSlwToolMatrixSTU ActionTable;//��Ϊ
	OSlwToolMatrixSTU Qtable;//qtable

}OSlwToolQRLDataSTU;

typedef struct OSLW_TOOL_RL_BASIC_STRUCT {
	OSlwMemoryBasicSTU *pmem;
	OSlwToolRandomBasicSTU *pRand;

	OSlwToolMatrixSTU ActNow;//��ǰ(δ��)��Ϊ
	OSlwToolMatrixSTU StateNow;
	OSlwToolMatrixSTU StateTermial;//״̬�յ�
	OSlwToolMatrixSTU StateLast;

	ParaType RewardNow;
	ParaType Gamma;


	ParaType RewardSum;//�ر��ܺ�
	ParaType RewardSumMax, RewardSumMin;

	ParaType StatusTermialJudgeThreshold;//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����

	lw_u32 RoundCount;//�غϼ�����
	lw_u32 RoundMax;


	void(*ActEnvFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);
	void(*BornFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);
	void(*BornFunByUser)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);

	void * (*ChooseFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);
	void * (*LearnFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);
	void * (*StateStoreFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pQRLB, OSlwToolMatrixSTU *state);
	lw_8(*isTermialFun)(struct OSLW_TOOL_RL_BASIC_STRUCT *pRLB);
	union
	{
		struct
		{
			lw_u8 IsTermialFlag : 4;
			lw_u8 StatusNear : 1;
			lw_u8 RoundOver : 1;
			lw_u8 RewardUpOver : 1;
			lw_u8 RewardDownOver : 1;
		}bits;

		lw_u8 all;

	}TermialJudgeFlag;

}OSlwToolRLBasicSTU;

/*
	SN
	RO round over
	RUO reward up over
	RDO reward down over
*/


#define OSlwToolRLT_SN (1<<4)
#define OSlwToolRLT_RO (1<<5)
#define OSlwToolRLT_RUO (1<<6)
#define OSlwToolRLT_RDO (1<<7)




void OSlwToolRL_Termial(
	void *pRL,
	lw_u8 TermialJudgeFlag,
	ParaType RewardSumMax,
	ParaType RewardSumMin,
	ParaType *pStatusTermial,
	ParaType Threshold,//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����)
	lw_u32 RoundMax
);

lw_8 OSlwToolRL_isTermial(void *pRL);

#define OSLW_RL_UPDATE(RL) 	do{((OSlwToolRLBasicSTU *)(RL))->RewardSum = _ParaAdd(((OSlwToolRLBasicSTU *)(RL))->RewardSum, ((OSlwToolRLBasicSTU *)(RL))->RewardNow);\
	((OSlwToolRLBasicSTU *)(RL))->RoundCount++;}while(0)

typedef struct OSLW_TOOL_Q_RL_BASIC_STRUCT {

	OSlwToolRLBasicSTU basic;
	OSlwToolQRLDataSTU DataTable;
	ParaType Ep;

	lw_u16 ActNowIndex;//��ǰ��Ϊ����
	lw_u16 ActLastIndex;
	lw_u16 StateNowIndex;
	lw_u16 StateTermialIndex;
	lw_u16 StateLastIndex;
	lw_u16 *pActChoose;//��ѡ���б�(������ͬ����ʱʹ��)


}OSlwToolQRLBasicSTU;

typedef void(*pActEnvFunType)(struct OSLW_TOOL_Q_RL_BASIC_STRUCT *pQRLB);
typedef void(*pBornFunType)(struct OSLW_TOOL_Q_RL_BASIC_STRUCT *pQRLB);



/*(Ver.=0.97)
//Q-learing��˳�����Ⱦ���,�����л���,��ѧϰ��������off-policy
//
//Slast------------>Snow-------->.........
//		Alast(Anow)		|
//						|
//						|
//						Learning(Slast,Anow,Snow)
//
*/



typedef struct OSLW_TOOL_Q_LEARNING_STRUCT {

	OSlwToolQRLBasicSTU basic;

	ParaType Q_Pre;
	ParaType Q_Now;
	ParaType Lr;


}OSlwToolQLearningSTU;


/*(Ver.=0.97)
//sarsa ��ֱ��ѧϰaction ������on-policy
//
//Slast------------>Snow-------->.........
//		Alast			  Anow
//							|
//							|
//							learing(Slast,Alast,Snow,Anow)
//
//
//lamber�������Ǽ�¼�켣,���������ô���Լ���
// Trace: a<1
//		action	1	2
//	state
//	1			0	1*a^4	4��֮ǰ
//	2			0	1*a^3	3��֮ǰ
//	3			0	1*a^2	2��֮ǰ
//	4			0	1*a^1	1��֮ǰ
//
//	Q����+=Trace����
//
*/

typedef struct OSLW_TOOL_SARSA_LAMBER_STRUCT {

	OSlwToolQRLBasicSTU basic;


	ParaType Q_Pre;
	ParaType Q_Now;
	ParaType Ep;
	ParaType Lr;

	ParaType Lamber;
	OSlwToolMatrixSTU Trace;



}OSlwToolSarsaLamberSTU;




void OSlwToolQLearningParaInitial
(
	OSlwToolQLearningSTU *pTQL,//thisָ��
	ParaType Ep,//��ֵ
	ParaType Lr,//ѧϰ����
	ParaType Gamma,//˥������
	OSlwToolRandomBasicSTU *prand,//�����������
	OSlwMemoryBasicSTU *pmem,//�ڴ����ָ�루��ΪNULL��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);

/*(Ver.=0.97)

void OSlwToolQLearningStaticInitial
	(
	OSlwToolQLearningSTU *pTQL, //thisָ��
	lw_u16 row, lw_u16 col, //qtable����
	ParaType *pState, //״̬�б��ڴ�
	ParaType *pAction, //��Ϊ�б�
	ParaType *pTable, //qtable
	lw_u16 *pactchoose//��ѡ��Ϊ�б�����Ϊ�б���ͬ��С
	);
	*/

void OSlwToolQLearningTableInitial
(
	OSlwToolQLearningSTU *pTQL,//thisָ��
	lw_u16 state_row, lw_u16 state_col,//״̬�������
	lw_u16 action_row, lw_u16 action_col,//��Ϊ�������
	void *pState,//״̬�б��ڴ棨��ΪNULL��
	void *pAction,//��Ϊ�б���ΪNULL,���Ƽ���
	void *pTable,//qtable����ΪNULL��
	void *pStateNow,//״̬�б��ڴ棨��ΪNULL,���Ƽ���
	void *pStateTermial,//״̬�յ�,��Ϸ�յ�(��ΪNULL�����Ƽ�)
	lw_u16 *pactchoose//״̬�б��ڴ棨��ΪNULL��
);


OSlwToolQLearningSTU* OSlwToolQLearningChooseFun(OSlwToolQLearningSTU *pQRLB);
OSlwToolQLearningSTU* OSlwToolQLearningLearnStateStore(OSlwToolQLearningSTU *pQRL, OSlwToolMatrixSTU *state);
OSlwToolQLearningSTU* OSlwToolQLearningLearnFun(OSlwToolQLearningSTU *pQRL);




void OSlwToolSarsaLamberParaInitial
(
	//���󲿷���Qlearning��ͬ
	OSlwToolSarsaLamberSTU *pTQL,
	ParaType Ep,
	ParaType Lr,
	ParaType Gamma,
	OSlwToolRandomBasicSTU *prand,
	OSlwMemoryBasicSTU *pmem,
	ParaType lamber,//�������� Lamber
	pActEnvFunType pActfun,
	pBornFunType pBornfun
);

void OSlwToolSarsaLamberTableInitial
(
	//���󲿷���Qlearning��ͬ
	OSlwToolSarsaLamberSTU *pTQL,
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pState,
	void *pAction,
	void *pTable,
	void *pStateNow,
	void *pStateTermial,
	lw_u16 *pactchoose,
	void *pTrace//�������� �켣����ΪNULL��
);


OSlwToolSarsaLamberSTU* OSlwToolSarsaLamberLearnFun(OSlwToolSarsaLamberSTU *pQRL);


//------------------------------------------------------
//<����ֵ�ط�>
//------------------------------------------------------
typedef struct OSLW_TOOL_DQN_EXP_REPLAY_FRAME_STRUCT
{
	OSlwToolDListNodeConnectSTU con, *pcon;
	ParaType Importance;
	ParaType _new_Importance;

}OSlwToolDQNetExpReplayFrameSTU;


typedef enum {
	OSlwToolDQNetExpReplay_NoP = 0,
	OSlwToolDQNetExpReplay_PER
}OSlwToolDQNetExpReplayTypeNUM;


typedef struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT
{
	OSlwToolDQNetExpReplayTypeNUM PriorType;

	OSlwToolTableSTU MemTable;
	OSlwExternMemorySTU DivResult;
	OSlwExternMemorySTU SampleResult;

	ParaType Rmin, Rmax;
	ParaType Sum;
	ParaType Div;


	ParaType Alpha, Beta, Epsi;

	lw_u16 State_Col;
	lw_u16 Action_Col;

	OSlwToolQRLBasicSTU *pTQL;
	ParaType Min, Max;

	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*AppendFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe, lw_u16 Num, lw_u8 method);
	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*UpdateFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe, lw_u16 num);
	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*MinMaxFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);
	lw_u16(*SampleFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);

}OSlwToolDQNetExpReplaySTU;

typedef OSlwToolDQNetExpReplaySTU OSlwToolExpReplaySTU;


#define OSLW_TOOL_EXPRE_CAL_P(ERR,EXPRE) _ParaPow(_ParaAdd((ERR), (EXPRE).Epsi), (EXPRE).Alpha)

OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayAppend(OSlwToolDQNetExpReplaySTU *pExpRe, lw_u16 Num, lw_u8 method);
OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayInsert(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF);
OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayDelete(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF);
OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayUpdate(OSlwToolDQNetExpReplaySTU *pExpRe, lw_u16 num);
OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayMinMax(OSlwToolDQNetExpReplaySTU *pExpRe);
lw_u16 OSlwToolDQNetExpReplaySample(OSlwToolDQNetExpReplaySTU *pExpRe);

#define OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU) (sizeof(ParaType) *(EXP_STU).State_Col)
#define OSLW_TOOL_DQN_EXP_REWARD_SIZE(EXP_STU) (sizeof(ParaType))
#define OSLW_TOOL_DQN_EXP_ACTION_SIZE(EXP_STU) (sizeof(ParaType)*(EXP_STU).Action_Col)

#define OSLW_TOOL_DQN_EXP_FRAME_SIZE(EXP_STU) (\
	sizeof(OSlwToolDQNetExpReplayFrameSTU) +/*(Ver.=0.97)�ڵ㳤��*/\
	(OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU)<<1) + \
	OSLW_TOOL_DQN_EXP_REWARD_SIZE(EXP_STU) + \
	OSLW_TOOL_DQN_EXP_ACTION_SIZE(EXP_STU) \
)

#define OSLW_TOOL_DQN_EXP_STATE_LAST(EXP_STU,PBASE) ((size_t)(PBASE)+sizeof(OSlwToolDQNetExpReplayFrameSTU))

#define OSLW_TOOL_DQN_EXP_ACT(EXP_STU,PBASE) ((size_t)(PBASE)+sizeof(OSlwToolDQNetExpReplayFrameSTU)+OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU))

#define OSLW_TOOL_DQN_EXP_R(EXP_STU,PBASE) ((size_t)(PBASE)+sizeof(OSlwToolDQNetExpReplayFrameSTU)+\
	OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU)+\
	OSLW_TOOL_DQN_EXP_ACTION_SIZE(EXP_STU)\
)

#define OSLW_TOOL_DQN_EXP_STATE_NOW(EXP_STU,PBASE) ((size_t)(PBASE)+sizeof(OSlwToolDQNetExpReplayFrameSTU)\
	+OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU)\
	+OSLW_TOOL_DQN_EXP_REWARD_SIZE(EXP_STU)\
	+OSLW_TOOL_DQN_EXP_ACTION_SIZE(EXP_STU)\
)
//------------------------------------------------------
//</����ֵ�ط�>
//------------------------------------------------------



//------------------------------------------------------
//<DQN>
//------------------------------------------------------

typedef enum {
	OSlwToolDQNetOptim_NormalDQN = 0,
	OSlwToolDQNetOptim_DoubleDQN = 1
}OSlwToolDQNetOptimNum;

typedef struct OSLW_TOOL_DEEP_Q_NET_STRUCT
{
	OSlwToolQRLBasicSTU basic;//����
	OSlwToolBPnnSTU MainNet;//Q��ʵ
	OSlwToolBPnnSTU TargetNet;//QĿ�� �첽����

	OSlwToolDQNetExpReplaySTU ExpReplay;//experience replay
	OSlwToolMatrixSTU StateFactor;
	OSlwToolDQNetOptimNum Optim;

	lw_u16 Count;
	lw_u16 CountMax;
	lw_u16 Count_Copy;
	lw_u16 NetCopyThreshold;//


	//OSlwMemoryBasicSTU *pDyMem;//�ڴ������

	struct OSLW_TOOL_DEEP_Q_NET_STRUCT* (*StoreMemFun)(struct OSLW_TOOL_DEEP_Q_NET_STRUCT *pQRLB);
	//struct OSLW_TOOL_DEEP_Q_NET_STRUCT* (*LearnFun)(struct OSLW_TOOL_DEEP_Q_NET_STRUCT *pQRLB);

}OSlwToolDQNetSTU;




OSlwToolDQNetSTU* OSlwToolDQNetChoose(OSlwToolDQNetSTU *pDQN);
OSlwToolDQNetSTU* OSlwToolDQNetStoreMem(OSlwToolDQNetSTU *pDQN);
OSlwToolDQNetSTU* OSlwToolDQNetLearning(OSlwToolDQNetSTU *pDQN);


void OSlwToolDQNetInitial(OSlwToolDQNetSTU *pDQN);//DQN��ʼ�����������ȱ�����


void OSlwToolDQNetParaInitial//DQN�ڴ��ʼ������
(
	OSlwToolDQNetSTU *pDQN,//thisָ��
	ParaType Ep,//��ֵ
	ParaType Gamma,//˥������
	OSlwToolDQNetOptimNum Optim,
	OSlwToolRandomBasicSTU *prand,//�����������
	OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);


void OSlwToolDQNetInterfaceInitial
(
	OSlwToolDQNetSTU *pDQN,//thisָ��
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor//״̬��һ������(��ΪNULL)
);

void OSlwToolDQNetStoreMemInitial//DQN�ڴ�����ʼ��
(
	OSlwToolDQNetSTU *pDQN,//thisָ��
	OSlwToolDQNetExpReplayTypeNUM PER,//�Ƿ����PER�Ż�
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	lw_u16 copy_th,//���Ƶ���ֵ
	ParaType Rmax, ParaType Rmin,//���·�Χ�����Ż�״̬�������壩
	ParaType Alpha, ParaType Beta,//ab���������Ż�״̬�������壩
	ParaType Epsi
);



void OSlwToolDQNetAppendInitial
(
	OSlwToolDQNetSTU *pDQN,
	lw_u16 row,
	lw_u16 col,
	OSlwNNinitFunType pFun,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType initd1, ParaType initd2,
	OSlwToolNNLayerActFunSTU *pTemplet
);

void OSlwToolDQNetTarinInitial(
	OSlwToolDQNetSTU *pDQN,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
);

void OSlwToolDQNetAllDataInit(OSlwToolDQNetSTU *pDQN);
//------------------------------------------------------
//</DQN>
//------------------------------------------------------



//------------------------------------------------------
//<DDPG>
//------------------------------------------------------

typedef enum
{
	OSlwToolACtoActor = 0,
	OSlwToolACtoCritic = 1,
}OSlwToolACtoNet;


typedef enum
{
	OSlwToolDDPG_CopyMethod_Hard = 0,
	OSlwToolDDPG_CopyMethod_Soft = 1,

}OSlwToolDDPG_CopyMethod;

typedef struct OSLW_TOOL_POLICY_GRAD_STRUCT {
	OSlwToolRLBasicSTU basic;
	OSlwToolExpReplaySTU ExpReplay;

	OSlwToolMatrixSTU StateFactor;

	ParaType ActionMax, ActionMin;


}OSlwToolPGradSTU;


typedef struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT {

	OSlwToolPGradSTU PGradBasic;

	OSlwToolBPnnSTU ActorMainNet, ActorTargetNet;
	OSlwToolBPnnSTU CriticMainNet, CriticTargetNet;

	ParaType ActionVar;

	lw_u16 Count;
	lw_u16 CountMax;

	struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT* (*StoreMemFun)(struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT *pQRLB);

	union
	{
		ParaType tSoftReplace;
		struct
		{
			lw_u16 Count_Copy;
			lw_u16 NetCopyThreshold;
		}HardReplace;
	}Copy;

	OSlwToolDDPG_CopyMethod CopyMethod;


}OSlwToolDDPGradSTU;

OSlwToolDDPGradSTU* OSlwToolDDPGradChoose(OSlwToolDDPGradSTU *pDDPG);
OSlwToolDDPGradSTU* OSlwToolDDPGradStoreMem(OSlwToolDDPGradSTU *pDDPG);
OSlwToolDDPGradSTU* OSlwToolDDPGradLearning(OSlwToolDDPGradSTU *pDDPG);


void OSlwToolDDPGradInitial(OSlwToolDDPGradSTU *pDDPG);//DDPG��ʼ�����������ȱ�����


void OSlwToolDDPGradParaInitial//DDPG�ڴ��ʼ������
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	ParaType Gamma,//˥������
	OSlwToolRandomBasicSTU *prand,//�����������
	OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
);


void OSlwToolDDPGradInterfaceInitial
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	lw_u16 state_dim, lw_u16 action_dim,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
);

void OSlwToolDDPGradStoreMemInitial//DDPG�ڴ�����ʼ��
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	ParaType copy_data//���Ƶ���ֵ
);



void OSlwToolDDPGradAppendInitial
(
	OSlwToolDDPGradSTU *pDDPG,
	OSlwToolACtoNet Kind,
	lw_u16 row,
	lw_u16 col,
	OSlwNNinitFunType pFun,
	OSlwToolRandomBasicSTU *pTRB,
	ParaType initd1, ParaType initd2,
	OSlwToolNNLayerActFunSTU *pTemplet
);

void OSlwToolDDPGradTarinInitial(
	OSlwToolDDPGradSTU *pDDPG,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
);
void OSlwToolDDPGradAllDataInit(OSlwToolDDPGradSTU *pDDPG);




//------------------------------------------------------
//</DDPG>
//------------------------------------------------------

//------------------------------------------------------
//<PPO>
//------------------------------------------------------
typedef struct OSLW_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT {
	OSlwToolPGradSTU PGradBasic;
	OSlwToolBPnnSTU ActorNewNet, ActorOldNet;
	OSlwToolBPnnSTU CriticNet;

	ParaType ClipE;
	ParaType GAE_tau;
	ParaType GAE_gamma;
	ParaType beta_entropy;

	lw_u16 Count;
	lw_u16 CountEpsion;


	struct OSLW_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT* (*StoreMemFun)(struct OSLW_TOOL_PEOXIMAL_POLICY_OPTIM_STRUCT *pQRLB);

}OSlwToolPPOptimSTU;


OSlwToolPPOptimSTU* OSlwToolPPOptimChoose(OSlwToolPPOptimSTU *pPPO);
OSlwToolPPOptimSTU* OSlwToolPPOptimStoreMem(OSlwToolPPOptimSTU *pPPO);
OSlwToolPPOptimSTU* OSlwToolPPOptimLearning(OSlwToolPPOptimSTU *pPPO);


void OSlwToolPPOptimInitial(OSlwToolPPOptimSTU *pPPO);//PPO��ʼ�����������ȱ�����


void OSlwToolPPOptimParaInitial//PPO�ڴ��ʼ������
(
	OSlwToolPPOptimSTU *pPPO,//thisָ��
	ParaType ClipE,
	ParaType tau,
	ParaType gamma,
	ParaType beta,
	OSlwToolRandomBasicSTU *prand,//�����������
	OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��

);


void OSlwToolPPOptimInterfaceInitial
(
	OSlwToolPPOptimSTU *pPPO,//thisָ��
	lw_u16 state_dim, lw_u16 action_dim,
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin
);

void OSlwToolPPOptimStoreMemInitial//PPO�ڴ�����ʼ��
(
	OSlwToolPPOptimSTU *pPPO,//thisָ��
	lw_u16 MemPoolLength//���䳤��
);


void OSlwToolPPOptimAppendInitial//PPO������׷��,һ�λ�׷������������
(
	OSlwToolPPOptimSTU *pPPO,//thisָ��
	OSlwToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	OSlwToolNNLayerActFunSTU **AFlist,//�����
	ParaType *nl_list//ѧϰ����
);


//------------------------------------------------------
//</PPO>
//------------------------------------------------------


#endif // !(OSLW_SIMPLE_LEVEL >= 2)

#endif
