/**
 *
 *
 * 				Trie Tree (×ÖµäÊ÷)
 * 					by  GRH
 *
 *
 */

#include"trietree.h"

/**
 *
 * malloc in trie tree
 *
 */
void* _trietree_malloc(size_t len)
{
	if( len <= 0 )
		return NULL;

	void* p = malloc(len);

	if( !p )
		return p;
	else
	{
		memset( p,0x00,len );
		return p;
	}
}

/**
 *
 * free in trie tree
 *
 */
void _trietree_free(void* p)
{
	return free(p);
}

/**
 *
 * calculated offset in slot array
 *
 */
unsigned long _trietree_slot_offset(char letter)
{
	char _letter;

#ifdef CASE_SENSITIVE
	_letter = letter;	
#else
	if( letter >= 'A' && letter <= 'Z' )
		_letter = letter + 32;
	else
		_letter = letter;
#endif

	if( _letter >= 'a' && _letter <= 'z' )
	{
		return _letter - 'a';
	}
	else if( _letter >= 'A' && _letter <= 'Z' )
	{
		return _letter - 'A' + 26;
	}
	else if( _letter >= '0' && _letter <= '9' )
	{
		return _letter - '0' + 26 + 26;
	}
	else
	{
		return -1;
	}
}

/**
 *
 * insert a item into trietree
 *
 */
int trietree_insert( TT_ROOT* trietree_root,char* key,void* data,unsigned long data_len)
{
	char* pkey = NULL;
	unsigned int ikeylen;
	unsigned int level;
	int offset;

	TT_NODE* trietree_tmp = NULL;

	ikeylen = strlen(key);
	trietree_tmp = trietree_root->node;

	if(!trietree_tmp)
	{
		trietree_root->node = (TT_NODE*)_trietree_malloc(sizeof(TT_NODE));
		if(!trietree_root->node)
			return SYSERR;
		memset(trietree_root->node,0x00,sizeof(TT_NODE));
		trietree_root->node->level = 1;
		trietree_tmp = trietree_root->node;
	}

	level = trietree_root->node->level;
	
	for( pkey = key;ikeylen > 0;ikeylen-- )
	{
		offset = _trietree_slot_offset(*pkey);
		if(offset<0)
			return UNKNOWKEY;	
		if(!(trietree_tmp->slots[offset]))
		{
			trietree_tmp->slots[offset] = (TT_NODE*)_trietree_malloc(sizeof(TT_NODE));
			if(!trietree_tmp->slots[offset])
				return SYSERR;
			memset(trietree_tmp->slots[offset],0x00,sizeof(TT_NODE));
			trietree_tmp->slots[offset]->level = level+1;
			trietree_tmp->slots[offset]->parent = trietree_tmp;
			trietree_tmp->slots[offset]->parent_offset = offset;
			trietree_tmp->count++;
		}
		trietree_tmp = trietree_tmp->slots[offset];
	}
	
	//copy data
	trietree_tmp->datalen = data_len;
	if( trietree_tmp->datalen )
	{
		trietree_tmp->data = _trietree_malloc(trietree_tmp->datalen+(4-(trietree_tmp->datalen%4)));
		memset(trietree_tmp->data,0x00,trietree_tmp->datalen+(4-(trietree_tmp->datalen%4)));
		memcpy(trietree_tmp->data,data,trietree_tmp->datalen+(4-(trietree_tmp->datalen%4)));
	}

	trietree_root->count++;
	return 0;
}

/**
 *
 * search the key in the tree
 *
 */
TT_NODE* trietree_search(TT_ROOT* trietree_root,char* key)
{	
	char* pkey;
	int ikeylen;
	int offset;
	TT_NODE* trietree_tmp = NULL;

	if(!(trietree_root->node))
	{
		return NULL;
	}

	trietree_tmp = trietree_root->node;
	ikeylen = strlen(key);

	for(pkey = key;ikeylen > 0;ikeylen--)
	{
		offset = _trietree_slot_offset(*pkey);
		if(offset<0)
			return (TT_NODE*)UNKNOWKEY;	
		if(trietree_tmp->slots[offset])
			trietree_tmp = trietree_tmp->slots[offset];
		else
			return NULL;
	}

	return trietree_tmp->data ? trietree_tmp : NULL;
}

/**
 *
 * delete a key from the tree
 *
 */
int trietree_delete( TT_ROOT* trietree_root,char* key )
{
	TT_NODE* trietree_tmp;
	TT_NODE* trietree_tmp2;

	trietree_tmp = trietree_search( trietree_root,key );
	if(!trietree_tmp)
		return -1;

	if(trietree_tmp->data)
	{
		free(trietree_tmp->data);
		trietree_tmp->data=NULL;
	}
	trietree_root->count--;

	while(trietree_tmp->parent)
	{
		if(trietree_tmp->count||trietree_tmp->data)
			break;
		else
		{
			trietree_tmp2=trietree_tmp;
			trietree_tmp = trietree_tmp->parent;
			trietree_tmp->slots[trietree_tmp2->parent_offset]=NULL;
			free(trietree_tmp2);
			trietree_tmp->count--;
		}
	}		
	return 0;
}

/**
 *
 * destory the tree
 *
 */
void _trietree_destory( TT_NODE* trietree_node )
{
	int loop;
	if(!trietree_node)
		return;

	for(loop = TT_DICT_NUM;loop>0;loop--)
	{
		if( trietree_node->slots[loop-1] )
		{
			_trietree_destory(trietree_node->slots[loop-1]);
		}
	}
	if(trietree_node->data)
	{
		free(trietree_node->data);
	}
	free(trietree_node);
}

int trietree_update( TT_ROOT* trietree_root,char* key,void* data,unsigned long datalen)
{
	TT_NODE* trietree_tmp = trietree_search(trietree_root,key);
	if(!trietree_tmp)
	{
		return -1;
	}	
	
	free(trietree_tmp->data);
	trietree_tmp->datalen = datalen;
	trietree_tmp->data = _trietree_malloc(trietree_tmp->datalen + (4-(trietree_tmp->datalen%4)));
	memset(trietree_tmp->data,0x00,trietree_tmp->datalen + (4-(trietree_tmp->datalen%4)));
	memcpy(trietree_tmp->data,data,trietree_tmp->datalen);
	
	return 0;
}

/**
 *
 * destory the tree
 *
 */
void trietree_destory( TT_ROOT* trietree_root )
{
	_trietree_destory( trietree_root->node );
}


int main()
{
	TT_NODE* a;
	printf("%d\n",_trietree_slot_offset('1'));	
	printf("-----------------------------------------------\n");
	trietree_init(root);
	printf("-----------------------------------------------\n");
	trietree_insert(&root,"ab123sgr","abcd",4);
	trietree_insert(&root,"ab123","a",1);
	printf("-----------------------------------------------\n");
	a=trietree_search(&root,"ab123sgr");
	printf("%s\n",a->data);
	printf("-----------------------------------------------\n");
	trietree_delete(&root,"ab123sgr");
	a=trietree_search(&root,"ab123");
	printf("%s\n",a->data);
	printf("-----------------------------------------------\n");
	trietree_update(&root,"ab123","qweqwe",6);
	a=trietree_search(&root,"ab123");
	printf("%s\n",a->data);
	printf("-----------------------------------------------\n");
	trietree_destory(&root);
}






