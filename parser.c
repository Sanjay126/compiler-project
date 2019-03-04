#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "parserDef.h"

Grammar gram;
char* ntArray[] = {"factor","output_par","relationalOp","fieldDefinitions","arithmeticExpression","remaining_list","otherFunctions","typeDefinition","moreFields","declaration","inputParameters","dataType","funCallStmt","singleOrRecId","parameter_list","conditionalStmt","optionalReturn","global_or_not","outputParameters","op1","ioStmt","stmt","primitiveDatatype","function","otherStmts","ex2","declarations","iterativeStmt","constructedDatatype","assignmentStmt","returnStmt","B1","term","more_ids","typeDefinitions","allVar","C1","term2","program","mainFunction","op2","idList","stmts","fieldDefinition","input_par","booleanExpression","logicalOp"};

char* symbolArray[] = {"factor","output_par","relationalOp","fieldDefinitions","arithmeticExpression","remaining_list","otherFunctions","typeDefinition","moreFields","declaration","inputParameters","dataType","funCallStmt","singleOrRecId","parameter_list","conditionalStmt","optionalReturn","global_or_not","outputParameters","op1","ioStmt","stmt","primitiveDatatype","function","otherStmts","ex2","declarations","iterativeStmt","constructedDatatype","assignmentStmt","returnStmt","B1","term","more_ids","typeDefinitions","allVar","C1","term2","program","mainFunction","op2","idList","stmts","fieldDefinition","input_par","booleanExpression","logicalOp","TK_ASSIGNOP",    "TK_COMMENT",    "TK_FIELDID",    "TK_ID",    "TK_NUM",    "TK_RNUM",    "TK_FUNID",    "TK_RECORDID",    "TK_WITH",    "TK_PARAMETERS",    "TK_END",    "TK_WHILE",    "TK_INT",    "TK_REAL",    "TK_TYPE",    "TK_MAIN",    "TK_GLOBAL",    "TK_PARAMETER",    "TK_LIST",    "TK_SQL",    "TK_SQR",    "TK_INPUT",    "TK_OUTPUT",    "TK_SEM",    "TK_COLON",    "TK_DOT",    "TK_COMMA",    "TK_ENDWHILE",    "TK_OP",    "TK_CL",    "TK_IF",    "TK_THEN",    "TK_ENDIF",    "TK_READ",    "TK_WRITE",    "TK_RETURN",    "TK_PLUS",    "TK_MINUS",    "TK_MUL",    "TK_DIV",    "TK_CALL",    "TK_RECORD",    "TK_ENDRECORD",    "TK_ELSE",    "TK_AND",    "TK_OR",    "TK_NOT",    "TK_LT",    "TK_LE",    "TK_EQ",    "TK_GT",    "TK_GE",    "TK_NE",    "TK_DOLLAR",    "eps"};

char* tokenArray[] = {   "TK_ASSIGNOP",    "TK_COMMENT",    "TK_FIELDID",    "TK_ID",    "TK_NUM",    "TK_RNUM",    "TK_FUNID",    "TK_RECORDID",    "TK_WITH",    "TK_PARAMETERS",    "TK_END",    "TK_WHILE",    "TK_INT",    "TK_REAL",    "TK_TYPE",    "TK_MAIN",    "TK_GLOBAL",    "TK_PARAMETER",    "TK_LIST",    "TK_SQL",    "TK_SQR",    "TK_INPUT",    "TK_OUTPUT",    "TK_SEM",    "TK_COLON",    "TK_DOT",    "TK_COMMA",    "TK_ENDWHILE",    "TK_OP",    "TK_CL",    "TK_IF",    "TK_THEN",    "TK_ENDIF",    "TK_READ",    "TK_WRITE",    "TK_RETURN",    "TK_PLUS",    "TK_MINUS",    "TK_MUL",    "TK_DIV",    "TK_CALL",    "TK_RECORD",    "TK_ENDRECORD",    "TK_ELSE",    "TK_AND",    "TK_OR",    "TK_NOT",    "TK_LT",    "TK_LE",    "TK_EQ",    "TK_GT",    "TK_GE",    "TK_NE",    "TK_DOLLAR",    "eps"};

