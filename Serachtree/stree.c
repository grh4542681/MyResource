/*
 *
 * 			Binary Search Tree
 *
 * 			by GRH
 *
 * 
 */


#include"stree.h"

/**
 * insert a node into the tree
 *
 * 向树种插入一个节点
 *
 */
int stree_insert(ST_NODE** stree_root,ST_D_NODE* stree_data)
{
	ST_NODE* stree_new_node;
	ST_NODE** stree_tmp;

	stree_new_node = NULL;
	stree_tmp = NULL;

	//malloc mem for new node
	stree_new_node = (ST_NODE*)malloc( sizeof( ST_NODE ) );
	if( !stree_new_node )
	{
		return -1;
	}	
	memset(stree_new_node, 0x00, sizeof( ST_NODE ));
	stree_new_node->data = (ST_D_NODE*)malloc( sizeof( ST_D_NODE ) );
	if( !(stree_new_node->data) )
	{
		return -1;
	}
	memset(stree_new_node, 0x00, sizeof( ST_D_NODE ) );
	stree_new_node->parent = NULL;
	stree_new_node->left = NULL;
	stree_new_node->right = NULL;
	stree_new_node->data->key = stree_data->key;
	stree_new_node->data->value_size = stree_data->value_size;
	stree_new_node->data->value = (void*)malloc( stree_new_node->data->value_size+(4-(stree_new_node->data->value_size)%4) );
	memset(stree_new_node->data->value, 0x00, stree_new_node->data->value_size+(4-(stree_new_node->data->value_size)%4) );
	if( !(stree_new_node->data->value) )
	{
		return -1;
	}
	memcpy(stree_new_node->data->value,stree_data->value,stree_new_node->data->value_size);

	if( !(*stree_root) ) //root node is empty
	{
		*stree_root = stree_new_node;
		return 0;
	}
	else
	{
		stree_tmp = stree_root;
		while(1)	//node has child
		{
			if( ST_NODE_KEY(stree_new_node) < ST_NODE_KEY(*stree_tmp) ) //left
			{
				if( !((*stree_tmp)->left) )
				{
					(*stree_tmp)->left = stree_new_node;
					stree_new_node->parent = *stree_tmp;
					break;
				}
				stree_tmp = &(*stree_tmp)->left;
				continue;
			}
			else if( ST_NODE_KEY(stree_new_node) > ST_NODE_KEY(*stree_tmp) )  //right
			{
				if( !((*stree_tmp)->right) )
				{
					(*stree_tmp)->right = stree_new_node;
					stree_new_node->parent = *stree_tmp;
					break;
				}
				stree_tmp = &(*stree_tmp)->right;
				continue;
			}
			else		//same key
			{
				return -1;
			}	
		}	
	}
}

/**
 * find the node's Successor node
 *
 * 找到给定节点的后继节点
 *
 */
ST_NODE* _stree_successor_node(ST_NODE* stree_node)
{
	ST_NODE* stree_tmp = NULL;

	if(!(stree_node))
		return (ST_NODE*)-1;

	if(!(stree_node->right))
		return NULL;
	
	stree_tmp = stree_node->right;
	while(stree_tmp->left)
	{
		stree_tmp = stree_tmp->left;	
	}
	return stree_tmp;	
}

/**
 * delete a node from the tree
 *
 * 从树中删除一个节点
 *
 */
int stree_delete(ST_NODE** stree_root,unsigned long key)
{
	ST_NODE** stree_parentLR = NULL;
	ST_NODE* stree_node_successor = NULL;

	//found the node which will be deleted
	ST_NODE* stree_node = _stree_search(stree_root,key);
	if(((long)stree_node) < 0 )
		return -1;

	//the node be its parent's left or right or no parent
	if( !(stree_node->parent) )
	{
		stree_parentLR = stree_root;
	}
	else if( ST_NODE_KEY(stree_node->parent->left) == key )
	{
		stree_parentLR = &(stree_node->parent->left);
	}
	else
	{
		stree_parentLR = &(stree_node->parent->right);
	}

	//begin delete
	//1.the node just has one child or no child
	if( !(stree_node->left) || !(stree_node->right) )
	{
		*stree_parentLR = (!(stree_node->left))&&(!(stree_node->right)) ? NULL : ( stree_node->left ? stree_node->left : stree_node->right );
		if( *stree_parentLR )
		{
			(*stree_parentLR)->parent = stree_node->parent;
		}
	}
	//2.the node doesn't have a successor node
	else if( !(stree_node->right->left) )
	{
		stree_node->right->left = stree_node->left;
		stree_node->left->parent = stree_node->right;
		*stree_parentLR = stree_node->right;
		(*stree_parentLR)->parent = stree_node->parent;
	}
	//3.the node has a successor node
	else
	{
		stree_node_successor = _stree_successor_node(stree_node);
		stree_node_successor->parent->left = stree_node_successor->right;
		if(stree_node_successor->right)
		{
			stree_node_successor->right->parent = stree_node_successor->parent;
		}
		stree_node_successor->left = stree_node->left;
		stree_node->left->parent = stree_node_successor;
		stree_node_successor->right = stree_node->right;
		stree_node->right->parent = stree_node_successor;
		*stree_parentLR	= stree_node_successor;		
		(*stree_parentLR)->parent = stree_node->parent;
	}
	//free the node's mem
	free(stree_node->data->value);
	free(stree_node->data);
	free(stree_node);
	stree_node=NULL;
	return 0;			
}

/**
 * destory the three
 *
 * 销毁整颗树
 *
 */
