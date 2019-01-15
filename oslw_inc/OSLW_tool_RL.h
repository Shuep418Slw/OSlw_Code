/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.h
 *
 *  Created on: 2017-11-27
 *      Author: slw
 */

#ifndef OSLW_TOOL_RL_H_
#define OSLW_TOOL_RL_H_

#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#include "OSLW_tool_basic_math.h"
#include "OSLW_tool_random.h"
#include "OSLW_tool_NN.h"
#include "OSLW_memory.h"
#if !(OSLW_SIMPLE_LEVEL >= 2)

/*(Ver.=0.9~)(Beg.=0.9)
数据结构:
例如	状态 3*2（一共3种状态 每一种状态2个特征量）
		动作 2*4 （一共4种动作 每一个动作2种操作）---由于C语言是按照先列再行的方式存储 所以对应的是转置 既动作矩阵实际是4*2 
		!!!!!!!!!!!!!!行代表有几个动作
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
typedef struct OSLW_TOOL_Q_RL_DATA_STATIC_BASIC_STRUCT{
	OSlwToolMatrixSTU StateTable;//状态
	OSlwToolMatrixSTU ActionTable;//行为
	OSlwToolMatrixSTU Qtable;//qtable
	
}OSlwToolQRLDataSTU;
	
typedef struct OSLW_TOOL_RL_BASIC_STRUCT {
	OSlwMemoryBasicSTU *pmem;
	OSlwToolRandomBasicSTU *pRand;

	OSlwToolMatrixSTU ActNow;//当前(未来)行为
	OSlwToolMatrixSTU StateNow;
	OSlwToolMatrixSTU StateTermial;//状态终点
	OSlwToolMatrixSTU StateLast;
	
	ParaType RewardNow;
	ParaType Gamma;


	ParaType RewardSum;//回报总和
	ParaType RewardSumMax, RewardSumMin;
	
	ParaType StatusTermialJudgeThreshold;//状态终点阈值 会计算当前状态与状态终点的欧式距离

	lw_u32 RoundCount;//回合计数器
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
	ParaType Threshold,//状态终点阈值 会计算当前状态与状态终点的欧式距离)
	lw_u32 RoundMax
);

lw_8 OSlwToolRL_isTermial(void *pRL);

#define OSLW_RL_UPDATE(RL) 	do{((OSlwToolRLBasicSTU *)(RL))->RewardSum = _ParaAdd(((OSlwToolRLBasicSTU *)(RL))->RewardSum, ((OSlwToolRLBasicSTU *)(RL))->RewardNow);\
	((OSlwToolRLBasicSTU *)(RL))->RoundCount++;}while(0)

typedef struct OSLW_TOOL_Q_RL_BASIC_STRUCT{

	OSlwToolRLBasicSTU basic;
	OSlwToolQRLDataSTU DataTable;
	ParaType Ep;

	lw_u16 ActNowIndex;//当前行为索引
	lw_u16 ActLastIndex;
	lw_u16 StateNowIndex;
	lw_u16 StateTermialIndex;
	lw_u16 StateLastIndex;
	lw_u16 *pActChoose;//待选择列表(当有相同数据时使用)


}OSlwToolQRLBasicSTU;

typedef void (*pActEnvFunType)(struct OSLW_TOOL_Q_RL_BASIC_STRUCT *pQRLB);
typedef void (*pBornFunType)(struct OSLW_TOOL_Q_RL_BASIC_STRUCT *pQRLB);



/*(Ver.=0.9~)(Beg.=0.9)
//Q-learing的顺序是先决策,再运行环境,再学习，所以是off-policy
//
//Slast------------>Snow-------->.........
//		Alast(Anow)		|
//						|
//						|
//						Learning(Slast,Anow,Snow)
//
*/



typedef struct OSLW_TOOL_Q_LEARNING_STRUCT{

	OSlwToolQRLBasicSTU basic;

	ParaType Q_Pre;
	ParaType Q_Now;
	ParaType Lr;

		
}OSlwToolQLearningSTU;


/*(Ver.=0.9~)(Beg.=0.9)
//sarsa 是直接学习action 所以是on-policy
//
//Slast------------>Snow-------->.........
//		Alast			  Anow		
//							|
//							|
//							learing(Slast,Alast,Snow,Anow)
//
//
//lamber的作用是记录轨迹,如果收敛那么可以加速
// Trace: a<1
//		action	1	2	
//	state
//	1			0	1*a^4	4步之前
//	2			0	1*a^3	3步之前
//	3			0	1*a^2	2步之前
//	4			0	1*a^1	1步之前
//
//	Q矩阵+=Trace矩阵
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
	OSlwToolQLearningSTU *pTQL,//this指针
	ParaType Ep,//阈值
	ParaType Lr,//学习速率
	ParaType Gamma,//衰减因子
	OSlwToolRandomBasicSTU *prand,//随机数产生器
	OSlwMemoryBasicSTU *pmem,//内存分配指针（可为NULL）
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);

/*(Ver.=0.9~)(Beg.=0.9)

void OSlwToolQLearningStaticInitial
	(
	OSlwToolQLearningSTU *pTQL, //this指针
	lw_u16 row, lw_u16 col, //qtable行列
	ParaType *pState, //状态列表内存
	ParaType *pAction, //行为列表
	ParaType *pTable, //qtable
	lw_u16 *pactchoose//备选行为列表与行为列表相同大小
	);
	*/

