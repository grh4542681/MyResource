#include "list.h"

void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

void _list_add(struct list_head *new,struct list_head *prev,struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

void list_add(struct list_head *new, struct list_head *head)
{
	_list_add(new, head, head->next);
}

void list_add_tail(struct list_head *new, struct list_head *head)
{
	_list_add(new, head->prev, head);
}


void _list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(struct list_head *del)
{
	_list_del(del->prev,del->next);
}

int list_empty(const struct list_head *head)
{
	return head->next == head;
}