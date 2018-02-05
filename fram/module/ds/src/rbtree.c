/**
 *
 *
 *		Red-Black tree
 *
 *
 *		By GRH
 *
 *
 */

#include"rbtree.h"

/**
 * left rotate
 *
 * 左旋
 *
 */
RB_NODE* _rbtree_l_rotate(RB_NODE* rbtree_node)
{
	RB_NODE* rbtree_tmp = NULL;
	RB_NODE** rbtree_parentLR = NULL;

	if( !(rbtree_node->right) )
		return NULL;		
	else
		rbtree_tmp = rbtree_node->right;
	
	if(RB_IS_LEAF(rbtree_node->parent))
	{
		rbtree_tmp->parent = RB_LEAF;
		rbtree_node->parent = rbtree_tmp;
		if(RB_IS_LEAF(rbtree_tmp->left))
		{
			rbtree_tmp->left = rbtree_node;
			rbtree_node->right = RB_LEAF;
		}
		else
		{
			rbtree_tmp->left->parent = rbtree_node;
			rbtree_node->right = rbtree_tmp->left;
			rbtree_tmp->left = rbtree_node;
		}
	}
	else
	{
		rbtree_parentLR = RB_PARENT_LR(rbtree_node);
		rbtree_tmp->parent = rbtree_node->parent;
		*rbtree_parentLR = rbtree_tmp;
		rbtree_node->parent = rbtree_tmp;
		if(RB_IS_LEAF(rbtree_tmp->left))
		{
			rbtree_tmp->left = rbtree_node;
			rbtree_node->right = RB_LEAF;
		}
		else
		{
			rbtree_tmp->left->parent = rbtree_node;
			rbtree_node->right = rbtree_tmp->left;
			rbtree_tmp->left = rbtree_node;
		}
	}
	while(!(RB_IS_LEAF(rbtree_tmp->parent)))
	{
		rbtree_tmp = rbtree_tmp->parent;
	}
	return rbtree_tmp;
}

/**
 * right rotate
 *
 * 右旋
 *
 */
RB_NODE* _rbtree_r_rotate(RB_NODE* rbtree_node)
{
	RB_NODE* rbtree_tmp = NULL;
	RB_NODE** rbtree_parentLR = NULL;

	if( !(rbtree_node->left) )
		return NULL;
	else
		rbtree_tmp = rbtree_node->left;

	if(RB_IS_LEAF(rbtree_node->parent))
	{
		rbtree_tmp->parent = RB_LEAF;
		rbtree_node->parent = rbtree_tmp;
		if(RB_IS_LEAF(rbtree_tmp->right))
		{
			rbtree_tmp->right = rbtree_node;
			rbtree_node->left = RB_LEAF;		
		}
		else
		{
			rbtree_tmp->right->parent = rbtree_node;
			rbtree_node->left = rbtree_tmp->right;
			rbtree_tmp->right = rbtree_node;
		}
	}
	else
	{
		rbtree_parentLR = RB_PARENT_LR(rbtree_node);
		rbtree_tmp->parent = rbtree_node->parent;
		*rbtree_parentLR = rbtree_tmp;
		rbtree_node->parent = rbtree_tmp;
		if(RB_IS_LEAF(rbtree_tmp->right))
		{
			rbtree_tmp->right = rbtree_node;
			rbtree_node->left = RB_LEAF;
		}
		else
		{
			rbtree_tmp->right->parent = rbtree_node;
			rbtree_node->left = rbtree_tmp->right;
			rbtree_tmp->right = rbtree_node;
		}
	}
	while(!(RB_IS_LEAF(rbtree_tmp->parent)))
	{
		rbtree_tmp = rbtree_tmp->parent;
	}
	return rbtree_tmp;
}

/**
 * search the tree by key
 *
 * 查找节点返回树节点地址
 *
 */
RB_NODE* _rbtree_search(RB_NODE** rbtree_root,unsigned long key)
{
	RB_NODE** rbtree_tmp;
	rbtree_tmp = NULL;

	if( *rbtree_root )
	{
		rbtree_tmp = rbtree_root;
		while(!RB_IS_LEAF(*rbtree_tmp))
		{
			if( RB_NODE_KEY(*rbtree_tmp)==key )
				return *rbtree_tmp;
			else if( RB_NODE_KEY(*rbtree_tmp) > key)
				rbtree_tmp = &((*rbtree_tmp)->left);
			else
				rbtree_tmp = &((*rbtree_tmp)->right);
		}
	}
	return (RB_NODE*)-1;
}

