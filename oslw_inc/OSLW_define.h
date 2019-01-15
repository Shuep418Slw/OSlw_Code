/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_define.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_DEFINE_H_
#define OSLW_DEFINE_H_


#include <setjmp.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef char lw_8;
typedef unsigned char lw_u8;
typedef int16_t lw_16;
typedef uint16_t lw_u16;


typedef int32_t lw_32;
typedef uint32_t lw_u32;
typedef int64_t lw_64;
typedef uint64_t lw_u64;

typedef float lw_sf;
typedef double lw_df;

typedef intptr_t lw_ptr;

//--------------------------------
//<重要宏定义>
//--------------------------------

//操作系统使能
#define OSLW_OS_ENABLE 1

//操作系统时间片
#define OSLW_CORE_TICK_MS 1

//操作系统精简等级
#define OSLW_SIMPLE_LEVEL 1

//操作系统高速运行
#define OSLW_SPEED_RUNNING 0


//精简模式
#if OSLW_SIMPLE_LEVEL
	#define OSLW_SIMPLE_MODE 1
#else
	#define OSLW_SIMPLE_MODE 0
#endif // OSLW_SIMPLE_LEVEL

//CPU任务数量
#define OSLW_TASK_NUM_MAX 8

#define DEBUG
#if !defined(DEBUG)
#define OSLW_assert(is_error) ((void)0)
#else
#define OSLW_assert(is_error)   if((is_error)) {while(1);}
#endif

#if OSLW_SIMPLE_MODE
#define _OSLW_SAVE(A) (setjmp((A)))
#define _OSLW_LOAD(A,B,PTA) ((PTA)->TaskFun(PTA))
typedef lw_u16 _BP;
#else
#define _OSLW_SAVE(A) (setjmp((A)))
#define _OSLW_LOAD(A,B,PTA) (longjmp((A),(B)))
typedef jmp_buf _BP;
#endif




//--------------------------------
//</重要宏定义>
//--------------------------------


#ifndef NULL
	#define NULL ((void*)0)
#endif

#ifndef PI
	#define PI 3.14159265358979f
#endif


//parameter define
typedef lw_32 fixpoint;

#define OSLW_GLOBAL_MATH_Q 1
#define OSLW_GLOBAL_MATH_FLOAT 2
#define OSLW_GLOBAL_MATH_DOUBLE 3
#define OSLW_GLOBAL_MATH_TYPE OSLW_GLOBAL_MATH_FLOAT

#define   FP_A_TO_FP_B(A,B,X)  ((A >= B) ? ((fixpoint) X >> (A - B)):((fixpoint) X << (B - A)))
#define OSLW_GLOBAL_MATH_Q_FORM 15
#define OSLW_PARA_NAME_LENGTH 5

#define OSLW_PARA_NORMAL_LENGTH 2

#define OSLW_PARA_LIST_LENGRH 2

#define OSLW_PAPA_CTRL_NAME_LENGTH 5

typedef enum{

	ParaUnitStatus_Free=0x00,
	ParaUnitStatus_Lock=0x01,
	ParaUnitStatus_Protect=0x02
	
	

}ParaUnitStatusNUM;

typedef enum{
	
	ParaOrder_IO_Prec=0,
	ParaOrder_IO_Normal=1,
	ParaOrder_IO_Data=2,
	
	ParaOrder_I_SelfUp,
	ParaOrder_I_SelfDown,
	ParaOrder_O_Draw,
	
	ParaOrder_I_UnlockSave,
	ParaOrder_I_UnlockNSave,
	ParaOrder_I_lock
}OSlwParaOrderNUM;

typedef enum{
	
	ParaLOrder_Once=0,
	ParaLOrder_More=1,
	
}OSlwParaListOrderNUM;


//<宏组说明>数据类型定义 定义参数所用的类型</宏组说明>
#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	 typedef fixpoint ParaType;
	 #include "IQmathLib.h"
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_FLOAT
	 typedef float ParaType;
		//#include "arm_math.h"
		#include "math.h"
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_DOUBLE
	 typedef double ParaType;
		#include "math.h"
#else
	#error "MATH TYPE IS NOT DEFINED"