Node **First;
Node *Follow;

Node intialiseNode(int id){
	Node n = (Node)malloc(sizeof(struct node));
	n->id = id;
	n->next = NULL;
	return n;
}

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
	// s->pt_node = NULL;
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

Node addNode(int id,Node head){
	// if(head==)
	Node newNode=(Node)malloc(sizeof(struct node));
	Node temp=head;
	newNode->id=id;
	newNode->next=NULL;
	if(head==NULL)
		return newNode;
	while(temp->next!=NULL){
		if(temp->id==id){
			// free(newNode);
			return head;
		}
		temp=temp->next;
	}
	if(temp->id==id){
		// free(newNode);
		return head;
	}
	temp->next=newNode;
	return head;
}
Node createCopyNodeList(Node head){
	Node newNode=NULL;
	Node temp = head;
	while(temp){
		newNode=addNode(temp->id,newNode);
		temp=temp->next;
	}
	return newNode;
}

Node joinNodeList(Node n1,Node n2){
	Node copy=createCopyNodeList(n2);
	// printf("copy created\n");
	Node temp=n1;
	if(n1==NULL)return copy;
	while(temp->next){
		temp=temp->next;
	}
	temp->next=copy;
	return n1;
}
Node returningNode[2];
Node* FirstSetUtil(Node ruleHead);
void getFirstOfNT(int i){
	if(First[i][0]!=NULL||First[i][1]!=NULL)return;
	Node currentFirst[2]={NULL,NULL};
	RuleRHS rule=gram.rules[i];
		while(rule){
			Node temp=rule->head;
			Node* retrievedFirst=FirstSetUtil(temp);
			currentFirst[0]=joinNodeList(currentFirst[0],retrievedFirst[0]);
			currentFirst[1]=joinNodeList(currentFirst[1],retrievedFirst[1]);
			rule=rule->next;

		}
	First[i]=currentFirst;
	return ;

}
Node* FirstSetUtil(Node ruleHead){
	Node currentFirst[2];
	Node temp=ruleHead;
	if(temp->id>=no_of_nt) return First[temp->id];
	getFirstOfNT(temp->id);
	currentFirst[0]=First[temp->id][0];
	currentFirst[1]=First[temp->id][1];
	if(currentFirst[1]!=NULL)
		if(temp->next)
		{
			//remove epsilon
			Node* retrievedFirst=FirstSetUtil(temp->next);
			currentFirst[0]= joinNodeList(currentFirst[0],retrievedFirst[0]);
			currentFirst[1]=joinNodeList(currentFirst[1],retrievedFirst[1]);
		}
	returningNode[0]=joinNodeList(NULL,currentFirst[0]);
	returningNode[0]=joinNodeList(NULL,currentFirst[0]);
	return returningNode;
	// while(temp){
	// 	int j=temp->id;
	// 	if(j==101){			//case 2 from slides
	// 		First[i][1]=First[i][0];
	// 		First[i][0]=NULL;
	// 		First[i][1]=addNode(j,First[i][1]);
	// 		// return First[i];
	// 	}
	// 	else if(j!=i){
	// 		// printf("\n\nCASE 3\n\n");
	// 		Node* firstOfJ=getFirstOfNT(temp->id);
	// 		// printf("%d\n", firstOfJ[0]->id);
	// 		First[i][0]=joinNodeList(First[i][0],firstOfJ[0]);
	// 		First[i][1]=joinNodeList(First[i][1],firstOfJ[1]);
	// 		if(firstOfJ[1]!=NULL)	//case 3 from slides
	// 			epsFlag=1;
	// 		else{
	// 			epsFlag=0;
	// 			break;
	// 		}
	// 	}
	// 	temp=temp->next;
	// }
	// if(epsFlag==1&&First[i][1]==NULL&&First[i][0]!=NULL){      //last case from slides
	// 	// printf("\n\nCASE 4\n\n");
	// 	First[i][1]=First[i][0];
	// 	First[i][0]=NULL;
	// 	First[i][1]=addNode(101,First[i][1]);
	// }
}
void populateFirst(Grammar gram){

	for(int i=no_of_nt;i<no_of_t+no_of_nt;i++){
		First[i][0]=addNode(i,First[i][0]); 	//case 1 from slides.
	}
	for(int i=0;i<no_of_nt;i++){
		getFirstOfNT(i);
	}
	printf("First Done\n");
}
void populateFollow(Grammar gram){
	Follow[program]=addNode(TK_DOLLAR+no_of_nt,Follow[program]);
	for(int i=0;i<no_of_nt;i++){
		RuleRHS rule=gram.rules[i];
		while(rule){
			Node temp=rule->head;
			while(temp){
				if(temp->next!=NULL)
					if(First[temp->next->id][1]!=NULL){
						Follow[temp->id]=joinNodeList(Follow[temp->id],First[temp->next->id][1]);
						Node temp2=temp->next;
						while(temp2->next){
							if(First[temp2->next->id][1]!=NULL){
								Follow[temp->id]=joinNodeList(Follow[temp->id],First[temp2->next->id][1]);
								break;
							}
							else
								Follow[temp->id]=joinNodeList(Follow[temp->id],First[temp2->next->id][0]);
							temp2=temp2->next;
						}
					}
					else{
						Follow[temp->id]=joinNodeList(Follow[temp->id],First[temp->next->id][0]);
						break;
					}
				else{
					Follow[temp->id]=joinNodeList(Follow[temp->id],Follow[i]);
				}
				temp=temp->next;
			}
			rule=rule->next;
		}
	}
}
char* getTokenFromId(int id){
	return symbolArray[id];
}

