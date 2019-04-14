#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "symbolTable.h"

char* tArray[] = {"TK_WITH","TK_THEN","TK_TYPE","TK_EQ","TK_DOLLAR","TK_COMMA","TK_RNUM","TK_CL","TK_AND","TK_PARAMETER","TK_DIV","TK_ID","TK_GLOBAL","TK_GE","TK_LIST","TK_IF","TK_RECORD","TK_NUM","TK_NE","TK_LE","TK_DOT","TK_INPUT","TK_MAIN","TK_CALL","TK_PLUS","TK_ELSE","TK_COMMENT","TK_ASSIGNOP","TK_WRITE","TK_COLON","TK_WHILE","TK_REAL","TK_READ","TK_NOT","TK_SQL","TK_OUTPUT","TK_SQR","TK_ENDIF","TK_MINUS","TK_SEM","TK_FIELDID","TK_PARAMETERS","TK_OP","TK_ENDWHILE","TK_MUL","TK_FUNID","TK_INT","TK_RETURN","TK_GT","TK_END","TK_LT","TK_OR","TK_ENDRECORD","eps","TK_RECORDID"};
Grammar gram;
int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Invalid Arguements.\n");
		return 1;
	}
	int ASTnodes;
	int ParseTreeNodes;
	symbolTable* symtab;
	parseTable T;
	ParseTree PT = parseInputSourceCode(argv[1], T, PT);
	// printParseTree(PT,"parsetree.txt");
	// printPARSETREENEW(PT,"parsetree1.txt");
	noOfASTnodes=noOfPTNode;
	PT = createAST(PT);
	symtab= createSymbolTable(PT);
	int opt;
    clock_t    start_time, end_time;

    double total_CPU_time, total_CPU_time_in_seconds;
	
	printf("(a) FIRST and FOLLOW not set automated\n");
	printf("(b) Both lexical and syntax analysis modules implemented\n");
	printf("(c) modules compile properly\n");
	printf("(d) modules work with testcases 2, 3 and 4 only \n");
	printf("(e) parse tree is being constructed\n");
	
	while(1){
		printf("\n---------------------------------------------------\n");
		printf("Press a number to choose the corresponding option\n");
		printf("Press 1 for making the comment free code.\n");
		printf("Press 2 for printing the token list generated by the lexer\n");
		printf("Press 3 for printing the parse tree and errors.\n");
		printf("Press 4 for displaying the memory allocated\n");
		printf("Press 5 for printing symbol table\n");
		printf("Press 6 for printing list of global variables\n");
		printf("Press 7 for printing total memory required by each function\n");
		printf("Press 8 for printing the type expressions of records\n");
		printf("Press 9 for printing the time taken and report errors\n");
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
				intialiseGlobalVariablesLexer();
				FILE *fp = fopen(argv[1],"r");

				while(1){
					TokenInfo tk = getNextToken(fp);
					if(tk==NULL)
						continue;
					printf("TOKEN: '%s' VALUE: '%s'  LINENO.: %llu\n", tArray[tk->tid], tk->name, tk->lineNo);
					if(tk->tid==TK_DOLLAR)
						break;
				}
				fclose(fp);
			}break;
			case 3:{
				intialiseGlobalVariablesLexer();
				// initialiseGlobalVariablesParser();
				
				printf("\nInorder traversal of Abstract Syntax Tree in file AST.txt\n");
				printParseTree(PT,"AST.txt");
				// printPARSETREENEW(PT,"parsetree2.txt");
				// symbolTable* ST =  createSymbolTable(PT);
				freeMemory();
			}break;
			case 4:
				ParseTreeNodes=noOfPTNode;
				ASTnodes=noOfASTnodes;
				printf(" \nParseTree number of Nodes=%d\t\tmemory allocated=%d\n",ParseTreeNodes,ParseTreeNodes*12);
				printf(" \nAbstract Syntax Tree number of Nodes=%d\t\tmemory allocated=%d\n",ASTnodes,ASTnodes*12);
    			float compression=((float)(ParseTreeNodes-ASTnodes)/ParseTreeNodes)*100;
    			printf(" \nCompression Percentage = %f",compression); 
    			break;
				//start_time = clock();
				// {
				// 	intialiseGlobalVariablesLexer();
				// 	parseTable T;
				// 	ParseTree PT = parseInputSourceCode(argv[1], T, PT);
				// 	printParseTree(PT,"parsetree.txt");
				// 	freeMemory();
    //             }

    //             end_time = clock();

    //             total_CPU_time  =  (double) (end_time - start_time);
    //             total_CPU_time_in_seconds =   total_CPU_time / CLOCKS_PER_SEC;

    //             printf("\nTOTAL_CPU_TIME:\t%lf\nTOTAL_CPU_TIME_IN_SECONDS:\t%lf\n\n", total_CPU_time, total_CPU_time_in_seconds);
                // break;
             // Print both total_CPU_time and total_CPU_time_in_seconds 
			case 5:
				printSymbolTable(symtab);
				break;
			case 6:
				printGlobalVariables(symtab);
				break;
			case 7:
				printMemoryReq(symtab);
				break;
			case 8:
				printAllRecords(symtab);
				break;
			case 9:
			case 10:
				printf("not Implemented");
				break;
			default :
				printf("\n\n---Invalid Option---\n\n");
				continue;
		}
	}
	return 0;
}