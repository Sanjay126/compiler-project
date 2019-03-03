#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "parserDef.h"

Grammar gram;

Node **First;
Node *Follow;

Node addNode(int id,Node head){
	// if(head==)
	Node newNode=(Node)malloc(sizeof(struct node));
	newNode->id=id;
	newNode->next=head;
	return newNode;
}
// Node getFirstOfNT(Grammar gram,int i){
// 	if(First[i][0]==NULL&&First[i][1]==NULL){
// 		RuleRHS rule=gram.rules[i-no_of_t];
// 			while(rule){
// 				Node temp=rule->head;
// 				while(temp){
// 					int j=temp->id;
// 					if(First[j][0]!=NULL)
// 				}
// 			}
// 	}
// 	else if(First[i][0]!=NULL)
// 		return First[i][0];
// 	else
// 		return First[i][1];
// }
// void populateFirst(Grammar gram){
	
// 	for(int i=1;i<=no_of_nt;i++){
// 		First[i]=getFirstOfNT(gram,i);
// 	}
// 	for(int i=no_of_nt+1;i<=no_of_t+no_of_nt;i++){
// 		First[i][0]=addNode(i,First[i][0]);
// 	}
// }

// void FirstAndFollow(Grammar gram){
// 	First = (Node**)malloc(sizeof(Node*)*(no_of_nt+no_of_t+1));
// 	Follow = (Node*)malloc(sizeof(Node)*(no_of_nt+1));
// 	for(int i=0; i<no_of_nt+no_of_t+1; i++){
// 		First[i] = (Node*)malloc(sizeof(Node)*2);
// 	}
// 	populateFirst(gram);
// 	populateFollow(gram);
// }

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
	FILE *gramFile = fopen("Grammar.txt","r");
	gram.rules = (RuleRHS*)malloc(sizeof(RuleRHS)*no_of_nt);
	for(int i=0; i<no_of_nt; i++)
		gram.rules[i]=NULL;
	

	int ruleNo=1;
	char buff[MAX_LENGTH];
	while(!feof(gramFile))
	{
		fscanf(gramFile,"%[^\n]", buff);
		char* lhs = strtok(buff, " ");
		int lhsIndex = findIndex(lhs);
		// Node lhsNode = (Node)malloc(sizeof(struct node));
		// lhsNode->
		char* token = strtok(NULL, " ");
		RuleRHS r = (RuleRHS)malloc(sizeof(struct ruleRHS));
		r->next = NULL;
		r->head = NULL;
		r->size = 0;
		r->ruleNo = ruleNo;
		while(token!=NULL){
			int tokenIndex = findIndex(token);
			r->head = addNode(tokenIndex, r->head);
			r->size++;
		}
		if(gram.rules[lhsIndex]==NULL){
			gram.rules[lhsIndex] = r;
		}
		else{
			RuleRHS h = gram.rules[lhsIndex];
			while(h!=NULL && h->next!=NULL){
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