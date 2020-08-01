/*(Ver.=0.97)
 * OSLW_tool.c
 *
 *  Created on: 2017-11-27
 *      Author: slw
 */

#include "OSLW_include.h"
#if !(OSLW_SIMPLE_LEVEL >= 2)

#if OSLW_TOOL_IMPORT_RL || OSLW_TOOL_IMPORT_ALL
void OSlwToolRL_Termial(
	void *pRL,
	lw_u8 TermialJudgeFlag,
	ParaType RewardSumMax,
	ParaType RewardSumMin,
	ParaType *pStatusTermial,
	ParaType Threshold,//״̬�յ���ֵ ����㵱ǰ״̬��״̬�յ��ŷʽ����)
	lw_u32 RoundMax
)
{
	OSlwToolRLBasicSTU *_rl;
	OSLW_assert(!(pRL));

	_rl = pRL;

	_rl->isTermialFun = (lw_8 (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_isTermial;
	_rl->TermialJudgeFlag.all = TermialJudgeFlag;
	_rl->RewardSumMax = RewardSumMax;
	_rl->RewardSumMin = RewardSumMin;
	_rl->TermialJudgeFlag.bits.IsTermialFlag = 0;

	_rl->StatusTermialJudgeThreshold = Threshold;

	_rl->RoundMax = RoundMax;
	if (pStatusTermial)
	{
		OSlwToolMatrixInitial(&(_rl->StateTermial), 1, _rl->StateNow.col, pStatusTermial);
	}

}

lw_8 OSlwToolRL_isTermial(void *pRL)
{
	OSlwToolRLBasicSTU *_rl;
	ParaType cmpres;
	OSLW_assert(!(pRL));

	_rl = pRL;
	if (_rl->TermialJudgeFlag.bits.IsTermialFlag)
	{
		return 1;
	}

	if (_rl->TermialJudgeFlag.bits.StatusNear)
	{
		cmpres = pOSlwToolMatrixLossSquare(NULL, &(_rl->StateNow), &(_rl->StateTermial));
		if (cmpres <= _rl->StatusTermialJudgeThreshold)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RoundOver)
	{
		if (_rl->RoundCount > _rl->RoundMax)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RewardUpOver)
	{
		if (_rl->RewardSum > _rl->RewardSumMax)
		{
			return 1;
		}
	}

	if (_rl->TermialJudgeFlag.bits.RewardDownOver)
	{
		if (_rl->RewardSum<_rl->RewardSumMin)
		{
			return 1;
		}
	}


	return 0;

}


static void OSlwToolRL_Born(OSlwToolRLBasicSTU *rl)
{
	rl->RewardSum = _ParaFrom(0);
	rl->RoundCount = 0;
	rl->BornFunByUser(rl);
}



#if OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_Q_LEARNING


void OSlwToolQLearningParaInitial
(
    OSlwToolQLearningSTU *pTQL,
    ParaType Ep,
    ParaType Lr,
    ParaType Gamma,
    OSlwToolRandomBasicSTU *prand,
    OSlwMemoryBasicSTU *pmem,
    pActEnvFunType pActfun,
    pBornFunType pBornfun
)
{

    OSLW_assert(!(pTQL));
    OSLW_assert(!(prand));
    OSLW_assert(!(pActfun));
    OSLW_assert(!(pBornfun));

    memset(pTQL,0,sizeof(OSlwToolQLearningSTU));
    pTQL->basic.Ep=Ep;
    pTQL->Lr=Lr;
    pTQL->basic.basic.Gamma=Gamma;
    pTQL->basic.basic.pRand=prand;
    pTQL->basic.basic.ActEnvFun=(void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pActfun;
    pTQL->basic.basic.BornFun= (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_Born;
	pTQL->basic.basic.BornFunByUser = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pBornfun;

    pTQL->basic.basic.pmem = pmem;

    pTQL->basic.basic.LearnFun = (void* (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolQLearningLearnFun;
    pTQL->basic.basic.ChooseFun = (void* (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolQLearningChooseFun;
    pTQL->basic.basic.StateStoreFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *, OSlwToolMatrixSTU *))OSlwToolQLearningLearnStateStore;
    pTQL->basic.basic.isTermialFun = (lw_8 (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_isTermial;

}


void OSlwToolQLearningTableInitial
(
    OSlwToolQLearningSTU *pTQL,
    lw_u16 state_row, lw_u16 state_col,
    lw_u16 action_row, lw_u16 action_col,
    void *pState,
    void *pAction,
    void *pTable,
    void *pStateNow,
    void *pStateTermial,
    lw_u16 *pactchoose
)

{
    OSLW_assert(!(pTQL));

    if (pTQL->basic.basic.pmem)
    {
        if (!pAction)
        {
            pAction = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(action_row*action_col));
        }

        if (!pState)
        {
            pState = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*state_col));
        }

        if (!pTable)
        {
            pTable = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*action_row));
        }

        if (!pStateNow)
        {
            pStateNow = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, (state_col * 1) * sizeof(lw_u16));
        }

        if (!pactchoose)
        {
            pactchoose = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, (action_col * 1) * sizeof(lw_u16));
        }


    }
    else;

    OSlwToolMatrixInitial(&(pTQL->basic.DataTable.ActionTable), action_row, action_col,pAction);
    OSlwToolMatrixInitial(&(pTQL->basic.DataTable.StateTable), state_row, state_col,pState);
    OSlwToolMatrixInitial(&(pTQL->basic.DataTable.Qtable), state_row, action_row,pTable);
    OSlwToolMatrixInitial(&(pTQL->basic.basic.StateNow), 1, state_col, pStateNow);
    OSlwToolMatrixInitial(&(pTQL->basic.basic.StateTermial), 1, state_col, pStateTermial);


    pOSlwToolMatrixSet(&(pTQL->basic.DataTable.Qtable), _ParaFrom(0),NULL);

    pTQL->basic.pActChoose = pactchoose;
}


OSlwToolQLearningSTU* OSlwToolQLearningLearnStateStore(OSlwToolQLearningSTU *pQRL,OSlwToolMatrixSTU *state)
{

    OSlwToolQRLDataSTU *pTQRLDS;
    OSlwToolQRLBasicSTU *pQRLB;
    ParaType *ps, *pq;

    OSLW_assert(!(pQRL));

    pQRLB = &(pQRL->basic);
    pTQRLDS = &(pQRL->basic.DataTable);

    if (pQRLB->basic.pmem == NULL)//�޷��ڴ����
    {
        OSLW_assert(1);
        return NULL;
    }
    if (state==NULL)
    {
        state = &(pQRLB->basic.StateNow);
    }

    if (state->col != pTQRLDS->StateTable.col)
    {
        OSLW_assert(1);
        return NULL;
    }


    ps = pQRLB->basic.pmem->ReAlloc(pQRLB->basic.pmem, pTQRLDS->StateTable.a,
                              PARA_MEM_CAL(pTQRLDS->StateTable.length + state->col)//����ԭ���Ĵ�С+1��
                             );

    pq = pQRLB->basic.pmem->ReAlloc(pQRLB->basic.pmem, pTQRLDS->Qtable.a,
                              PARA_MEM_CAL(pTQRLDS->Qtable.length + pTQRLDS->Qtable.col)//����ԭ���Ĵ�С+1��
                             );

    memcpy(ps + pTQRLDS->StateTable.length, state->a, PARA_MEM_CAL(state->col));
    memset(pq + pTQRLDS->Qtable.length, 0, PARA_MEM_CAL(pTQRLDS->Qtable.col));

    OSlwToolMatrixInitial(&(pTQRLDS->StateTable), pTQRLDS->StateTable.row + 1, pTQRLDS->StateTable.col, ps);//���ô�С
    OSlwToolMatrixInitial(&(pTQRLDS->Qtable), pTQRLDS->Qtable.row + 1, pTQRLDS->Qtable.col, pq);//���ô�С

    return pQRL;
}



OSlwToolQLearningSTU* OSlwToolQLearningLearnFun(OSlwToolQLearningSTU *pQRL)
{

    OSlwToolQRLDataSTU *pTQRLDS;
    OSlwToolQRLBasicSTU *pQRLB;
	//OSlwToolRLBasicSTU *rl;
    ParaType *p,dmax;
    lw_u16 maxindex, row;
    OSlwToolMatrixSTU mat;

    OSLW_assert(!(pQRL));

    pQRLB=&(pQRL->basic);
    pTQRLDS= &(pQRL->basic.DataTable);

	OSLW_RL_UPDATE(pQRL);

    /*(Ver.=0.97)
    mat.length = pTQRLDS->state.length;
    mat.a = pTQRLDS->state.a;

    pOSlwToolMatrixIndex(&mat, pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);

    mat.length=pTQRLDS->action.length;
    mat.a=pTQRLDS->table.a+pTQRLDS->table.col*pQRLB->StateNowIndex;
    pOSlwToolMatrixMaxMin(&(mat),&dmax,&maxindex,NULL,NULL);
    */

    if (pOSlwToolMatrixFind(&(pTQRLDS->StateTable), &(pQRLB->basic.StateNow), 0, 0, &row) == OSlwMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
        pQRLB->basic.StateStoreFun((OSlwToolRLBasicSTU *)pQRL, NULL);
    }

    pQRLB->StateNowIndex = row;

    mat.length = pTQRLDS->Qtable.col;
    mat.a = pTQRLDS->Qtable.a + pTQRLDS->Qtable.col*row;

    pOSlwToolMatrixMaxMin(&(mat), &dmax, &maxindex, NULL, NULL);


    p = &(OSLW_TOOL_M_GET(pTQRLDS->Qtable,pQRLB->StateLastIndex,pQRLB->ActNowIndex));
    if(!(pQRLB->basic.isTermialFun((OSlwToolRLBasicSTU *)pQRLB)))//�ж��Ƿ����յ�
        pQRL->Q_Now=_ParaAdd(pQRLB->basic.RewardNow,_ParaMpy(pQRLB->basic.Gamma,dmax));
    else
        pQRL->Q_Now=pQRLB->basic.RewardNow;


    *p=_ParaAdd((*p),_ParaMpy(pQRL->Lr,_ParaSub(pQRL->Q_Now,(*p))));

    //������һ�־���ֵ
    //pQRLB->StateLast = pQRLB->StateNow;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;
    //pQRLB->ActLast = pQRLB->ActNow;
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;

    return pQRL;
}

#endif // OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_Q_LEARNING



#if OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_SARSA || OSLW_TOOL_IMPORT_RL_Q_LEARNING

OSlwToolQLearningSTU* OSlwToolQLearningChooseFun(OSlwToolQLearningSTU *pQRLB)
{

    ParaType drand, dmax;
    lw_u16 selrand, maxindex, row;
    OSlwToolQRLDataSTU *pTQRLDS;
	OSlwToolRLBasicSTU *pRL;
    OSlwToolMatrixSTU mat;

    OSLW_assert(!(pQRLB));

	pRL = &(pQRLB->basic.basic);
    drand = pRL->pRand->rand(pRL->pRand, _ParaFint(0), _ParaFint(1));
    pTQRLDS = &(pQRLB->basic.DataTable);


    //����state ��ѯstate��ID
    /*(Ver.=0.97)
    mat.col = pTQRLDS->action.col;
    mat.row = 1;
    mat.length = pTQRLDS->action.length;
    col = 1;
    pOSlwToolMatrixIndex(&(pTQRLDS->state), pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);
    mat.a = pTQRLDS->table.a + pTQRLDS->table.col*pQRLB->StateNowIndex;
    */

    if (pOSlwToolMatrixFind(&(pTQRLDS->StateTable), &(pRL->StateNow), 0, 0, &row) == OSlwMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
		pRL->StateStoreFun(pRL, NULL);
    }

    pQRLB->basic.StateNowIndex = row;

    mat.length = pTQRLDS->Qtable.col;
    mat.a = pTQRLDS->Qtable.a + pTQRLDS->Qtable.col*row;


    if (drand < pQRLB->basic.Ep)
    {

        //��ѯQ������Ϊ
        pOSlwToolMatrixMaxMin(&mat, &dmax, &maxindex, NULL, NULL);
        row = mat.length;
        //�õ�Q��������
        pOSlwToolMatrixIndex(&mat, dmax, pQRLB->basic.pActChoose, &row);
        //���ѡȡһ��Q������Ϊ
		selrand = pRL->pRand->randint(pRL->pRand, 0, row);
        pQRLB->basic.ActNowIndex = pQRLB->basic.pActChoose[selrand];


    }
    else
    {
        //ֱ�����ѡȡ��Ϊ
		selrand = pRL->pRand->randint(pRL->pRand, 0, pTQRLDS->Qtable.col);
        pQRLB->basic.ActNowIndex = selrand;

    }

    //������Ϊ��֮ǰ��״̬
    pRL->ActNow.a = pTQRLDS->ActionTable.a + pQRLB->basic.ActNowIndex*pTQRLDS->ActionTable.col;


    return pQRLB;
}
#endif // OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_SARSA || OSLW_TOOL_IMPORT_RL_Q_LEARNING


#if OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_SARSA

void OSlwToolSarsaLamberParaInitial
(
    OSlwToolSarsaLamberSTU *pTQL,
    ParaType Ep,
    ParaType Lr,
    ParaType Gamma,
    OSlwToolRandomBasicSTU *prand,
    OSlwMemoryBasicSTU *pmem,
    ParaType lamber,
    pActEnvFunType pActfun,
    pBornFunType pBornfun
)
{
    OSLW_assert(!(pTQL));
    OSLW_assert(!(prand));
    OSLW_assert(!(pActfun));

    memset(pTQL, 0, sizeof(OSlwToolQLearningSTU));
    pTQL->Ep = Ep;
    pTQL->Lr = Lr;
    pTQL->basic.basic.Gamma = Gamma;
    pTQL->basic.basic.pRand = prand;
    pTQL->basic.basic.pmem = pmem;
    pTQL->basic.basic.ActEnvFun = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pActfun;
	pTQL->basic.basic.BornFun = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_Born;
    pTQL->basic.basic.BornFunByUser = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pBornfun;
    pTQL->Lamber = lamber;

    pTQL->basic.basic.LearnFun = (void* (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolSarsaLamberLearnFun;

    pTQL->basic.basic.ChooseFun = (void* (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolQLearningChooseFun;
    pTQL->basic.basic.StateStoreFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *, OSlwToolMatrixSTU *))OSlwToolQLearningLearnStateStore;
    pTQL->basic.basic.isTermialFun = (lw_8 (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_isTermial;

}

void OSlwToolSarsaLamberTableInitial
(
    OSlwToolSarsaLamberSTU *pTQL,
    lw_u16 state_row, lw_u16 state_col,
    lw_u16 action_row, lw_u16 action_col,
    void *pState,
    void *pAction,
    void *pTable,
    void *pStateNow,
    void *pStateTermial,
    lw_u16 *pactchoose,
    void *pTrace
)

{
    OSLW_assert(!(pTQL));
    OSlwToolQLearningTableInitial
    (
        (OSlwToolQLearningSTU *)pTQL,
        state_row, state_col,
        action_row, action_col,
        pState,
        pAction,
        pTable,
        pStateNow,
        pStateTermial,
        pactchoose
    );
    if (pTQL->basic.basic.pmem)
    {
        if (!pTrace)
        {
            pTrace = pTQL->basic.basic.pmem->Malloc(pTQL->basic.basic.pmem, PARA_MEM_CAL(state_row*action_row));
        }
    }
    else;


    OSlwToolMatrixInitial(&(pTQL->Trace), state_row, action_row, pTrace);


    return;
}

OSlwToolSarsaLamberSTU* OSlwToolSarsaLamberLearnFun(OSlwToolSarsaLamberSTU *pTSL)
{
    OSlwToolQRLDataSTU *pTQRLDS;
    OSlwToolQRLBasicSTU *pQRLB;
	OSlwToolRLBasicSTU *pRL;
    ParaType pre, err, buf1, buf2, *p, *q;
    lw_u16 row;
    lw_u32 num, i;
    OSlwToolMatrixSTU mat;


    OSLW_assert(!(pTSL));

    pQRLB = &(pTSL->basic);
	pRL = &(pTSL->basic.basic);
    pTQRLDS = &(pTSL->basic.DataTable);
	OSLW_RL_UPDATE(pTSL);
    /*(Ver.=0.97)
        mat.length = pTQRLDS->state.length;
        mat.a = pTQRLDS->state.a;
        pOSlwToolMatrixIndex(&mat, pQRLB->StateNow, &(pQRLB->StateNowIndex), &col);

        pre = (OSLW_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateLastIndex, pQRLB->ActLastIndex));
    	*/

    if (pOSlwToolMatrixFind(&(pTQRLDS->StateTable), &(pRL->StateNow), 0, 0, &row) == OSlwMartixRes_NoFind)//û���ҵ�˵��Ҫ׷��״̬
    {

        row = pTQRLDS->StateTable.row;//�������һ��
				pRL->StateStoreFun((OSlwToolRLBasicSTU *)pTSL, NULL);
    }

    pQRLB->StateNowIndex = row;

    pre = (OSLW_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateLastIndex, pQRLB->ActLastIndex));

    if (!(pRL->isTermialFun((OSlwToolRLBasicSTU *)pQRLB)))
        pTSL->Q_Now = _ParaAdd(pRL->RewardNow, _ParaMpy(pRL->Gamma, OSLW_TOOL_M_GET(pTQRLDS->Qtable, pQRLB->StateNowIndex, pQRLB->ActNowIndex)));
    else
		pTSL->Q_Now = pRL->RewardNow;

    err = _ParaSub(pTSL->Q_Now, pre);


    mat.length = pTQRLDS->Qtable.col;
    mat.a= &(OSLW_TOOL_M_GET(pTSL->Trace, pQRLB->StateLastIndex, 0));

    pOSlwToolMatrixSet(&(mat), _ParaFrom(0),NULL);
    OSLW_TOOL_M_GET(pTSL->Trace, pQRLB->StateLastIndex, pQRLB->ActLastIndex) = _ParaFrom(1);

    buf1 = _ParaMpy(pTSL->Lr, err);
    buf2 = _ParaMpy(pTSL->Lamber, pRL->Gamma);

    //Ϊ�˽�Լ�ռ�û��ʹ�þ�������
    num = pTQRLDS->Qtable.length;
    p = pTQRLDS->Qtable.a;
    q = pTSL->Trace.a;

    for ( i = 0; i < num; i++)
    {
        p[i] = _ParaAdd(p[i], _ParaMpy(buf1, q[i]));
        q[i] = _ParaMpy(q[i], buf2);
    }

    //������һ�־���ֵ
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;

    return pTSL;

}

#endif // OSLW_TOOL_IMPORT_ALL || OSLW_TOOL_IMPORT_RL_SARSA


#if OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_EXP_RE)

OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayAppend(OSlwToolDQNetExpReplaySTU *pExpRe, lw_u16 Num, lw_u8 method)
{
	void *q;
	lw_u32 len;
	ParaType ActIndex;
	OSlwToolDQNetExpReplayFrameSTU *pexpref;


	OSLW_assert(!(pExpRe));

	/*(Ver.=0.97)
	//�ɰ汾 û�в���table
	pbase = ((lw_u8 *)(pExpRe->MemPool.pData) + Num*OSLW_TOOL_DQN_EXP_FRAME_SIZE(*pExpRe));
	//state last ����
	p = OSLW_TOOL_DQN_EXP_STATE_LAST(*pExpRe, pbase);
	q = (lw_u8 *)pExpRe->pTQL->StateLast.a;
	len = OSLW_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	OSLW_TOOL_MEMCPY(p, q, len);

	//������������(��ParaType����)
	ActIndex = _ParaFint(pExpRe->pTQL->ActNowIndex);
	q = (lw_u8 *)(&ActIndex);
	len = OSLW_TOOL_DQN_EXP_REWARD_SIZE(*pExpRe);
	OSLW_TOOL_MEMCPY(p, q, len);

	//����Reward
	q = (lw_u8 *)(&(pExpRe->pTQL->RewardNow));
	len = OSLW_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
	OSLW_TOOL_MEMCPY(p, q, len);

	//����state now
	q = (lw_u8 *)pExpRe->pTQL->StateNow.a;
	len = OSLW_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	OSLW_TOOL_MEMCPY(p, q, len);
	*/

	//state last ����
	len = OSLW_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	q = pExpRe->pTQL->basic.StateLast.a;
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 1, q, len) != 0)
	{
		OSLW_assert(1);
	}

	//������������(��ParaType����)
	if (method)//�������0 ���� ֱ�ӱ���
	{
		q = pExpRe->pTQL->basic.ActNow.a;
		len = OSLW_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
		if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 2, q, len) != 0)
		{
			OSLW_assert(1);
		}

	}
	else//�����0 ���� ��������
	{
		ActIndex = _ParaFint(pExpRe->pTQL->ActNowIndex);
		q = (&ActIndex);
		len = OSLW_TOOL_DQN_EXP_ACTION_SIZE(*pExpRe);
		if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 2, q, len) != 0)
		{
			OSLW_assert(1);
		}
	}


	//����Reward
	q = (&(pExpRe->pTQL->basic.RewardNow));
	len = OSLW_TOOL_DQN_EXP_REWARD_SIZE(*pExpRe);
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 3, q, len) != 0)
	{
		OSLW_assert(1);
	}
	//����state now
	q = pExpRe->pTQL->basic.StateNow.a;
	len = OSLW_TOOL_DQN_EXP_STATE_SIZE(*pExpRe);
	if (pExpRe->MemTable.WriteFun(&(pExpRe->MemTable), Num, 4, q, len) != 0)
	{
		OSLW_assert(1);
	}

	if (pExpRe->PriorType == OSlwToolDQNetExpReplay_PER)//��������Ż�
	{
		pexpref = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), Num, 0, NULL);

		_OSlwToolDQNetExpReplayDelete(pExpRe, pexpref);

		pexpref->Importance = pExpRe->Rmax;

		_OSlwToolDQNetExpReplayInsert(pExpRe, pexpref);

	}



	/*(Ver.=0.97)
	//�ɰ汾 û�в���table
	pbase = ((lw_u8 *)(pExpRe->MemPool.pData) + (Num + 1)*OSLW_TOOL_DQN_EXP_FRAME_SIZE(*pExpRe));

	if (p != pbase)
	{
	OSLW_assert(1);
	}
	*/

	return pExpRe;

}


OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayMinMax(OSlwToolDQNetExpReplaySTU *pExpRe)
{
	lw_32 num, i;
	ParaType loop_max;
	OSlwToolDListNodeConnectSTU *pcon;
	OSlwToolDQNetExpReplayFrameSTU *p;

	pcon = pExpRe->DivResult.pData;
	num = pExpRe->DivResult.uData;



	i = num;
	while (i<0 && !(pcon[i].pLast))
	{
		i--;
	}

	if (i < 0)
	{
		pExpRe->Min = pExpRe->Rmax;
	}
	else
	{
		p = (OSlwToolDQNetExpReplayFrameSTU *)(pcon[i].pLast);
		pExpRe->Min = p->Importance;
	}

	i = 0;
	while (i< num && !(pcon[i].pNext))
	{
		i++;
	}

	if (i == num)
	{
		loop_max = pExpRe->Rmin;
	}
	else
	{
		p = (OSlwToolDQNetExpReplayFrameSTU *)(pcon[i].pNext);
		loop_max = p->Importance;
	}

	pExpRe->Max = loop_max;

	return pExpRe;
}

OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayInsert(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF)
{

	lw_32 hash_index;
	OSlwToolDListNodeConnectSTU *pcon;
	OSlwToolDQNetExpReplayFrameSTU *pLast, *pNext;

	OSLW_assert(!(pExpRe));
	OSLW_assert(!(pExpReF));


	//����1 ��Ҫ�Զ�λ����ϣֵ���㣩
	hash_index = _ParaInt(_ParaCeil(_ParaDiv(_ParaSub(pExpReF->Importance, pExpRe->Rmin), pExpRe->Div)));

	hash_index = hash_index > (lw_32)(pExpRe->DivResult.uData) ? pExpRe->DivResult.uData : hash_index;
	hash_index = hash_index < 0 ? 0 : hash_index;

	hash_index = pExpRe->DivResult.uData - hash_index;//��������

	pcon = pExpRe->DivResult.pData;

	//����2 ��ѯ����
	pcon += hash_index;
	pLast = (OSlwToolDQNetExpReplayFrameSTU *)pcon;
	pNext = (OSlwToolDQNetExpReplayFrameSTU *)pcon->pNext;
	while (pNext)
	{
		if (pNext->Importance <= pExpReF->Importance)//�ȸü���֡С
		{
			break;
		}
		pLast = (OSlwToolDQNetExpReplayFrameSTU *)pLast->con.pNext;
		pNext = (OSlwToolDQNetExpReplayFrameSTU *)pNext->con.pNext;
	}

	if (pNext == NULL)//������β��
	{
		OSLW_TOOL_DLIST_NODE_CONNECT(pLast, pExpReF);//����
		pcon->pLast = (OSlwToolDListNodeConnectSTU *)pExpReF;
	}
	else
	{
		OSLW_TOOL_DLIST_NODE_INSERT(pLast, pExpReF);//����
	}

	pExpReF->pcon = pcon;

	//����3 ͳ����ֵ����

	pExpRe->Sum = _ParaAdd(pExpRe->Sum, pExpReF->Importance);

	/*(Ver.=0.97)
	if (pExpRe->pMax == NULL)
	{
	pExpRe->pMax = pExpReF;
	}
	else if (pExpRe->pMax->Importance <= pExpReF->Importance)
	{
	pExpRe->pMax = pExpReF;
	}
	else;

	if (pExpRe->pMin == NULL)
	{
	pExpRe->pMin = pExpReF;
	}
	else if (pExpRe->pMin->Importance > pExpReF->Importance)
	{
	pExpRe->pMin = pExpReF;
	}
	else;
	*/
	return pExpRe;
}