/**
 * search the tree by key
 *
 * 查找节点返回数据节点地址
 *
 */
RB_D_NODE* rbtree_search(RB_NODE** rbtree_root,unsigned long key)
{
	RB_NODE* rbtree_node = _rbtree_search(rbtree_root,key);
	if(((long)rbtree_node) < 0)
		return (RB_D_NODE*)-1;
	else
		return rbtree_node->data;
}

/**
 * fix up the tree after insert
 *
 * 插入节点后修复红黑树
 *
 */
int _rbtree_insert_fixup(RB_NODE** rbtree_root,RB_NODE* rbtree_node)
{
	int ret;
	rbtree_node->color = RB_RED;
	RB_NODE* rbtree_tmp = rbtree_node;

	if( (!(RB_IS_LEAF(rbtree_node->parent))) && (!(RB_IS_LEAF(rbtree_node->parent->parent))))	//have grand-pa node
	{
		//father node is left to grand-pa node
		if(RB_IS_LEAF(rbtree_node->parent->parent->right) || ((!RB_IS_LEAF(rbtree_node->parent->parent->left)) && (RB_NODE_KEY(rbtree_node->parent) == RB_NODE_KEY(rbtree_node->parent->parent->left))))
		{
		//	printf("insert fix up left\n");
			while(RB_IS_RED(rbtree_tmp->parent))
			{
				//case 1
				if(RB_IS_RED(rbtree_tmp->parent->parent->right) && RB_IS_RED(rbtree_tmp->parent->parent->left))
				{
		//			printf("insert fix up case 1\n");
					rbtree_tmp->parent->parent->color = RB_RED;
					rbtree_tmp->parent->parent->left->color = RB_BLACK;
					rbtree_tmp->parent->parent->right->color = RB_BLACK;
					rbtree_tmp = rbtree_tmp->parent->parent;
					continue;
				}
				else
				{
					//case 2
					if(RB_IS_LEAF(rbtree_tmp->parent->left) || ((!RB_IS_LEAF(rbtree_tmp->parent->right))&&(RB_NODE_KEY(rbtree_tmp) == RB_NODE_KEY(rbtree_tmp->parent->right))))
					{
		//				printf("insert fix up case 2\n");
						rbtree_tmp = rbtree_tmp->parent;
						if(!_rbtree_l_rotate(rbtree_tmp))
							return -1;
					}
					//case 3
					else
					{
		//				printf("insert fix up case 3\n");
						rbtree_tmp->parent->color = RB_BLACK;
						rbtree_tmp->parent->parent->color = RB_RED;
						if(!_rbtree_r_rotate(rbtree_tmp->parent->parent))
							return -1;
					}
				}
			}
		}
		//father node is right to grand-pa node
		else
		{
		//	printf("insert fix up right\n");
			while(RB_IS_RED(rbtree_tmp->parent))
			{
				//case 1
				if(RB_IS_RED(rbtree_tmp->parent->parent->right) && RB_IS_RED(rbtree_tmp->parent->parent->left))
				{
		//			printf("insert fix up case 1\n");
					rbtree_tmp->parent->parent->color = RB_RED;
					rbtree_tmp->parent->parent->left->color = RB_BLACK;
					rbtree_tmp->parent->parent->right->color = RB_BLACK;
					rbtree_tmp = rbtree_tmp->parent->parent;
					continue;
				}
				else
				{
					//case 2
					if(RB_IS_LEAF(rbtree_tmp->parent->left) || ((!RB_IS_LEAF(rbtree_tmp->parent->right))&&(RB_NODE_KEY(rbtree_tmp) == RB_NODE_KEY(rbtree_tmp->parent->right))))
					{
		//				printf("insert fix up case 2\n");
						rbtree_tmp->parent->color = RB_BLACK;
						rbtree_tmp->parent->parent->color = RB_RED;
						if(!_rbtree_l_rotate(rbtree_tmp->parent->parent))
							return -1;
					}
					//case 3
					else
					{
		//				printf("insert fix up case 3\n");
						rbtree_tmp = rbtree_tmp->parent;
						if(!_rbtree_r_rotate(rbtree_tmp))
							return -1;
					}
				}
			}
		}
	}
	//change root
	while(!(RB_IS_LEAF(rbtree_tmp->parent)))
	{
		rbtree_tmp = rbtree_tmp->parent;
	}
	rbtree_tmp->color = RB_BLACK;
	*rbtree_root = rbtree_tmp;
	return 0;
}

