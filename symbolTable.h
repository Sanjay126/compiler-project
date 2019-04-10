#include "symboltableDef.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

symbolTable createSymbolTable(parseTree PT, int size);

scopeTable createScopeTable(char* scope,entry *current);

void printSymbolTable(symbolTable st, int size);

//entry* getInputParameter(symbolTable st,char)