OSlwToolDQNetExpReplaySTU *_OSlwToolDQNetExpReplayDelete(OSlwToolDQNetExpReplaySTU *pExpRe, OSlwToolDQNetExpReplayFrameSTU *pExpReF)
{
	lw_u8 _f_status;
//	lw_32 i, num;
	//OSlwToolDListNodeConnectSTU *pcon;

	//pcon = pExpRe->DivResult.pData;
	//��ѯɾ��
	if (pExpReF->con.pLast == NULL && pExpReF->con.pNext == NULL)
	{
		return NULL;
	}
	_f_status = (((size_t)(pExpReF->pcon->pLast) == (size_t)pExpReF) << 1) | ((size_t)(pExpReF->pcon->pNext) == (size_t)pExpReF);
	switch (_f_status)
	{

	case 1:
	case 0:
		OSLW_TOOL_DLIST_NODE_DELECT(pExpReF);
		break;

	case 2://��β�� ����Ψһ
		pExpReF->pcon->pLast = pExpReF->con.pLast;
		pExpReF->con.pLast->pNext = NULL;
		break;

	case 3://Ψһ
		pExpReF->pcon->pLast = NULL;
		pExpReF->pcon->pNext = NULL;

		break;
	default:
		OSLW_assert(1);
		break;
	}

	//�⹳
	pExpReF->con.pLast = NULL;
	pExpReF->con.pNext = NULL;
	pExpReF->pcon = NULL;


	//ͳ����ֵ����

	pExpRe->Sum = _ParaSub(pExpRe->Sum, pExpReF->Importance);

	/*(Ver.=0.97)
	num = pExpRe->DivResult.uData;

	if (pExpRe->pMax ==pExpReF)
	{
	i = 0;
	while (i< num && !(pcon[i].pNext))
	{
	i++;
	}

	if (i == num)
	{
	pExpRe->pMax = NULL;
	}
	else
	{
	pExpRe->pMax = pcon[i].pNext;
	}

	}

	if (pExpRe->pMin == pExpReF)
	{
	i = num;
	while (i && !(pcon[i].pLast))
	{
	i--;
	}

	if (i == num)
	{
	pExpRe->pMax = NULL;
	}
	else
	{
	pExpRe->pMax = pcon[i].pNext;
	}

	}
	*/

	return pExpRe;
}



OSlwToolDQNetExpReplaySTU *OSlwToolDQNetExpReplayUpdate(OSlwToolDQNetExpReplaySTU *pExpRe, lw_u16 num)
{
	OSlwToolDQNetExpReplayFrameSTU **ppsample;

	if (pExpRe->PriorType == OSlwToolDQNetExpReplay_PER)
	{
		ppsample = (OSlwToolDQNetExpReplayFrameSTU **)(pExpRe->SampleResult.pData);
		while (num--)
		{
			_OSlwToolDQNetExpReplayDelete(pExpRe, *ppsample);

			(*ppsample)->Importance = (*ppsample)->_new_Importance;

			_OSlwToolDQNetExpReplayInsert(pExpRe, *ppsample);
			ppsample++;
		}

	}

	return pExpRe;
}



lw_u16 OSlwToolDQNetExpReplaySample(OSlwToolDQNetExpReplaySTU *pExpRe)
{
	void **pp;
	lw_u16 len_div;
	lw_u16 n;
	lw_u16 drand = 0;
	lw_u16 num = 0;
	ParaType para_div, sample_th, now_Sum, now_min, loop_max, loop_min;
	OSlwToolDListNodeConnectSTU *pcon;
	OSlwToolDQNetExpReplayFrameSTU *pexpref;
	OSlwToolRLBasicSTU *pRL;
	lw_u8 _loop_status;

	OSLW_assert(!(pExpRe));

	pcon = pExpRe->DivResult.pData;
	pp = (void **)pExpRe->SampleResult.pData;
	pRL = &(pExpRe->pTQL->basic);
	if (pExpRe->PriorType == OSlwToolDQNetExpReplay_PER)
	{
		para_div = _ParaDiv(pExpRe->Sum, _ParaFint(pExpRe->SampleResult.uData));
		now_min = pExpRe->Rmin;
		sample_th = pRL->pRand->rand(pRL->pRand, now_min, _ParaAdd(para_div, now_min));
		now_Sum = _ParaFrom(0);
		_loop_status = 1;
		loop_max = _ParaSub(pExpRe->Rmin, pExpRe->Epsi);
		loop_min = _ParaAdd(pExpRe->Rmax, pExpRe->Epsi);

		for (n = 0; n < pExpRe->DivResult.uData; n++)//ѭ����ϣ���ɢ��
		{
			pexpref = (OSlwToolDQNetExpReplayFrameSTU *)(pcon[n].pNext);
			while (pexpref)//����ѭ��
			{
				//ѭ������
				now_Sum = _ParaAdd(now_Sum, pexpref->Importance);
				/*(Ver.=0.97)
				if (now_Sum >= sample_th && num < pExpRe->SampleResult.uData)
				{
				now_min = _ParaAdd(para_div, now_min);
				sample_th = pExpRe->pTQL->pRand->rand(pExpRe->pTQL->pRand, para_div + now_min, now_min);
				*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
				num++;
				}
				*/
				switch (_loop_status)
				{
				case 2:
					if (now_Sum >= _ParaAdd(now_min, para_div))
					{
						//��ǰ�ĺ�>=��һ��
						//˵���ո�Խ��
						now_min = _ParaAdd(para_div, now_min);
						sample_th = pRL->pRand->rand(pRL->pRand, now_min, _ParaAdd(para_div, now_min));
						_loop_status = 1;
					}
					else
					{
						break;
					}
				case 1:
					if (now_Sum >= sample_th)
					{
						*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
						num++;
						_loop_status = 2;
					}
					break;

				default:
					OSLW_assert(1);
					break;
				}

				//˳��������ֵ
				if (loop_max < pexpref->Importance)
				{
					loop_max = pexpref->Importance;
				}

				if (loop_min > pexpref->Importance)
				{
					loop_min = pexpref->Importance;
				}
				drand++;
				pexpref = (OSlwToolDQNetExpReplayFrameSTU *)(pexpref->con.pNext);

			}//end while

		}//endfor
		pExpRe->Min = loop_min;
		pExpRe->Max = loop_max;

	}// end pExpRe->PriorType == OSlwToolDQNetExpReplay_PER
	else
	{
		len_div = pExpRe->MemTable.Row.uData / pExpRe->SampleResult.uData;
		for (n = 0; n < pExpRe->MemTable.Row.uData; n += len_div)
		{
			drand = pRL->pRand->randint(pRL->pRand, n, n + len_div);
			*pp++ = pExpRe->MemTable.ReadFun(&(pExpRe->MemTable), drand, 0, NULL);
			num++;
		}
	}

	return num;


}
#endif // OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_EXP_RE)


#if OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_DQN && OSLW_TOOL_IMPORT_NN && OSLW_TOOL_IMPORT_NN_BPnn && OSLW_TOOL_IMPORT_RL_EXP_RE)

