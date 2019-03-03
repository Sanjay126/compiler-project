#include "parserDef.h"

#ifndef PARSER
#define PARSER

Node addNode(int id,Node head);

Node getFirstOfNT(Grammar gram,int i);
void populateFirst(Grammar gram);
void FirstAndFollow(Grammar gram);


int hash(char *v, int M);
void populateHashTable();
int findIndex(char* s);
void buildRules();

void createParseTable(parseTable T);
#endif