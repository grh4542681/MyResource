/**
 *
 *
 *				memory pool(thread no safe)
 *
 *
 *
 * 								by GRH 20160517
 *
 */

#include"mem_pool.h"

void* mem_pool_malloc(MEM_POOL_UINT size)
{
	void *p;
	p = malloc(size);
	if(!p)
	{
		return NULL;
	}	
	memset(p,0x00,size);
	return p;
}

void mem_pool_free(void *p)
{
	free(p);
}

/**
 *
 * mem pool init(1 page default)
 *
 */
int mem_pool_init(struct MEM_POOL_HEAD* pool_head)
{
	pool_head->page_size = MEM_PAGE_SIZE;
	pool_head->page_list = mem_pool_malloc(pool_head->page_size);
	if(!pool_head->page_list)
	{
		return -1;
	}

	memset(pool_head->page_list ,0x00 ,pool_head->page_size);

	pool_head->page_list->count = 0;
	pool_head->page_list->fail_num = 0;
	pool_head->page_list->free = pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK));
	pool_head->page_list->pfree = (MEM_POOL_ADDRESS*)(pool_head->page_list + MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)));
	pool_head->page_list->next = NULL;
	pool_head->page_list->prev = NULL;

	pool_head->page_num = 1;
	pool_head->page_current = pool_head->page_list;
	pool_head->lpage_list = NULL;	
	
	return 0;
}

/**
 *
 * memory pool reset
 *
 */
void mem_pool_reset(struct MEM_POOL_HEAD* pool_head)
{
	struct _MEM_POOL_BLOCK* pool_page_tmp = NULL;
	struct _MEM_POOL_LARGE* pool_lpage_tmp1 = NULL;
	struct _MEM_POOL_LARGE* pool_lpage_tmp2 = NULL;
	
	pool_page_tmp = pool_head->page_list;

	while( !pool_page_tmp )
	{
		pool_page_tmp->count = 0;
		pool_page_tmp->fail_num = 0;
		pool_page_tmp->free = pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK));
		pool_page_tmp->pfree = (MEM_POOL_ADDRESS*)(pool_head->page_list + MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)));
		pool_page_tmp = pool_page_tmp->next;
	}
	pool_head->page_current = pool_head->page_list;

	if( pool_head->lpage_list )
	{
		for(pool_lpage_tmp1=pool_head->lpage_list,pool_lpage_tmp2=pool_lpage_tmp1->next;;pool_lpage_tmp1=pool_lpage_tmp2,pool_lpage_tmp2=pool_lpage_tmp1->next)
		{
			mem_pool_free(pool_lpage_tmp1);
			if(!pool_lpage_tmp2)
			{
				break;
			}
		}
	}
}

/**
 *
 * memory pool destory
 *
 */
void mem_pool_destory(struct MEM_POOL_HEAD* pool_head)
{
	struct _MEM_POOL_BLOCK* pool_page_tmp1 = NULL;
	struct _MEM_POOL_BLOCK* pool_page_tmp2 = NULL;
	struct _MEM_POOL_LARGE* pool_lpage_tmp1 = NULL;
	struct _MEM_POOL_LARGE* pool_lpage_tmp2 = NULL;
	
	if( pool_head->page_list )
	{
		for(pool_page_tmp1=pool_head->page_list,pool_page_tmp2=pool_page_tmp1->next;;pool_page_tmp1=pool_page_tmp2,pool_page_tmp2=pool_page_tmp1->next)
		{
			mem_pool_free(pool_page_tmp1);
			printf("--free page\n");
			if(!pool_page_tmp2)
			{
				break;
			}
		}
	}

	if( pool_head->lpage_list )
	{
		for(pool_lpage_tmp1=pool_head->lpage_list,pool_lpage_tmp2=pool_lpage_tmp1->next;;pool_lpage_tmp1=pool_lpage_tmp2,pool_lpage_tmp2=pool_lpage_tmp1->next)
		{
			mem_pool_free(pool_lpage_tmp1);
			printf("--free lpage\n");
			if(!pool_lpage_tmp2)
			{
				break;
			}
		}
	}
	memset(pool_head,0x00,sizeof(struct MEM_POOL_HEAD));
}

