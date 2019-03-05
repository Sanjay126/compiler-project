/*
Group Number - 12
Sanjay Malhotra 2016A7PS0126P
Nilay Arora 2016A7PS0117P
Tushar Goel 2016A7PS0023P
Adit Shastri 2016A7PS0121P
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "parserDef.h"

//global variable for grammer struct
Grammar gram;

//Arrays for non-terminals,all symbols and token resp.
char* ntArray[] = {"assignmentStmt","global_or_not","booleanExpression","highPrecedenceOperators","singleOrRecId","constructedDatatype","dataType","primitiveDatatype","typeDefinitions","more_ids","idList","relationalOp","temp","output_par","stmt","declarations","input_par","logicalOp","newallvar","program","remaining_list","all","term","stmts","returnStmt","declaration","iterativeStmt","otherFunctions","factor","allnew","new_24","function","arithmeticExpression","var","fieldDefinitions","allVar","mainFunction","funCallStmt","parameter_list","fieldDefinition","typeDefinition","expPrime","outputParameters","termPrime","lowPrecedenceOperators","optionalReturn","elsePart","moreFields","conditionalStmt","otherStmts","ioStmt","inputParameters"};

char* symbolArray[] = {"assignmentStmt","global_or_not","booleanExpression","highPrecedenceOperators","singleOrRecId","constructedDatatype","dataType","primitiveDatatype","typeDefinitions","more_ids","idList","relationalOp","temp","output_par","stmt","declarations","input_par","logicalOp","newallvar","program","remaining_list","all","term","stmts","returnStmt","declaration","iterativeStmt","otherFunctions","factor","allnew","new_24","function","arithmeticExpression","var","fieldDefinitions","allVar","mainFunction","funCallStmt","parameter_list","fieldDefinition","typeDefinition","expPrime","outputParameters","termPrime","lowPrecedenceOperators","optionalReturn","elsePart","moreFields","conditionalStmt","otherStmts","ioStmt","inputParameters", "TK_WITH","TK_THEN","TK_TYPE","TK_EQ","TK_DOLLAR","TK_COMMA","TK_RNUM","TK_CL","TK_AND","TK_PARAMETER","TK_DIV","TK_ID","TK_GLOBAL","TK_GE","TK_LIST","TK_IF","TK_RECORD","TK_NUM","TK_NE","TK_LE","TK_DOT","TK_INPUT","TK_MAIN","TK_CALL","TK_PLUS","TK_ELSE","TK_COMMENT","TK_ASSIGNOP","TK_WRITE","TK_COLON","TK_WHILE","TK_REAL","TK_READ","TK_NOT","TK_SQL","TK_OUTPUT","TK_SQR","TK_ENDIF","TK_MINUS","TK_SEM","TK_FIELDID","TK_PARAMETERS","TK_OP","TK_ENDWHILE","TK_MUL","TK_FUNID","TK_INT","TK_RETURN","TK_GT","TK_END","TK_LT","TK_OR","TK_ENDRECORD","eps","TK_RECORDID"};

char* tokenArray[] = {"TK_WITH","TK_THEN","TK_TYPE","TK_EQ","TK_DOLLAR","TK_COMMA","TK_RNUM","TK_CL","TK_AND","TK_PARAMETER","TK_DIV","TK_ID","TK_GLOBAL","TK_GE","TK_LIST","TK_IF","TK_RECORD","TK_NUM","TK_NE","TK_LE","TK_DOT","TK_INPUT","TK_MAIN","TK_CALL","TK_PLUS","TK_ELSE","TK_COMMENT","TK_ASSIGNOP","TK_WRITE","TK_COLON","TK_WHILE","TK_REAL","TK_READ","TK_NOT","TK_SQL","TK_OUTPUT","TK_SQR","TK_ENDIF","TK_MINUS","TK_SEM","TK_FIELDID","TK_PARAMETERS","TK_OP","TK_ENDWHILE","TK_MUL","TK_FUNID","TK_INT","TK_RETURN","TK_GT","TK_END","TK_LT","TK_OR","TK_ENDRECORD","eps","TK_RECORDID"};

//Data structures for storing first and follows sets
Node **First;
Node *Follow;

hashTable Table; //hashtable global variable for storing all the symbols -> Non-terminals followed by tokens(Used for fast retreival of their ID)           

//Initializing Linked List Node
Node intialiseNode(int id){
	Node n = (Node)malloc(sizeof(struct node));
	n->id = id;
	n->next = NULL;
	return n;
}

//Stack ADT
SNode intialiseSNode(int id,ParseTree pt_node){
	SNode n = (SNode)malloc(sizeof(struct snode));
	n->id = id;
	n->next = NULL;
	n->pt_node = pt_node;
	return n;	
}
Stack intialiseStack(){
	Stack s = (Stack)malloc(sizeof(struct stack));
	s->size = 0;
	s->head = NULL;
	return s;
}
Stack pushStack(Stack s, int id,ParseTree pt_node){
	SNode n = intialiseSNode(id,pt_node);
	n->next = s->head;
	s->size++;
	s->head = n;
	return s;
}
Stack popStack(Stack s){
	if(s->head==NULL)
		return s;
	SNode n = s->head;
	s->head = n->next;
	free(n);
	s->size--;
	return s;
}
SNode topStack(Stack s){
	if (s->head==NULL)
		return NULL;
	return s->head;
}

//Linkied List Utility Functions
Node addNode(int id,Node head, int dupAllowed){
	Node newNode=(Node)malloc(sizeof(struct node));
	Node temp=head;
	newNode->id=id;
	newNode->next=NULL;
	if(head==NULL)
		return newNode;
	while(temp->next!=NULL){
		if(temp->id==id && !dupAllowed)
			return head;
		temp=temp->next;
	}
	if(temp->id==id)
		return head;
	temp->next=newNode;
	return head;
}
Node createCopyNodeList(Node head, int dupAllowed){
	Node newNode=NULL;
	Node temp = head;
	while(temp){
		newNode=addNode(temp->id,newNode, dupAllowed);
		temp=temp->next;
	}
	return newNode;
}
Node joinNodeList(Node n1,Node n2){
	Node copy=createCopyNodeList(n2, 0);
	Node temp=n1;
	if(n1==NULL)
		return copy;
	while(temp->next)
		temp=temp->next;
	temp->next=copy;
	return n1;
}

//Returning Symbol from ID
char* getTokenFromId(int id){
	return symbolArray[id];
}
// void printFirst(){
// 	for(int i=0; i<no_of_nt+no_of_t+1; i++){
// 		Node temp, temp2;
// 		printf("First of %s %d ==> ", getTokenFromId(i), i);
// 		if(First[i][0]!=NULL){
// 			temp = First[i][0];
// 			while(temp!=NULL){
// 				printf("%s\t",getTokenFromId(temp->id));
// 				temp=temp->next;
// 			}
// 		}
// 		else if(First[i][1]!=NULL){
// 			temp2 = First[i][1];
// 			while(temp2!=NULL){
// 				printf("%s\t",getTokenFromId(temp2->id));
// 				temp2=temp2->next;
// 			}
// 		}
// 		else{
// 			printf("ERROR\n");
// 			continue;
// 		}
// 		printf("\n");
// 	}				
// }

//HashTable ADT
int hash(char *v, int M){ 
	int h = 0, a = 257;
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}
void populateHashTable(){
	Table = (hashTable)malloc(no_of_sym*sizeof(hashNode*));
	for(int i = 0;i < no_of_sym;i++){
		Table[i] = (hashNode*)malloc(sizeof(hashNode));
		Table[i]->next == NULL;
	}
	for(int i = 0;i < no_of_sym;i++){
		int hval = hash(symbolArray[i],no_of_sym);
		if(Table[hval]->next == NULL){
			Table[hval]->next = (hashNode*)malloc(sizeof(hashNode));
			Table[hval]->next->index = i;
			strcpy(Table[hval]->next->s, symbolArray[i]);
			Table[hval]->next->next = NULL;
		}
		else{
			hashNode* ptr = Table[hval]->next;
			while(ptr != NULL && ptr->next != NULL)
				ptr = ptr->next;
			ptr->next = (hashNode*)malloc(sizeof(hashNode));
			ptr->next->index = i;
			strcpy(ptr->next->s, symbolArray[i]);
			ptr->next->next = NULL;
		}
	}
}
int findIndex(char* s){
	int in = hash(s,no_of_sym);
	hashNode* ptr = Table[in];
	while(ptr != NULL){
		if(strcmp(ptr->s,s) == 0)
			return ptr->index; 
		ptr = ptr->next;
	}
	return -1;
}

//Reading Grammer from grammet.txt and storing it
void buildRules(){
	populateHashTable();
	FILE *gramFile = fopen("grammar.txt","r");
	gram.rules = (RuleRHS*)malloc(sizeof(RuleRHS)*no_of_nt);
	for(int i=0; i<no_of_nt; i++)
		gram.rules[i]=NULL;
	int ruleNo=1;
	char* buff=NULL;
	size_t len=0;
	while(getline(&buff, &len, gramFile) != -1)
	{
		char* lhs = strtok(buff, " \n\r");
		int lhsIndex = findIndex(lhs);
		char* token = strtok(NULL, " \n\r");
		RuleRHS r = (RuleRHS)malloc(sizeof(struct ruleRHS));
		r->next = NULL;
		r->head = NULL;
		r->size = 0;
		r->ruleNo = ruleNo;
		while(token!=NULL){
			int tokenIndex = findIndex(token);
			Node newNode = intialiseNode(tokenIndex);;
			newNode->next = r->head;
			r->head = newNode;
			r->size++;
			token = strtok(NULL, " \n\r");
		}
		if(gram.rules[lhsIndex]==NULL)
			gram.rules[lhsIndex] = r;
		else{
			RuleRHS h = gram.rules[lhsIndex];
			while(h->next!=NULL)
				h = h->next;
			h->next = r;
		}
		ruleNo++;
	}
	fclose(gramFile);
}

//creating parse tree node
ParseTree createPTNode(int id){
	ParseTree new = (ParseTree)malloc(sizeof(struct parseTree));
	new->next = NULL;
	new->children = NULL;
	new->tk = NULL;
	new->parent = NULL;
	new->non_term_id = id;
	return new;
}

//reversing a linked list
Node reverseList(Node ls){
	Node curr = createCopyNodeList(ls, 1);
	Node prev = NULL, next = NULL;
	while(curr!=NULL){
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	return prev;
}

//returns First of Right Hand Side of a grammer rule
Node getFirst(Node head,int* epsFlag){
	if (head==NULL)
		return NULL;
	Node calculatedFirst=NULL;
	Node temp=head;
	while(temp){
		if(First[temp->id][1]==NULL){
			calculatedFirst=joinNodeList(calculatedFirst,First[temp->id][0]);
			return calculatedFirst;
		}
		else
			calculatedFirst=joinNodeList(calculatedFirst,First[temp->id][1]);
		temp=temp->next;
	}
	*epsFlag=1;
	return calculatedFirst;
}

//Prints a linked List
void printNodeList(Node list){
	Node temp=list;
	while(temp){
		printf("%s\t",symbolArray[temp->id] );
		temp=temp->next;
	}
	printf("\n");
}

/*
Creates Parse Table
Parse table is a 2D matrix with each cell as Integer
A number greater than 0 is ruleNo which is basically lineNo in grammer.txt
0 represents that is in syn set
-1 represents error
*/
parseTable createParseTable(parseTable T){
	T = (parseTable)malloc(no_of_nt*sizeof(int*));

	for(int i = 0;i < no_of_nt;i++)
		T[i] = (int*)malloc(no_of_t*sizeof(int));

	for(int i = 0;i < no_of_nt;i++)
		memset(T[i],-1,no_of_t*sizeof(int));

	for(int i = 0;i < no_of_nt;i++){
		RuleRHS ruleIter = gram.rules[i];
		while(ruleIter != NULL){
			Node RHSIter = ruleIter->head;
			int epsFlag=0;
			Node firstIter;
			Node revList = reverseList(RHSIter);
			firstIter=getFirst(revList,&epsFlag);
			while(firstIter){	
				T[i][firstIter->id-no_of_nt] = ruleIter->ruleNo;
				firstIter = firstIter->next;
			}
			if(epsFlag){
				Node followIter = Follow[i];
				while(followIter){
					T[i][followIter->id-no_of_nt] = ruleIter->ruleNo;
					followIter = followIter->next;
				}
			}
			ruleIter = ruleIter->next;
		}
		for(int j = 0;j < no_of_t;j++){
			Node followIter = Follow[i];
			while(followIter){
				if(T[i][followIter->id-no_of_nt] == -1)
					T[i][followIter->id-no_of_nt] = 0;
				followIter = followIter->next;
			}
		}
	}
	return T;
}