#endif
//<宏组说明>数据类型定义 定义参数所用的算法</宏组说明>
#if OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_Q
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMpy(A,B) (_IQmpy(A,B))
	#define _ParaDiv(A,B) (_IQdiv(A,B))
	#define _ParaInt(A) (_IQint(A))
	#define _ParaFrac(A) (_IQfrac(A))
	#define _ParaFint(A) (((ParaType)(A))<<OSLW_GLOBAL_MATH_Q_FORM)
	#define _ParaToF(A) ((float)((double)(A)/(float)(1<<OSLW_GLOBAL_MATH_Q_FORM)))
	#define _ParaFrom(A) (_IQ(A))
	#define _ParaSin(A) (_IQsin(A))
	#define _ParaCos(A) (_IQcos(A))
	#define _ParaTan(A) (_IQtan(A))	
	#define _ParaSqrt(A) (_IQsqrt(A))		
	#define _ParaAbs(A) (_IQabs(A))
	#define PARA_LEN 4
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_FLOAT
/*(Ver.=0.9~)(Beg.=0.9)
	#define _ParaAdd(A,B) ((lw_sf)(A)+(lw_sf)(B))
	#define _ParaSub(A,B) ((lw_sf)(A)-(lw_sf)(B))
	#define _ParaMpy(A,B) ((lw_sf)(A)*(lw_sf)(B))
	#define _ParaDiv(A,B) ((lw_sf)(A)/(lw_sf)(B))
	#define _ParaInt(A) ((long)(A))
	#define _ParaFrac(A) ((A)-long(A))
	#define _ParaFint(A)	((lw_sf)A)
	#define _ParaToF(A) ((lw_sf)(A))
	#define _ParaFrom(A) ((lw_sf)A)
	#define _ParaSin(A) (arm_sin_f32(A))
	#define _ParaCos(A) (arm_cos_f32(A))
	#define _ParaTan(A) (arm_sin_f32(A)/arm_cos_f32(A))		
	#define _ParaSqrt(A) (__sqrtf(A))
	#define _ParaAbs(A) (A > 0.0f?A:-A)		
	#define PARA_LEN 4
	*/
	#define _ParaAdd(A,B) ((lw_sf)(A)+(lw_sf)(B))
	#define _ParaSub(A,B) ((lw_sf)(A)-(lw_sf)(B))
	#define _ParaMpy(A,B) ((lw_sf)(A)*(lw_sf)(B))
	#define _ParaDiv(A,B) ((lw_sf)(A)/(lw_sf)(B))
	#define _ParaInt(A) ((long)(A))
	#define _ParaFrac(A) ((A)-long(A))
	#define _ParaFint(A)	((lw_sf)A)
	#define _ParaToF(A) ((lw_sf)(A))
	#define _ParaFrom(A) ((lw_sf)A)
	#define _ParaSin(A) (sinf(A))
	#define _ParaCos(A) (cosf(A))
	#define _ParaTan(A) (sinf(A)/cosf(A))		
	#define _ParaSqrt(A) (sqrtf(A))
	#define _ParaExp(A) (expf(A))
	#define _ParaAbs(A) (A > 0.0f?A:-A)		
	#define _ParaPow(A,B) (powf((A),(B)))
	#define _ParaCeil(A) (ceilf((A)))
	#define _ParaLn(A) (logf((A)))
	#define _ParaLog(A) (logf10((A)))
	#define PARA_LEN 4
#elif OSLW_GLOBAL_MATH_TYPE==OSLW_GLOBAL_MATH_DOUBLE
	#define _ParaAdd(A,B) ((A)+(B))
	#define _ParaSub(A,B) ((A)-(B))
	#define _ParaMpy(A,B) ((A)*(B))
	#define _ParaDiv(A,B) ((A)/(B))
	#define _ParaInt(A) ((long)(A))
	#define _ParaFrac(A) ((A)-long(A))
	#define _ParaFint(A)	((lw_df)(A))
	#define _ParaToF(A) ((lw_df)(A))	
	#define _ParaFrom(A)	((lw_df)A)	
	#define _ParaSin(A) (sin(A))
	#define _ParaCos(A) (cos(A))
	#define _ParaTan(A) (tan(A))	
	#define _ParaSqrt(A) (sqrt(A))
	#define _ParaExp(A) (exp(A))
	#define _ParaAbs(A) (A > 0.0f?A:-A)
	#define _ParaPow(A,B) (pow((A),(B)))
	#define _ParaCeil(A) (ceil((A)))
	#define _ParaLn(A) (log((A)))
	#define _ParaLog(A) (log10((A)))
	#define PARA_LEN 8
#else
	#error "MATH TYPE IS NOT DEFINED"
#endif

#if PARA_LEN == 8
	#define PARA_MEM_CAL(D) ((lw_u32)(D)*sizeof(ParaType))