void OSlwToolDQNetInitial(OSlwToolDQNetSTU *pDQN)
{
    OSLW_assert(!(pDQN));

    memset(pDQN, 0, sizeof(OSlwToolDQNetSTU));

	OSlwToolBPnnInit(&(pDQN->MainNet),1);
	OSlwToolBPnnInit(&(pDQN->TargetNet),1);
	pDQN->TargetNet.Train.Flag.NeedTrain = OSlwToolNNNeedTrain_NeedNot;

    pDQN->StoreMemFun = (struct OSLW_TOOL_DEEP_Q_NET_STRUCT *(*)(struct OSLW_TOOL_DEEP_Q_NET_STRUCT *))OSlwToolDQNetStoreMem;
    pDQN->basic.basic.ChooseFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolDQNetChoose;
    pDQN->basic.basic.LearnFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolDQNetLearning;

    pDQN->ExpReplay.AppendFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *, lw_u16, lw_u8))OSlwToolDQNetExpReplayAppend;
    pDQN->ExpReplay.SampleFun = (lw_u16 (*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *))OSlwToolDQNetExpReplaySample;
    pDQN->ExpReplay.UpdateFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *, lw_u16))OSlwToolDQNetExpReplayUpdate;
    pDQN->ExpReplay.MinMaxFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *))OSlwToolDQNetExpReplayMinMax;
}


void OSlwToolDQNetParaInitial//DQN�ڴ��ʼ������
(
    OSlwToolDQNetSTU *pDQN,//thisָ��
    ParaType Ep,//��ֵ
    ParaType Gamma,//˥������
    OSlwToolDQNetOptimNum Optim,
    OSlwToolRandomBasicSTU *prand,//�����������pQRL->Ep
    OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
    pActEnvFunType pActfun,//�������к���ָ��
    pBornFunType pBornfun//������ʼ������ָ��
)
{
    OSLW_assert(!(pDQN));
    OSLW_assert(!(prand));
	OSLW_assert(!(pMem));
    OSLW_assert(!(pActfun));
    OSLW_assert(!(pBornfun));

    pDQN->basic.Ep = Ep;
    pDQN->basic.basic.Gamma = Gamma;
    pDQN->Optim = Optim;
    pDQN->basic.basic.pRand = prand;
    pDQN->basic.basic.pmem = pMem;
    pDQN->basic.basic.ActEnvFun = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pActfun;
	pDQN->basic.basic.BornFun = OSlwToolRL_Born;
    pDQN->basic.basic.BornFunByUser = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pBornfun;

    pDQN->basic.basic.isTermialFun = (lw_8 (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_isTermial;

}

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
)
{
    OSLW_assert(!(pDQN));

    if (pDQN->basic.basic.pmem)
    {
        if (!pAction)
        {
            pAction = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(action_row*action_col));
        }

        if (!pStateNow)
        {
            pStateNow = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateLast)
        {
            pStateLast = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateTermial)
        {
            pStateTermial = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));
        }

        if (!pStateFactor)
        {
            pStateFactor = pDQN->basic.basic.pmem->Malloc(pDQN->basic.basic.pmem, PARA_MEM_CAL(state_col * 1));//
            OSlwToolMatrixInitial(&(pDQN->StateFactor), 1, state_col, pStateFactor);//
            pOSlwToolMatrixSet(&(pDQN->StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
        }
    }

    OSlwToolMatrixInitial(&(pDQN->basic.basic.StateNow), 1, state_col, pStateNow);
    OSlwToolMatrixInitial(&(pDQN->basic.basic.StateLast), 1, state_col, pStateLast);
    OSlwToolMatrixInitial(&(pDQN->basic.basic.StateTermial), 1, state_col, pStateTermial);

    OSlwToolMatrixInitial(&(pDQN->basic.DataTable.ActionTable), action_row, action_col, pAction);

    OSlwToolMatrixInitial(&(pDQN->StateFactor), 1, state_col, pStateFactor);

    pDQN->basic.DataTable.StateTable.col = state_col;

}

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
)
{
    OSlwMemoryBasicSTU *pmem;
    lw_u16 col_kind[] = { sizeof(OSlwToolDQNetExpReplayFrameSTU),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
    OSLW_assert(!(pDQN));
    OSLW_assert(!(MemPoolLength));
    OSLW_assert(!(batch_size));

    pDQN->ExpReplay.pTQL = (OSlwToolQRLBasicSTU *)pDQN;
    pDQN->ExpReplay.PriorType = PER;
    pDQN->NetCopyThreshold = copy_th;
    //�ڴ����
    pmem = pDQN->basic.basic.pmem;

    //������װ
    pDQN->ExpReplay.DivResult.uData = batch_size;
    pDQN->ExpReplay.SampleResult.uData = batch_size;
    //pDQN->ExpReplay.MemPool.uData = MemPoolLength;

    pDQN->ExpReplay.State_Col = pDQN->basic.DataTable.StateTable.col;
	pDQN->ExpReplay.Action_Col = pDQN->basic.DataTable.ActionTable.col;

    col_kind[1] *= pDQN->ExpReplay.State_Col;
    col_kind[4] *= pDQN->ExpReplay.State_Col;

	OSlwToolTableInitial(&(pDQN->ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);

	pDQN->ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pDQN->ExpReplay.SampleResult.uData * sizeof(void *));



    if (PER == OSlwToolDQNetExpReplay_PER)
    {
        //������װ

        pDQN->ExpReplay.Alpha = Alpha;
        pDQN->ExpReplay.Beta = Beta;
        pDQN->ExpReplay.Epsi = Epsi;

        Rmax = OSLW_TOOL_EXPRE_CAL_P(Rmax, pDQN->ExpReplay);
        Rmin = OSLW_TOOL_EXPRE_CAL_P(Rmin, pDQN->ExpReplay);

        pDQN->ExpReplay.Rmax = Rmax;
        pDQN->ExpReplay.Rmin = Rmin;

        pDQN->ExpReplay.Div = _ParaDiv(_ParaSub(Rmax, Rmin), _ParaFrom(batch_size));


        pDQN->ExpReplay.DivResult.pData = pmem->Malloc(pmem, pDQN->ExpReplay.DivResult.uData * sizeof(OSlwToolDListNodeConnectSTU));

        memset(pDQN->ExpReplay.DivResult.pData, 0, pDQN->ExpReplay.DivResult.uData * sizeof(OSlwToolDListNodeConnectSTU));


    }
    //pDQN->ExpReplay.MemPool.pData = pmem->Malloc(pmem, pDQN->ExpReplay.MemPool.uData*OSLW_TOOL_DQN_EXP_FRAME_SIZE(pDQN->ExpReplay));



}
void OSlwToolDQNetAppendInitial
(
    OSlwToolDQNetSTU *pDQN,
    lw_u16 row,
    lw_u16 col,
	OSlwNNinitFunType pFun,
    OSlwToolRandomBasicSTU *pTRB,
    ParaType initd1,ParaType initd2,
	OSlwToolNNLayerActFunSTU *pTemplet
)
{
    OSlwMemoryBasicSTU *pmem;
 

    OSLW_assert(!(pDQN));
	OSLW_assert(!(pFun));
    OSLW_assert(!(pTRB));

    pmem = (pDQN->basic.basic.pmem);

	OSlwToolBPnnFullConAppend(
		&(pDQN->MainNet),
		row, col,
		NULL, NULL,
		NULL, NULL,
		pFun, pTRB, initd1, initd2,
		pTemplet, pmem
	);

	OSlwToolBPnnFullConAppend(
		&(pDQN->TargetNet),
		row, col,
		NULL, NULL,
		NULL, NULL,
		NULL, NULL, initd1, initd2,
		pTemplet, pmem
	);


}


void OSlwToolDQNetTarinInitial(
    OSlwToolDQNetSTU *pDQN,
    ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
)
{
    OSLW_assert(!(pDQN));
    OSLW_assert(!(loss));
	OSlwToolBPnnTrainInit
    (
        &(pDQN->MainNet),
        qref,
        pDQN->basic.basic.pmem,
		loss,
        nl
    );
}


void OSlwToolDQNetAllDataInit(OSlwToolDQNetSTU *pDQN)
{

	OSLW_assert(!(pDQN));
	OSlwToolBPnnAllDataInit(&(pDQN->MainNet), pDQN->basic.basic.pmem);
	OSlwToolBPnnAllDataInit(&(pDQN->TargetNet), pDQN->basic.basic.pmem);
	OSlwToolBPnnCopy(&(pDQN->TargetNet), &(pDQN->MainNet));
}

OSlwToolDQNetSTU* OSlwToolDQNetStoreMem(OSlwToolDQNetSTU *pDQN)
{
    lw_u16 i = 0;

    OSLW_assert(!(pDQN));
    i = pDQN->ExpReplay.MemTable.Row.uData;

	pDQN->ExpReplay.AppendFun(&(pDQN->ExpReplay), pDQN->Count, 0);

    if (++(pDQN->Count) >= i)
    {
        pDQN->Count = 0;
    }
    if (pDQN->CountMax < i)
        pDQN->CountMax++;

    //״̬����
    pOSlwToolMatrixSet(&(pDQN->basic.basic.StateLast), 0, &(pDQN->basic.basic.StateNow));

    return pDQN;

}


OSlwToolDQNetSTU* OSlwToolDQNetLearning(OSlwToolDQNetSTU *pDQN)
{
    lw_u16 i, sample_len, actindex;
    ParaType *pStateLast, *pActIndex, *pReward, *pStateNow;
    ParaType dmax;
    ParaType ISweight;
    lw_u16  maxindex;
    OSlwToolQRLBasicSTU *pQRLB;
    //OSlwToolQRLDataSTU *pTQRLDS;
    OSlwToolMatrixSTU mat;
    OSlwToolDQNetExpReplayFrameSTU *pbase;
    void **ppsample;
    OSLW_assert(!(pDQN));

    pQRLB = &(pDQN->basic);
    //pTQRLDS = &(pQRLB->DataTable);

	pDQN->StoreMemFun(pDQN);

	OSLW_RL_UPDATE(pDQN);

    if (pDQN->CountMax < pDQN->ExpReplay.MemTable.Row.uData)//û�д���
    {
        return pDQN;
    }

    if ((pDQN->Count_Copy >= pDQN->NetCopyThreshold))//�ж��Ƿ�������ֵ
    {
        pDQN->Count_Copy = 0;
        OSlwToolBPnnCopy(&(pDQN->TargetNet), &(pDQN->MainNet));//����������
    }
    else
    {
        pDQN->Count_Copy++;
    }


    sample_len = pDQN->ExpReplay.SampleFun(&(pDQN->ExpReplay));//�������
    ppsample = pDQN->ExpReplay.SampleResult.pData;
    for (i = 0; i < sample_len; i++)
    {
        pbase = (OSlwToolDQNetExpReplayFrameSTU *)ppsample[i];//
        //�õ�����

        pStateLast = (ParaType *)OSLW_TOOL_DQN_EXP_STATE_LAST(pDQN->ExpReplay, pbase);
        pActIndex = (ParaType *)OSLW_TOOL_DQN_EXP_ACT(pDQN->ExpReplay, pbase);
        pReward = (ParaType *)OSLW_TOOL_DQN_EXP_R(pDQN->ExpReplay, pbase);
        pStateNow = (ParaType *)OSLW_TOOL_DQN_EXP_STATE_NOW(pDQN->ExpReplay, pbase);


        //qeval�������qnow ����qlast
        mat.length = pDQN->ExpReplay.State_Col;
        mat.a = pStateLast;
        //pOSlwToolMatrixSet(&(pDQN->MainNet.x), 0, &(mat));
        pOSlwToolMatrixDot(&(pDQN->MainNet.x), &(mat), &(pDQN->StateFactor));//���Թ�һ������


        //qtarget���� ��� qnext ����qnow
        mat.length = pDQN->ExpReplay.State_Col;
        mat.a = pStateNow;
        //pOSlwToolMatrixSet(&(pDQN->TargetNet.x), 0, &(mat));
        pOSlwToolMatrixDot(&(pDQN->TargetNet.x), &(mat), &(pDQN->StateFactor));//���Թ�һ������

		pDQN->TargetNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDQN->TargetNet));

		pDQN->MainNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDQN->MainNet));


        switch (pDQN->Optim)
        {
        case OSlwToolDQNetOptim_NormalDQN:
            pOSlwToolMatrixMaxMin(&(pDQN->TargetNet.y), &dmax, &maxindex, NULL, NULL);
            break;

        case OSlwToolDQNetOptim_DoubleDQN://����DoubleDQN�����Ż�
            pOSlwToolMatrixMaxMin(&(pDQN->MainNet.y), &dmax, &maxindex, NULL, NULL);
            dmax = pDQN->TargetNet.y.a[maxindex];
            break;

        default:
            OSLW_assert(1);
            break;
        }

        actindex = (lw_u16)(_ParaInt(*pActIndex));

        pOSlwToolMatrixSet(&(pDQN->MainNet.ref), 0, &(pDQN->MainNet.y));//ʹ���Ϊ0

		/*
        if (!(pQRLB->basic.isTermialFun((OSlwToolRLBasicSTU *)pQRLB)))
        {
            pDQN->MainNet.ref.a[actindex] = (*pReward) + _ParaMpy(pQRLB->basic.Gamma, dmax);
        }
        else
        {
            pDQN->MainNet.ref.a[actindex] = (*pReward);
        }
		*/
		pDQN->MainNet.ref.a[actindex] = (*pReward) + _ParaMpy(pQRLB->basic.Gamma, dmax);
        //OSlwToolBPnnCalErr(&(pDQN->MainNet));
        //OSlwToolBPnnUpdate(&(pDQN->MainNet));


        if (pDQN->ExpReplay.PriorType == OSlwToolDQNetExpReplay_PER)
        {

            ISweight = _ParaPow(_ParaDiv(pDQN->ExpReplay.Min, pbase->Importance), pDQN->ExpReplay.Beta);
			pDQN->MainNet._nl_factor = _ParaMpy(ISweight, _ParaFrom(1.5));

            OSlwToolBPnnCalErr(&(pDQN->MainNet));

            ISweight = _ParaPow(_ParaAdd(pDQN->MainNet.Error, pDQN->ExpReplay.Epsi),pDQN->ExpReplay.Alpha);

            ISweight = ISweight > pDQN->ExpReplay.Rmax ? pDQN->ExpReplay.Rmax : ISweight;
            ISweight = ISweight < pDQN->ExpReplay.Rmin ? pDQN->ExpReplay.Rmax : ISweight;

            pbase->_new_Importance = ISweight;

        }
        else
        {
			OSlwToolBPnnCalErr(&(pDQN->MainNet));//ѵ�������� ������������
        }

    }

    OSlwToolBPnnReview(&(pDQN->MainNet));

    pDQN->ExpReplay.UpdateFun(&(pDQN->ExpReplay), sample_len);


    /*(Ver.=0.97)
    //�ɰ汾
    for (i = 0; i < pDQN->CountMax; i++)
    {
        drand = pDQN->basic.basic.pRand->rand(pDQN->basic.basic.pRand, _ParaFint(1), _ParaFint(0));

        if (drand > pDQN->P)
        {
            continue;
        }
        pdata = pDQN->MemPool.a + (i << 2);

        pDQN->TargetNet.x.a[0] = pdata[3];//qtarget���� ��� qnext
        pDQN->MainNet.x.a[0] = pdata[0];//qeval�������qnow

        OSlwToolBPnnRunning(&(pDQN->TargetNet));
        OSlwToolBPnnRunning(&(pDQN->MainNet));

        pOSlwToolMatrixMaxMin(&(pDQN->TargetNet.y), &dmax, &maxindex, NULL, NULL);

        actindex = _ParaInt(pdata[1]);

        pOSlwToolMatrixSet(&(pDQN->MainNet.ref), 0, &(pDQN->MainNet.y));//ʹ���Ϊ0
        reward = pdata[2];
        if (pQRLB->StateNow != pQRLB->StateTermial)
        {
            pDQN->MainNet.ref.a[actindex] = reward + _ParaMpy(pQRLB->Gamma, dmax);
        }
        else
        {
            pDQN->MainNet.ref.a[actindex] = reward;
        }

        OSlwToolBPnnCalErr(&(pDQN->MainNet));
        OSlwToolBPnnUpdate(&(pDQN->MainNet));
    }


    //������һ�־���ֵ
    pQRLB->StateLast = pQRLB->StateNow;
    pQRLB->StateLastIndex = pQRLB->StateNowIndex;
    pQRLB->ActLastIndex = pQRLB->ActNowIndex;
    */


	return pDQN;
}


