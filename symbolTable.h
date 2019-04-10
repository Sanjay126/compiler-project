#include "symboltableDef.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

symbolTable createSymbolTable(parseTree PT, int size);

scopeTable createScopeTable(char* scope,entry *current);

void printSymbolTable(symbolTable st, int size);

void Insert();

void CloseScope();

symbolTable OpenScope(struct symbolTable ST,struct entry* en);

struct entry* Lookup(struct symbolTable ST, char* name,char* type, int lineNo);

//entry* getInputParameter(symbolTable st,char)