#elif PARA_LEN == 4
	#define PARA_MEM_CAL(D) ((lw_u32)(D)*sizeof(ParaType))	
#endif // PARA_LEN == 8


//task define
#define TASK_PRIVATE_MEMORY_SECTION_LENGTH 2

#define OSLW_TASK_BREAKPOINT_DEEP 1

	
#if OSLW_SIMPLE_MODE
#undef OSLW_TASK_BREAKPOINT_DEEP
#define OSLW_TASK_BREAKPOINT_DEEP 1
#endif	
	
#define OSLW_TASK_NAME_LENGTH_MAX 4

#if OSLW_TASK_NAME_LENGTH_MAX<3
#error "LENGTH OF TASK NAME IS TOO SHORT"
#endif

typedef struct OSLW_EXTERN_MEMORY_STRUCT{
	void *pData;
	lw_u32 uData;
}OSlwExternMemorySTU;


//core define
#define OSLW_CORE_PARA_PAGE_NUM 2

#if OSLW_CORE_PARA_PAGE_NUM <2
	#error "parameter num is to little"
#endif

#if OSLW_SIMPLE_LEVEL >= 3
#define OSLW_CPU_USAGE_MAX ((lw_u8)(200))
#else
#define OSLW_CPU_USAGE_MAX ((lw_u16)(1000))
#endif // OSLW_SIMPLE_LEVEL >= 3


//gift define

#define OSLW_GIFT_EN 1

#define OSLW_GIFT_UNIT_STR_LENGTH 8

#define OSLW_GIFT_UNIT_MAX 3

#define OSLW_GIFT_POSTMAN_MAX 2

#if OSLW_GIFT_POSTMAN_MAX > OSLW_TASK_NUM_MAX
	#error "postman is too much"
#endif

//communication
#define OSLW_TASK_COMMUN_ENABLE 0

#define OSLW_COMMUN_TX_LENGTH 20

#define OSLW_COMMUN_RX_LENGTH 8

#define	OSLW_GIFT_COMMUN_RXD_LENGTH 8

#define OSLW_COMMUN_ADDRESS 1

#define OSLW_COMMUN_READY_LEN_LIST_LENGTH 3


//show deifne
#define OSLW_TASK_SHOW_ENABLE 0

#define OSLW_SHOW_NO_KEEP 1
#define OSLW_SHOW_HAVE_KEEP 2

#define OSLW_SHOW_SCREEN OSLW_SHOW_NO_KEEP

#define OSLW_SHOW_WIN_LENGTH 3

#define OSLW_SHOW_MOVE_CURSOR_FREE 1
#define OSLW_SHOW_MOVE_WIN_FREE 2

#define OSLW_SHOW_MOVE_METHOD OSLW_SHOW_MOVE_WIN_FREE

#define OSLW_SHOW_MOVE_CURSOR_POS (OSLW_SHOW_WIN_LENGTH-1)/2

#if OSLW_SHOW_MOVE_CURSOR_POS >= OSLW_SHOW_WIN_LENGTH
	#error "Curosr position is over"
#endif

#define OSLW_SHOW_BUFFER_LEN 10

#define OSLW_SHOW_PAGE_LIST_LEN 3

#define OSLW_SHOW_CMD_UP 18
#define OSLW_SHOW_CMD_DOWN 17
#define OSLW_SHOW_CMD_TURN 12
#define OSLW_SHOW_CMD_LOCK 15
#define OSLW_SHOW_CMD_CAN 24
#define OSLW_SHOW_CMD_NEXT '\n'
//timer define
#define OSLW_TIMER_CTRL_LIST_LEN 5


#if OSLW_SIMPLE_LEVEL >= 3 && OSLW_TIMER_CTRL_LIST_LEN > 3

#undef OSLW_TIMER_CTRL_LIST_LEN
#define OSLW_TIMER_CTRL_LIST_LEN 3

#endif

//memory define
#define OSLW_MEMORY_EN 1

//IMPORT MAP MALLOC
#define OSLW_MEMORY_IMPORT_MAP 1
//IMPORT SIMPLE MALLOC
#define OSLW_MEMORY_IMPORT_SIMPLE 1
//IMPORT LIST MALLOC
#define OSLW_MEMORY_IMPORT_LIST 1

#define OSLW_MEM_ALLOC_FAST 0

typedef struct OSLW_MEM_SIZE_STRUCT {
#if OSLW_MEM_ALLOC_FAST
	lw_u32 ModNum;
	lw_u16 MovNum;
#endif
	lw_u16 SectionSize;

}OSlwMemSizeSTU;