/**
 * insert node into rb-tree
 *
 * 插入节点
 *
 */
int rbtree_insert(RB_NODE** rbtree_root,RB_D_NODE* rbtree_data)
{
	RB_NODE* rbtree_new_node;
	RB_NODE** rbtree_tmp;

	rbtree_new_node = NULL;
	rbtree_tmp = NULL;

	//malloc mem for new node
	rbtree_new_node = (RB_NODE*)malloc( sizeof( RB_NODE ) );
	if( !rbtree_new_node )
	{
		return -1;
	}	
	memset(rbtree_new_node, 0x00, sizeof( RB_NODE ));
	rbtree_new_node->data = (RB_D_NODE*)malloc( sizeof( RB_D_NODE ) );
	if( !(rbtree_new_node->data) )
	{
		return -1;
	}
	memset(rbtree_new_node, 0x00, sizeof( RB_D_NODE ) );
	rbtree_new_node->parent = RB_LEAF;
	rbtree_new_node->left = RB_LEAF;
	rbtree_new_node->right = RB_LEAF;
	rbtree_new_node->color = RB_RED;
	rbtree_new_node->data->key = rbtree_data->key;
	rbtree_new_node->data->value_size = rbtree_data->value_size;
	rbtree_new_node->data->value = (void*)malloc( rbtree_new_node->data->value_size+(4-(rbtree_new_node->data->value_size)%4) );
	memset(rbtree_new_node->data->value, 0x00, rbtree_new_node->data->value_size+(4-(rbtree_new_node->data->value_size)%4) );
	if( !(rbtree_new_node->data->value) )
	{
		return -1;
	}
	memcpy(rbtree_new_node->data->value,rbtree_data->value,rbtree_new_node->data->value_size);

//	printf("\ninsert key [%d]\n",rbtree_new_node->data->key);

	//insert node
	if( !(*rbtree_root) ) //root node is empty
	{
		*rbtree_root = rbtree_new_node;
	}
	else
	{
		rbtree_tmp = rbtree_root;
		while(1)	//node has child
		{
			if( RB_NODE_KEY(rbtree_new_node) < RB_NODE_KEY(*rbtree_tmp) ) //left
			{
				if( RB_IS_LEAF((*rbtree_tmp)->left) )
				{
					(*rbtree_tmp)->left = rbtree_new_node;
					rbtree_new_node->parent = *rbtree_tmp;
					break;
				}
				rbtree_tmp = &(*rbtree_tmp)->left;
				continue;
			}
			else if( RB_NODE_KEY(rbtree_new_node) > RB_NODE_KEY(*rbtree_tmp) )  //right
			{
				if( RB_IS_LEAF((*rbtree_tmp)->right) )
				{
					(*rbtree_tmp)->right = rbtree_new_node;
					rbtree_new_node->parent = *rbtree_tmp;
					break;
				}
				rbtree_tmp = &(*rbtree_tmp)->right;
				continue;
			}
			else		//same key
			{
				return -1;
			}	
		}	
	}
	//fix up the tree
	return _rbtree_insert_fixup(rbtree_root,rbtree_new_node);	
}

/*
 * find the node's Successor node
 *
 * 找到节点的后继节点
 *
 */
RB_NODE* _rbtree_successor(RB_NODE* rbtree_node)
{
	RB_NODE* rbtree_tmp = NULL;

	if( (!rbtree_node) || RB_IS_LEAF(rbtree_node->right) )
		return NULL;
	
	rbtree_tmp = rbtree_node->right;

	while(!RB_IS_LEAF(rbtree_tmp->left))
		rbtree_tmp = rbtree_tmp->left;

	return rbtree_tmp;	
}

