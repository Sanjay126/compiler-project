#include "parserDef.h"

#ifndef PARSER
#define PARSER

Node addNode(int id,Node head, int end);
Node createCopyNodeList(Node head);
Node joinNodeList(Node n1,Node n2);

Node getFirstOfNT(Grammar gram,int i);
void populateFirst(Grammar gram);
void populateFollow(Grammar gram);
void FirstAndFollow(Grammar gram);


int hash(char *v, int M);
void populateHashTable();
int findIndex(char* s);
void buildRules();

void createParseTable(parseTable T);
#endif