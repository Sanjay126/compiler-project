/*
Group Number - 12
Sanjay Malhotra 2016A7PS0126P
Nilay Arora 2016A7PS0117P
Tushar Goel 2016A7PS0023P
Adit Shastri 2016A7PS0121P
*/
# include "lexerDef.h"

#ifndef LEXER
#define LEXER

//removing comments
void removeComments(char *testcaseFile, char *cleanFile);

//retreiving next token from file
TokenInfo getNextToken(FILE *fp);

void intialiseGlobalVariablesLexer();

#endif