/**
 * fix up the tree after delete node
 *
 * 删除节点后修复红黑树
 *
 */
int _rbtree_delete_fixup(RB_NODE** rbtree_root,RB_NODE* rbtree_node)
{
	RB_NODE* rbtree_self = rbtree_node;
	RB_NODE* rbtree_brother = NULL;
	
	while( ( rbtree_self != *rbtree_root ) && ( (!RB_IS_LEAF(rbtree_self) && ( rbtree_self->color == RB_BLACK ) ) ) )
	{
		if( RB_NODE_KEY(rbtree_self) == RB_NODE_KEY(rbtree_self->parent->left) )
		{
		//	printf("delete fix up left\n");
			rbtree_brother = rbtree_self->parent->right;	
			//case 1
			if(RB_IS_RED(rbtree_brother))
			{
		//		printf("delete fix up case 1\n");
				rbtree_brother->color = RB_BLACK;
				rbtree_self->parent->color = RB_RED;
				*rbtree_root = _rbtree_l_rotate(rbtree_self->parent);
				rbtree_brother = rbtree_self->parent->right;
			}
			else
			{
				//case 2
				if(RB_IS_BLACK(rbtree_brother->left) && RB_IS_BLACK(rbtree_brother->right))
				{
		//			printf("delete fix up case 2\n");
		//			printf("bro[%d]\n",RB_NODE_KEY(rbtree_brother));
					rbtree_brother->color = RB_RED;
					rbtree_self = rbtree_self->parent;
				}
				else
				{
					//case 3
					if((!RB_IS_LEAF(rbtree_brother->right)) && RB_IS_BLACK(rbtree_brother->right))
					{
		//				printf("delete fix up case 3\n");
						rbtree_brother->color = RB_RED;
						rbtree_brother->left->color = RB_BLACK;
						*rbtree_root = _rbtree_r_rotate(rbtree_brother);
						rbtree_brother = rbtree_self->parent->right;
					}
					//case 4
		//			printf("delete fix up case 4\n");
					rbtree_brother->color = rbtree_self->parent->color;
					rbtree_self->parent->color = RB_BLACK;
					rbtree_brother->right->color = RB_BLACK;
					*rbtree_root = _rbtree_l_rotate(rbtree_self->parent);
					break;
				}
			}
		}
		else
		{
		//	printf("delete fix up right\n");
			rbtree_brother = rbtree_self->parent->left;	
			//case 1
			if(RB_IS_RED(rbtree_brother))
			{
		//		printf("delete fix up case 1\n");
				rbtree_brother->color = RB_BLACK;
				rbtree_self->parent->color = RB_RED;
				*rbtree_root = _rbtree_r_rotate(rbtree_self->parent);
				rbtree_brother = rbtree_self->parent->right;
			}
			else
			{
				//case 2
				if(RB_IS_BLACK(rbtree_brother->left) && RB_IS_BLACK(rbtree_brother->right))
				{
		//			printf("delete fix up case 2\n");
					rbtree_brother = RB_RED;
					rbtree_self = rbtree_self->parent;
				}
				else
				{
					//case 3
					if((!RB_IS_LEAF(rbtree_brother->right)) && RB_IS_BLACK(rbtree_brother->right))
					{
		//				printf("delete fix up case 3\n");
						rbtree_brother->color = RB_RED;
						rbtree_brother->left->color = RB_BLACK;
						*rbtree_root = _rbtree_l_rotate(rbtree_brother);
						rbtree_brother = rbtree_self->parent->right;
					}
					//case 4
		//			printf("delete fix up case 4\n");
					rbtree_brother->color = rbtree_self->parent->color;
					rbtree_self->parent->color = RB_BLACK;
					rbtree_brother->right->color = RB_BLACK;
					*rbtree_root = _rbtree_r_rotate(rbtree_self->parent);
					break;
				}
			}
		}
	}
	rbtree_self->color=RB_BLACK;
}