void stree_destory(ST_NODE* stree_tmp)
{
	if( !stree_tmp )
	{
		return;
	}
	//destory left three
	if( stree_tmp->left )
	{
		stree_destory(stree_tmp->left);	
	}
	//desroty right three	
	if( stree_tmp->right )
	{
		stree_destory(stree_tmp->right);	
	}
	//destory myself
	free(stree_tmp->data->value);
	free(stree_tmp->data);
	free(stree_tmp);
	stree_tmp=NULL;
}

/**
 * found the matching tree node
 *
 * 找到匹配树节点
 *
 */
ST_NODE* _stree_search(ST_NODE** stree_root,unsigned long key)
{
	ST_NODE** stree_tmp;

	stree_tmp = NULL;

	if( *stree_root )
	{
		stree_tmp = stree_root;
		while( *stree_tmp )
		{
			if( ST_NODE_KEY(*stree_tmp) == key )  //found and return 
			{
				return (*stree_tmp);
			}
			else if( ST_NODE_KEY(*stree_tmp) > key ) //must be in left tree
			{
				stree_tmp = &((*stree_tmp)->left);
			}
			else									//must be in right tree
			{
				stree_tmp = &((*stree_tmp)->right);
			}
		}
	}	
	return (ST_NODE*)-1; //not found
}

/**
 * found the matching data node
 *
 * 找到匹配树节点的数据区
 *
 */
ST_D_NODE* stree_search(ST_NODE** stree_root,unsigned long key)
{
	ST_NODE* stree_node = _stree_search(stree_root,key);
	if(((long)stree_node) < 0)
		return (ST_D_NODE*)-1;
	else
		return stree_node->data;
}

/**
 *  pre-travel tree and get sum of nodes
 *
 *  前序遍历 并取到树总结点数
 *
 */
void stree_DLR(ST_NODE* stree_tmp,int* num)
{
	if( !stree_tmp )
	{
		return;
	}
	//root
	(*num)++;
	printf("%x\n",stree_tmp);

	//left
	if( stree_tmp->left )
	{
		stree_DLR(stree_tmp->left,num);	
	}	

	//right
	if( stree_tmp->right )
	{
		stree_DLR(stree_tmp->right,num);	
	}
}

/**
 * inorder_travel tree and get sum of nodes
 *
 * 中序遍历 并取到树总结点数
 *
 */
void stree_LDR(ST_NODE* stree_tmp,int* num)
{
	if( !stree_tmp )
	{
		return;
	}
	//left
	if( stree_tmp->left )
	{
		stree_LDR(stree_tmp->left,num);	
	}	

	//root
	(*num)++;
	printf("%s ",(char*)ST_NODE_VALUE(stree_tmp));

	//right
	if( stree_tmp->right )
	{
		stree_LDR(stree_tmp->right,num);	
	}
}

/**
 * after_travel tree get sum of nodes
 *
 * 后序遍历 并取到树总结点数
 *
 */
void stree_LRD(ST_NODE* stree_tmp,int* num)
{
	if( !stree_tmp )
	{
		return;
	}
	//left
	if( stree_tmp->left )
	{
		stree_LRD(stree_tmp->left,num);	
	}	
	
	//right
	if( stree_tmp->right )
	{
		stree_LRD(stree_tmp->right,num);	
	}

	//root
	(*num)++;
	printf("%x\n",stree_tmp);
}

//test
int main()
{
	int num=0;
	stree_init(stree_root);
	ST_D_NODE a;
	a.key=(unsigned long)100;
	a.value="100";
	a.value_size=3;
	ST_D_NODE b;
	b.key=(unsigned long)104;
	b.value="104";
	b.value_size=3;
	ST_D_NODE c;
	c.key=(unsigned long)9;
	c.value="009";
	c.value_size=3;
	ST_D_NODE d;
	d.key=(unsigned long)98;
	d.value="098";
	d.value_size=3;
	ST_D_NODE e;
	e.key=(unsigned long)102;
	e.value="102";
	e.value_size=3;
	ST_D_NODE f;
	f.key=(unsigned long)90;
	f.value="090";
	f.value_size=3;
	ST_D_NODE g;
	g.key=(unsigned long)1000;
	g.value="000";
	g.value_size=3;
	
	stree_insert(&stree_root,&a);	
	stree_insert(&stree_root,&b);	
	stree_insert(&stree_root,&c);	
	stree_insert(&stree_root,&d);	
	stree_insert(&stree_root,&e);	
	stree_insert(&stree_root,&f);	
	stree_insert(&stree_root,&g);	

	stree_LDR(stree_root,&num);
	printf("\nnum:%d\n",num);

/*
	ST_D_NODE *data=stree_search(&stree_root,(unsigned long)99);
	printf("99:[%x]\n",data);
	printf("99:[%s]\n",(char*)data->value);
*/

	stree_delete(&stree_root,(unsigned long)100);
	stree_delete(&stree_root,(unsigned long)102);
	ST_D_NODE h;
	h.key=(unsigned long)2;
	h.value="002";
	h.value_size=3;
	ST_D_NODE i;
	i.key=(unsigned long)1;
	i.value="001";
	i.value_size=3;
	stree_insert(&stree_root,&h);	
	stree_insert(&stree_root,&i);	

	num=0;
	stree_LDR(stree_root,&num);
	printf("\nnum:%d\n",num);

	stree_delete(&stree_root,(unsigned long)9);
	num=0;
	stree_LDR(stree_root,&num);
	printf("\nnum:%d\n",num);

	stree_destory(stree_root);
}