OSlwToolDQNetSTU* OSlwToolDQNetChoose(OSlwToolDQNetSTU *pDQN)
{

    ParaType drand, dmax;
    lw_u16 selrand;
    //OSlwToolMatrixSTU mat;
    OSlwToolQRLBasicSTU *pQRLB;
    OSlwToolQRLDataSTU *pTQRLDS;
	OSlwToolRLBasicSTU *pRL;
    OSLW_assert(!(pDQN));

    pQRLB = &(pDQN->basic);
	pRL = &(pQRLB->basic);
	drand = pRL->pRand->rand(pRL->pRand, _ParaFint(0), _ParaFint(1));
    pTQRLDS = &(pQRLB->DataTable);



    if (drand < pQRLB->Ep)
    {
        //��װ����
        /*(Ver.=0.97)dmax= _ParaDiv(pQRLB->StateNow, pDQN->StateFactor);//��һ��
        pDQN->MainNet.x.a[0] = dmax;*/
        //pOSlwToolMatrixSet(&(pDQN->MainNet.x), 0, &(pQRLB->StateNow));
        pOSlwToolMatrixDot(&(pDQN->MainNet.x), &(pRL->StateNow), &(pDQN->StateFactor));//���Թ�һ������
        //��������
		pDQN->MainNet.Train.mini_batch_now = 1;
        OSlwToolBPnnForward(&(pDQN->MainNet));
		pDQN->MainNet.Train.AllBatchCount = 0;
		pDQN->MainNet.Train._batch_stream_count = 0;
        //��ѯQ������Ϊ
        OSlwToolMatrixMaxMin(&(pDQN->MainNet.y), &dmax, &selrand, NULL, NULL);

//		if (isnan(pDQN->MainNet.y.a[0]))
//		{
//			printf("------------\n-------------");
//		}

        pQRLB->ActNowIndex = selrand;


    }
    else
    {
        //ֱ�����ѡȡ��Ϊ
        selrand = pRL->pRand->randint(pRL->pRand, 0, pTQRLDS->ActionTable.row);
        pQRLB->ActNowIndex = selrand;

    }

    //������Ϊ
	pRL->ActNow.a = pTQRLDS->ActionTable.a + pQRLB->ActNowIndex*pTQRLDS->ActionTable.col;

    return pDQN;
}
#endif // OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_DQN && OSLW_TOOL_IMPORT_NN && OSLW_TOOL_IMPORT_NN_BPnn)


#if OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_DDPG && OSLW_TOOL_IMPORT_NN && OSLW_TOOL_IMPORT_NN_BPnn && OSLW_TOOL_IMPORT_RL_EXP_RE)


void OSlwToolDDPGradInitial(OSlwToolDDPGradSTU *pDDPG)//DDPG��ʼ�����������ȱ�����
{
	OSLW_assert(!(pDDPG));

	memset(pDDPG, 0, sizeof(OSlwToolDDPGradSTU));

	OSlwToolBPnnInit(&(pDDPG->CriticMainNet),1);
	OSlwToolBPnnInit(&(pDDPG->CriticTargetNet),1);
	OSlwToolBPnnInit(&(pDDPG->ActorMainNet),1);
	OSlwToolBPnnInit(&(pDDPG->ActorTargetNet),1);

	pDDPG->CriticTargetNet.Train.Flag.NeedTrain = OSlwToolNNNeedTrain_NeedNot;
	pDDPG->ActorTargetNet.Train.Flag.NeedTrain = OSlwToolNNNeedTrain_NeedNot;

	pDDPG->StoreMemFun = (struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT *(*)(struct OSLW_TOOL_DEEP_DETE_POLICY_GRAD_STRUCT *))OSlwToolDDPGradStoreMem;
	pDDPG->PGradBasic.basic.ChooseFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolDDPGradChoose;
	pDDPG->PGradBasic.basic.LearnFun = (void *(*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolDDPGradLearning;

	pDDPG->PGradBasic.ExpReplay.AppendFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *, lw_u16, lw_u8))OSlwToolDQNetExpReplayAppend;
	pDDPG->PGradBasic.ExpReplay.SampleFun = (lw_u16 (*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *))OSlwToolDQNetExpReplaySample;
	pDDPG->PGradBasic.ExpReplay.UpdateFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *, lw_u16))OSlwToolDQNetExpReplayUpdate;
	pDDPG->PGradBasic.ExpReplay.MinMaxFun = (struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *(*)(struct OSLW_TOOL_DQN_EXP_REPLAY_STRUCT *))OSlwToolDQNetExpReplayMinMax;


}

void OSlwToolDDPGradParaInitial//DDPG������ʼ������
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	ParaType Gamma,//˥������
	OSlwToolRandomBasicSTU *prand,//�����������
	OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
	pActEnvFunType pActfun,//�������к���ָ��
	pBornFunType pBornfun//������ʼ������ָ��
)
{

	OSLW_assert(!(pDDPG));
	OSLW_assert(!(prand));
	OSLW_assert(!(pMem));
	OSLW_assert(!(pActfun));
	OSLW_assert(!(pBornfun));

	pDDPG->PGradBasic.basic.Gamma = Gamma;

	pDDPG->PGradBasic.basic.pRand = prand;
	pDDPG->PGradBasic.basic.pmem = pMem;
	pDDPG->PGradBasic.basic.ActEnvFun = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pActfun;
	pDDPG->PGradBasic.basic.BornFun = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_Born;
	pDDPG->PGradBasic.basic.BornFunByUser = (void (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))pBornfun;

	pDDPG->PGradBasic.basic.isTermialFun = (lw_8 (*)(struct OSLW_TOOL_RL_BASIC_STRUCT *))OSlwToolRL_isTermial;


}


