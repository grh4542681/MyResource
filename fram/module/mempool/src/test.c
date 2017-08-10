



int main()
{
	int ret;
	void *p = NULL;
	void *p1 = NULL;
	printf("%d\n",MEM_ALIGN(126));
	printf("--------------------------------------\n");
	mem_pool_create(pool);
	ret = mem_pool_init(&pool);
	printf("ret=%d\n",ret);
	printf("--------------------------------------\n");
	mem_pool_reset(&pool);
	printf("--------------------------------------\n");
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	printf("1-------------------------------------\n");
	p1=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p1);
	printf("--------------------------------------\n");
	p1=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p1);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	printf("--------------------------------------\n");
	p=_mem_pool_alloc_block(&pool,4096);
	printf("%x\n",p);
	printf("--------------------------------------\n");
	p=_mem_pool_alloc_large(&pool,10240);
	printf("%x\n",p);
	p=_mem_pool_alloc_large(&pool,10240);
	printf("%x\n",p);
	p=_mem_pool_alloc_large(&pool,10240);
	printf("%x\n",p);
	printf("--------------------------------------\n");
	p=mem_pool_palloc(&pool,4096);
	printf("%x\n",p);
	p=mem_pool_palloc(&pool,10240);
	printf("%x\n",p);
	printf("--------------------------------------\n");
	mem_pool_pfree(&pool,p);
	printf("--------------------------------------\n");
	mem_pool_pfree(&pool,p1);
	printf("--------------------------------------\n");
	mem_pool_destory(&pool);
}
