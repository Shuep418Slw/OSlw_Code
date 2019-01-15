/*(Ver.=0.9~)(Beg.=0.9)
 * OSLW_memory.h
 *
 *  Created on: 2017-7-14
 *      Author: slw
 */

#ifndef OSLW_MEMORY_H_
#define OSLW_MEMORY_H_

#include "OSLW_define.h"
#include <stdlib.h>

#if OSLW_MEM_ALLOC_FAST
	#define OSLW_MEM_SIZE_MPY(LEN,MS) (((lw_u32)LEN) << (MS).MovNum )
	#define OSLW_MEM_SIZE_DIV(LEN,MS) (((lw_u32)LEN) >> (MS).MovNum)
	#define OSLW_MEM_SIZE_MOD(LEN,MS) (((lw_u32)LEN) &  (MS).ModNum)
#else
	#define OSLW_MEM_SIZE_MPY(LEN,MS) (((MS).SectionSize) * (LEN))
	#define OSLW_MEM_SIZE_DIV(LEN,MS) (((LEN) / ((MS).SectionSize)))
	#define OSLW_MEM_SIZE_MOD(LEN,MS) ((LEN) % ((MS).SectionSize))
#endif // OSLW_MEM_ALLOC_FAST


typedef struct OSLW_MEMORY_BASIC_STRUCT {

	lw_32(*init)
		(
			struct OSLW_MEMORY_BASIC_STRUCT *pMU,//存储器结构体
			void *pMem,//内存池地址
			lw_u16 lenMem,//内存片数量
			lw_u16 MemSize,//内存片大小(sizeof)
			lw_u32 sizeofmem//sizeof(内存区域)的大小用来，验证内存大小是否对，一般sizeofmem>=计算得到的内存大小
			);

	void * (*Malloc)(struct OSLW_MEMORY_BASIC_STRUCT *pMU, lw_u32 dsize);
	void * (*ReAlloc)(struct OSLW_MEMORY_BASIC_STRUCT *pMU, void *p, lw_u32 dsize);
	void * (*Free)(struct OSLW_MEMORY_BASIC_STRUCT *pMU, void *p);

	OSlwMemSizeSTU MemSectionSize;

	//相对概念 对于不同的分配方式概念不同
	lw_u32 memMax;//内存总大小
	lw_u32 memSurplus;//剩余内存数量

}OSlwMemoryBasicSTU;

typedef OSlwMemoryBasicSTU *lw_mem;



#if OSLW_MEMORY_IMPORT_MAP

typedef struct OSLW_MEMORY_MAP_STRUCT{

	OSlwMemoryBasicSTU basic;

	OSlwExternMemorySTU Mem;
	
	OSlwExternMemorySTU MemInfo;
	
	void *pMemButtom,*pMemInfoButtom;//底部
		
}OSlwMemoryMapSTU;

void * OSlwMemoryMapAlloc(OSlwMemoryMapSTU *pMU,lw_u32 dsize);
void * OSlwMemoryMapFree(OSlwMemoryMapSTU *pMU,void *p);
void* OSlwMemoryMapReAlloc(OSlwMemoryMapSTU *pMU, void *p, lw_u32 dsize);
lw_32 OSlwMemoryMapInital(OSlwMemoryMapSTU *pMU,
	void *pMem,
	lw_u16 lenMem,	
	lw_u16 MemSize,
	lw_u32 sizeofmem
	);
#endif //OSLW_MEMORY_IMPORT_MAP


	
#if OSLW_MEMORY_IMPORT_SIMPLE	
	
typedef struct OSLW_MEMORY_SIMPLE_STRUCT {

	OSlwMemoryBasicSTU basic;

	OSlwExternMemorySTU Mem;

	size_t pMemNow,pMemButtom;//底部

}OSlwMemorySimpleSTU;


void * OSlwMemorySimpleAlloc(OSlwMemorySimpleSTU *pMU, lw_u32 dsize);
void * OSlwMemorySimpleFree(OSlwMemorySimpleSTU *pMU, void *p);
void* OSlwMemorySimpleReAlloc(OSlwMemorySimpleSTU *pMU, void *p, lw_u32 dsize);
lw_32 OSlwMemorySimpleInital(OSlwMemorySimpleSTU *pMU,
	void *pMem,
	lw_u16 lenMem,
	lw_u16 MemSize,
	lw_u32 sizeofmem
);

#endif //OSLW_MEMORY_IMPORT_SIMPLE	

	
#if OSLW_MEMORY_IMPORT_LIST	
	
typedef struct OSLW_MEMORY_LIST_NODE_STRUCT {

	struct OSLW_TOOL_DLIST_NODE_CONNECT_STRUCT con;
	lw_u32 len;

}OSlwMemoryListNodeSTU;


