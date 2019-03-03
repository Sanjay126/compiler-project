#include "parser.h"

void createParseTable(char* F, table T){
	T = (table)malloc(num_of_nt*sizeof(int));

	for(int i = 0;i < num_of_nt;i++)
		T[i] = (int*)malloc(num_of_t*sizeof(int));

	for(int i = 0;i < num_of_nt;i++)
		for(int j = 0;j < num_of_t;j++)
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