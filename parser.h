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


//Grammer utility functions
char* getTokenFromId(int id);
Grammar getGrammar();


//Reading First and Follow from files
void ComputeFirstAndFollowSets(Grammar gram);

//Parsing and forming ParseTree
ParseTree parseInputSourceCode(char *testcaseFile, parseTable T,ParseTree PT);

//Printing ParseTree
void printParseTree(ParseTree PT, char *outfile);
void printPARSETREENEW(ParseTree PT, char *outfile);

void freeMemory();

#endif