typedef struct OSLW_MEMORY_LIST_STRUCT {

	OSlwMemoryBasicSTU basic;

	OSlwExternMemorySTU Mem;

	void *pMemButtom;

	OSlwMemoryListNodeSTU *phead, *ptail;

}OSlwMemoryListSTU;


void * OSlwMemoryListAlloc(OSlwMemoryListSTU *pMU, lw_u32 dsize);
void * OSlwMemoryListFree(OSlwMemoryListSTU *pMU, void *p);
void* OSlwMemoryListReAlloc(OSlwMemoryListSTU *pMU, void *p, lw_u32 dsize);
lw_32 OSlwMemoryListInital(OSlwMemoryListSTU *pMU,
	void *pMem,
	lw_u16 lenMem,
	lw_u16 MemSize,
	lw_u32 sizeofmem
);
#endif //OSLW_MEMORY_IMPORT_LIST
	
	
#define OSLW_MEM_LIST_STD_SIZE(PMU,DSIZE) do{\
	lw_u16 _mod_res;\
	(DSIZE) += sizeof(OSlwMemoryListNodeSTU);\
	_mod_res=OSLW_MEM_SIZE_MOD((DSIZE) , (PMU)->basic.MemSectionSize);\
	(DSIZE) += (_mod_res?((PMU)->basic.MemSectionSize.SectionSize-_mod_res):0);\
}while(0)


#define OSLW_MEM_MAP_LEN(LEN,NUM) (((lw_u32)(LEN)*(NUM))+(sizeof(long **)*(NUM)))
#define OSLW_MEM_SIMPLE_LEN(LEN,NUM) ((lw_u32)(LEN)*(NUM))
#define OSLW_MEM_LIST_LEN(LEN,NUM) (((lw_u32)(LEN)*(NUM))+sizeof(OSlwMemoryListNodeSTU))



#define OSLW_MEM_MAP_DEFAULT {OSlwMemoryMapInital}
#define OSLW_MEM_SIMPLE_DEFAULT {OSlwMemorySimpleInital}
#define OSLW_MEM_LIST_DEFAULT {OSlwMemoryListInital}


#if OSLW_MEM_ALLOC_FAST

#define OSLW_MEM_MAP_DEF(NAME,LEN,NUM) OSlwMemoryMapSTU NAME##_struct=OSLW_MEM_MAP_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[OSLW_MEM_MAP_LEN(LEN,NUM)/sizeof(int)+1]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));


#define OSLW_MEM_SIMPLE_DEF(NAME,LEN,NUM) OSlwMemorySimpleSTU NAME##_struct=OSLW_MEM_SIMPLE_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[OSLW_MEM_SIMPLE_LEN(LEN,NUM)/sizeof(int)+1]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));


#define OSLW_MEM_LIST_DEF(NAME,LEN,NUM) OSlwMemoryListSTU NAME##_struct=OSLW_MEM_LIST_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
int _##NAME##_heap[OSLW_MEM_LIST_LEN(LEN,NUM)/sizeof(int)+1]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));

#else
#define OSLW_MEM_MAP_DEF(NAME,LEN,NUM) OSlwMemoryMapSTU NAME##_struct=OSLW_MEM_MAP_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[OSLW_MEM_MAP_LEN(LEN,NUM)]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));


#define OSLW_MEM_SIMPLE_DEF(NAME,LEN,NUM) OSlwMemorySimpleSTU NAME##_struct=OSLW_MEM_SIMPLE_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[OSLW_MEM_SIMPLE_LEN(LEN,NUM)]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));


#define OSLW_MEM_LIST_DEF(NAME,LEN,NUM) OSlwMemoryListSTU NAME##_struct=OSLW_MEM_LIST_DEFAULT;\
const lw_u16 _##NAME##_page_len = LEN;const lw_u16 _##NAME##_page_num = NUM;\
lw_u8 _##NAME##_heap[OSLW_MEM_LIST_LEN(LEN,NUM)]; \
 OSlwMemoryBasicSTU *p##NAME=(OSlwMemoryBasicSTU *)(&(NAME##_struct));

#endif //  OSLW_MEM_ALLOC_FAST






#define OSLW_MEM_INIT(NAME) do{p##NAME->init((void *)(p##NAME), (void *)(_##NAME##_heap), _##NAME##_page_num, _##NAME##_page_len, sizeof(lw_u8)*sizeof(_##NAME##_heap));}while(0)



#endif /*(Ver.=0.9~)(Beg.=0.9) OSLW_MEMORY_H_ */
