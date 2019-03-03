# include "lexerDef.h"

#ifndef LEXER
#define LEXER

void removeComments(char *testcaseFile, char *cleanFile);
char getNextChar(FILE *fp);
tokenInfo* generateNewToken(char* str,tokenId tid);
tokenInfo* cmpFunc(char* str);
tokenInfo* getNextToken(FILE *fp);


#endif
