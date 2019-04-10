#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"	
#include "parser.h"	
#include "ast.h"
#include "symboltableDef.h"
#include "symbolTable.h"

char* symbolArray[] = {"assignmentStmt","global_or_not","booleanExpression","highPrecedenceOperators","singleOrRecId","constructedDatatype","dataType","primitiveDatatype","typeDefinitions","more_ids","idList","relationalOp","temp","output_par","stmt","declarations","input_par","logicalOp","newallvar","program","remaining_list","all","term","stmts","returnStmt","declaration","iterativeStmt","otherFunctions","factor","allnew","new_24","function","arithmeticExpression","var","fieldDefinitions","allVar","mainFunction","funCallStmt","parameter_list","fieldDefinition","typeDefinition","expPrime","outputParameters","termPrime","lowPrecedenceOperators","optionalReturn","elsePart","moreFields","conditionalStmt","otherStmts","ioStmt","inputParameters", "TK_WITH","TK_THEN","TK_TYPE","TK_EQ","TK_DOLLAR","TK_COMMA","TK_RNUM","TK_CL","TK_AND","TK_PARAMETER","TK_DIV","TK_ID","TK_GLOBAL","TK_GE","TK_LIST","TK_IF","TK_RECORD","TK_NUM","TK_NE","TK_LE","TK_DOT","TK_INPUT","TK_MAIN","TK_CALL","TK_PLUS","TK_ELSE","TK_COMMENT","TK_ASSIGNOP","TK_WRITE","TK_COLON","TK_WHILE","TK_REAL","TK_READ","TK_NOT","TK_SQL","TK_OUTPUT","TK_SQR","TK_ENDIF","TK_MINUS","TK_SEM","TK_FIELDID","TK_PARAMETERS","TK_OP","TK_ENDWHILE","TK_MUL","TK_FUNID","TK_INT","TK_RETURN","TK_GT","TK_END","TK_LT","TK_OR","TK_ENDRECORD","eps","TK_RECORDID"};


int hashKey(char *v, int M){ 
	int h = 0, a = 257;
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}


Records enterRecords(ParseTree PT){
	Records rs = (struct records*)malloc(sizeof(struct records));
	rs->head = NULL;
	rs->count=0;
	ParseTree func = PT->children;
	while(func){
		ParseTree stmts;
		if(func->non_term_id==function){
			stmts = func->children->next->next->next;
		}
		else if(func->non_term_id==mainFunction){
			stmts = func->children;
		}
		else{
			return rs;
		}
		if(stmts!=NULL && stmts->children!=NULL && stmts->children->non_term_id==typeDefinitions){
			ParseTree td = stmts->children->children;
			while(td){
				Record r = (Record)malloc(sizeof(struct record));
				r->name = (char*)malloc(sizeof(char)*strlen(td->children->tk->name)+1);
				strcpy(r->name, td->children->tk->name);
				r->size=0;
				r->head = NULL;
				ParseTree fd = td->children->next;

				while(fd){
					rec_dec rd = (rec_dec)malloc(sizeof(struct recordDecl));
					rd->type = (char*)malloc(sizeof(char)*strlen(fd->children->tk->name)+1);
					rd->name = (char*)malloc(sizeof(char)*strlen(fd->children->next->tk->name)+1);
					strcpy(rd->type, fd->children->tk->name);
					strcpy(rd->name, fd->children->next->tk->name);
					rd->next = r->head;
					r->head=rd;
					fd=fd->next;
					r->size++;
				}
				rs->count++;
				r->next = rs->head;
				rs->head = r;
				td=td->next;
			}
		}
		func = func->next;
	}
	return rs;
}

// void printRecords(){

// }

symbolTable openScope(symbolTable ST,int sz,char* scope){
	ST.size++;
	scopeTable s = (scopeTable)malloc(sizeof(struct scopetable));
	s->size = sz;
	strcpy(s->scope, scope);
	s->arr = (entry**)malloc(sizeof(entry*)*sz);
	for(int i = 0;i < sz;i++)
		s->arr[i] == NULL;
	if(ST.curr){
		s->prevScope = ST.curr->prevScope;
		ST.curr->prevScope = s;
	}
	else{
		s->prevScope = NULL;
		ST.curr = s;
	}
	return ST;
}

symbolTable closeScope(symbolTable ST){
	ST.size--;
	scopeTable s = ST.curr;
	ST.curr = s->prevScope;
	// freeScopeTable(s);
	return ST;
}

int numberOfDeclarations(ParseTree PT){
	int count = 0;
	if(PT==NULL)
		return count;
	ParseTree input_par = PT->children->next;
}

symbolTable insert(symbolTable ST, entry *en){
	scopeTable s = ST.curr->prevScope;
	scopeTable s1 = ST.curr;
	int in = hashKey(en->name,s->size);
	if(strcmp(en->scope, "global")){
		entry* ptr = s1->arr[in];
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
	}
	else{

	}

	return ST;
}

entry* lookup(symbolTable ST, char* name){
	scopeTable ptr = ST.curr;
	int in;

	while(ptr)
	{
		in = hashKey(name,ptr->size);
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
// symbolTable createSymbolTable(ParseTree PT, int size){
// 	symbolTable ST;
// 	ST.size=0;
// 	ST=openScope(ST,50,"global");
// 	ParseTree ptr= PT->children;
// 	if(PT==NULL)
// 		return ST;
// 	else{
// 		while(ptr){
// 			entry *en=(entry*)malloc(sizeof(entry));
// 			en->name=ptr->next->tk->name;
// 			en->type="function";
// 			en->lineNo=ptr->next->tk->lineNo;
// 			ST=insert(ST,en)
// 			int size=noOfDeclarations(ptr);
// 			ST=openScope(ST,size,en->name);
			
// 			while(ptr)

// 		}
// 	}

// }


symbolTable addInGlobal(symbolTable ST,entry *en){

}
// entry* makeRecordEntry(ParseTree PT);


symbolTable createSymbolTable(ParseTree PT, int size){
	symbolTable ST;
	ST.size=0;
	
}


