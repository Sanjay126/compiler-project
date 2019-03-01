# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <stdbool.h>
# include "lexerdef.h"

#ifndef LEXER
#define LEXER

tokenInfo* getNextToken(FILE *fp);
tokenInfo* newToken(char* str,tokenId tid);
char getNextChar(FILE *fp);

#endif
