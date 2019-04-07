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

int main()
{
	ParseTree nodes[7];
	for(int i = 0;i < 7;i++)
	{
		ParseTree nodes[i] = (ParseTree)malloc(sizeof(parseTree));
		nodes[i]->next = NULL;
		nodes[i]->children = NULL;
		nodes[i]->non_term_id = i;
	}

	node[1]->next = node[2];
	node[2]->next = node[3];
	node[4]->next = node[5];
	node[5]->next = node[6];
	node[2]->children = node[4];
	node[0]->children = node[1];

	printParseTree(node[0],"parseTree.txt");
	del_child(node[0],2);
	printParseTree(node[0],"ParseTree2.txt");
	del_child(node[0],1);
	printParseTree(node[0],"ParseTree3.txt");

	return 0;
}