//printing parse table
void printParseTable(parseTable T){
	char* filename = "parseTable.txt";
	FILE* fp23 = fopen(filename, "w");
	char * a = "A";
	fprintf(fp23, "%23s\t", a);
	for(int i=0;i<no_of_t;i++){
		fprintf(fp23, "%13s",symbolArray[i+no_of_nt]);
	}
	fprintf(fp23, "\n");
	for(int i=0; i<no_of_nt; i++){
		fprintf(fp23, "%23s\t",symbolArray[i] );
		for(int j=0; j<no_of_t; j++){
			fprintf(fp23, "%13d", T[i][j]);
		}
		fprintf(fp23, "\n");
	}
	fclose(fp23);
}

//Reading first and follow from txt files and populating the data structures
void ReadFromFileFirstAndFollow(Grammar gram){
	First = (Node**)malloc(sizeof(Node*)*(no_of_nt+no_of_t+1));
	Follow = (Node*)malloc(sizeof(Node)*(no_of_nt+1));
	for(int i=0; i<no_of_nt+1; i++){
		Follow[i]=NULL;
	}
	for(int i=0; i<no_of_nt+no_of_t+1; i++){
		First[i] = (Node*)malloc(sizeof(Node)*2);
		First[i][0]=NULL;
		First[i][1]=NULL;
	}
	FILE* fp_first = fopen("first.txt", "r");
	char *buff=NULL;
	size_t len;
	while(getline(&buff, &len, fp_first)!=-1){
		char* token;
		char* lhs = strtok(buff, " \n\r");
		int lhsIndex = findIndex(lhs);
		if(lhsIndex==-1)
			continue;
		token = strtok(NULL," \n\r");
		int tokenIndex=findIndex(token);
		if(tokenIndex==-1)
			continue;
		int epsFlag=0;
		if(tokenIndex==eps+no_of_nt)
			epsFlag=1;
		Node head = intialiseNode(tokenIndex);
		Node temp = head;
		while(token!=NULL){
			token = strtok(NULL," \n\r");
			if(token==NULL)
				break;
			tokenIndex = findIndex(token);
			if(tokenIndex==-1)
				break;
			if(tokenIndex==eps+no_of_nt)
				epsFlag=1;
			temp->next = intialiseNode(tokenIndex);
			temp = temp->next;
		}
		if(tokenIndex==-1)
			continue;
		First[lhsIndex][epsFlag] = head;
	}
	fclose(fp_first);

	FILE* fp_follow = fopen("follow.txt", "r");
	while(getline(&buff, &len, fp_follow)!=-1){
		char* token;
		char* lhs = strtok(buff, " \n\r");
		int lhsIndex = findIndex(lhs);
		if(lhsIndex==-1)
			continue;
		token = strtok(NULL," \n\r");
		int tokenIndex=findIndex(token);
		if(tokenIndex==-1)
			continue;
		Node head = intialiseNode(tokenIndex);
		Node temp = head;
		while(token!=NULL){
			token = strtok(NULL," \n\r");
			if(token==NULL)
				break;
			tokenIndex = findIndex(token);
			if(tokenIndex==-1)
				break;
			temp->next = intialiseNode(tokenIndex);
			temp = temp->next;
		}
		if(tokenIndex==-1)
			continue;
		Follow[lhsIndex] = head;
	}
	fclose(fp_follow);
}


