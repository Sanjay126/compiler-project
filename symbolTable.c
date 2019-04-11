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

// char* symbolArray[] = {"assignmentStmt","global_or_not","booleanExpression","highPrecedenceOperators","singleOrRecId","constructedDatatype","dataType","primitiveDatatype","typeDefinitions","more_ids","idList","relationalOp","temp","output_par","stmt","declarations","input_par","logicalOp","newallvar","program","remaining_list","all","term","stmts","returnStmt","declaration","iterativeStmt","otherFunctions","factor","allnew","new_24","function","arithmeticExpression","var","fieldDefinitions","allVar","mainFunction","funCallStmt","parameter_list","fieldDefinition","typeDefinition","expPrime","outputParameters","termPrime","lowPrecedenceOperators","optionalReturn","elsePart","moreFields","conditionalStmt","otherStmts","ioStmt","inputParameters", "TK_WITH","TK_THEN","TK_TYPE","TK_EQ","TK_DOLLAR","TK_COMMA","TK_RNUM","TK_CL","TK_AND","TK_PARAMETER","TK_DIV","TK_ID","TK_GLOBAL","TK_GE","TK_LIST","TK_IF","TK_RECORD","TK_NUM","TK_NE","TK_LE","TK_DOT","TK_INPUT","TK_MAIN","TK_CALL","TK_PLUS","TK_ELSE","TK_COMMENT","TK_ASSIGNOP","TK_WRITE","TK_COLON","TK_WHILE","TK_REAL","TK_READ","TK_NOT","TK_SQL","TK_OUTPUT","TK_SQR","TK_ENDIF","TK_MINUS","TK_SEM","TK_FIELDID","TK_PARAMETERS","TK_OP","TK_ENDWHILE","TK_MUL","TK_FUNID","TK_INT","TK_RETURN","TK_GT","TK_END","TK_LT","TK_OR","TK_ENDRECORD","eps","TK_RECORDID"};


int hashKey(char *v, int M){ 
	int h = 0, a = 257;
	// printf("HASH -- %s ---\n", v);
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}


symbolTable* enterRecords(ParseTree PT,symbolTable* ST){
	ST->recs = (struct records*)malloc(sizeof(struct records));
	ST->recs->head = NULL;
	ST->recs->count=0;
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
			return ST;
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
				ST->recs->count++;
				r->next = ST->recs->head;
				ST->recs->head = r;
				td=td->next;
			}
		}
		func = func->next;
	}
	return ST;
}


