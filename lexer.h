# include "lexerDef.h"

#ifndef LEXER
#define LEXER

void removeComments(char *testcaseFile, char *cleanFile);
char getNextChar(FILE *fp);
TokenInfo generateNewToken(char* str,tokenId tid);
TokenInfo cmpFunc(char* str);
TokenInfo getNextToken(FILE *fp);


#endif
