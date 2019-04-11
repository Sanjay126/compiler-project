#include "symboltableDef.h"
#include "parser.h"
#include "parserDef.h"
#include "lexer.h"
#include "ast.h"

#ifndef SYMBOLTABLE
#define SYMBOLTABLE

symbolTable* createSymbolTable(ParseTree PT);

// scopeTable createScopeTable(char* scope,entry *current);

void printSymbolTable(symbolTable* st);

symbolTable* insert(symbolTable* ST, entry *en);
// symbolTable closeScope(symbolTable ST);
// symbolTable openScope(symbolTable ST,int sz);

entry* lookup(symbolTable* ST, char* name);

void freeScopeTable(scopeTable s);

// symbolTable addInGlobal(symbolTable ST,entry *en);
// entry* makeRecordEntry(ParseTree PT);

symbolTable* enterRecords(ParseTree PT,symbolTable* ST);
Record  lookupRecord(Records r,char* name);
symbolTable* closeScope(symbolTable* ST);
symbolTable *openScope(symbolTable *ST,int sz,char* scope);


#endif