void OSlwToolQLearningTableInitial
(
	OSlwToolQLearningSTU *pTQL,//this指针
	lw_u16 state_row, lw_u16 state_col,//状态表的行列
	lw_u16 action_row, lw_u16 action_col,//行为表的行列
	void *pState,//状态列表内存（可为NULL）
	void *pAction,//行为列表（可为NULL,不推荐）
	void *pTable,//qtable（可为NULL）
	void *pStateNow,//状态列表内存（可为NULL,不推荐）
	void *pStateTermial,//状态终点,游戏终点(可为NULL，不推荐)
	lw_u16 *pactchoose//状态列表内存（可为NULL）
);


OSlwToolQLearningSTU* OSlwToolQLearningChooseFun(OSlwToolQLearningSTU *pQRLB);
OSlwToolQLearningSTU* OSlwToolQLearningLearnStateStore(OSlwToolQLearningSTU *pQRL, OSlwToolMatrixSTU *state);
OSlwToolQLearningSTU* OSlwToolQLearningLearnFun(OSlwToolQLearningSTU *pQRL);




void OSlwToolSarsaLamberParaInitial
(
	//绝大部分与Qlearning相同
	OSlwToolSarsaLamberSTU *pTQL,
	ParaType Ep,
	ParaType Lr,
	ParaType Gamma,
	OSlwToolRandomBasicSTU *prand,
	OSlwMemoryBasicSTU *pmem,
	ParaType lamber,//新增参数 Lamber
	pActEnvFunType pActfun,
	pBornFunType pBornfun
);

void OSlwToolSarsaLamberTableInitial
(
	//绝大部分与Qlearning相同
	OSlwToolSarsaLamberSTU *pTQL,
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pState,
	void *pAction,
	void *pTable,
	void *pStateNow,
	void *pStateTermial,
	lw_u16 *pactchoose,
	void *pTrace//新增参数 轨迹（可为NULL）
);


OSlwToolSarsaLamberSTU* OSlwToolSarsaLamberLearnFun(OSlwToolSarsaLamberSTU *pQRL);


//------------------------------------------------------
//<经验值回放>
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

	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*AppendFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe,lw_u16 Num, lw_u8 method);
	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*UpdateFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe,lw_u16 num);
	struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*MinMaxFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);
	lw_u16 (*SampleFun)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *pExpRe);

}OSlwToolDQNetExpReplaySTU;

typedef OSlwToolDQNetExpReplaySTU OSlwToolExpReplaySTU;


#define OSLW_TOOL_EXPRE_CAL_P(ERR,EXPRE) _ParaPow(_ParaAdd((ERR), (EXPRE).Epsi), (EXPRE).Alpha)

OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayAppend(OSlwToolDQNetExpReplaySTU *pExpRe,lw_u16 Num, lw_u8 method);
OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayInsert(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF);
OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayDelete(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF);
OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayUpdate(OSlwToolDQNetExpReplaySTU *pExpRe,lw_u16 num);
OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayMinMax(OSlwToolDQNetExpReplaySTU *pExpRe);
lw_u16 OSlwToolDQNetExpReplaySample(OSlwToolDQNetExpReplaySTU *pExpRe);

#define OSLW_TOOL_DQN_EXP_STATE_SIZE(EXP_STU) (sizeof(ParaType) *(EXP_STU).State_Col)
#define OSLW_TOOL_DQN_EXP_REWARD_SIZE(EXP_STU) (sizeof(ParaType))
#define OSLW_TOOL_DQN_EXP_ACTION_SIZE(EXP_STU) (sizeof(ParaType)*(EXP_STU).Action_Col)

#define OSLW_TOOL_DQN_EXP_FRAME_SIZE(EXP_STU) (\
	sizeof(OSlwToolDQNetExpReplayFrameSTU) +/*(Ver.=0.9~)(Beg.=0.9)节点长度*/\
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
//</经验值回放>
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
	OSlwToolQRLBasicSTU basic;//基类
	OSlwToolBPNNSTU MainNet;//Q现实
	OSlwToolBPNNSTU TargetNet;//Q目标 异步更新

	OSlwToolDQNetExpReplaySTU ExpReplay;//experience replay
	OSlwToolMatrixSTU StateFactor; 
	OSlwToolDQNetOptimNum Optim;

	lw_u16 Count;
	lw_u16 CountMax;
	lw_u16 Count_Copy;
	lw_u16 NetCopyThreshold;//


	//OSlwMemoryBasicSTU *pDyMem;//内存分配类

	struct OSLW_TOOL_DEEP_Q_NET_STRUCT* (*StoreMemFun)(struct OSLW_TOOL_DEEP_Q_NET_STRUCT *pQRLB);
	//struct OSLW_TOOL_DEEP_Q_NET_STRUCT* (*LearnFun)(struct OSLW_TOOL_DEEP_Q_NET_STRUCT *pQRLB);

}OSlwToolDQNetSTU; 