/**
 *
 * alloc memory on page
 *
 */
static MEM_POOL_ADDRESS *_mem_pool_alloc_block(struct MEM_POOL_HEAD* pool_head,MEM_POOL_UINT size)
{
	void *p = NULL;
	struct _MEM_POOL_BLOCK* page_tmp1 = NULL;
	struct _MEM_POOL_BLOCK* page_tmp2 = NULL;
	struct _MEM_POOL_BLOCK* page_alloc = NULL;
	struct _MEM_POOL_BLOCK* page_tail = NULL;

	// find a free page
	for(page_tmp1=pool_head->page_current,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
	{
		if(page_tmp1->free >= size)
		{
			page_alloc = page_tmp1;
			break;
		}
		page_tmp1->fail_num++;
		if( !page_tmp2 )
		{
			break;
		}
	}
	
	// alloc a new page
	if( !page_alloc )
	{
		page_tail = mem_pool_malloc(pool_head->page_size);
		if( !page_tail )
		{
			return NULL;
		}
		
		memset(page_tail, 0x00, pool_head->page_size);
		page_tail->count = 0;
		page_tail->fail_num = 0;
		page_tail->free = pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK));
		page_tail->pfree = (MEM_POOL_ADDRESS*)(page_tail + MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)));
		page_tail->next = NULL;
		page_tail->prev = page_tmp1;
		page_tmp1->next = page_tail;

		pool_head->page_num++;
		page_alloc = page_tmp1->next;
	}

	// update page current
	for(page_tmp1=pool_head->page_current,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
	{
		if((page_tmp1->fail_num < 8)||(!page_tmp2))
		{
			pool_head->page_current = page_tmp1;
			break;
		}
	}

	// update page
	page_alloc->free -= size;
	p = page_alloc->pfree;
	page_alloc->pfree += size;
	page_alloc->count++;

	return p;
}

/**
 *
 * alloc memory on system by large size
 *
 */
static MEM_POOL_ADDRESS *_mem_pool_alloc_large(struct MEM_POOL_HEAD* pool_head,MEM_POOL_UINT size)
{
	void *p = NULL;
	struct _MEM_POOL_LARGE* page_tmp = NULL;
	struct _MEM_POOL_LARGE* page_node = NULL;

	size = size + MEM_ALIGN(sizeof(struct _MEM_POOL_LARGE));
	page_tmp = pool_head->lpage_list;
	
	if( !page_tmp )
	{
		p = mem_pool_malloc(size);
		if( !p )
		{
			return NULL;
		}
		memset( p, 0x00, size);
		pool_head->lpage_list = p;
		pool_head->lpage_list->next = NULL;
		pool_head->lpage_list->prev = NULL;
	}
	else
	{
		while( page_tmp->next )
		{
			page_tmp = page_tmp->next;
		}
		p = mem_pool_malloc(size);
		if( !p )
		{
			return NULL;
		}
		memset( p, 0x00, size);
		page_tmp->next = p;
		page_tmp->next->prev = page_tmp;
	}
	return (MEM_POOL_ADDRESS*)p + MEM_ALIGN(sizeof(struct _MEM_POOL_LARGE));
}

/**
 *
 * alloc memory align
 *
 */
MEM_POOL_ADDRESS *mem_pool_palloc(struct MEM_POOL_HEAD* pool_head,MEM_POOL_UINT size)
{
	if(MEM_ALIGN(size) >= pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)))
	{
		return _mem_pool_alloc_large(pool_head,MEM_ALIGN(size));
	}
	else
	{
		return _mem_pool_alloc_block(pool_head,MEM_ALIGN(size));
	}
}

/**
 *
 * alloc memmory no align
 *
 */
MEM_POOL_ADDRESS *mem_pool_pnalloc(struct MEM_POOL_HEAD* pool_head,MEM_POOL_UINT size)
{
	if(size >= pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)))
	{
		return _mem_pool_alloc_large(pool_head,size);
	}
	else
	{
		return _mem_pool_alloc_block(pool_head,size);
	}

}

