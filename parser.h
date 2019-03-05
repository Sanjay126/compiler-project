/*
Group Number - 12
Sanjay Malhotra 2016A7PS0126P
Nilay Arora 2016A7PS0117P
Tushar Goel 2016A7PS0023P
Adit Shastri 2016A7PS0121P
*/
#include "parserDef.h"

#ifndef PARSER
#define PARSER

//Utility Functions for handling linked list
Node addNode(int id,Node head, int end, int dup);
Node createCopyNodeList(Node head);
Node joinNodeList(Node n1,Node n2);
Node reverseList(Node ls);

//printing first
void printFirst(); 

//Stack operations
Node intialiseNode(int id);
SNode intialiseSNode(int id,ParseTree pt_node);
Stack intialiseStack();
Stack pushStack(Stack s, int id,ParseTree pt_node);
Stack popStack(Stack s);
SNode topStack(Stack s);

//Hashtable implementation
int hash(char *v, int M);
void populateHashTable();
int findIndex(char* s);

// Storing grammer from grammer File
void buildRules();

//Grammer utility functions
char* getTokenFromId(int id);
Grammar getGrammar();

//Creating Parse Table
parseTable createParseTable(parseTable T);
void printParseTable(parseTable T);

//Reading First and Follow from files
void ReadFromFileFirstAndFollow(Grammar gram);

//Parsing and forming ParseTree
ParseTree createPTNode(int id);
ParseTree parseInputSourceCode(char *testcaseFile, parseTable T,ParseTree PT);

//Printing ParseTree
void inorderTraversal(ParseTree PT, FILE* fp1);
void printParseTree(ParseTree PT, char *outfile);
#endif