Grammar getGrammar(){
	return gram;
}
void FirstAndFollow(Grammar gram){
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
	populateFirst(gram);
	// populateFollow(gram);
}
void printFirst(){
	for(int i=0; i<no_of_nt+no_of_t+1; i++){
		Node temp, temp2;
		printf("First of %s %d ==>\n", getTokenFromId(i), i);
		if(First[i][0]!=NULL){
			temp = First[i][0];
			printf("\ntemp started ==>");
			while(temp!=NULL){
				printf("%s\t",getTokenFromId(temp->id));
				temp=temp->next;
			}
		}
		if(First[i][1]!=NULL){
			temp2 = First[i][1];
			printf("\ntemp2 started ==>");
			while(temp2!=NULL){
				printf("%s\t",getTokenFromId(temp2->id));
				temp2=temp2->next;
			}
		}
		if(First[i][0]!=NULL && First[i][1]!=NULL){
			printf("ERROR\n");
			continue;
		}
		printf("\n");
	}				
}

int size_table = 102;
hashTable Table; 

int hash(char *v, int M){ 
	int h = 0, a = 257;
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}

void populateHashTable(){

	Table = (hashTable)malloc(size_table*sizeof(hashNode*));
	for(int i = 0;i < size_table;i++)
	{
		Table[i] = (hashNode*)malloc(sizeof(hashNode));
		Table[i]->next == NULL;
	}


	int i = 0;

	for(;i < no_of_nt;i++)
	{
		int hval = hash(ntArray[i],size_table);
		if(Table[hval]->next == NULL)
		{
			Table[hval]->next = (hashNode*)malloc(sizeof(hashNode));
			Table[hval]->next->index = i;
			strcpy(Table[hval]->next->s, ntArray[i]);
			Table[hval]->next->next = NULL;
		}

		else
		{
			hashNode* ptr = Table[hval]->next;
			while(ptr != NULL && ptr->next != NULL)
				ptr = ptr->next;

			ptr->next = (hashNode*)malloc(sizeof(hashNode));
			ptr->next->index = i;
			strcpy(ptr->next->s, ntArray[i]);
			ptr->next->next = NULL;
		}
	}

	for(;i<size_table;i++)
	{
		int hval = hash(tokenArray[i-no_of_nt],size_table);
		if(Table[hval]->next == NULL)
		{
			Table[hval]->next = (hashNode*)malloc(sizeof(hashNode));
			Table[hval]->next->index = i;
			strcpy(Table[hval]->next->s, tokenArray[i-no_of_nt]);
			Table[hval]->next->next = NULL;
		}

		else
		{
			hashNode* ptr = Table[hval]->next;
			while(ptr != NULL && ptr->next != NULL)
				ptr = ptr->next;

			ptr->next = (hashNode*)malloc(sizeof(hashNode));
			ptr->next->index = i;
			strcpy(ptr->next->s, tokenArray[i-no_of_nt]);
			ptr->next->next = NULL;
		}
	}
}

