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

symbolTable OpenScope(struct symbolTable ST){
	ST->size++;
	scopeTable s = (scopeTable)malloc(sizeof(struct scopetable));
	s->size = 0;
	s->scope = ST->curr->scope + 1;
	s->head = NULL;
	s->prevScope = ST->curr;
	ST->curr = s;
	return ST;
}

symbolTable closeScope(struct symbolTable ST){
	ST->size--;
	scopeTable s = ST->curr;
	ST->curr = s->prevScope;
	freeScopeTable(s);
	return ST;
}

symbolTable Insert(struct symbolTable ST, char* name,char* type, int lineNo){
	scopeTable s = ST->curr;
	struct entry *newen = (struct entry*)malloc(sizeof(struct entry));
	ST->
}

symbolTable createSymbolTable(parseTree PT, int size){
	
}