int rbtree_delete(RB_NODE** rbtree_root,unsigned long key)
{
	RB_NODE** rbtree_parentLR = NULL;
	RB_NODE* rbtree_successor = NULL;
	RB_NODE* rbtree_bench = NULL;
	RB_NODE* rbtree_delete = NULL;
	COLOR_T delete_color=0;
	
//	printf("\ndelete key[%d]\n",key);

	RB_NODE* rbtree_node = _rbtree_search(rbtree_root,key);
	if(((long)rbtree_node)<0)
		return -1;

	if( RB_IS_LEAF(rbtree_node->parent) )
		rbtree_parentLR = rbtree_root;
	else
		rbtree_parentLR = RB_PARENT_LR(rbtree_node);

	if( RB_IS_LEAF(rbtree_node->left) || RB_IS_LEAF(rbtree_node->right) )
	{
//		printf("delete case 1 \n");
		rbtree_delete = rbtree_node;
		delete_color = rbtree_node->color;

		if( RB_IS_LEAF(rbtree_node->left) && RB_IS_LEAF(rbtree_node->right) )
		{
			rbtree_bench = rbtree_node;
		_rbtree_delete_fixup(rbtree_root,rbtree_bench);
			*rbtree_parentLR = RB_LEAF;
		}			
		else if( RB_IS_LEAF(rbtree_node->left) )
		{
			rbtree_bench = rbtree_node->right;	
			*rbtree_parentLR = rbtree_node->right;
			(*rbtree_parentLR)->parent = rbtree_node->parent;
		}
		else
		{
			rbtree_bench = rbtree_node->left;	
			*rbtree_parentLR = rbtree_node->left;
			(*rbtree_parentLR)->parent = rbtree_node->parent;
		}
	}
	else if( RB_IS_LEAF(rbtree_node->right->left) )
	{
//		printf("delete case 2 \n");
		rbtree_delete = rbtree_node;
		delete_color = rbtree_node->color;
		rbtree_bench = rbtree_node->right;

		rbtree_node->right->left = rbtree_node->left;
		rbtree_node->left->parent = rbtree_node->right;
		*rbtree_parentLR = rbtree_node->right;
		(*rbtree_parentLR)->parent = rbtree_node->parent;
	}
	else
	{
//		printf("delete case 3 \n");
		rbtree_successor = _rbtree_successor(rbtree_node);
		rbtree_delete = rbtree_successor;
		delete_color = rbtree_successor->color;
		rbtree_bench = rbtree_successor->right;

		rbtree_successor->parent->left = rbtree_successor->right;
		if( !RB_IS_LEAF(rbtree_successor->right) )
		{		
			rbtree_successor->right->parent = rbtree_successor->parent;
		}
		rbtree_successor->left = rbtree_node->left;
		rbtree_node->left->parent = rbtree_successor;
		rbtree_successor->right = rbtree_node->right;
		rbtree_node->right->parent = rbtree_successor;
		*rbtree_parentLR = rbtree_successor;
		(*rbtree_parentLR)->parent = rbtree_node->parent;
		(*rbtree_parentLR)->color = rbtree_node->color;
		
	}
	if((delete_color == RB_BLACK) && (!RB_IS_LEAF(*rbtree_parentLR)))
	{
		_rbtree_delete_fixup(rbtree_root,rbtree_bench);
	}
}

/**
 * destory the tree
 *
 * 销毁整棵树
 *
 */
void rbtree_destory(RB_NODE* rbtree_tmp)
{
	if( !rbtree_tmp )
		return;
	if( !RB_IS_LEAF(rbtree_tmp->left) )
		rbtree_destory(rbtree_tmp->left);
	if( !RB_IS_LEAF(rbtree_tmp->right) )
		rbtree_destory(rbtree_tmp->right);
	free(rbtree_tmp->data->value);
	free(rbtree_tmp->data);
	free(rbtree_tmp);
	rbtree_tmp=NULL;	
}

/**
 *  pre-travel tree and get sum of nodes
 *
 *  前序遍历 并取到树总结点数
 *
 */
void rbtree_DLR(RB_NODE* rbtree_tmp,int* num)
{
	if( !rbtree_tmp )
	{
		return;
	}
	//root
	(*num)++;
	printf("%x\n",rbtree_tmp);

	//left
	if( !(RB_IS_LEAF(rbtree_tmp->left)) )
	{
		rbtree_DLR(rbtree_tmp->left,num);	
	}	

	//right
	if( !(RB_IS_LEAF(rbtree_tmp->right)) )
	{
		rbtree_DLR(rbtree_tmp->right,num);	
	}
}

