#include "symboltableDef.h"
#include "parser.h"
#include "parserDef.h"
#include "lexer.h"
#include "ast.h"

#ifndef SYMBOLTABLE
#define SYMBOLTABLE

symbolTable createSymbolTable(ParseTree PT, int size);

// scopeTable createScopeTable(char* scope,entry *current);

void printSymbolTable(symbolTable st, int size);

symbolTable insert(symbolTable ST, entry *en);
// symbolTable closeScope(symbolTable ST);
// symbolTable openScope(symbolTable ST,int sz);

entry* lookup(symbolTable ST, char* name);

void freeScopeTable(scopeTable s);

// symbolTable addInGlobal(symbolTable ST,entry *en);
// entry* makeRecordEntry(ParseTree PT);
#endif

