#ifndef __RBTREE_H__ 
#define __RBTREE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <linux/types.h>

//data node
typedef struct rbtree_data_node{
	unsigned long key;
	unsigned long value_size;
	void* value;
}RB_D_NODE;

//tree node
typedef struct rbtree_node{
	struct rbtree_node* parent;
	struct rbtree_node* left;
	struct rbtree_node* right;
	struct rbtree_data_node* data;
	unsigned int color;
}RB_NODE;

typedef unsigned int COLOR_T;

#define RB_NODE_KEY(rbtree_node_p)	((rbtree_node_p)->data->key)
#define RB_NODE_VALUE(rbtree_node_p)	((rbtree_node_p)->data->value)

#define RB_RED 0
#define RB_BLACK 1

#define RB_IS_RED(rbtree_node_p)	(!((rbtree_node_p)->color))
#define RB_IS_BLACK(rbtree_node_p)	(((rbtree_node_p)->color))

#define RB_SET_RED(rbtree_node_p)	((rbtree_node_p)->color=RB_RED)
#define RB_SET_BLACK(rbtree_node_p)	((rbtree_node_p)->color=RB_BLACK)

//Leaf node
struct rbtree_node _rb_leaf_node={NULL,NULL,NULL,NULL,RB_BLACK};
#define RB_LEAF &_rb_leaf_node
#define RB_IS_LEAF(rbtree_node) ((rbtree_node)==&_rb_leaf_node)
#define RB_GET_LEAF(rbtree_leaf_node_p)	struct rbtree_node* ((rbtree_leaf_node_p)=&_rb_leaf_node)

//the node is it's parent left or right
#define RB_PARENT_LR(rbtree_node)	(RB_NODE_KEY(rbtree_node)<RB_NODE_KEY(rbtree_node->parent)? &rbtree_node->parent->left : &rbtree_node->parent->right)

RB_NODE* _rbtree_successor(RB_NODE*);
RB_NODE* _rbtree_search(RB_NODE** ,unsigned long);
RB_NODE* _rbtree_l_rotate(RB_NODE*);									//left rotate
RB_NODE* _rbtree_r_rotate(RB_NODE*);									//right rotate
int _rbtree_insert_fixup(RB_NODE**,RB_NODE*);								//insert fix up
int _rbtree_delete_fixup(RB_NODE**,RB_NODE*);								//delete fix up

#define rbtree_init(name)	struct rbtree_node* name;\
											name = NULL;
int rbtree_insert(RB_NODE**,RB_D_NODE*);       					    //insert a node into tree
int rbtree_delete(RB_NODE**,unsigned long);          				//delete a node from tree
RB_D_NODE* rbtree_search(RB_NODE** ,unsigned long);  				//search a node by key from the tree
void rbtree_destory(RB_NODE*);                           			//destory the tree

void rbtree_DLR(RB_NODE*,int*);
void rbtree_LDR(RB_NODE*,int*);
void rbtree_LRD(RB_NODE*,int*);


#endif