int findIndex(char* s)
{
	int in = hash(s,size_table);

	hashNode* ptr = Table[in];

	while(ptr != NULL)
	{
		if(strcmp(ptr->s,s) == 0)
			return ptr->index; 
		ptr = ptr->next;
	}

	return -1;
}


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
			Node newNode = (Node)malloc(sizeof(struct node));
			newNode->id = tokenIndex;
			newNode->next = r->head;
			r->head = newNode;
			r->size++;
			token = strtok(NULL, " \n\r");

		}
		if(gram.rules[lhsIndex]==NULL){
			gram.rules[lhsIndex] = r;
		}
		else{
			RuleRHS h = gram.rules[lhsIndex];
			while(h->next!=NULL){
				h = h->next;
			}
			h->next = r;
		}
		ruleNo++;
	}
	fclose(gramFile);
}

ParseTree createPTNode(int id){
	ParseTree new = (ParseTree)malloc(sizeof(struct parseTree));
	new->next = NULL;
	new->children = NULL;
	new->tk = NULL;
	new->parent = NULL;
	new->non_term_id = id;
	return new;
}
//A number greater than zero in parseTable represents the ruleNo; 0 represents Syn;-1 represents error
parseTable createParseTable(parseTable T){
	T = (parseTable)malloc(no_of_nt*sizeof(int*));

	for(int i = 0;i < no_of_nt;i++)
		T[i] = (int*)malloc(no_of_t*sizeof(int));

	for(int i = 0;i < no_of_nt;i++)
		for(int j = 0;j < no_of_t;j++)
			T[i][j] = -1;


	for(int i = 0;i < no_of_nt;i++)
	{
		RuleRHS ruleIter = gram.rules[i];
		while(ruleIter != NULL)
		{
			Node RHSIter = ruleIter->head;
			while(RHSIter && RHSIter->next)
				RHSIter = RHSIter->next;

			Node firstIter;

			if(First[RHSIter->id][0] == NULL)
				firstIter = First[RHSIter->id][1];
			else
				firstIter = First[RHSIter->id][0];

			while(firstIter)
			{	
				if(firstIter->id != 101)
					T[i][firstIter->id-no_of_nt] = ruleIter->ruleNo;

				else
				{
					Node followIter = Follow[i];
					while(followIter)
					{
						T[i][followIter->id-no_of_nt] = ruleIter->ruleNo;
						followIter = followIter->next;
					}
				}
				firstIter = firstIter->next;
			}
			ruleIter = ruleIter->next;
		}

		//setting parseTable value 0 for all the current error terms to add them to syn set
		for(int j = 0;j < no_of_t;j++)
		{
			Node followIter = Follow[i];
			while(followIter)
			{
				if(T[i][followIter->id-no_of_nt] == -1)
					T[i][followIter->id-no_of_nt] = 0;

				followIter = followIter->next;
			}
		}
	}
	return T;
}

