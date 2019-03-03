#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "lexer.h"
#include "parserDef.h"

Node First[no_of_nt+no_of_t+1][2];
Node Follow[no_of_nt+1];
Node addNode(int id,Node head){
	Node newNode=(Node)malloc(sizeof(struct node));
	newNode->id=id;
	newNode->next=head;
	return newNode;
}
Node getFirstOfNT(Grammer gram,int i){
	if(First[i][0]==NULL&&First[j][0]==NULL){
		RuleRHS rule=gram.rules[i-no_of_t];
			while(rule){
				Node temp=rule->head;
				while(temp){
					int j=temp->id;
					if(First[j][0]!=NULL)
				}
			}
	}
	else if(First[i][0]!=NULL)
		return First[i][0];
	else
		return First[i][1];
}
void populateFirst(Grammer gram){
	
	for(int i=1;i<=no_of_nt;i++){
		First[i]=getFirstOfNT(gram,i);
	}
	for(int i=no_of_nt+1;i<=no_of_t+no_of_nt;i++){
		First[i][0]=addNode(i,First[i][0]);
	}
}

void FirstAndFollow(Grammer gram){
	populateFirst(Grammer);
	populateFollow(Grammer);
}

int size_table = 102;
hashTable Table; 

int hash(char *v, int M){ 
	int h = 0, a = 257;
    for (; *v != 0; v++)
        h = (a*h + *v) % M;
    return h;
}

void pupulateHashTable(){

	Table = (hashTable)malloc(size_table*sizeof(hashNode*));
	for(int i = 0;i < size_table;i++)
	{
		Table[i] = (hashNode*)malloc(sizeof(hashNode));
		Table[i]->next == NULL;
	}


	int i = 0;

	for(;i < num_of_nt;i++)
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
		int hval = hash(tokenArray[i-num_of_nt],size_table);
		if(Table[hval]->next == NULL)
		{
			Table[hval]->next = (hashNode*)malloc(sizeof(hashNode));
			Table[hval]->next->index = i;
			strcpy(Table[hval]->next->s, tokenArray[i-num_of_nt]);
			Table[hval]->next->next = NULL;
		}

		else
		{
			hashNode* ptr = Table[hval]->next;
			while(ptr != NULL && ptr->next != NULL)
				ptr = ptr->next;

			ptr->next = (hashNode*)malloc(sizeof(hashNode));
			ptr->next->index = i;
			strcpy(ptr->next->s, tokenArray[i-num_of_nt]);
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

void createParseTable(char* F, table T){
	T = (table)malloc(no_of_nt*sizeof(int));

	for(int i = 0;i < no_of_nt;i++)
		T[i] = (int*)malloc(no_of_t*sizeof(int));

	for(int i = 0;i < no_of_nt;i++)
		for(int j = 0;j < no_of_t;j++)
			T[i][j] = -1;

	FILE *gramFile = fopen("grammer.txt","r");

	char buff[MAX_LENGTH];


	while(!feof(gramFile))
	{
		fscanf(gramFile,"%[^\n]", buff);
		
		char lhs[MAX_LENGTH];
		int i = 0;
		while(buff[i] != ' ')
		{
			lhs[i] = buff[i];
			i++;
		}	

		buff[i] = '\0';
	}
	


}