/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_tool.h
 *
 *  Created on: 2017-11-13
 *      Author: slw
 */

#ifndef OSLW_TOOL_RANDOM_H_
#define OSLW_TOOL_RANDOM_H_


#include "OSLW_define.h"
#include "OSLW_parameter.h"
#include "OSLW_tool_basic.h"
#if !(OSLW_SIMPLE_LEVEL >= 3)

typedef struct OSLW_TOOL_RANDOM_BASIC_STRUCT{

	ParaType Rmin, Rmax;

	ParaType(*rand)(void *pRand, ParaType Rmin, ParaType Rmax);
	lw_32(*randint)(void *pRand, ParaType Rmin, ParaType Rmax);
	ParaType(*randn)(void *pRand, ParaType Cent, ParaType Var);
	void(*Seed)(void *pRand, lw_u32 seed);
#if OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM
	ParaType _normal_last;
	lw_u8 _normal_gen_flag;

#elif OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_LLN

#elif OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_MP
	ParaType _normal_last;
	lw_u8 _normal_gen_flag;
#else
	
#endif // OSLW_TOOL_RAND_NORM_METHOD == OSLW_TOOL_RAND_NORM_BM

}OSlwToolRandomBasicSTU;
ParaType OSlwToolRandomNormal(OSlwToolRandomBasicSTU *pRB, ParaType Cent, ParaType Var);


typedef struct OSLW_TOOL_RANDOM_CHAOS_LOGISTIC_STRUCT{
	OSlwToolRandomBasicSTU basic;	
	
	lw_df u,x;	
}OSlwToolRandomChaosLogisticSTU;

extern OSlwToolRandomChaosLogisticSTU OSlwTRCLog;
extern OSlwToolRandomBasicSTU *pRCLog;
ParaType OSlwToolRandomChaosLogisticFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 OSlwToolRandomChaosLogisticFunInt(void *pRand,ParaType Rmin,ParaType Rmax);


typedef struct OSLW_TOOL_RANDOM_CHAOS_TENT_STRUCT{
	OSlwToolRandomBasicSTU basic;	
	
	lw_df a,x;
}OSlwToolRandomChaosTentSTU;

extern OSlwToolRandomChaosTentSTU OSlwTRCTent;
extern OSlwToolRandomBasicSTU *pRCTent;
ParaType OSlwToolRandomChaosTentFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 OSlwToolRandomChaosTentFunInt(void *pRand,ParaType Rmin,ParaType Rmax);


typedef struct OSLW_TOOL_RANDOM_CHAOS_KENT_STRUCT{
	OSlwToolRandomBasicSTU basic;	
	
	lw_df a,x;
}OSlwToolRandomChaosKentSTU;

extern OSlwToolRandomChaosKentSTU OSlwTRCKent;
extern OSlwToolRandomBasicSTU *pRCKent;
ParaType OSlwToolRandomChaosKentFun(void *pRand,ParaType Rmin,ParaType Rmax);
lw_32 OSlwToolRandomChaosKentFunInt(void *pRand,ParaType Rmin,ParaType Rmax);




typedef struct OSLW_TOOL_RANDOM_LCG_STRUCT {
	OSlwToolRandomBasicSTU basic;
	lw_u32 rand_x;
	lw_u32 rand_a;
	lw_u32 rand_c;
	lw_u32 rand_mod;
}OSlwToolRandomLCGSTU;

extern OSlwToolRandomLCGSTU OSlwTRLcg;
extern OSlwToolRandomBasicSTU *pRLcg;

ParaType OSlwToolRandomLCGFun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 OSlwToolRandomLCGFunInt(void *pRand, ParaType Rmin, ParaType Rmax);

typedef struct OSLW_TOOL_RANDOM_WELL512_STRUCT {
	OSlwToolRandomBasicSTU basic;
	lw_u32 _state[16];
	lw_u32 _index;
}OSlwToolRandomWELL512STU;


ParaType OSlwToolRandomWELL512Fun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 OSlwToolRandomWELL512FunInt(void *pRand, ParaType Rmin, ParaType Rmax);
void OSlwToolRandomWELL512Seed(OSlwToolRandomWELL512STU *pRand, lw_u32 seed);



typedef struct OSLW_TOOL_RANDOM_MT_STRUCT {
	OSlwToolRandomBasicSTU basic;
	lw_u32 _mt[624];
	lw_u16 _index;
}OSlwToolRandomMTSTU;


ParaType OSlwToolRandomMTFun(void *pRand, ParaType Rmin, ParaType Rmax);
lw_32 OSlwToolRandomMTFunInt(void *pRand, ParaType Rmin, ParaType Rmax);
void OSlwToolRandomMTSeed(OSlwToolRandomMTSTU *pRand, lw_u32 seed);


void OSlwToolMatrixRandomInitial(OSlwToolMatrixSTU *m,void *pRand,ParaType Rmin,ParaType Rmax);
void *OSlwToolMatrixToRandnChip(OSlwToolMatrixSTU *s, OSlwToolMatrixSTU *m, OSlwToolRandomBasicSTU *pRand, ParaType var, ParaType Rmin, ParaType Rmax);


#endif // !(OSLW_SIMPLE_LEVEL >= 3)

#endif