OSlwToolDQNetSTU* OSlwToolDQNetChoose(OSlwToolDQNetSTU *pDQN);
OSlwToolDQNetSTU* OSlwToolDQNetStoreMem(OSlwToolDQNetSTU *pDQN);
OSlwToolDQNetSTU* OSlwToolDQNetLearning(OSlwToolDQNetSTU *pDQN);


void OSlwToolDQNetInitial(OSlwToolDQNetSTU *pDQN);//DQN初始化函数，最先被调用


void OSlwToolDQNetParaInitial//DQN内存初始化函数
(
	OSlwToolDQNetSTU *pDQN,//this指针
	ParaType Ep,//阈值
	ParaType Gamma,//衰减因子
	OSlwToolDQNetOptimNum Optim,
	OSlwToolRandomBasicSTU *prand,//随机数产生器
	OSlwMemoryBasicSTU *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);


void OSlwToolDQNetInterfaceInitial
(
	OSlwToolDQNetSTU *pDQN,//this指针
	lw_u16 state_row, lw_u16 state_col,
	lw_u16 action_row, lw_u16 action_col,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor//状态归一化因子(可为NULL)
);

void OSlwToolDQNetStoreMemInitial//DQN内存分配初始化
(
	OSlwToolDQNetSTU *pDQN,//this指针
	OSlwToolDQNetExpReplayTypeNUM PER,//是否继续PER优化
	lw_u16 MemPoolLength,//记忆长度
	lw_u16 batch_size,//抽取的记忆长度
	lw_u16 copy_th,//复制的阈值
	ParaType Rmax, ParaType Rmin,//大致范围（不优化状态下无意义）
	ParaType Alpha, ParaType Beta,//ab参数（不优化状态下无意义）
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


typedef struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT{

	OSlwToolPGradSTU PGradBasic;

	OSlwToolBPNNSTU ActorMainNet, ActorTargetNet;
	OSlwToolBPNNSTU CriticMainNet, CriticTargetNet;

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


void OSlwToolDDPGradInitial(OSlwToolDDPGradSTU *pDDPG);//DDPG初始化函数，最先被调用


void OSlwToolDDPGradParaInitial//DDPG内存初始化函数
(
	OSlwToolDDPGradSTU *pDDPG,//this指针
	ParaType Gamma,//衰减因子
	OSlwToolRandomBasicSTU *prand,//随机数产生器
	OSlwMemoryBasicSTU *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
);


void OSlwToolDDPGradInterfaceInitial
(
	OSlwToolDDPGradSTU *pDDPG,//this指针
	lw_u16 state_dim,lw_u16 action_dim,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor,//状态归一化因子(可为NULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
);

void OSlwToolDDPGradStoreMemInitial//DDPG内存分配初始化
(
	OSlwToolDDPGradSTU *pDDPG,//this指针
	lw_u16 MemPoolLength,//记忆长度
	lw_u16 batch_size,//抽取的记忆长度
	ParaType copy_data//复制的阈值
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
	OSlwToolBPNNSTU ActorNewNet, ActorOldNet;
	OSlwToolBPNNSTU CriticNet;

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


void OSlwToolPPOptimInitial(OSlwToolPPOptimSTU *pPPO);//PPO初始化函数，最先被调用


void OSlwToolPPOptimParaInitial//PPO内存初始化函数
(
	OSlwToolPPOptimSTU *pPPO,//this指针
	ParaType ClipE,
	ParaType tau,
	ParaType gamma,
	ParaType beta,
	OSlwToolRandomBasicSTU *prand,//随机数产生器
	OSlwMemoryBasicSTU *pMem,//内存指针
	pActEnvFunType pActfun,//环境运行函数指针
	pBornFunType pBornfun//环境初始化函数指针
	
);


void OSlwToolPPOptimInterfaceInitial
(
	OSlwToolPPOptimSTU *pPPO,//this指针
	lw_u16 state_dim, lw_u16 action_dim,
	void *pAction,//动作列表内存（可为NULL）
	void *pStateNow,//当前状态（可为NULL）
	void *pStateLast,//之前状态（可为NULL）
	void *pStateTermial,//终结状态（可为NULL）
	void *pStateFactor,//状态归一化因子(可为NULL)
	ParaType ActionMax, ParaType ActionMin
);

void OSlwToolPPOptimStoreMemInitial//PPO内存分配初始化
(
	OSlwToolPPOptimSTU *pPPO,//this指针
	lw_u16 MemPoolLength//记忆长度
);


void OSlwToolPPOptimAppendInitial//PPO神经网络追加,一次会追加两个神经网络
(
	OSlwToolPPOptimSTU *pPPO,//this指针
	OSlwToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	OSlwToolNNLayerActFunSTU **AFlist,//激活函数
	ParaType *nl_list//学习速率
);


//------------------------------------------------------
//</PPO>
//------------------------------------------------------


#endif // !(OSLW_SIMPLE_LEVEL >= 2)

#endif
