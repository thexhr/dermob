/*-
 * Copyright (c) 2006 Matthias Schmidt <xhr @ staatsfeind . org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS'
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/* $Id: list.c,v 1.2 2006/08/15 12:52:48 matthias Exp $ */

#include <errno.h>

#include "dermob.h"
#include "list.h"
#include "mach.h"

void print_section(struct section *);
void print_load_command(struct load_command *);
void print_mo_header(struct mach_header *);
void print_fat_header(struct fat_header *);
void print_fat_arch(struct fat_arch *);
void print_lc_towlevel_hints(struct twolevel_hints_command *two);
void display_cmd_name(int);
void display_cpu_arch(int);
void print_lc_towlevel_hints(struct twolevel_hints_command *two);
void print_lc_dysymtab(struct dysymtab_command *dsym);
void print_lc_load_dylib(struct dylib_command *dly);
void print_lc_segment(struct segment_command *sc);
void print_lc_symtab(struct symtab_command *symc);
void mprintf(const char *fmt, ...);

struct list *
list_create_list()
{
	struct list	*list;

	list = malloc(sizeof(struct list));

	if (list != NULL) {
		list->head = list->tail = NULL;
		list->len = 0;
		return (list);
	}

	return (NULL);
}

void
list_insert_node(struct list *list, void *content, unsigned int code)
{
	struct node	*new;

	new = malloc(sizeof(struct node));

	if (!new) {
		printf("malloc: %s\n", strerror(errno));
		return;
	}

	new->content = content;
	new->code = code;
	
	if (list->head == NULL) {
		list->head = new;
		list->tail = new;
		/* We'll get two uninitialised pointer at the first node, if we
		 * don't set prev and next to NULL */
		new->next = new->prev = NULL;
		list->len = 1;
	} else {
		list->head->prev = new;
		new->next = list->head;
		new->prev = NULL;
		list->head = new;
		list->len++;
	}
}

void
list_traverse_list(struct list *list)
{
	struct node	*cursor;
	int na, nl, ns;
	
	na = nl = ns = 0;
	
	// Traverse the list backwards
	for (cursor = list->tail; cursor != NULL; cursor = cursor->prev) {
		if (cursor->code == 0x1)
			print_fat_header((struct fat_header *)cursor->content);
		else if (cursor->code == 0x2) {
			mprintf(" Architecture %d\n", ++na);
			print_fat_arch((struct fat_arch *) cursor->content);
		} else if (cursor->code == 0x3)
			print_mo_header((struct mach_header *) cursor->content);
		else if (cursor->code == 0x4) {
			mprintf(" - Load command:	%d\n", ++nl);
			print_load_command((struct load_command *) cursor->content);
		} else if (cursor->code == 0x5) {
			mprintf("   + Section %d\n", ++ns);
			print_section((struct section *) cursor->content);
		} else if (cursor->code == 0x6)
			print_lc_segment((struct segment_command *) cursor->content);
		else if (cursor->code == 0x7)
			print_lc_symtab((struct symtab_command *) cursor->content);
		else if (cursor->code == 0x8)
			print_lc_load_dylib((struct dylib_command *) cursor->content);
		else if (cursor->code == 0x9)
			print_lc_dysymtab((struct dysymtab_command *) cursor->content);
	}
	
}



void
list_free_list(struct list *list)
{
	struct node	*cursor;
	
	while ((cursor = list->head)) {
		list->head = cursor->next;
		free(cursor->content);
		free(cursor);
	}
	
	free(list);
}

void
list_remove_node(struct list *list, struct node *temp)
{
	/* cursor is the head of the list */
	if ((temp == list->head) && (temp != list->tail)) {
		list->head = temp->next;
		list->head->prev = NULL;
	/* cursor is the tail of the list */
	} else if ((temp != list->head) && (temp == list->tail)) {
		list->tail = temp->prev;
		list->tail->next = NULL;
	/* cursor is the only node */
	} else if ((temp == list->head) && (temp == list->tail)) {
		list->tail = list->head = NULL;
	/* cursor is between head and tail */
	} else {
		temp->prev->next = temp->next;
		temp->next->prev = temp->prev;
	}
	
	free(temp->content);
	free(temp);
}

void
list_remove(struct list *list, int (*cond)(struct node*))
{
	struct node	*cursor,
			*temp;

	cursor = list->head;
	while (cursor != NULL) {
		temp = cursor;

		cursor = (cursor != list->tail) ? cursor->next : NULL;

		if ((*cond)(temp)) 
			list_remove_node(list, temp);
	} 
}
