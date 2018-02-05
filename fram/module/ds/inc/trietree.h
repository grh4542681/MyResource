#ifndef __TRIETREE_H__
#define __TRIETREE_H__

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//error code
#define SYSERR	-1
#define UNKNOWKEY -2

#define CASE_SENSITIVE

#ifdef CASE_SENSITIVE	//区分大小写
	#define TT_DICT_NUM (26*2+10)
/*
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
	'0','1','2','3','4','5','6','7','8','9'
*/
#else
	#define TT_DICT_NUM (26+10)
/*
	'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
	'0','1','2','3','4','5','6','7','8','9'
*/
#endif

typedef struct trietree_node{
	unsigned int level;							//floor
	unsigned int parent_offset;					//offset in parent node slots array
	unsigned int count;							//children number
	unsigned int datalen;						//data length
	struct trietree_node* parent;				//parent node pointer
	struct trietree_node* slots[TT_DICT_NUM];	//children pointers array
	void* data;									//data area
}TT_NODE;

typedef struct trietree_root{
	unsigned count;
	struct trietree_node* node;
}TT_ROOT;

#define trietree_init(root)	TT_ROOT root={0,NULL};

unsigned long _trietree_slot_offset(char);
int trietree_insert( TT_ROOT*,char*,void*,unsigned long );
TT_NODE* trietree_search( TT_ROOT*,char* );
int trietree_delete( TT_ROOT*,char* );
void _trietree_destory( TT_NODE* );
void trietree_destory( TT_ROOT* );
int trietree_update( TT_ROOT*,char*,void*,unsigned long );

#endif