symbolTable *createSymbolTable(ParseTree PT){
	ParseTree func = PT->children;
	int gcount = 0;
	entry* head = NULL;
	while(func){
		ParseTree stmts;		
		entry* newen = (entry*)malloc(sizeof(entry));
		int varCount  = 0;

		if(func->non_term_id==function){
			stmts = func->children->next->next->next;
		}
		else if(func->non_term_id==mainFunction){
			stmts = func->children;
		}
		ParseTree decl;
		if(stmts->children->non_term_id == declarations)
			decl = stmts->children->children;
		else if(stmts->children->next->non_term_id == declarations)
			decl = stmts->children->next->children;
		else
		{
			decl = NULL;
		}

		while(decl)
		{
			entry* newen2 = (entry*)malloc(sizeof(entry));
			newen2->type = (char*)malloc(sizeof(char)*(strlen(decl->children->tk->name)+1));
			newen2->name = (char*)malloc(sizeof(char)*(strlen(decl->children->next->tk->name)+1));
			strcpy(newen2->type,decl->children->tk->name);
			strcpy(newen2->name,decl->children->next->tk->name);
			newen2->lineNo = decl->children->tk->lineNo;
			newen2->global = 0;
			newen2->inout = 0;
			newen2->record_or_not = 0;
			if(decl->children->tk->tid == TK_RECORDID)
				newen2->record_or_not = 1;
			if(decl->children->next->next)
			{
				newen2->global = 1;
				gcount++;
			}
			else{
				varCount++;
			}
			newen2->next = head;
			head = newen2;
			decl = decl->next;
		}		
		gcount++;

		if(func->non_term_id==function){
			ParseTree in = func->children->next->children;
			ParseTree out = func->children->next->next->children;
			while(out)
			{
				entry* newen2 = (entry*)malloc(sizeof(struct entry));
				newen2->type = (char*)malloc(sizeof(char)*(strlen(out->tk->name)+1));
				newen2->name = (char*)malloc(sizeof(char)*(strlen(out->next->tk->name)+1));
				newen2->scope = (char*)malloc(sizeof(char)*(strlen(func->children->tk->name)+1));
				strcpy(newen2->scope,func->children->tk->name);
				strcpy(newen2->type,out->tk->name);
				strcpy(newen2->name,out->next->tk->name);
				newen2->lineNo = out->tk->lineNo;
				newen2->inout = 2;
				newen2->global = 0;
				newen2->next = head;
				head = newen2;
				newen2->record_or_not = 0;
				if(out->tk->tid == TK_RECORDID)
					newen2->record_or_not = 1;
				varCount++;
				out = out->next->next;
			}
			while(in)
			{
				entry* newen2 = (entry*)malloc(sizeof(struct entry));
				newen2->type = (char*)malloc(sizeof(char)*(strlen(in->tk->name)+1));
				newen2->name = (char*)malloc(sizeof(char)*(strlen(in->next->tk->name)+1));
				newen2->scope = (char*)malloc(sizeof(char)*(strlen(func->children->tk->name)+1));
				strcpy(newen2->type,in->tk->name);
				strcpy(newen2->name,in->next->tk->name);
				strcpy(newen2->scope,func->children->tk->name);
				newen2->lineNo = in->tk->lineNo;
				newen2->inout = 1;
				newen2->global = 0;
				newen2->next = head;
				head = newen2;
				newen2->record_or_not = 0;
				if(in->tk->tid == TK_RECORDID)
					newen2->record_or_not = 1;
				varCount++;
				in = in->next->next;
			}
			newen->type = (char*)malloc(sizeof(char)*(strlen("function")+1));
			newen->name = (char*)malloc(sizeof(char)*(strlen(func->children->tk->name)+1));
			newen->scope = (char*)malloc(sizeof(char)*(strlen("global")+1));
			strcpy(newen->scope,"global");
			strcpy(newen->type,"function");
			strcpy(newen->name,func->children->tk->name);
			newen->countVariables = varCount;
			newen->lineNo = func->children->tk->lineNo;
			newen->global = 1;
			newen->inout = 0;
			newen->next = head;
			head = newen;

		}
		else if(func->non_term_id==mainFunction){
			newen->type = (char*)malloc(sizeof(char)*(strlen("function")+1));
			newen->name = (char*)malloc(sizeof(char)*(strlen("mainFunction")+1));
			newen->scope = (char*)malloc(sizeof(char)*(strlen("global")+1));
			strcpy(newen->scope,"global");
			strcpy(newen->type,"function");
			strcpy(newen->name,"mainFunction");
			newen->countVariables = varCount;
			newen->global = 1;
			newen->inout = 0;
			newen->next = head;
			head = newen;
		}


		func  = func->next;
	}

	symbolTable *ST  = (symbolTable*)malloc(sizeof(symbolTable));
	
	ST->size = 0;
	ST->curr = NULL;
	ST =  enterRecords(PT,ST);
	ST = openScope(ST,gcount,"global");


	entry* ptr = head;
	// entry* prev = head;
	while(ptr)
	{
		printf("%s--->", ptr->name);
		if(ptr->global){
			ST = insert(ST, ptr);
		}
		// printf("%s - %s - %d\n", ptr->name, ptr->type, ptr->global);
		ptr = ptr->next;
	}

	printf("\n");
	ptr = head;
	while(ptr)
	{
		if(strcmp(ptr->type,"function") == 0)
		{
			entry* funcEntry = lookup(ST, ptr->name);
			ST =  openScope(ST,ptr->countVariables,ptr->name);
			funcEntry->funcScopePtr = ST->curr;
			entry* funcPtrr = ptr;
			ptr = ptr->next;

			while(ptr && strcmp(ptr->type,"function") != 0){
				if(strcmp(funcPtrr->name,"mainFunction") != 0){
					while(ptr && ptr->inout == 1 && !ptr->global){
						ST = insert(ST,ptr);
						ParamNode newNode = (ParamNode)malloc(sizeof(struct paramNode));
						newNode->paramName = (char*)malloc(strlen(ptr->name)+1);
						strcpy(newNode->paramName,ptr->name);
						newNode->next = ST->curr->inputParams;
						ST->curr->inputParams = newNode;
						ptr = ptr->next;
					}


					while(ptr && ptr->inout == 2 && !ptr->global){
						ST = insert(ST,ptr);
						ParamNode newNode = (ParamNode)malloc(sizeof(struct paramNode));
						newNode->paramName = (char*)malloc(strlen(ptr->name)+1);
						strcpy(newNode->paramName,ptr->name);
						newNode->next = ST->curr->inputParams;
						ST->curr->inputParams = newNode;
						ptr = ptr->next;
					}
				}
				if(ptr  &&  !ptr->global)
					ST = insert(ST,ptr);
				if(ptr)
					ptr = ptr->next;
			}
			ST = closeScope(ST);
		}
	}

	return ST;
}


symbolTable *openScope(symbolTable *ST,int sz,char* scope){
	ST->size++;
	scopeTable s = (scopeTable)malloc(sizeof(struct scopetable));
	s->size = sz;
	s->scope = (char*)malloc(sizeof(char)*(strlen(scope)+1));
	strcpy(s->scope, scope);
	s->arr = (entry**)malloc(sizeof(entry*)*sz);
	for(int i = 0;i < sz;i++)
		s->arr[i] = NULL;

	s->prevScope = ST->curr;
	ST->curr = s;
	return ST;
}

