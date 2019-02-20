/*
 * OSLW_define.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_DEFINE_H_
#define OSLW_DEFINE_H_


#include <setjmp.h>

typedef char lw_8;
typedef unsigned char lw_u8;
typedef int lw_16;
typedef unsigned int lw_u16;


typedef long lw_32;
typedef unsigned long lw_u32;
typedef long long lw_64;
typedef unsigned long long lw_u64;

typedef float lw_sf;
typedef double lw_df;

#if !defined(DEBUG)
#define OSLW_assert(_ignore) ((void)0)
#else
#define OSLW_assert(_expr)   if(!(_expr)) { return;}
#endif

#define SAVE(A) (setjmp((A)))
#define LOAD(A,B) (longjmp((A),(B)))
typedef jmp_buf _BP;

#ifndef NULL
	#define NULL 0
#endif

//parameter define
typedef lw_32 fixpoint;

#define OSLW_GLOBAL_MATH_Q 1
#define OSLW_GLOBAL_MATH_FLOAT 2
#define OSLW_GLOBAL_MATH_DOUBLE 3
#define OSLW_GLOBAL_MATH_TYPE OSLW_GLOBAL_MATH_FLOAT

#define   FP_A_TO_FP_B(A,B,X)  ((A >= B) ? ((fixpoint) X >> (A - B)):((fixpoint) X << (B - A)))
#define OSLW_GLOBAL_MATH_Q_FORM 15

//task define
#define TASK_PRIVATE_MEMORY_SECTION_LENGTH 2
#define OSLW_TASK_BREAKPOINT_DEEP 3

#define OSLW_TASK_NAME_LENGTH_MAX 4

#if OSLW_TASK_NAME_LENGTH_MAX<3
#error "LENGTH OF TASK NAME IS TOO SHORT"
#endif

//core define
#define OSLW_TASK_NUM_MAX 64



#endif /* OSLW_DEFINE_H_ */


