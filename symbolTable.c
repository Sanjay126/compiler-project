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
void printAllRecords(symbolTable* s){
	int norecord=1;
	if(!s)
		return;
	Records recs = s->recs;
	Record temp=recs->head;
	while(temp){
		printf("\n%20s\t",temp->name);
		rec_dec decl=temp->head;
		while(decl){
			printf("%5s, ",decl->type);
			decl=decl->next;
		}
		printf("\t%2d\n",temp->size);
		temp=temp->next;
	}
		
}	
void printMemoryReq(symbolTable* s){
	if(!s||!s->curr)
		return;
	entry** arr = s->curr->arr;
	for(int i=0; i<s->curr->size; i++){
		if(arr[i]!=NULL){
			entry* ptr = arr[i];
			while(ptr){
				if((strcmp(ptr->type, "function")==0))
					printf("\n %20s\t%4d \n",ptr->name,ptr->width);
				ptr=ptr->next;
			}
		}
	}
}
void printGlobalVariables(symbolTable* ST){
	printScopeTable(ST->curr,0,0);
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

entry* makeNewEntry(int global, int record_or_not, int inout){
	entry* en1 = (entry*)malloc(sizeof(entry));
	
	en1->name = NULL;
	en1->type = NULL;
	en1->scope = NULL;
	en1->next =  NULL;
	en1->recVal =  NULL;
	en1->funcScopePtr =  NULL;

	en1->global =  global;
	en1->record_or_not =  record_or_not;
	en1->inout = inout;
	en1->countVariables =  0;
	
	en1->lineNo =  0;
	return en1;
}

symbolTable *createSymbolTable(ParseTree PT){
	ParseTree func = PT->children;
	int gcount = 0;
	entry* head = NULL;
	while(func){
		ParseTree stmts;		
		entry* newen = makeNewEntry(1, 0, 0);
		int varCount  = 0;

		if(func->non_term_id==function)
			stmts = func->children->next->next->next;
		else if(func->non_term_id==mainFunction)
			stmts = func->children;
		ParseTree decl;
		if(stmts->children->non_term_id == declarations)
			decl = stmts->children->children;
		else if(stmts->children->next->non_term_id == declarations)
			decl = stmts->children->next->children;
		else
			decl = NULL;

		while(decl){
			entry* newen2 = makeNewEntry(decl->children->next->next!=NULL, decl->children->tk->tid == TK_RECORDID, 0);
			newen2->type = (char*)malloc(sizeof(char)*(strlen(decl->children->tk->name)+1));
			newen2->name = (char*)malloc(sizeof(char)*(strlen(decl->children->next->tk->name)+1));
			strcpy(newen2->type,decl->children->tk->name);
			strcpy(newen2->name,decl->children->next->tk->name);
			newen2->lineNo = decl->children->tk->lineNo;
			if(decl->children->next->next!=NULL)
				gcount++;
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
			while(out){
				entry* newen2 = makeNewEntry(0,out->tk->tid == TK_RECORDID, 2);
				newen2->type = (char*)malloc(sizeof(char)*(strlen(out->tk->name)+1));
				newen2->name = (char*)malloc(sizeof(char)*(strlen(out->next->tk->name)+1));
				newen2->scope = (char*)malloc(sizeof(char)*(strlen(func->children->tk->name)+1));
				strcpy(newen2->scope,func->children->tk->name);
				strcpy(newen2->type,out->tk->name);
				strcpy(newen2->name,out->next->tk->name);
				newen2->lineNo = out->tk->lineNo;
				newen2->next = head;
				head = newen2;
				varCount++;
				out = out->next->next;
			}
			while(in){
				entry* newen2 = makeNewEntry(0, in->tk->tid==TK_RECORDID, 1);
				newen2->type = (char*)malloc(sizeof(char)*(strlen(in->tk->name)+1));
				newen2->name = (char*)malloc(sizeof(char)*(strlen(in->next->tk->name)+1));
				newen2->scope = (char*)malloc(sizeof(char)*(strlen(func->children->tk->name)+1));
				strcpy(newen2->type,in->tk->name);
				strcpy(newen2->name,in->next->tk->name);
				strcpy(newen2->scope,func->children->tk->name);
				newen2->lineNo = in->tk->lineNo;
				newen2->next = head;
				head = newen2;
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
	while(ptr){
		if(ptr->global){
			ST = insert(ST, ptr);
		}
		ptr = ptr->next;
	}

	ptr = head;

	while(ptr && strcmp(ptr->type,"function") == 0){
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
					newNode->next = ST->curr->outputParams;
					ST->curr->outputParams = newNode;
					ptr = ptr->next;
				}
			}
			if(ptr){
				if(!ptr->global){
					ST = insert(ST,ptr);

					ptr = ptr->next;
				}
				else if(strcmp(ptr->type, "function")!=0){
					ptr = ptr->next;
				}
			}

		}
		ST = closeScope(ST);
	}

	return ST;
}


symbolTable *openScope(symbolTable *ST,int sz,char* scope){
	ST->size++;
	scopeTable s = (scopeTable)malloc(sizeof(struct scopetable));
	s->size = sz;
	s->inputParams = NULL;
	s->outputParams = NULL;
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
	ST->curr = ST->curr->prevScope;
	return ST;
}

int numberOfDeclarations(ParseTree PT){
	int count = 0;
	if(PT==NULL)
		return count;
	ParseTree input_par = PT->children->next;
}

symbolTable* insert(symbolTable* ST, entry *en){
	scopeTable s;
	int in;
	entry* lookedup=lookup(ST,en->name);
	if(lookedup){

		printf("Line No. %d : <%s> already declared.\n", en->lineNo, en->name);
		//TODO error reporting
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
				if(strcmp(recPtr->type, "int")==0){
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

	entry* en1 = makeNewEntry(en->global, en->record_or_not, en->inout);
	en1->name = (char*)malloc(sizeof(char)*(strlen(en->name)+1));
	en1->type = (char*)malloc(sizeof(char)*(strlen(en->type)+1));
	if(en->scope){
		en1->scope = (char*)malloc(sizeof(char)*(strlen(en->scope)+1));
		strcpy(en1->scope, en->scope);
	}
	strcpy(en1->name, en->name);
	strcpy(en1->type, en->type);
	en1->lineNo =  en->lineNo;
	en1->next =  NULL;
	en1->recVal =  en->recVal;
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

void printScopeTable(scopeTable s, int spaces,int all){
	if(!s)
		return;
	entry** arr = s->arr;
	for(int i=0; i<s->size; i++){
		if(arr[i]!=NULL){
			entry* ptr = arr[i];
			while(ptr){
				for(int j=0; j<spaces; j++)
					printf("\t\t");
				printf("%20s\t\t%20s\t%d\n", ptr->type, ptr->name, ptr->record_or_not);
				if((strcmp(ptr->type, "function")==0)&&all)
					printScopeTable(ptr->funcScopePtr, spaces+1,all);

				if(ptr->record_or_not){
					RecordValue recval = ptr->recVal;
					while(recval){
						printf("\t\t\t\t%20s\t\t%20d\n", recval->name, recval->isInt);
						recval = recval->next;
					}
				}
				ptr=ptr->next;
			}
		}
	}
	if(all){
		ParamNode in = s->inputParams;
		ParamNode out = s->outputParams;
		
		printf("\n");
		while(in){
			printf("\t\tINPUT  --  %20s\n", in->paramName);
			in = in->next;
		}
		
		printf("\n");
		while(out){
			printf("\t\tOUTPUT  --  %20s\n", out->paramName);
			out = out->next;
		}
	}
	printf("\n");
}

void printSymbolTable(symbolTable* ST){
	scopeTable s = ST->curr;
	printScopeTable(s, 0,1);
}

void checkTypeAssign(symbolTable* ST, ParseTree p,char* type)
{
	if(p == NULL)
		return;
	ParseTree ptr = p->children;
	while(ptr)
	{
		checkTypeAssign(ST,ptr,type);
		if(ptr->non_term_id==TK_ID + no_of_nt && !ptr->notFound)
		{
			entry* tmp = lookup(ST,ptr->tk->name);
			char c[25];
			if(!tmp->record_or_not)
				strcpy(c,tmp->type);
			else
			{
				if(!ptr->next || ptr->next->non_term_id != TK_FIELDID + no_of_nt)
					strcpy(c,tmp->type);
				if(ptr->next && ptr->next->non_term_id == TK_FIELDID + no_of_nt)
				{
					entry* tmp2 = lookup(ST,ptr->next->tk->name);
					RecordValue ls = tmp->recVal;
					while(ls)
					{
						if(strcmp(ls->name,ptr->next->tk->name) == 0)
							break;
						ls = ls->next;
					}
					if(ls->isInt)
						strcpy(c,"int");
					else
						strcpy(c,"real");
				}
			}

			if(strcmp(c,type) != 0)
				printf("Line No %llu:Expected type %s not %s\n", ptr->tk->lineNo,type,c);
		}

		if(ptr->non_term_id == TK_NUM + no_of_nt && strcmp(type,"real")==0)
			printf("Line No %llu:Expected type %s not %s\n", ptr->tk->lineNo,type,"int");


		if(ptr->non_term_id == TK_RNUM + no_of_nt && strcmp(type,"int")==0)
			printf("Line No %llu:Expected type %s not %s\n", ptr->tk->lineNo,type,"real");

		ptr = ptr->next;	
	}
	return;
}

void semanticAnalyser(symbolTable* ST, ParseTree ptr){
	if(ptr==NULL)
		return;
	ParseTree p = ptr->children;
	while(p && (p->non_term_id==declarations || p->non_term_id==typeDefinitions))
				p = p->next;
	// if(p==NULL)
	// 	return;	
	while(p){
		if(p->non_term_id==TK_ID + no_of_nt){
			entry* en = lookup(ST, p->tk->name);
			if(en==NULL){
				//TODO ERROR HANDLING
				p->notFound = 1;
				// if()
				printf("Line No. %llu : <%s> is undeclared.\n", p->tk->lineNo, p->tk->name);
				// return;
			}
			else if(p->next!=NULL && p->next->non_term_id==TK_FIELDID + no_of_nt){
				RecordValue recv = en->recVal;
				int flag=0;
				while(recv){
					if(strcmp(recv->name, p->next->tk->name)==0){
						flag = 1;
						break;
					}
					recv = recv->next;
				}
				if(!flag)
					printf("Line No %llu : No such field for the record : %s\n", p->next->tk->lineNo,p->next->tk->name);
				p = p->next;
			}
		}
		else{
			semanticAnalyser(ST, p);
			if(p->non_term_id == funCallStmt){
				entry* funcEntry = lookup(ST, p->children->next->tk->name);
				if(funcEntry!=NULL){
					ParamNode outputParams = funcEntry->funcScopePtr->outputParams;
					ParamNode inputParams = funcEntry->funcScopePtr->inputParams;
					ParseTree outputCall = p->children->children;
					ParseTree inputCall = p->children->next->next->children;
					int flag = 0;
					while(outputCall && outputParams)
					{
						if(outputCall->notFound)
						{
							flag = 1;
							break;
						}
						char* callType = lookup(ST,outputCall->tk->name)->type;
						scopeTable temp = ST->curr;
						ST->curr = funcEntry->funcScopePtr;
						char* paramType = lookup(ST,outputParams->paramName)->type;
						ST->curr = temp;
						if(strcmp(callType,paramType) != 0)
						{
							printf("Line No %llu: Expected Arguement of type %s passed argument of type %s\n",p->children->next->tk->lineNo,paramType,callType);
						}
						outputCall = outputCall->next;
						outputParams = outputParams->next;
					}
					if(outputCall && flag == 0)
						printf("Line No %llu: Output Arguements more than required\n", p->children->next->tk->lineNo);
					if(outputParams && flag == 0)
						printf("Line No %llu: Output Arguements less than required\n", p->children->next->tk->lineNo);

					flag = 0;
					while(inputCall && inputParams && !inputCall->notFound)
					{
						if(inputCall->notFound)
						{
							flag = 1;
							break;
						}
						char* callType = lookup(ST,inputCall->tk->name)->type;
						scopeTable temp = ST->curr;
						ST->curr = funcEntry->funcScopePtr;
						char* paramType = lookup(ST,inputParams->paramName)->type;
						ST->curr = temp;
						if(strcmp(callType,paramType) != 0)
						{
							printf("Line No %llu: Expected Arguement of type %s passed argument of type %s\n",p->children->next->tk->lineNo,paramType,callType);
						}
						inputCall = inputCall->next;
						inputParams = inputParams->next;
					}
					if(inputCall && flag == 0)
						printf("Line No %llu: input Arguements more than required\n", p->children->next->tk->lineNo);
					if(inputParams != NULL && flag == 0)
						printf("Line No %llu: input Arguements more than required\n", p->children->next->tk->lineNo);
				}
				else{
					printf("Line No %llu : Function %s Not Find\n",p->children->next->tk->lineNo,p->children->next->tk->name);
				}
			}
			if(p->non_term_id == returnStmt)
			{
				ParamNode outputParams = ST->curr->outputParams;
				ParseTree returnParams = p->children;
				while(returnParams && outputParams)
				{
					char* returnName = returnParams->tk->name;
					char* outputName = outputParams->paramName;
					if(strcmp(returnName,outputName) != 0)
						printf("Line No %llu: Expected Return identifier %s not %s\n",returnParams->tk->lineNo, outputName,returnName);
					returnParams = returnParams->next;
					outputParams = outputParams->next;
				}
				if(returnParams)
					printf("Line No %llu: More than required of variables are returned\n",returnParams->tk->lineNo);
				if(outputParams)
					printf("Line No %llu: Less than required of variables are returned\n",p->children->tk->lineNo);
			}
			if(p->non_term_id == assignmentStmt)
			{
				ParseTree ptr = p->children->children;
				if(!ptr->notFound)
				{
					entry* en = lookup(ST,ptr->tk->name);
					if(!en->record_or_not)
						checkTypeAssign(ST,p,en->type);
					else
					{
						if(!ptr->next)
							checkTypeAssign(ST,p,en->type);
						if(ptr->next && !ptr->next->notFound)
						{
							RecordValue ls = en->recVal;
							while(ls)
							{
								if(strcmp(ls->name,ptr->next->tk->name) == 0)
									break;
								ls = ls->next;
							}
							if(ls->isInt)
								checkTypeAssign(ST,p,"int");
							else
								checkTypeAssign(ST,p,"real");
						}
					}	
				}
			}

			if(p->non_term_id == booleanExpression)
			{
				ParseTree ptr = p->children;
				while(ptr->non_term_id != TK_ID+no_of_nt)
					ptr = ptr->next;
				if(!ptr->notFound)
				{
					entry* en = lookup(ST,ptr->tk->name);
					if(!en->record_or_not)
						checkTypeAssign(ST,p,en->type);
					else
					{
						if(!ptr->next || ptr->next->non_term_id != TK_FIELDID+no_of_nt)
							checkTypeAssign(ST,p,en->type);
						if(ptr->next && !ptr->next->notFound && ptr->next->non_term_id != TK_FIELDID+no_of_nt)
						{
							RecordValue ls = en->recVal;
							while(ls)
							{
								if(strcmp(ls->name,ptr->next->tk->name) == 0)
									break;
								ls = ls->next;
							}
							if(ls->isInt)
								checkTypeAssign(ST,p,"int");
							else
								checkTypeAssign(ST,p,"real");
						}
					}
				}
			}

			if(p->non_term_id == ioStmt)
			{
				entry* en = lookup(ST,p->children->next->children->tk->name);
				if(en && en->record_or_not && !p->children->next->children->next)
					printf("Line No: %llu Type mismatch \n", p->children->tk->lineNo);
			}		

			if(p->non_term_id == iterativeStmt)
			{
				ParamNode ls = NULL;
				ParseTree ptr = p->children->children;
				while(ptr)
				{
					if(ptr->non_term_id == TK_ID + no_of_nt)
					{
						ParamNode newNode = (ParamNode)malloc(sizeof(struct paramNode));
						newNode->paramName = (char*)malloc(sizeof(char)*(strlen(ptr->tk->name)+1));
						newNode->paramName = ptr->tk->name;
						newNode->next = NULL;
						if(ls == NULL)
							ls = newNode;
						else
						{
							newNode->next = ls;
							ls = newNode;
						}
					}
					ptr = ptr->next;
				}
				int flag = 0;
				ptr = p->children;
				while(ptr)
				{
					if(ptr->non_term_id == assignmentStmt)
					{
						char* name = ptr->children->children->tk->name;
						ParamNode ptr2 = ls;
						while(ptr2)
						{
							if(strcmp(ptr2->paramName,name) == 0)
							{
								flag = 1;
								break;
							}
							ptr2 = ptr2->next;
						} 

						if(flag == 1)
							break;
					}
					ptr = ptr->next;
				}

				if(flag == 0)
				{
					ptr = p->children->children;
					while(ptr && ptr->non_term_id != TK_ID+no_of_nt)
						ptr = ptr->next;
					printf("Line No %llu: No Variables in while loop are getting updated\n",ptr->tk->lineNo);
				}
			}		
		}
		p = p->next;
	}
}

void semanticAnalysis(symbolTable* ST, ParseTree PT){
	scopeTable globalScopeTable = ST->curr;
	ParseTree funcPT = PT->children;
	while(funcPT){
		if(funcPT->non_term_id == function){
			entry* funcEntry = lookup(ST, funcPT->children->tk->name);
			ST->curr = funcEntry->funcScopePtr;
			ParseTree ptr = funcPT->children->next->next->next;
			if(ptr->children == NULL){
				funcPT = funcPT->next;
				continue;
			}
			// ptr = ptr->children;
			// while(ptr && (ptr->non_term_id==declarations || ptr->non_term_id==typeDefinitions))
			// 	ptr = ptr->next;
			semanticAnalyser(ST, ptr);
			ST->curr = ST->curr->prevScope;
		}
		else{
			entry* funcEntry = lookup(ST, "mainFunction");
			ST->curr = funcEntry->funcScopePtr;
			// printf("---aaa--- %s\n", funcEntry->funcScopePtr->name);
			ParseTree ptr = funcPT->children;
			if(ptr->children == NULL){
				funcPT = funcPT->next;
				continue;
			}
			// ptr = ptr->children;
			// while(ptr && (ptr->non_term_id==declarations || ptr->non_term_id==typeDefinitions))
			// 	ptr = ptr->next;
			semanticAnalyser(ST, ptr);
			ST->curr = ST->curr->prevScope;
		}
		funcPT = funcPT->next;
	}
}