void printNumber(char* str, FILE* fp1){
	int d=0;
	while(str!=NULL){
		d = d*10 + ((*str)-48);
		str++;
	}
	fprintf(fp1, "%d\t", d);
}
void printReal(char* str, FILE* fp1){
	int d = 0;
	while(str!=NULL && *str!='.'){
		d = d*10 + ((*str)-48);
		str++;
	}
	fprintf(fp1, "%d%s\t", d, str);
}

void inorderTraversal(ParseTree PT, FILE* fp1, int level, ParseTree parent){
	if(PT==NULL)
		return;
	//traverse left
	inorderTraversal(PT->children, fp1, level+1, PT);
	char* dash = "----";
	char* yes = "yes";
	char* no = "no";
	char* ROOT = "ROOT";
	if(PT->tk == NULL){
		fprintf(fp1,"%23s\t%23s\t%23s\t%23s\t", dash,dash,dash,dash);
		if(parent==NULL)
			fprintf(fp1, "%23s\t", ROOT);
		else
			fprintf(fp1, "%23s\t", symbolArray[parent->non_term_id]);
		fprintf(fp1, "%23s\t%23s\n", no, symbolArray[PT->non_term_id]);
	}
	else{
		fprintf(fp1,"%23s\t%23llu\t%23s", PT->tk->name, PT->tk->lineNo, symbolArray[PT->tk->tid+no_of_nt]);
		if(PT->tk->tid==TK_NUM)
			printNumber(PT->tk->name, fp1);
		else if(PT->tk->tid==TK_RNUM)
			printReal(PT->tk->name, fp1);
		else
			fprintf(fp1, "%23s\t", dash);
		if(parent==NULL)
			fprintf(fp1, "%23s\t", ROOT);
		else
			fprintf(fp1, "%23s\t", symbolArray[parent->non_term_id]);
		fprintf(fp1, "%23s\t%23s\n",yes, dash);
	}
	
	//traverse right
	inorderTraversal(PT->next, fp1, level, parent);
}