//tool define
typedef struct OSLW_TOOL_DLIST_NODE_CONNECT_STRUCT {
	struct OSLW_TOOL_DLIST_NODE_CONNECT_STRUCT *pLast, *pNext;
}OSlwToolDListNodeConnectSTU;


//String Debug Define
//是否使能StringDebug
#define OSLW_STR_DBG_EN 1

#define OSLW_STR_DBG_SIMPLE_MODE 0

//StringDebug 输出字符串大小
#define OSLW_STR_DBG_OUT_LEN 100
//StringDebug 名字字符串大小
#define OSLW_STR_DBG_NAME_LEN 10
//StringDebug 数值字符串大小
#define OSLW_STR_DBG_DATA_LEN 90

//StringDebug 内存大小
#define OSLW_STR_DBG_MEM_SIZE 20
#define OSLW_STR_DBG_MEM_LEN 100

//StringDebug 哈希表大小
#define OSLW_STR_DBG_HASH_LEN 8

//2号精简模式下DATA的数量
#define OSLW_STR_DBG_S2_DATA_LEN 20


#if OSLW_TASK_NUM_MAX<=8
	#define __OSLW_LOG2_TASK_NUM 3
	typedef lw_u8 OSlwGroupAllType;
#elif OSLW_TASK_NUM_MAX>8 && OSLW_TASK_NUM_MAX<=16
	#define __OSLW_LOG2_TASK_NUM 4
	typedef lw_u16 OSlwGroupAllType;
#elif OSLW_TASK_NUM_MAX>16 && OSLW_TASK_NUM_MAX<=32
	#define __OSLW_LOG2_TASK_NUM 5
	typedef lw_u32 OSlwGroupAllType;
#elif OSLW_TASK_NUM_MAX>32 && OSLW_TASK_NUM_MAX<=64
	#define __OSLW_LOG2_TASK_NUM 6
	typedef lw_u64 OSlwGroupAllType;
#else
	#error "TASK NUM IS OVER"
#endif


#if OSLW_SIMPLE_LEVEL == 2 && OSLW_TASK_NUM_MAX>32
	#error "TASK NUM IS OVER PLEASE DEFINE OSLW_SIMPLE_LEVEL OR OSLW_TASK_NUM_MAX AGAIN"
#endif // OSLW_SIMPLE_LEVEL == 2 && OSLW_TASK_NUM_MAX>32

#if OSLW_SIMPLE_LEVEL ==3 && OSLW_TASK_NUM_MAX>16
	#error "TASK NUM IS OVER PLEASE DEFINE OSLW_SIMPLE_LEVEL OR OSLW_TASK_NUM_MAX AGAIN"
#endif // OSLW_SIMPLE_LEVEL ==3 && OSLW_TASK_NUM_MAX>16


#if OSLW_SIMPLE_LEVEL ==4 && OSLW_TASK_NUM_MAX>8
	#error "TASK NUM IS OVER PLEASE DEFINE OSLW_SIMPLE_LEVEL OR OSLW_TASK_NUM_MAX AGAIN"
#endif // OSLW_SIMPLE_LEVEL ==4 && OSLW_TASK_NUM_MAX>8