void OSlwToolDDPGradInterfaceInitial
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	lw_u16 state_dim, //״̬ά��
	lw_u16 action_dim,//����ά��
	void *pAction,//�����б��ڴ棨��ΪNULL��
	void *pStateNow,//��ǰ״̬����ΪNULL��
	void *pStateLast,//֮ǰ״̬����ΪNULL��
	void *pStateTermial,//�ս�״̬����ΪNULL��
	void *pStateFactor,//״̬��һ������(��ΪNULL)
	ParaType ActionMax, ParaType ActionMin,
	ParaType ActionVar
)
{
	OSLW_assert(!(pDDPG));

	if (pDDPG->PGradBasic.basic.pmem)
	{
		if (!pAction)
		{
			pAction = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(1 * action_dim));
		}

		if (!pStateNow)
		{
			pStateNow = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateLast)
		{
			pStateLast = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateTermial)
		{
			pStateTermial = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
		}

		if (!pStateFactor)
		{
			pStateFactor = pDDPG->PGradBasic.basic.pmem->Malloc(pDDPG->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));//
			OSlwToolMatrixInitial(&(pDDPG->PGradBasic.StateFactor), 1, state_dim, pStateFactor);//
			pOSlwToolMatrixSet(&(pDDPG->PGradBasic.StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
		}
	}

	OSlwToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateNow), 1, state_dim, pStateNow);
	OSlwToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateLast), 1, state_dim, pStateLast);
	OSlwToolMatrixInitial(&(pDDPG->PGradBasic.basic.StateTermial), 1, state_dim, pStateTermial);
	OSlwToolMatrixInitial(&(pDDPG->PGradBasic.basic.ActNow), 1, action_dim, pAction);

	OSlwToolMatrixInitial(&(pDDPG->PGradBasic.StateFactor), 1, state_dim, pStateFactor);

	pDDPG->PGradBasic.ActionMax = ActionMax;
	pDDPG->PGradBasic.ActionMin = ActionMin;
	pDDPG->ActionVar = ActionVar;

}

