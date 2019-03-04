#include "parserDef.h"

#ifndef PARSER
#define PARSER

Node addNode(int id,Node head, int end);
Node createCopyNodeList(Node head);
Node joinNodeList(Node n1,Node n2);

Node getFirstOfNT(int i);
void populateFirst(Grammar gram);
void populateFollow(Grammar gram);
void FirstAndFollow(Grammar gram);
void printFirst();

int hash(char *v, int M);
void populateHashTable();
int findIndex(char* s);
void buildRules();
char* getTokenFromId(int id);
void createParseTable(parseTable T);
Grammar getGrammar();
void printParseTree(parseTree PT, char *outfile);
void parseInputSourceCode(char *testcaseFile, table T);
#endif