#define _OSLW_2_POW(NUM) ((OSlwGroupAllType)1<<NUM)
#define _OSLW_IS_2_POW(n) ((n)==((n)&~(n)+1))
//------------------------------------------
//<共用体名>OSlwCoreGroupUN</共用体名>
//<共用体功能>内核标志组共用体 根据OSLW_TASK_NUM_MAX确定长度</共用体功能>
//------------------------------------------
typedef union OSLW_CORE_GROUP_UNION{
	OSlwGroupAllType all;
#if OSLW_TASK_NUM_MAX<=8
	struct{
		lw_u8 bit0:1;
		lw_u8 bit1:1;
		lw_u8 bit2:1;
		lw_u8 bit3:1;
		lw_u8 bit4:1;
		lw_u8 bit5:1;
		lw_u8 bit6:1;
		lw_u8 bit7:1;
	}bits;
#elif OSLW_TASK_NUM_MAX>8 && OSLW_TASK_NUM_MAX<=16
	struct{
		lw_u16 bit0:1;
		lw_u16 bit1:1;
		lw_u16 bit2:1;
		lw_u16 bit3:1;
		lw_u16 bit4:1;
		lw_u16 bit5:1;
		lw_u16 bit6:1;
		lw_u16 bit7:1;
		lw_u16 bit8:1;
		lw_u16 bit9:1;
		lw_u16 bit10:1;
		lw_u16 bit11:1;
		lw_u16 bit12:1;
		lw_u16 bit13:1;
		lw_u16 bit14:1;
		lw_u16 bit15:1;
	}bits;
#elif OSLW_TASK_NUM_MAX>16 && OSLW_TASK_NUM_MAX<=32
	struct{
		lw_u32 bit0 : 1;
		lw_u32 bit1 : 1;
		lw_u32 bit2 : 1;
		lw_u32 bit3 : 1;
		lw_u32 bit4 : 1;
		lw_u32 bit5 : 1;
		lw_u32 bit6 : 1;
		lw_u32 bit7 : 1;
		lw_u32 bit8 : 1;
		lw_u32 bit9 : 1;
		lw_u32 bit10 : 1;
		lw_u32 bit11 : 1;
		lw_u32 bit12 : 1;
		lw_u32 bit13 : 1;
		lw_u32 bit14 : 1;
		lw_u32 bit15 : 1;
		lw_u32 bit16 : 1;
		lw_u32 bit17 : 1;
		lw_u32 bit18 : 1;
		lw_u32 bit19 : 1;
		lw_u32 bit20 : 1;
		lw_u32 bit21 : 1;
		lw_u32 bit22 : 1;
		lw_u32 bit23 : 1;
		lw_u32 bit24 : 1;
		lw_u32 bit25 : 1;
		lw_u32 bit26 : 1;
		lw_u32 bit27 : 1;
		lw_u32 bit28 : 1;
		lw_u32 bit29 : 1;
		lw_u32 bit30 : 1;
		lw_u32 bit31 : 1;
	}bits;
#elif OSLW_TASK_NUM_MAX>32 && OSLW_TASK_NUM_MAX<=64
	struct{
		lw_u64 bit0 : 1;
		lw_u64 bit1 : 1;
		lw_u64 bit2 : 1;
		lw_u64 bit3 : 1;
		lw_u64 bit4 : 1;
		lw_u64 bit5 : 1;
		lw_u64 bit6 : 1;
		lw_u64 bit7 : 1;
		lw_u64 bit8 : 1;
		lw_u64 bit9 : 1;
		lw_u64 bit10 : 1;
		lw_u64 bit11 : 1;
		lw_u64 bit12 : 1;
		lw_u64 bit13 : 1;
		lw_u64 bit14 : 1;
		lw_u64 bit15 : 1;
		lw_u64 bit16 : 1;
		lw_u64 bit17 : 1;
		lw_u64 bit18 : 1;
		lw_u64 bit19 : 1;
		lw_u64 bit20 : 1;
		lw_u64 bit21 : 1;
		lw_u64 bit22 : 1;
		lw_u64 bit23 : 1;
		lw_u64 bit24 : 1;
		lw_u64 bit25 : 1;
		lw_u64 bit26 : 1;
		lw_u64 bit27 : 1;
		lw_u64 bit28 : 1;
		lw_u64 bit29 : 1;
		lw_u64 bit30 : 1;
		lw_u64 bit31 : 1;
		lw_u64 bit32 : 1;
		lw_u64 bit33 : 1;
		lw_u64 bit34 : 1;
		lw_u64 bit35 : 1;
		lw_u64 bit36 : 1;
		lw_u64 bit37 : 1;
		lw_u64 bit38 : 1;
		lw_u64 bit39 : 1;
		lw_u64 bit40 : 1;
		lw_u64 bit41 : 1;
		lw_u64 bit42 : 1;
		lw_u64 bit43 : 1;
		lw_u64 bit44 : 1;
		lw_u64 bit45 : 1;
		lw_u64 bit46 : 1;
		lw_u64 bit47 : 1;
		lw_u64 bit48 : 1;
		lw_u64 bit49 : 1;
		lw_u64 bit50 : 1;
		lw_u64 bit51 : 1;
		lw_u64 bit52 : 1;
		lw_u64 bit53 : 1;
		lw_u64 bit54 : 1;
		lw_u64 bit55 : 1;
		lw_u64 bit56 : 1;
		lw_u64 bit57 : 1;
		lw_u64 bit58 : 1;
		lw_u64 bit59 : 1;
		lw_u64 bit60 : 1;
		lw_u64 bit61 : 1;
		lw_u64 bit62 : 1;
		lw_u64 bit63 : 1;
	}bits;
#else
	#error "TASK NUM IS OVER"
#endif
}OSlwCoreGroupUN;


#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_DEFINE_H_ */


