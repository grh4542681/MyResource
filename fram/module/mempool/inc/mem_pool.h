#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MEM_PAGE_SIZE 8*1024	// 页大小
#define MEM_ALIGN_BIT	8		// 对齐位

typedef int MEM_POOL_INT;
typedef char MEM_POOL_CHAR;
typedef unsigned int MEM_POOL_UINT;
typedef unsigned char MEM_POOL_UCHAR;
typedef long MEM_POOL_ADDRESS;

// 页内存结构
struct _MEM_POOL_BLOCK{
	MEM_POOL_UINT count;					// 分配次数
	MEM_POOL_UINT free;						// 空闲大小
	MEM_POOL_UINT fail_num;					// 分配失败次数
	MEM_POOL_ADDRESS* pfree;				// 空闲内存指针
	struct _MEM_POOL_BLOCK* next;			// 链表next
	struct _MEM_POOL_BLOCK* prev;			// 链表prev
};

// 大内存结构
struct _MEM_POOL_LARGE{
	struct _MEM_POOL_LARGE* next;			// 链表next
	struct _MEM_POOL_LARGE* prev;			// 链表prev
};

// 内存池结构
typedef struct MEM_POOL_HEAD{
	MEM_POOL_UINT page_size;				// 页大小
	MEM_POOL_UINT page_num;					// 页数
	struct _MEM_POOL_BLOCK* page_current;	// 页偏移
	struct _MEM_POOL_BLOCK* page_list;		// 内存页链表
	struct _MEM_POOL_LARGE* lpage_list;		// 大内存链表
}MEM_PH;

// 计算对齐字节数
#define MEM_ALIGN(size)	(((size)+(MEM_ALIGN_BIT-1))&(~(MEM_ALIGN_BIT-1)))

#define mem_pool_create(name) struct MEM_POOL_HEAD name={0,0,NULL,NULL,NULL}	// 创建内存池
int mem_pool_init(struct MEM_POOL_HEAD*);										// 初始化内存池
void mem_pool_reset(struct MEM_POOL_HEAD*);										// 重置内存池
void mem_pool_destory(struct MEM_POOL_HEAD*);									// 销毁内存池

void *mem_pool_malloc(MEM_POOL_UINT);											// malloc
void mem_pool_free(void*);														// free
static MEM_POOL_ADDRESS *_mem_pool_alloc_block(struct MEM_POOL_HEAD*,MEM_POOL_UINT);		// 页内存分配
static MEM_POOL_ADDRESS *_mem_pool_alloc_large(struct MEM_POOL_HEAD*,MEM_POOL_UINT);		// 大内存分配
MEM_POOL_ADDRESS *mem_pool_palloc(struct MEM_POOL_HEAD*,MEM_POOL_UINT);						// 分配内存（地址对齐）
MEM_POOL_ADDRESS *mem_pool_pnalloc(struct MEM_POOL_HEAD*,MEM_POOL_UINT);					// 分配内存（地址未对其）
static void _mem_pool_free_block(struct MEM_POOL_HEAD*,MEM_POOL_ADDRESS*);					// 释放页内存
static void _mem_pool_free_large(struct MEM_POOL_HEAD*,MEM_POOL_ADDRESS*);					// 释放大内存
void mem_pool_pfree(struct MEM_POOL_HEAD*,MEM_POOL_ADDRESS*);								// 释放内存

#endif
