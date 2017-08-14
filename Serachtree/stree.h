#ifndef __STREE_H__
#define __STREE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linux/types.h>

//data node define

typedef struct stree_data_node {
	unsigned long key;
	unsigned long value_size;
	void* value;
}ST_D_NODE;

//tree node define

typedef struct stree_node {
	struct stree_node* parent;
	struct stree_node* left;
	struct stree_node* right;
	struct stree_data_node* data;
}ST_NODE;

#define ST_NODE_KEY(stree_node)	(stree_node)->data->key   		//get tree node key
#define ST_NODE_VALUE(stree_node) (stree_node)->data->value		//get tree node value

#define stree_init(name)	struct stree_node* name;\
								name = NULL;						//init root pointer

ST_NODE* _stree_successor_node(ST_NODE*);
ST_NODE* _stree_search(ST_NODE** ,unsigned long);

int stree_insert(ST_NODE**,ST_D_NODE*);  		//insert a node into tree
int stree_delete(ST_NODE**,unsigned long);			//delete a node from tree
ST_D_NODE* stree_search(ST_NODE** ,unsigned long);	//search a node by key from the tree
void stree_destory(ST_NODE*);							//destory the tree

void stree_DLR(ST_NODE*,int*);
void stree_LDR(ST_NODE*,int*);
void stree_LRD(ST_NODE*,int*);

#endif