void printParseTable(parseTable T){
	// int i, j;
	for(int i=0; i<no_of_nt; i++){
		// printf("%s\n", );
		for(int j=0; j<no_of_t; j++){
			printf("%d ", T[i][j]);
		}
		printf("\n");
	}
}

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
		// int f=0;
		int tokenIndex=findIndex(token);
		if(tokenIndex==-1)
			continue;
		int epsFlag=0;
		if(tokenIndex==eps)
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
			if(tokenIndex==eps)
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
	// for(int i=0; i<no_of_nt+1; i++){
	// 	Node temp;
	// 	if(Follow[i]!=NULL)
	// 		temp = Follow[i];
	// 	else 
	// 		continue;
	// 	printf("%s ==> ", getTokenFromId(i));

	// 	while(temp!=NULL){
	// 		printf("%d\t", temp->id);
	// 		temp = temp->next;
	// 	}
	// 	printf("\n");

	// }
}

void inorderTraversal(ParseTree PT, FILE* fp1){
	if(PT==NULL)
		return;
	if(PT->non_term_id)
		printf("----\t----\t");
	else
		printf("%s\t%llu\t", PT->tk->name, PT->tk->lineNo);
	// printf("");
	inorderTraversal(PT->children, fp1);
	inorderTraversal(PT->next, fp1);
}

void printParseTree(ParseTree PT, char *outfile){
	FILE* fp1 = fopen(outfile, "w");
	// printf("")
	inorderTraversal(PT, fp1);
	fclose(fp1);
}

void parseInputSourceCode(char *testcaseFile, parseTable T){
	Stack s = intialiseStack();
	s = pushStack(s,TK_DOLLAR+no_of_nt,NULL);
	ParseTree PT = NULL;
	PT = createPTNode(program);
	s = pushStack(s,program,PT);
	ParseTree ptr = PT->children;
	buildRules();
	// gram = getGrammar();
	ReadFromFileFirstAndFollow(gram);
	/*
	TO CALL
	build_rules
	first and follow
	*/

	// parseTable T;
	T = createParseTable(T);

	FILE* fp = fopen(testcaseFile,"r");

	TokenInfo token = getNextToken(fp);
	int errorFlag=0;
	while(1){
		// printf("bla\n");
		int X = topStack(s)->id;
		printf("%s\t%s\n", ntArray[X],tokenArray[token->tid]);
		if(token->tid == TK_DOLLAR && topStack(s)->id == TK_DOLLAR)
			break;
		else if(token->tid == topStack(s)->id)
		{
			s = popStack(s);
			ptr = createPTNode(-1);
			ptr->tk = token;
			ptr = topStack(s)->pt_node;
			token = getNextToken(fp);
		}
		else if(topStack(s)->id < no_of_nt)
		{
			if(T[X][token->tid] > 0)
			{
				s = popStack(s);
				int ruleNo = T[X][token->tid];
				RuleRHS ruleIter = gram.rules[X];
				while(ruleIter && ruleIter->ruleNo != ruleNo)
				{
					ruleIter = ruleIter->next;
				}

				Node rhsIter = ruleIter->head;

				while(rhsIter)
				{
					ParseTree newNode = createPTNode(rhsIter->id);
					s = pushStack(s,rhsIter->id,newNode);
					if(ptr == NULL)
						ptr = newNode;
					else
					{
						newNode->next = ptr;
						ptr = newNode;
					}
					rhsIter = rhsIter->next;
				}

				ptr = ptr->children;
			}
			else if(T[X][token->tid] == 0) // Panic Mode : syn set
			{
				printf("ERROR1 : Unexpected Token: %s at line no. %llu\n",tokenArray[token->tid], token->lineNo);
				s = popStack(s);
				ptr = topStack(s)->pt_node;
				errorFlag = 1;
			}
			else //Panic Mode : Error
			{
				printf("ERROR2 : Unexpected Token: %s at line no. %llu\n",tokenArray[token->tid], token->lineNo);
				token = getNextToken(fp);
				errorFlag = 1;
			}	
		}
		else
		{
			errorFlag = 1;
			break;
			//we dont know: token in stack not match current token from input
		}
	}
	fclose(fp);
	if(!errorFlag)
		printf("Input source code is syntactically correct...........\n");
	return;
}