/**
 * inorder_travel tree and get sum of nodes
 *
 * 中序遍历 并取到树总结点数
 *
 */
void rbtree_LDR(RB_NODE* rbtree_tmp,int* num)
{
	if( !rbtree_tmp )
	{
		return;
	}
	//left
	if( !(RB_IS_LEAF(rbtree_tmp->left)) )
	{
		rbtree_LDR(rbtree_tmp->left,num);	
	}	

	//root
	(*num)++;
	printf("%s-%d ",(char*)RB_NODE_VALUE(rbtree_tmp),rbtree_tmp->color);

	//right
	if( !(RB_IS_LEAF(rbtree_tmp->right)) )
	{
		rbtree_LDR(rbtree_tmp->right,num);	
	}
}

/**
 * after_travel tree get sum of nodes
 *
 * 后序遍历 并取到树总结点数
 *
 */
void rbtree_LRD(RB_NODE* rbtree_tmp,int* num)
{
	if( !rbtree_tmp )
	{
		return;
	}
	//left
	if( !(RB_IS_LEAF(rbtree_tmp->left)) )
	{
		rbtree_LRD(rbtree_tmp->left,num);	
	}	
	
	//right
	if( !(RB_IS_LEAF(rbtree_tmp->right)) )
	{
		rbtree_LRD(rbtree_tmp->right,num);	
	}

	//root
	(*num)++;
	printf("%x\n",rbtree_tmp);
}

void LevelOrder(RB_NODE* rbtree_node)
{
	int ilevel = 0;
	int iflag = 0;
	RB_NODE** stack1 = NULL;
	RB_NODE** stack2 = NULL;
	RB_NODE** pointer1 = NULL;
	RB_NODE** pointer2 = NULL;
	unsigned int stack1_len = 0;
	unsigned int stack2_len = 0;
	

	if( !rbtree_node )
		return;

	stack1_len = sizeof(RB_NODE*)*(((int)pow(2,ilevel))+1);
	stack1 = (RB_NODE**)malloc(stack1_len);

	if(!stack1)
	{
		return;
	}
	
	memset(stack1,0x00, stack1_len);
	*stack1 = rbtree_node;

	pointer1 = stack1;
	while( stack1 )
	{
		printf("Level[%d]--",ilevel);
		pointer1 = stack1;
		while( *pointer1 )
		{
			if( !RB_IS_LEAF((*pointer1)->left) || !RB_IS_LEAF((*pointer1)->right) )
			{
				ilevel++;
				stack2_len = sizeof(RB_NODE*)*(((int)pow(2,ilevel))+1);
				stack2 = (RB_NODE**)malloc(stack2_len);
				memset(stack2,0x00, stack2_len);
				pointer2 = stack2;
				break;
			}
			else
			{
				pointer2 = NULL;
				stack2 = NULL;
			}
			pointer1++;
		}
		pointer1 = stack1;
		while( *pointer1 )
		{
			printf("%03d-%d\t",(*pointer1)->data->key,(*pointer1)->color);
			if( !RB_IS_LEAF((*pointer1)->left) && pointer2)
			{
				*pointer2 = (*pointer1)->left;
				pointer2++;
			}
			if( !RB_IS_LEAF((*pointer1)->right) && pointer2)
			{
				*pointer2 = (*pointer1)->right;
				pointer2++;
			}
			pointer1++;
		}
		printf("\n");

		free(stack1);
		stack1 = NULL;
		stack1 = stack2;
	}	
}

