#include "parserDef.h"

#ifndef PARSER
#define PARSER

Node addNode(int id,Node head, int end, int dup);
Node createCopyNodeList(Node head);
Node joinNodeList(Node n1,Node n2);

// void getFirstOfNT(int i);
// Node* FirstSetUtil(Node ruleHead);
// void populateFirst(Grammar gram);
// void populateFollow(Grammar gram);
// void FirstAndFollow(Grammar gram);
void printFirst();


Node intialiseNode(int id);
SNode intialiseSNode(int id,ParseTree pt_node);
Stack intialiseStack();
Stack pushStack(Stack s, int id,ParseTree pt_node);
Stack popStack(Stack s);
SNode topStack(Stack s);

Node reverseList(Node ls);

int hash(char *v, int M);
void populateHashTable();
int findIndex(char* s);
void buildRules();
char* getTokenFromId(int id);
ParseTree createPTNode(int id);
parseTable createParseTable(parseTable T);
void printParseTable(parseTable T);
Grammar getGrammar();
void inorderTraversal(ParseTree PT, FILE* fp1);
void printParseTree(ParseTree PT, char *outfile);
void ReadFromFileFirstAndFollow(Grammar gram);
ParseTree parseInputSourceCode(char *testcaseFile, parseTable T,ParseTree PT);
#endif