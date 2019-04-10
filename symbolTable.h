#include "symboltableDef.h"
#include "parser.h"
#include "parserDef.h"
#include "lexer.h"
#include "ast.h"

#ifndef SYMBOLTABLE
#define SYMBOLTABLE

symbolTable createSymbolTable(ParseTree PT, int size);

scopeTable createScopeTable(char* scope,entry *current);

void printSymbolTable(symbolTable st, int size);

symbolTable Insert(symbolTable ST, entry *en);
symbolTable closeScope(symbolTable ST);
symbolTable OpenScope(symbolTable ST,int sz);

entry* Lookup(symbolTable ST, char* name);

void freeScopeTable(scopeTable s);

#endif