#if 0
int main()
{
	int num=0;
	rbtree_init(rbtree_root);
/*
	RB_D_NODE a={100,3,"100"};
	rbtree_insert(&rbtree_root,&a);
	RB_D_NODE b={101,3,"101"};
	rbtree_insert(&rbtree_root,&b);
	RB_D_NODE c={102,3,"102"};
	rbtree_insert(&rbtree_root,&c);
	RB_D_NODE d={103,3,"103"};
	rbtree_insert(&rbtree_root,&d);
	RB_D_NODE e={104,3,"104"};
	rbtree_insert(&rbtree_root,&e);
	RB_D_NODE f={105,3,"105"};
	rbtree_insert(&rbtree_root,&f);
	RB_D_NODE g={106,3,"106"};
	rbtree_insert(&rbtree_root,&g);
	RB_D_NODE h={107,3,"107"};
	rbtree_insert(&rbtree_root,&h);
	RB_D_NODE i={108,3,"108"};
	rbtree_insert(&rbtree_root,&i);
	RB_D_NODE j={110,3,"110"};
	rbtree_insert(&rbtree_root,&j);
*/
	RB_D_NODE x;
		x.value = malloc(4);
	int i = 0;
	char a[4];
	for( i ; i<64 ; i++)
	{
		x.key = i;
		x.value_size = 4;
		memset(x.value,0x00,4);
		sprintf(x.value,"%d",i);
		rbtree_insert(&rbtree_root,&x);
	}
	num = 0;
	rbtree_LDR(rbtree_root,&num);
	printf("num[%d]\n",num);
	printf("root[%s]\n",RB_NODE_VALUE(rbtree_root));
	LevelOrder(rbtree_root);
	printf("----------------------------------------\n");
/*
	RB_D_NODE f={103,3,"103"};
	rbtree_insert(&rbtree_root,&f);
	RB_D_NODE a={108,3,"108"};
	rbtree_insert(&rbtree_root,&a);
	RB_D_NODE e={104,3,"104"};
	rbtree_insert(&rbtree_root,&e);
	RB_D_NODE i={100,3,"100"};
	rbtree_insert(&rbtree_root,&i);
	RB_D_NODE b={107,3,"107"};
	rbtree_insert(&rbtree_root,&b);
	RB_D_NODE h={101,3,"101"};
	rbtree_insert(&rbtree_root,&h);
	RB_D_NODE g={102,3,"102"};
	rbtree_insert(&rbtree_root,&g);
	RB_D_NODE c={106,3,"106"};
	rbtree_insert(&rbtree_root,&c);
	RB_D_NODE d={105,3,"105"};
	rbtree_insert(&rbtree_root,&d);
*/
	/*
	rbtree_LDR(rbtree_root,&num);
	printf("num[%d]\n",num);
	RB_D_NODE* data=rbtree_search(&rbtree_root,10);
	printf("105:data[%s]\n",(char*)data->value);
	printf("root[%s]\n",RB_NODE_VALUE(rbtree_root));
	*/

/*
	rbtree_delete(&rbtree_root,1);
	rbtree_delete(&rbtree_root,10);
	rbtree_delete(&rbtree_root,11);
	rbtree_delete(&rbtree_root,12);
	rbtree_delete(&rbtree_root,13);
	rbtree_delete(&rbtree_root,14);
	rbtree_delete(&rbtree_root,15);
	rbtree_delete(&rbtree_root,127);
	rbtree_delete(&rbtree_root,128);
	rbtree_delete(&rbtree_root,129);
	rbtree_delete(&rbtree_root,130);
*/
	i=0;
	for(i;i<30;i++)
	{
		rbtree_delete(&rbtree_root,i);
	}
	num = 0;
	rbtree_LDR(rbtree_root,&num);
	printf("num[%d]\n",num);
	printf("root[%s]\n",RB_NODE_VALUE(rbtree_root));

	LevelOrder(rbtree_root);

	i = 0;
	for( i ; i<30 ; i++)
	{
		x.key = i;
		x.value_size = 4;
		memset(x.value,0x00,4);
		sprintf(x.value,"%d",i);
		rbtree_insert(&rbtree_root,&x);
	}
	for( i=65 ; i<100 ; i++)
	{
		x.key = i;
		x.value_size = 4;
		memset(x.value,0x00,4);
		sprintf(x.value,"%d",i);
		rbtree_insert(&rbtree_root,&x);
	}
	for( i=-30 ; i<0 ; i++)
	{
		x.key = i;
		x.value_size = 4;
		memset(x.value,0x00,4);
		sprintf(x.value,"%d",i);
		rbtree_insert(&rbtree_root,&x);
	}
	num = 0;
	rbtree_LDR(rbtree_root,&num);
	printf("num[%d]\n",num);
	printf("root[%s]\n",RB_NODE_VALUE(rbtree_root));
	LevelOrder(rbtree_root);

	rbtree_destory(rbtree_root);
}

#endif


