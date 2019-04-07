#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.c"
// #include "ast.h"

ParseTree ast;

ParseTree del_child(ParseTree node,int i)
{
	ParseTree ptr = node->children;
	if(i == 0)
	{
		if(ptr->children == NULL)
		{
			node->children = ptr->next;
			free(ptr);
		}
		else
		{
			ParseTree tmp = ptr->next;
			node->children = ptr->children;
			ParseTree ptr2 = ptr->children;
			free(ptr);
			while(ptr2 != NULL && ptr2->next)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			ptr2->parent = node;
			ptr2->next = tmp;
		}
	}
	else
	{
		int count = 0;
		ParseTree prev;

		while(count < i && ptr)
		{
			prev = ptr;
			ptr = ptr->next;
			count++;
		}
		if(ptr->next == NULL)
		{
			prev->next = ptr->children;
			ParseTree ptr2 = ptr->children;
			while(ptr2)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			free(ptr);
		}
		else
		{
			prev->next = ptr->children;
			ParseTree ptr2 = ptr->children;
			while(ptr2 && ptr2->next)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			ptr2->parent = node;
			ptr2->next = ptr->next;
			free(ptr);
		}
	}

	return node;
}