symbolTable* closeScope(symbolTable* ST){
	// ST->size--;
	// scopeTable s = ST->curr;
	ST->curr = ST->curr->prevScope;
	// freeScopeTable(s);
	return ST;
}

int numberOfDeclarations(ParseTree PT){
	int count = 0;
	if(PT==NULL)
		return count;
	ParseTree input_par = PT->children->next;
}

symbolTable* insert(symbolTable* ST, entry *en){
	// printf("aaa--%s\n", en->name);
	scopeTable s;
	int in;
	entry* lookedup=lookup(ST,en->name);
	if(lookedup){

		printf("variable already declared %s\n",en->name);
		//TODO error reporting
		// printf("\naa%daa\n", lookedup->global);
		return ST;
	}
	// if(strcmp(en->scope, "global")==0){
	// 	s = ST->curr->prevScope;
	// }
	// else{
	s = ST->curr;
	// }
	if(en->record_or_not==1){
		Record rec=lookupRecord(ST->recs,en->type);
		if(rec){
			// en->record = rec->head;
			rec_dec recPtr = rec->head;
			RecordValue head = (RecordValue)malloc(sizeof(struct recordValue));
			RecordValue ptr = head, curr;
			while(recPtr){
				ptr->name = (char*)malloc((strlen(recPtr->name)+1)*sizeof(char));
				strcpy(ptr->name, recPtr->name);
				if(strcmp(recPtr->type, "int")){
					ptr->isInt = 1;
					// ptr->value=0;
				}
				else{
					ptr->isInt = 0;
				}
				recPtr = recPtr->next;
				if(recPtr){
					curr = (RecordValue)malloc(sizeof(struct recordValue));
					curr->next = NULL;
					ptr->next = curr;
					ptr = curr;
				}
			}
			en->recVal = head;
			//TODO  make a recval  and add  entry to  scopeTable
		}
		else{
			printf("no such  record defined %s",en->type);
			//TODO report error
			return ST;
		}
	}
	in = hashKey(en->name,s->size);

	entry* en1 = (entry*)malloc(sizeof(entry));
	en1->name = (char*)malloc(sizeof(char)*(strlen(en->name)+1));
	en1->type = (char*)malloc(sizeof(char)*(strlen(en->type)+1));
	if(en->scope){
		en1->scope = (char*)malloc(sizeof(char)*(strlen(en->scope)+1));
		strcpy(en1->scope, en->scope);
	}
	strcpy(en1->name, en->name);
	strcpy(en1->type, en->type);
	en1->lineNo =  en->lineNo;
	en1->global =  en->global;
	en1->next =  NULL;
	en1->recVal =  en->recVal;
	en1->record_or_not =  en->record_or_not;
	en1->inout =  en->inout;
	en1->countVariables =  en->countVariables;
	en1->funcScopePtr =  en->funcScopePtr;
	entry* ptr = s->arr[in];
	entry* prev = NULL;

	if(ptr == NULL)
		s->arr[in] = en1;
	else
	{
		while(ptr)
		{
			prev = ptr;
			ptr = ptr->next;
		}
		prev->next = en1;
	}

	return ST;
}

entry* lookup(symbolTable* ST, char* name){
	if(!ST || ST->curr->size ==  0)
		return  NULL;
	scopeTable ptr = ST->curr;
	int in;
	while(ptr!=NULL)
	{
		in = hashKey(name,ptr->size);
		if(ptr->arr[in] != NULL)
		{
			entry* ptr2 = ptr->arr[in];
			while(ptr2)
			{
				if(strcmp(ptr2->name,name) == 0)
					return ptr2;
				ptr2 = ptr2->next;
			}
		}
		ptr = ptr->prevScope;
	}
	
	return NULL;
}
Record  lookupRecord(Records r,char* name){
	Record record=r->head;
	while(record){
		if(strcmp(record->name,name)==0)
			return record;
		record=record->next;
	}
	return NULL;
}

void freeScopeTable(scopeTable s){
	for(int i = 0;i < s->size;i++)
		free(s->arr[i]);
	free(s->arr);
	free(s);
}

void printScopeTable(scopeTable s, int spaces){
	entry** arr = s->arr;
	for(int i=0; i<s->size; i++){
		if(arr[i]!=NULL){
			entry* ptr = arr[i];
			while(ptr){
				for(int j=0; j<spaces; j++)
					printf("\t\t");
				printf("TYPE  --  %20s\t\t NAME  --  %20s\n", ptr->type, ptr->name);
				if(strcmp(ptr->type, "function")==0)
					printScopeTable(ptr->funcScopePtr, spaces+1);
				ptr=ptr->next;
			}
		}
	}
}

void printSymbolTable(symbolTable* ST){
	scopeTable s = ST->curr;
	printScopeTable(s, 0);
}