void OSlwToolDDPGradStoreMemInitial//DDPG�ڴ�����ʼ��
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	lw_u16 MemPoolLength,//���䳤��
	lw_u16 batch_size,//��ȡ�ļ��䳤��
	ParaType copy_data//���Ƶ���ֵ
)
{

	OSlwMemoryBasicSTU *pmem;
	lw_u16 col_kind[] = { sizeof(OSlwToolDQNetExpReplayFrameSTU),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
	OSLW_assert(!(pDDPG));
	OSLW_assert(!(MemPoolLength));
	OSLW_assert(!(batch_size));

	pDDPG->PGradBasic.ExpReplay.pTQL = (OSlwToolQRLBasicSTU *)pDDPG;
	pDDPG->PGradBasic.ExpReplay.PriorType = OSlwToolDQNetExpReplay_NoP;

	if (copy_data >_ParaFint(1))//>1 ��Ϊ��ֱ�Ӹ���ģʽ ������DQN
	{
		pDDPG->CopyMethod = OSlwToolDDPG_CopyMethod_Hard;
		pDDPG->Copy.HardReplace.Count_Copy = 0;
		pDDPG->Copy.HardReplace.NetCopyThreshold = (lw_u16)_ParaInt(copy_data);
	}
	else//���� �ȸ�DM�����еķ���
	{
		pDDPG->CopyMethod = OSlwToolDDPG_CopyMethod_Soft;
		pDDPG->Copy.tSoftReplace = copy_data;
	}


	//�ڴ����
	pmem = pDDPG->PGradBasic.basic.pmem;

	//������װ
	pDDPG->PGradBasic.ExpReplay.DivResult.uData = batch_size;
	pDDPG->PGradBasic.ExpReplay.SampleResult.uData = batch_size;
	


	//����ֵ�ط� ��ʼ��
	pDDPG->PGradBasic.ExpReplay.State_Col = pDDPG->PGradBasic.basic.StateNow.length;
	pDDPG->PGradBasic.ExpReplay.Action_Col = pDDPG->PGradBasic.basic.ActNow.length;

	//OSlwTable ��ʼ�� ����׼��
	col_kind[1] *= pDDPG->PGradBasic.ExpReplay.State_Col;
	col_kind[2] *= pDDPG->PGradBasic.ExpReplay.Action_Col;
	col_kind[4] *= pDDPG->PGradBasic.ExpReplay.State_Col;

	OSlwToolTableInitial(&(pDDPG->PGradBasic.ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);

	pDDPG->PGradBasic.ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pDDPG->PGradBasic.ExpReplay.SampleResult.uData * sizeof(void *));

}

/*
void OSlwToolDDPGradAppendInitial//DDPG������׷��,һ�λ�׷������������
(
	OSlwToolDDPGradSTU *pDDPG,//thisָ��
	OSlwToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	OSlwToolNNLayerActFunSTU **AFlist,//�����
	ParaType *nl_list//ѧϰ����
)
{
	lw_u16 i;
	lw_u16 row, col;
	OSlwMemoryBasicSTU *pmem;
	OSlwToolRandomBasicSTU *pTRB;
	OSlwToolNNLayerFullConSTU *p1, *p2;
	OSlwToolBPnnSTU *mn, *tn;

	OSLW_assert(!(pDDPG));
	OSLW_assert(!(layer_list));
	OSLW_assert(!(AFlist));
	OSLW_assert(!(nl_list));

	pmem = (pDDPG->PGradBasic.basic.pmem);
	pTRB = pDDPG->PGradBasic.basic.pRand;

	if (Kind == OSlwToolACtoActor)
	{
		row = pDDPG->PGradBasic.basic.StateNow.length;//ֱ���������
		layer_list[layer_num - 1] = pDDPG->PGradBasic.basic.ActNow.length;

		mn = &(pDDPG->ActorMainNet);
		tn = &(pDDPG->ActorTargetNet);

	}
	else if (Kind == OSlwToolACtoCritic)
	{
		row = pDDPG->PGradBasic.basic.StateNow.length + pDDPG->PGradBasic.basic.ActNow.length;//���ۼһὫ����Ҳ��������
		layer_list[layer_num - 1] = 1;

		mn = &(pDDPG->CriticMainNet);
		tn = &(pDDPG->CriticTargetNet);
	}
	else
	{
		OSLW_assert(1);
	}

	for (i = 0; i < layer_num; i++)
	{
		col = layer_list[i];

		p1 = OSlwToolBPnnLayerAppend_Dynamic(
			mn
			, AFlist[i]
			, row, col
			, pTRB
			, _ParaFrom(0.5), _ParaFrom(-0.5)
			, nl_list[i]
			, NULL, NULL, pmem
			, _ParaFrom(0)
		);

		p2 = OSlwToolBPnnLayerAppend_Dynamic(
			tn
			, AFlist[i]
			, row, col
			, pTRB
			, _ParaFrom(0.5), _ParaFrom(-0.5)
			, nl_list[i]
			, NULL, NULL, pmem
			, _ParaFrom(0)
		);

		pOSlwToolMatrixSet(&(p2->w), 0, &(p1->w));
		pOSlwToolMatrixSet(&(p2->BiasUN.bias), 0, &(p1->BiasUN.bias));

		row = col;

	}


}

void OSlwToolDDPGradLossInitial(
	OSlwToolDDPGradSTU *pDDPG,
	LossFunTYPE loss
)
{
	OSlwMemoryBasicSTU *pmem;
	void *pref;

	OSLW_assert(!(pDDPG));
	OSLW_assert(!(loss));

	pmem = pDDPG->PGradBasic.basic.pmem;
	pref = pmem->Malloc(pmem, PARA_MEM_CAL(pDDPG->PGradBasic.basic.ActNow.length));

	OSlwToolBPnnLossInitial
	(
		&(pDDPG->ActorMainNet),
		pref,
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->PGradBasic.basic.pmem,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		NULL,
		OSlwToolNNTrainSaveMethod_OnceSave,
		OSlwToolNNTrainUpdateMethod_Manu
	);


	pref = pmem->Malloc(pmem, PARA_MEM_CAL(pDDPG->PGradBasic.basic.ActNow.length));

	OSlwToolBPnnLossInitial
	(
		&(pDDPG->CriticMainNet),
		pref,
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->PGradBasic.basic.pmem,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		pDDPG->PGradBasic.ExpReplay.SampleResult.uData,
		loss,
		OSlwToolNNTrainSaveMethod_OnceSave,
		OSlwToolNNTrainUpdateMethod_Manu
	);
	
}
*/


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
)
{
	OSlwMemoryBasicSTU *pmem;


	OSLW_assert(!(pDDPG));
	OSLW_assert(!(pFun));
	OSLW_assert(!(pTRB));

	pmem = (pDDPG->PGradBasic.basic.pmem);

	if (Kind== OSlwToolACtoActor)
	{
		OSlwToolBPnnFullConAppend(
			&(pDDPG->ActorMainNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			pFun, pTRB, initd1, initd2,
			pTemplet, pmem
		);

		OSlwToolBPnnFullConAppend(
			&(pDDPG->ActorTargetNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL, initd1, initd2,
			pTemplet, pmem
		);
	}
	else
	{
		OSlwToolBPnnFullConAppend(
			&(pDDPG->CriticMainNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			pFun, pTRB, initd1, initd2,
			pTemplet, pmem
		);

		OSlwToolBPnnFullConAppend(
			&(pDDPG->CriticTargetNet),
			row, col,
			NULL, NULL,
			NULL, NULL,
			NULL, NULL, initd1, initd2,
			pTemplet, pmem
		);
	}




}


void OSlwToolDDPGradTarinInitial(
	OSlwToolDDPGradSTU *pDDPG,
	ParaType *qref,
	LossFunTYPE loss,
	ParaType nl
)
{
	OSLW_assert(!(pDDPG));
	OSLW_assert(!(loss));
	OSlwToolBPnnTrainInit
	(
		&(pDDPG->ActorMainNet),
		qref,
		pDDPG->PGradBasic.basic.pmem,
		loss,
		nl
	);

	OSlwToolBPnnTrainInit
	(
		&(pDDPG->CriticMainNet),
		qref,
		pDDPG->PGradBasic.basic.pmem,
		loss,
		nl
	);
}


void OSlwToolDDPGradAllDataInit(OSlwToolDDPGradSTU *pDDPG)
{

	OSLW_assert(!(pDDPG));
	OSlwToolBPnnAllDataInit(&(pDDPG->ActorMainNet), pDDPG->PGradBasic.basic.pmem);
	OSlwToolBPnnAllDataInit(&(pDDPG->ActorTargetNet), pDDPG->PGradBasic.basic.pmem);
	OSlwToolBPnnAllDataInit(&(pDDPG->CriticMainNet), pDDPG->PGradBasic.basic.pmem);
	OSlwToolBPnnAllDataInit(&(pDDPG->CriticTargetNet), pDDPG->PGradBasic.basic.pmem);

	OSlwToolBPnnCopy(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet));
	OSlwToolBPnnCopy(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet));
}


OSlwToolDDPGradSTU* OSlwToolDDPGradChoose(OSlwToolDDPGradSTU *pDDPG)
{
	
	pOSlwToolMatrixDot(&(pDDPG->ActorMainNet.x), &(pDDPG->PGradBasic.basic.StateNow), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������

	//��������
	pDDPG->ActorMainNet.Train.mini_batch_now = 1;
	OSlwToolBPnnForward(&(pDDPG->ActorMainNet));
	pDDPG->ActorMainNet.Train.AllBatchCount = 0;
	pDDPG->ActorMainNet.Train._batch_stream_count = 0;

	OSlwToolMatrixToRandnChip(
		&(pDDPG->PGradBasic.basic.ActNow),
		&(pDDPG->ActorMainNet.y), 
		pDDPG->PGradBasic.basic.pRand,
		pDDPG->ActionVar,
		pDDPG->PGradBasic.ActionMin,
		pDDPG->PGradBasic.ActionMax
	);//��̬�ֲ������ �����޷�



	
	/*
	OU���� ����ʧ��
	OSlwMat m_rand, m_k;
	OSlwToolTableSTU *ptable;
	ParaType temp = 1;

	ptable = &(pDDPG->ActorMainNet.Train.DeltTable);
	m_rand.a = ptable->ReadFun(ptable, 0, 0, NULL);
	m_rand.length = ptable->ColSize / sizeof(ParaType);

	OSlwToolMatrixRandomInitial(&m_rand, pDDPG->basic.pRand, 1, -1);

	OSlwToolMatrixToRandnChip(
		&(m_rand),
		&(m_rand),
		pDDPG->basic.pRand,
		pDDPG->ActionVar/1000,
		pDDPG->ActionMax,
		pDDPG->ActionMin
	);

	m_k.length = 1;
	m_k.a = &temp;



	_OSlwToolBPnnReviewOnce(&(pDDPG->ActorMainNet), 0, &m_k);

	pOSlwToolMatrixDot(&(pDDPG->ActorMainNet.x), &(pDDPG->basic.StateNow), &(pDDPG->StateFactor));//���Թ�һ������
																								  
	OSlwToolBPnnRunning(&(pDDPG->ActorMainNet));//��������

	temp = -1;

	_OSlwToolBPnnReviewOnce(&(pDDPG->ActorMainNet), 0, &m_k);//�����

	memset(ptable->Row.pData, 0, ptable->Row.uData * ptable->ColSize);

	
	OSlwToolMatrixToRandnChip(
		&(pDDPG->basic.ActNow),
		&(pDDPG->ActorMainNet.y),
		pDDPG->basic.pRand,
		pDDPG->ActionVar,
		pDDPG->ActionMax,
		pDDPG->ActionMin
	);//��̬�ֲ������ �����޷�
	*/

	return pDDPG;

}

OSlwToolDDPGradSTU* OSlwToolDDPGradStoreMem(OSlwToolDDPGradSTU *pDDPG)
{
	lw_u16 i = 0;

	OSLW_assert(!(pDDPG));
	i = pDDPG->PGradBasic.ExpReplay.MemTable.Row.uData;

	pDDPG->PGradBasic.ExpReplay.AppendFun(&(pDDPG->PGradBasic.ExpReplay), pDDPG->Count, 1);

	if (++(pDDPG->Count) >= i)
	{
		pDDPG->Count = 0;
	}
	if (pDDPG->CountMax < i)
		pDDPG->CountMax++;

	//״̬����
	pOSlwToolMatrixSet(&(pDDPG->PGradBasic.basic.StateLast), 0, &(pDDPG->PGradBasic.basic.StateNow));

	return pDDPG;


}

OSlwToolDDPGradSTU* OSlwToolDDPGradLearning(OSlwToolDDPGradSTU *pDDPG)
{
	lw_u16 i, sample_len;
	ParaType *pStateLast, *pAct, *pReward, *pStateNow, negbuf = _ParaFrom(1)/*, qbuf*/;
	OSlwToolRLBasicSTU *pRL;
	OSlwToolMatrixSTU mat1, mat2, mat3;
	OSlwToolDQNetExpReplayFrameSTU *pbase;
	void **ppsample;
	//OSlwToolNNLayerFullConSTU *pTNL;

	OSLW_assert(!(pDDPG));

	pRL = &(pDDPG->PGradBasic.basic);
	

	pDDPG->StoreMemFun(pDDPG);

	OSLW_RL_UPDATE(pDDPG);

	if (pDDPG->CountMax < pDDPG->PGradBasic.ExpReplay.MemTable.Row.uData)//û�д���
	{
		return pDDPG;
	}


	//���㲽 ����
	sample_len = pDDPG->PGradBasic.ExpReplay.SampleFun(&(pDDPG->PGradBasic.ExpReplay));//�������
	ppsample = pDDPG->PGradBasic.ExpReplay.SampleResult.pData;
	
	for (i = 0; i < sample_len; i++)
	{
		pbase = (OSlwToolDQNetExpReplayFrameSTU *)ppsample[i];//�õ�����

		pStateLast = (ParaType *)OSLW_TOOL_DQN_EXP_STATE_LAST(pDDPG->PGradBasic.ExpReplay, pbase);
		pAct = (ParaType *)OSLW_TOOL_DQN_EXP_ACT(pDDPG->PGradBasic.ExpReplay, pbase);
		pReward = (ParaType *)OSLW_TOOL_DQN_EXP_R(pDDPG->PGradBasic.ExpReplay, pbase);
		pStateNow = (ParaType *)OSLW_TOOL_DQN_EXP_STATE_NOW(pDDPG->PGradBasic.ExpReplay, pbase);


		//��һ�� �� StateNow ���� ActorTarget���� �õ�ActionNext 
		mat1.length = pRL->StateNow.length;
		mat1.a = pStateNow;
		pOSlwToolMatrixDot(&(pDDPG->ActorTargetNet.x), &(mat1), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������
		pDDPG->ActorTargetNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDDPG->ActorTargetNet));//��������


		//�ڶ��� ����ActionNext �� stateNow ���� CrticTarget ���� qtarget=R+GAMMA*CrticTarget.yout

		pOSlwToolMatrixJoin(&(pDDPG->CriticTargetNet.x), &(pDDPG->ActorTargetNet.x), &(pDDPG->ActorTargetNet.y),0x01);//״̬ƴ�Ӷ���
		pDDPG->CriticTargetNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDDPG->CriticTargetNet));//��������
		
		mat1.length = 1;
		mat1.a = &(pRL->Gamma);

		mat2.length = 1;
		mat2.a = pReward;

		pOSlwToolMatrixMPYA(&(pDDPG->CriticMainNet.ref), &(pDDPG->CriticTargetNet.y), &mat1, &mat2);//ref ����


		//������ ����ActionNow��stateLast ���� criticmain �õ�qmain �õ� TD_error ���򴫲�
		mat1.length = pRL->StateNow.length;
		mat1.a = pStateLast;

		pOSlwToolMatrixDot(&(pDDPG->ActorMainNet.x), &(mat1), &(pDDPG->PGradBasic.StateFactor));//���Թ�һ������ �������actor���������

		mat2.length = pRL->ActNow.length;
		mat2.a = pAct;

		pOSlwToolMatrixJoin(&(pDDPG->CriticMainNet.x), &(pDDPG->ActorMainNet.x), &(mat2), 0x01);//״̬ƴ�Ӷ���
	
		pDDPG->CriticMainNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDDPG->CriticMainNet));//��������

		//qbuf = pDDPG->CriticMainNet.y.a[0];

		OSlwToolBPnnCalErr(&(pDDPG->CriticMainNet));//���򴫲����

		/*
		������
		���Ĳ� ֱ�ӽ�-qmain��Ϊactormain����� ���򴫲�

		OSlwToolBPnnRunning(&(pDDPG->ActorMainNet));//����actormain����

		mat3.length = 1;
		mat3.a = &negbuf;
		pTNL = pDDPG->ActorMainNet.Net.pTail->Data.pData;

		

		pOSlwToolMatrixSet(&(pTNL->outErr),qbuf, NULL);//ֱ���������Ϊ-qmain
		
		OSlwToolBPnnTrain(&(pDDPG->ActorMainNet));//���򴫲�
		*/
		
		//���Ĳ� 
		//���� actor���� ���ҽ�A����������ΪC���������
		pDDPG->ActorMainNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDDPG->ActorMainNet));//����actormain����

		pOSlwToolMatrixJoin(&(pDDPG->CriticMainNet.x), &(pDDPG->ActorMainNet.x), &(pDDPG->ActorMainNet.y), 0x01);//״̬ƴ�Ӷ���

		//���岽
		//����C���� ����qmain���� 
		pDDPG->CriticMainNet.Train.mini_batch_now = 1;
		OSlwToolBPnnForward(&(pDDPG->CriticMainNet));

		//������
		//����dq/da ȡ��dq/da??????????????????
		
		//mat3.a=OSlwToolBPnnGradForInput(&(pDDPG->CriticMainNet));//��һ��ԭ���������� ��Ϊȡ����0.96֮��GradForInput����
		LwMatSet(&(pDDPG->CriticMainNet.y), _ParaFrom(1));//�������Ϊ1 ���򴫵ݾ���dy/dx
		OSlwToolBPnnBackward(&(pDDPG->CriticMainNet));
		mat3.a = pDDPG->CriticMainNet.x.a;//�õ�d_critic/d_x
		mat3.length = mat2.length;
		mat3.a += mat1.length;//�����Ի���������

		//���߲�
		//dq/daparam=dq/da*da/daparam
		mat1.length = 1;
		mat1.a = &negbuf;

		
		pOSlwToolMatrixDot(&(pDDPG->ActorMainNet.y), &mat3, &mat1);//�õ������ݶ�

		OSlwToolBPnnCalErr(&(pDDPG->ActorMainNet));//���򴫲�


	}

	//��8�� ������������
	OSlwToolBPnnReview(&(pDDPG->ActorMainNet));
	OSlwToolBPnnReview(&(pDDPG->CriticMainNet));

	//��9�� �����滻

	if (pDDPG->CopyMethod == OSlwToolDDPG_CopyMethod_Hard)
	{
		if ((pDDPG->Copy.HardReplace.Count_Copy >= pDDPG->Copy.HardReplace.NetCopyThreshold))//�ж��Ƿ�������ֵ
		{
			pDDPG->Copy.HardReplace.Count_Copy = 0;
			OSlwToolBPnnCopy(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet));//����������
			OSlwToolBPnnCopy(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet));//����������
		}
		else
		{
			pDDPG->Copy.HardReplace.Count_Copy++;
		}
	}
	else if (pDDPG->CopyMethod == OSlwToolDDPG_CopyMethod_Soft)
	{
		OSlwToolBPnnSoftReplace(&(pDDPG->ActorTargetNet), &(pDDPG->ActorMainNet), pDDPG->Copy.tSoftReplace);
		OSlwToolBPnnSoftReplace(&(pDDPG->CriticTargetNet), &(pDDPG->CriticMainNet), pDDPG->Copy.tSoftReplace);
	}
	else
	{
		OSLW_assert(1);
	}


	return pDDPG;
}
#endif // OSLW_TOOL_IMPORT_ALL || (OSLW_TOOL_IMPORT_RL_DDPG && OSLW_TOOL_IMPORT_NN && OSLW_TOOL_IMPORT_NN_BPnn && OSLW_TOOL_IMPORT_RL_EXP_RE)