/**
 *
 * free page memory
 *
 */
static void _mem_pool_free_block(struct MEM_POOL_HEAD* pool_head,MEM_POOL_ADDRESS* ptr)
{
	struct _MEM_POOL_BLOCK* page_tmp1 = NULL;
	struct _MEM_POOL_BLOCK* page_tmp2 = NULL;
	struct _MEM_POOL_BLOCK* page_curr = NULL;
	
	for(page_tmp1=pool_head->page_list,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
	{
		if( ((ptr - (MEM_POOL_ADDRESS*)page_tmp1) > 0) && ((ptr - (MEM_POOL_ADDRESS*)page_tmp1) <= pool_head->page_size) )
		{
			page_curr = page_tmp1;
			break;
		}
		if( !page_tmp2 )
		{
			break;
		}
	}
	if( !page_curr )
	{
		free(ptr);
		return;
	} 
	page_curr->count--;

	if( page_curr->count <= 0 && page_curr->fail_num >= 8 )
	{
		page_curr->count = 0;
		page_curr->fail_num = 0;
		page_curr->free = pool_head->page_size - MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK));
		page_curr->pfree = (MEM_POOL_ADDRESS*)(page_curr + MEM_ALIGN(sizeof(struct _MEM_POOL_BLOCK)));
		if( !page_curr->prev && !page_curr->next )
		{
			
		}
		else if( !page_curr->prev )
		{
			pool_head->page_list = page_curr->next;
			for(page_tmp1=pool_head->page_list,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
			{
				if( !page_tmp2 )
				{
					break;
				}
			}
			page_tmp1->next = page_curr;
			page_curr->prev = page_tmp1;
			page_curr->next = NULL;
		}
		else
		{
			page_curr->next->prev = page_curr->prev;
			page_curr->prev->next = page_curr->next;
			for(page_tmp1=page_curr,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
			{
				if( !page_tmp2 )
				{
					break;
				}
			}
			page_tmp1->next = page_curr;
			page_curr->prev = page_tmp1;
			page_curr->next = NULL;
		}
	}
}

/**
 *
 * free large memory
 *
 */
static void _mem_pool_free_large(struct MEM_POOL_HEAD* pool_head,MEM_POOL_ADDRESS* ptr)
{
	struct _MEM_POOL_LARGE* page_tmp = (struct _MEM_POOL_LARGE*)ptr;
	if( !page_tmp->prev && !page_tmp->next )
	{
		pool_head->lpage_list = NULL;
	}
	else if( !page_tmp->prev )
	{
		page_tmp->next->prev = NULL;
		pool_head->lpage_list = page_tmp->next;
	}
	else if( !page_tmp->next )
	{
		page_tmp->prev->next = NULL;
	}
	else
	{
		page_tmp->prev->next = page_tmp->next;
		page_tmp->next->prev = page_tmp->prev;
	}
	mem_pool_free(page_tmp);
	page_tmp = NULL;
	ptr = NULL;
}

/**
 *
 * free memory in pool
 *
 */
void mem_pool_pfree(struct MEM_POOL_HEAD* pool_head,MEM_POOL_ADDRESS* ptr)
{
	struct _MEM_POOL_LARGE* page_tmp1 = NULL;
	struct _MEM_POOL_LARGE* page_tmp2 = NULL;
	if( pool_head->lpage_list )
	{
		for(page_tmp1=pool_head->lpage_list,page_tmp2=page_tmp1->next;;page_tmp1=page_tmp2,page_tmp2=page_tmp2->next)
		{
			if(page_tmp1 == (struct _MEM_POOL_LARGE*)(ptr-(MEM_ALIGN(sizeof(struct _MEM_POOL_LARGE)))));
			{
				_mem_pool_free_large(pool_head,ptr-(MEM_ALIGN(sizeof(struct _MEM_POOL_LARGE))));
				return;
			}
			if(!page_tmp2)
			{
				break;
			}
		}
	}
	_mem_pool_free_block(pool_head,ptr);
}

