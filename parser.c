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