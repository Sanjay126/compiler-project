#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
char* tArray[] = {   "TK_ASSIGNOP",    "TK_COMMENT",    "TK_FIELDID",    "TK_ID",    "TK_NUM",    "TK_RNUM",    "TK_FUNID",    "TK_RECORDID",    "TK_WITH",    "TK_PARAMETERS",    "TK_END",    "TK_WHILE",    "TK_INT",    "TK_REAL",    "TK_TYPE",    "TK_MAIN",    "TK_GLOBAL",    "TK_PARAMETER",    "TK_LIST",    "TK_SQL",    "TK_SQR",    "TK_INPUT",    "TK_OUTPUT",    "TK_SEM",    "TK_COLON",    "TK_DOT",    "TK_COMMA",    "TK_ENDWHILE",    "TK_OP",    "TK_CL",    "TK_IF",    "TK_THEN",    "TK_ENDIF",    "TK_READ",    "TK_WRITE",    "TK_RETURN",    "TK_PLUS",    "TK_MINUS",    "TK_MUL",    "TK_DIV",    "TK_CALL",    "TK_RECORD",    "TK_ENDRECORD",    "TK_ELSE",    "TK_AND",    "TK_OR",    "TK_NOT",    "TK_LT",    "TK_LE",    "TK_EQ",    "TK_GT",    "TK_GE",    "TK_NE",    "TK_DOLLAR",    "eps"};
Grammar gram;
int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Invalid Arguements.\n");
		return 1;
	}
	int opt;

	while(1){
		printf("\n---------------------------------------------------\n");
		printf("Press a number to choose the corresponding option\n");
		printf("Press 1 for making the comment free code.\n");
		printf("Press 2 for printing the token list generated by the lexer\n");
		printf("Press 3 for printing the parse tree and errors.\n");
		printf("Press 4 for printing the total time taken\n");
		printf("Press 0 to exit!\n");
		printf("---------------------------------------------------\n");

		scanf("%d", &opt);
		switch(opt){
			case 0:
				printf("\n\nEXITING...\n");
				return 0;
			case 1:{
				removeComments(argv[1], argv[2]);	// remove comments.
				printf("Comment free code done.\n");
			}break;
			case 2:
			{
				FILE *fp = fopen(argv[1],"r");

				while(1){
					tokenInfo *tk = getNextToken(fp);
					if(tk==NULL)
						continue;
					printf("TOKEN: '%s' VALUE: '%s'  LINENO.: %llu\n", tArray[tk->tid], tk->name, tk->lineNo);
					if(tk->tid==TK_DOLLAR)
						break;
				}
				fclose(fp);
				// printf("\n");
			}break;
			case 3:{
				// FirstAndFollow(gram);
				parseTable T;// = createParseTable(T);
				// ReadFromFileFirstAndFollow(gram);
				parseInputSourceCode(argv[1], T);
				// printParseTable(T);
				// for(int i=0; i<no_of_nt; i++){
				// 	// printf("%s\n", );
				// 	// for(int )

				// 	RuleRHS r = gram.rules[i];
				// 	while(r!=NULL){
				// 		Node h = r->head;
				// 		// printf("%s\n", );
				// 		printf("RuleNo. - %d \t%s\t ==> ", r->ruleNo, getTokenFromId(i));
				// 		while(h!=NULL){
				// 			printf("%s\t", getTokenFromId(h->id));
				// 			h = h->next;
				// 		}
				// 		printf("\n");
				// 		r = r->next;
				// 	}
				// }
			}break;
			default :
				printf("invalid option");
				return 0;
		}
	}
	return 0;
}