//
//
//
//void OSlwToolPPOptimInitial(OSlwToolPPOptimSTU *pPPO)//PPO��ʼ�����������ȱ�����
//{
//
//	OSLW_assert(!(pPPO));
//	memset(pPPO, 0, sizeof(OSlwToolPPOptimSTU));
//
//	OSlwToolBPnnInitial(&(pPPO->CriticNet));
//	OSlwToolBPnnInitial(&(pPPO->ActorNewNet));
//	OSlwToolBPnnInitial(&(pPPO->ActorOldNet));
//
//
//
//	pPPO->StoreMemFun = OSlwToolPPOptimStoreMem;
//	pPPO->PGradBasic.basic.ChooseFun = OSlwToolPPOptimChoose;
//	pPPO->PGradBasic.basic.LearnFun = OSlwToolPPOptimLearning;
//
//	pPPO->PGradBasic.ExpReplay.AppendFun = OSlwToolDQNetExpReplayAppend;
//	pPPO->PGradBasic.ExpReplay.SampleFun = OSlwToolDQNetExpReplaySample;
//	pPPO->PGradBasic.ExpReplay.UpdateFun = OSlwToolDQNetExpReplayUpdate;
//	pPPO->PGradBasic.ExpReplay.MinMaxFun = OSlwToolDQNetExpReplayMinMax;
//
//
//}
//
//void OSlwToolPPOptimParaInitial//PPO�ڴ��ʼ������
//(
//	OSlwToolPPOptimSTU *pPPO,//thisָ��
//	ParaType ClipE,
//	ParaType tau,
//	ParaType gamma,
//	ParaType beta,
//	OSlwToolRandomBasicSTU *prand,//�����������
//	OSlwMemoryBasicSTU *pMem,//�ڴ�ָ��
//	pActEnvFunType pActfun,//�������к���ָ��
//	pBornFunType pBornfun//������ʼ������ָ��
//
//)
//{
//	OSLW_assert(!(pPPO));
//	OSLW_assert(!(prand));
//	OSLW_assert(!(pMem));
//	OSLW_assert(!(pActfun));
//	OSLW_assert(!(pBornfun));
//
//	pPPO->ClipE = ClipE;
//	pPPO->GAE_gamma = gamma;
//	pPPO->GAE_tau = tau;
//	pPPO->beta_entropy = beta;
//
//	pPPO->PGradBasic.basic.pRand = prand;
//	pPPO->PGradBasic.basic.pmem = pMem;
//	pPPO->PGradBasic.basic.ActEnvFun = pActfun;
//	pPPO->PGradBasic.basic.BornFun = OSlwToolRL_Born;
//	pPPO->PGradBasic.basic.BornFunByUser = pBornfun;
//
//	pPPO->PGradBasic.basic.isTermialFun = OSlwToolRL_isTermial;
//
//
//}
//
//
//void OSlwToolPPOptimInterfaceInitial
//(
//	OSlwToolPPOptimSTU *pPPO,//thisָ��
//	lw_u16 state_dim, lw_u16 action_dim,
//	void *pAction,//�����б��ڴ棨��ΪNULL��
//	void *pStateNow,//��ǰ״̬����ΪNULL��
//	void *pStateLast,//֮ǰ״̬����ΪNULL��
//	void *pStateTermial,//�ս�״̬����ΪNULL��
//	void *pStateFactor,//״̬��һ������(��ΪNULL)
//	ParaType ActionMax, ParaType ActionMin
//)
//{
//	OSLW_assert(!(pPPO));
//
//	if (pPPO->PGradBasic.basic.pmem)
//	{
//		if (!pAction)
//		{
//			pAction = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(1 * action_dim));
//		}
//
//		if (!pStateNow)
//		{
//			pStateNow = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateLast)
//		{
//			pStateLast = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateTermial)
//		{
//			pStateTermial = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));
//		}
//
//		if (!pStateFactor)
//		{
//			pStateFactor = pPPO->PGradBasic.basic.pmem->Malloc(pPPO->PGradBasic.basic.pmem, PARA_MEM_CAL(state_dim * 1));//
//			OSlwToolMatrixInitial(&(pPPO->PGradBasic.StateFactor), 1, state_dim, pStateFactor);//
//			pOSlwToolMatrixSet(&(pPPO->PGradBasic.StateFactor), _ParaFint(1), NULL);//��ʼ��Ϊ1
//		}
//	}
//
//	OSlwToolMatrixInitial(&(pPPO->PGradBasic.basic.StateNow), 1, state_dim, pStateNow);
//	OSlwToolMatrixInitial(&(pPPO->PGradBasic.basic.StateLast), 1, state_dim, pStateLast);
//	OSlwToolMatrixInitial(&(pPPO->PGradBasic.basic.StateTermial), 1, state_dim, pStateTermial);
//	OSlwToolMatrixInitial(&(pPPO->PGradBasic.basic.ActNow), 1, action_dim, pAction);
//
//	OSlwToolMatrixInitial(&(pPPO->PGradBasic.StateFactor), 1, state_dim, pStateFactor);
//
//	pPPO->PGradBasic.ActionMax = ActionMax;
//	pPPO->PGradBasic.ActionMin = ActionMin;
//
//}
//
//void OSlwToolPPOptimStoreMemInitial//PPO�ڴ�����ʼ��
//(
//	OSlwToolPPOptimSTU *pPPO,//thisָ��
//	lw_u16 MemPoolLength//���䳤��
//)
//{
//	OSlwMemoryBasicSTU *pmem;
//	lw_u16 col_kind[] = { sizeof(OSlwToolDQNetExpReplayFrameSTU),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType),sizeof(ParaType) };
//	OSLW_assert(!(pPPO));
//	OSLW_assert(!(MemPoolLength));
//
//
//	pPPO->PGradBasic.ExpReplay.pTQL = (OSlwToolQRLBasicSTU *)pPPO;
//	pPPO->PGradBasic.ExpReplay.PriorType = OSlwToolDQNetExpReplay_NoP;
//
//
//	//�ڴ����
//	pmem = pPPO->PGradBasic.basic.pmem;
//
//	//������װ
//	pPPO->PGradBasic.ExpReplay.DivResult.uData = 1;
//	pPPO->PGradBasic.ExpReplay.SampleResult.uData = MemPoolLength;
//
//	//����ֵ�ط� ��ʼ��
//	pPPO->PGradBasic.ExpReplay.State_Col = pPPO->PGradBasic.basic.StateNow.length;
//	pPPO->PGradBasic.ExpReplay.Action_Col = pPPO->PGradBasic.basic.ActNow.length;
//
//	//OSlwTable ��ʼ�� ����׼��
//	col_kind[1] *= pPPO->PGradBasic.ExpReplay.State_Col;
//	col_kind[2] *= pPPO->PGradBasic.ExpReplay.Action_Col;
//	col_kind[4] *= pPPO->PGradBasic.ExpReplay.State_Col;
//
//	OSlwToolTableInitial(&(pPPO->PGradBasic.ExpReplay.MemTable), MemPoolLength, col_kind, 5, pmem, NULL, NULL);
//
//	pPPO->PGradBasic.ExpReplay.SampleResult.pData = pmem->Malloc(pmem, 2 * pPPO->PGradBasic.ExpReplay.SampleResult.uData * sizeof(void *));
//
//
//}


void OSlwToolPPOptimAppendInitial//PPO������׷��,һ�λ�׷������������
(
	OSlwToolPPOptimSTU *pPPO,//thisָ��
	OSlwToolACtoNet Kind,
	lw_u16 layer_num,
	lw_u16 *layer_list,
	OSlwToolNNLayerActFunSTU **AFlist,//�����
	ParaType *nl_list//ѧϰ����
)
{
	//lw_u16 i;
	//lw_u16 row, col;
	//OSlwMemoryBasicSTU *pmem;
	//OSlwToolRandomBasicSTU *pTRB;
	//OSlwToolNNLayerFullConSTU *p1, *p2;
	//OSlwToolBPnnSTU *mn, *tn;

	//OSLW_assert(!(pPPO));
	//OSLW_assert(!(layer_list));
	//OSLW_assert(!(AFlist));
	//OSLW_assert(!(nl_list));

	//pmem = (pPPO->PGradBasic.basic.pmem);
	//pTRB = pPPO->PGradBasic.basic.pRand;

	//if (Kind == OSlwToolACtoActor)
	//{
	//	row = pPPO->PGradBasic.basic.StateNow.length;//ֱ���������
	//	layer_list[layer_num - 1] = pPPO->PGradBasic.basic.ActNow.length * 2;//ͬʱ��������ľ�ֵ�뷽��

	//	mn = &(pPPO->ActorNewNet);
	//	tn = &(pPPO->ActorOldNet);


	//	for (i = 0; i < layer_num; i++)
	//	{
	//		col = layer_list[i];

	//		p1 = OSlwToolBPnnLayerAppend_Dynamic(
	//			mn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);

	//		p2 = OSlwToolBPnnLayerAppend_Dynamic(
	//			tn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);

	//		pOSlwToolMatrixSet(&(p2->w), 0, &(p1->w));
	//		pOSlwToolMatrixSet(&(p2->BiasUN.bias), 0, &(p1->BiasUN.bias));

	//		row = col;

	//	}

	//}
	//else if (Kind == OSlwToolACtoCritic)
	//{
	//	row = pPPO->PGradBasic.basic.StateNow.length;
	//	layer_list[layer_num - 1] = 1;

	//	mn = &(pPPO->CriticNet);

	//	for (i = 0; i < layer_num; i++)
	//	{
	//		col = layer_list[i];

	//		p1 = OSlwToolBPnnLayerAppend_Dynamic(
	//			mn
	//			, AFlist[i]
	//			, row, col
	//			, pTRB
	//			, _ParaFrom(1), _ParaFrom(-1)
	//			, nl_list[i]
	//			, NULL, NULL, pmem
	//			, _ParaFrom(0)
	//		);
	//		row = col;

	//	}

	//}
	//else
	//{
	//	OSLW_assert(1);
	//}

	
}


OSlwToolPPOptimSTU* OSlwToolPPOptimChoose(OSlwToolPPOptimSTU *pPPO)
{


	return NULL;
}
OSlwToolPPOptimSTU* OSlwToolPPOptimStoreMem(OSlwToolPPOptimSTU *pPPO)
{


	return NULL;
}
OSlwToolPPOptimSTU* OSlwToolPPOptimLearning(OSlwToolPPOptimSTU *pPPO)
{



	return NULL;
}




#endif //OSLW_TOOL_IMPORT_RL || OSLW_TOOL_IMPORT_ALL

#endif // !(OSLW_SIMPLE_LEVEL >= 2)