//printing pasre tree
void printParseTree(ParseTree PT, char *outfile){
	FILE* fp1 = fopen(outfile, "w");
	char* headings[] = {"lexeme","lineno","tokenName","valueIfNumber","parentNodeSymbol","isLeafNode","NodeSymbol"};
	for(int i=0; i<7; i++)
		fprintf(fp1, "%23s\t", headings[i]);
	fprintf(fp1, "\n\n");

	inorderTraversal(PT, fp1, 0, NULL);
	fclose(fp1);
}

//Parsing anf forming parsetree
ParseTree parseInputSourceCode(char *testcaseFile, parseTable T,ParseTree PT){
	
	Stack s = intialiseStack();
	
	s = pushStack(s,TK_DOLLAR+no_of_nt,NULL);
	
	PT = createPTNode(program);
	ParseTree head = PT;
	
	s = pushStack(s,program,PT);
	
	buildRules();
	ReadFromFileFirstAndFollow(gram);

	T = createParseTable(T);
	
	FILE* fp = fopen(testcaseFile,"r");

	TokenInfo token = getNextToken(fp);
	while(token==NULL){
		token = getNextToken(fp);
	}
	int errorFlag=0;
	while(1){
		int X = topStack(s)->id;
		printf("%s\t%s\n", symbolArray[X],tokenArray[token->tid]);
		if(token->tid == TK_DOLLAR && topStack(s)->id == TK_DOLLAR + no_of_nt){ //Case 1 : Top of stack is current token is equal to '$'
			break;
		}
		else if(token->tid + no_of_nt == topStack(s)->id) //Case 2 Top of stack = Token is not equal '$'
		{
			s = popStack(s);
			PT->tk = token;
			PT = topStack(s)->pt_node;
			token = getNextToken(fp);
			while(token==NULL){
				token = getNextToken(fp);
			}
		}
		else if(topStack(s)->id < no_of_nt)//Case 3 : Top of stack is non terminal
		{
			if(T[X][token->tid] > 0){ 
				s = popStack(s);
				int ruleNo = T[X][token->tid];
				RuleRHS ruleIter = gram.rules[X];
				while(ruleIter && ruleIter->ruleNo != ruleNo){
					ruleIter = ruleIter->next;
				}

				Node rhsIter = ruleIter->head;

				while(rhsIter){
					ParseTree newNode = createPTNode(rhsIter->id);
					if(rhsIter->id!=eps+no_of_nt)
						s = pushStack(s,rhsIter->id,newNode);
					else{
						newNode->non_term_id = -1;
						newNode->tk = (TokenInfo)malloc(sizeof(struct tokenInfo));
						newNode->tk->tid = eps;
						newNode->tk->lineNo = 0;
						newNode->tk->name = (char*)malloc(10*sizeof(char));
						strcpy(newNode->tk->name,"eps");
					}
					if(PT->children == NULL)
						PT->children = newNode;
					else{
						newNode->next = PT->children;
						PT->children = newNode;
					}
					rhsIter = rhsIter->next;
				}

				PT = topStack(s)->pt_node;
			}
			else if(T[X][token->tid] == 0) // Panic Mode : syn set
			{
				printf("Line %llu: The token of type %s for lexeme %s does not match with the expected token of type %s\n",token->lineNo,tokenArray[token->tid],token->name,symbolArray[topStack(s)->id]);
				s = popStack(s);
				PT = topStack(s)->pt_node;
				errorFlag = 1;
			}
			else //Panic Mode : Error
			{
				printf("Line %llu: The token of type %s for lexeme %s does not match with the expected token of type %s\n",token->lineNo,tokenArray[token->tid],token->name,symbolArray[topStack(s)->id]);
				token = getNextToken(fp);
				while(token==NULL){
					token = getNextToken(fp);
				}
				errorFlag = 1;
			}	
		}
		else //Case 4 : Top of stack is token but is not equal to the current token
		{
			errorFlag = 1;
			printf("Line %llu: The token %s for lexeme %s does not match with the expected token %s\n",token->lineNo,tokenArray[token->tid],token->name,symbolArray[topStack(s)->id]);
			s = popStack(s);
		}
	}
	fclose(fp);
	if(!errorFlag)
		printf("Input source code is syntactically correct...........\n");
	return head;
}
