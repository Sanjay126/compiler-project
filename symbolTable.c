#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"	
#include "parser.h"	
#include "ast.h"
#include "symbolTableDef.h"
#include "symbolTable.h"

int hash(char *v, int M){ 
	int h = 0, a = 257;
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}

symbolTable openScope(symbolTable ST,int sz){
	ST.size++;
	scopeTable s = (scopeTable)malloc(sizeof(struct scopetable));
	s->size = sz;
	if(ST.curr)
		s->scope = ST.curr->scope + 1;
	else
		s->scope = 1;
	s->arr = (entry**)malloc(sizeof(entry*)*sz);
	for(int i = 0;i < sz;i++)
		s->arr[i] == NULL;
	s->prevScope = ST.curr;
	ST.curr = s;
	return ST;
}

symbolTable closeScope(symbolTable ST){
	ST.size--;
	scopeTable s = ST.curr;
	ST.curr = s->prevScope;
	// freeScopeTable(s);
	return ST;
}

symbolTable insert(symbolTable ST, entry *en){
	scopeTable s = ST.curr;
	int in = hash(en->name,s->size);
	entry* ptr = s->arr[in];
	entry* prev = NULL;
	
	if(ptr == NULL)
		s->arr[in] = en;
	else
	{
		while(ptr)
		{
			prev = ptr;
			ptr = ptr->next;
		}
		prev->next = en;
		en->next = NULL;
	}

	return ST;
}

entry* lookup(symbolTable ST, char* name)
{
	scopeTable ptr = ST.curr;
	int in;

	while(ptr)
	{
		in = hash(name,ptr->size);
		if(ptr->arr[in] = NULL)
			ptr = ptr->prevScope;
		else
		{
			entry* ptr2 = ptr->arr[in];
			while(ptr2)
			{
				if(strcmp(ptr2->name,name) == 0)
					return ptr2;
				ptr2 = ptr2->next;
			}
			ptr = ptr->prevScope;
		}
	}

	return NULL;
}

void freeScopeTable(scopeTable s){
	for(int i = 0;i < s->size;i++)
		free(s->arr[i]);
	free(s->arr);
	free(s);
}