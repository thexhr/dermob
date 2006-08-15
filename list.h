#ifndef __LIST_H
#define __LIST_H

#define OPT_LEN	312

struct list * list_create_list();
void list_insert_node(struct list *list, void *content, unsigned int code);
void list_free_list(struct list *list);
void list_traverse_list(struct list *list);

struct list
{
	struct node	*head;
	struct node	*tail;
	unsigned int	len;	
};

struct node
{
	void		*content;
	unsigned int	code;
	struct node 	*next;
	struct node 	*prev;
};

#endif
