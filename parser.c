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
			free(newNode);
			return head;
		}
		temp=temp->next;
	}
	if(temp->id==id){
		free(newNode);
		return head;
	}
	temp->next=newNode;
	return head;
}
Node createCopyNodeList(Node head){
	Node newNode=NULL;
	Node temp=head;
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

Node* getFirstOfNT(int i){
	if(First[i][0]==NULL&&First[i][1]==NULL){
		RuleRHS rule=gram.rules[i];
			while(rule){
				Node temp=rule->head;
				int epsFlag=0;
				while(temp){
					int j=temp->id;
					if(j==101){			//case 2 from slides
						First[i][1]=First[i][0];
						First[i][0]=NULL;
						First[i][1]=addNode(j,First[i][1]);
						// return First[i];
					}
					else if(j!=i){
						// printf("\n\nCASE 3\n\n");
						Node* firstOfJ=getFirstOfNT(temp->id);
						// printf("%d\n", firstOfJ[0]->id);
						First[i][0]=joinNodeList(First[i][0],firstOfJ[0]);
						First[i][1]=joinNodeList(First[i][1],firstOfJ[1]);
						if(firstOfJ[1]!=NULL)	//case 3 from slides
							epsFlag=1;
						else{
							epsFlag=0;
							break;
						}
					}
					temp=temp->next;
				}
				if(epsFlag==1&&First[i][1]==NULL&&First[i][0]!=NULL){      //last case from slides
					// printf("\n\nCASE 4\n\n");
					First[i][1]=First[i][0];
					First[i][0]=NULL;
					First[i][1]=addNode(101,First[i][1]);
				}
				rule=rule->next;

			}
		return First[i];

	}
	else
		return First[i];
}
void populateFirst(Grammar gram){

	for(int i=no_of_nt;i<no_of_t+no_of_nt;i++){
		First[i][0]=addNode(i,First[i][0]); 	//case 1 from slides.
	}
	for(int i=0;i<no_of_nt;i++){
		Node* newFirst=getFirstOfNT(i);
		First[i][0]=newFirst[0];
		First[i][1]=newFirst[1];
	}
	printf("First Done\n");
}
void populateFollow(Grammar gram){
	//fill this
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
}

//A number greater than zero in parseTable represents the ruleNo; 0 represents Syn;-1 represents error
void createParseTable(parseTable T){
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
					T[i][firstIter->id] = ruleIter->ruleNo;

				else
				{
					Node followIter = Follow[i];
					while(followIter)
					{
						T[i][followIter->id] = ruleIter->ruleNo;
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
				if(T[i][followIter->id] == -1)
					T[i][followIter->id] = 0;

				followIter = followIter->next;
			}
		}
	}
}