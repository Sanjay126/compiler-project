#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.c"	
#include "lexer.c"
// #include "ast.h"

ParseTree ast;

ParseTree del_child(ParseTree node,int i)
{
	ParseTree ptr = node->children;
	if(i == 0)
	{
		if(ptr->children == NULL)
		{
			node->children = ptr->next;
			free(ptr);
		}
		else
		{
			ParseTree tmp = ptr->next;
			node->children = ptr->children;
			ParseTree ptr2 = ptr->children;
			free(ptr);
			while(ptr2 != NULL && ptr2->next)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			ptr2->parent = node;
			ptr2->next = tmp;
		}
	}
	else
	{
		int count = 0;
		ParseTree prev;

		while(count < i && ptr)
		{
			prev = ptr;
			ptr = ptr->next;
			count++;
		}
		if(ptr->next == NULL)
		{
			prev->next = ptr->children;
			ParseTree ptr2 = ptr->children;
			while(ptr2)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			free(ptr);
		}
		else
		{
			prev->next = ptr->children;
			ParseTree ptr2 = ptr->children;
			while(ptr2 && ptr2->next)
			{
				ptr2->parent = node;
				ptr2 = ptr2->next;
			}
			ptr2->parent = node;
			ptr2->next = ptr->next;
			free(ptr);
		}
	}

	return node;
}


ParseTree case1(ParseTree p){
	
	//<program> ===> <otherFunctions> <mainFunction>

	p = del_child(p, 0);
	return p;
}

ParseTree case2(ParseTree p){
	//<mainFunction>===> TK_MAIN <stmts> TK_END
	
	p = del_child(p, 2);
	p = del_child(p, 0);
	return p;
}

ParseTree case3(ParseTree p){
	//<otherFunctions> ===> <function> <otherFunctions1>

	p = del_child(p, 1);
	return p;
}

ParseTree case4(ParseTree p){
	// <otherFunctions> ===>eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case5(ParseTree p){
	//<function> ===> TK_FUNID <input_par> <output_par> TK_SEM  <stmts>  TK_END

	p = del_child(p, 5);
	p = del_child(p, 3);
	return p;
}

ParseTree case6(ParseTree p){
	// <input_par> ===> TK_INPUT  TK_PARAMETER  TK_LIST TK_SQL  <parameter_list>  TK_SQR
	
	for(int i=0; i<3; i++){
		p = del_child(p, 1);
		p = del_child(p, 0);
	}
	return p;
}

ParseTree case7(ParseTree p){
	// <output_par> ===> TK_OUTPUT  TK_PARAMETER  TK_LIST TK_SQL  <parameter_list> TK_SQR

	for(int i=0; i<3; i++){
		p = del_child(p, 1);
		p = del_child(p, 0);
	}
	return p;
}

ParseTree case8(ParseTree p){
	//  <output_par> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case9(ParseTree p){
	// <parameter_list> ===> <dataType>  TK_ID  <remaining_list>

	p = del_child(p, 2);
	p = del_child(p, 0);
	return p;
}

ParseTree case10(ParseTree p){
	// <dataType> ===>  <primitiveDatatype>

	p = del_child(p, 0);
	return p;
}

ParseTree case11(ParseTree p){
	// <dataType> ===> <constructedDatatype>

	p = del_child(p, 0);
	return p;
}

ParseTree case12(ParseTree p){
	// <primitiveDatatype> ===> TK_INT

	return p;
}

ParseTree case13(ParseTree p){
	// <primitiveDatatype> ===> TK_REAL
	
	return p;
}

ParseTree case14(ParseTree p){
	// <constructedDatatype> ===> TK_RECORD  TK_RECORDID

	p = del_child(p, 0);
	return p;
}

ParseTree case15(ParseTree p){
	//<remaining_list> ===> TK_COMMA <parameter_list>
	
	p = del_child(p, 0);
	p = del_child(p, 0);
	return p;
}

ParseTree case16(ParseTree p){
	//<remaining_list> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case17(ParseTree p){
	 // <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>

	p = del_child(p, 2);
	return p;
}

ParseTree case18(ParseTree p){
	 // <typeDefinitions> ===> <typeDefinition> <typeDefinitions1>
	
	p = del_child(p, 1);
	return p;
}

ParseTree case19(ParseTree p){
	// <typeDefinitions> ===>  eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case20(ParseTree p){
	// <typeDefinition> ===> TK_RECORD TK_RECORDID <fieldDefinitions> TK_ENDRECORD TK_SEM

	p = del_child(p, 4);
	p = del_child(p, 3);
	p = del_child(p, 2);
	p = del_child(p, 0);
	return p;
}

ParseTree case21(ParseTree p){
	 // <fieldDefinitions> ===> <fieldDefinition1> <fieldDefinition2> <moreFields>

	p = del_child(p, 2);
	return p;
}

ParseTree case22(ParseTree p){
	// <fieldDefinition> ===> TK_TYPE <primitiveDatatype> TK_COLON TK_FIELDID TK_SEM
	for(int i=0; i<3; i++){
		p = del_child(p, i);
	}
	return p;
}

ParseTree case23(ParseTree p){
	// <moreFields> ===> <fieldDefinition> <moreFields1>

	p = del_child(p, 1);
	return p;
}

ParseTree case24(ParseTree p){
	 //<moreFields> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case25(ParseTree p){
	// <declarations> ===> <declaration> <declarations1>

	p = del_child(p, 1);
	return p;
}

ParseTree case26(ParseTree p){
	// <declarations> ===> eps

	
	return p;
}

ParseTree case27(ParseTree p){
	// <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID  <global_or_not> TK_SEM

	
	return p;
}

ParseTree case28(ParseTree p){
	 //<global_or_not> ===> TK_COLON  TK_GLOBAL

	
	return p;
}

ParseTree case29(ParseTree p){
	 //<global_or_not> ===> eps

	
	return p;
}

ParseTree case30(ParseTree p){
	 // <otherStmts> ===> <stmt> <otherStmts1>

	
	return p;
}

ParseTree case31(ParseTree p){
	// <otherStmts> ===> eps

	
	return p;
}

ParseTree case32(ParseTree p){
	// <stmt> ===> <assignmentStmt>

	
	return p;
}

ParseTree case33(ParseTree p){
	// <stmt> ===> <funCallStmt>

	
	return p;
}

ParseTree case34(ParseTree p){
	// <stmt> ===> <iterativeStmt>

	
	return p;
}

ParseTree case35(ParseTree p){
	// <stmt> ===> <conditionalStmt>

	
	return p;
}

ParseTree case36(ParseTree p){
	 //<stmt> ===> <ioStmt>

	
	return p;
}

ParseTree case37(ParseTree p){
	// <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP  <arithmeticExpression> TK_SEM

	
	return p;
}








ParseTree case(ParseTree p){

	return p;
}


ParseTree createAST(ParseTree PT)
{
	if(PT->children == NULL)
		return PT;
	ParseTree ptr  = PT->chlidren;
	while(ptr)
	{
		createAST(ptr);
		ptr = ptr->next;
	}

	switch(PT -> ruleNo)
	{

	}

	return PT;
}
// int main()
// {
// 	ParseTree* node = (ParseTree*)malloc(sizeof(ParseTree)*7);
// 	for(int i = 0;i < 7;i++)
// 	{
// 		node[i] = (ParseTree)malloc(sizeof(struct parseTree));
// 		node[i]->next = NULL;
// 		node[i]->children = NULL;
// 		node[i]->non_term_id = i;
// 	}

// 	node[1]->next = node[2];
// 	node[2]->next = node[3];
// 	node[4]->next = node[5];
// 	node[5]->next = node[6];
// 	node[2]->children = node[4];
// 	node[0]->children = node[1];

// 	printParseTree(node[0],"parseTree.txt");
// 	node[0] = del_child(node[0],2);
// 	printParseTree(node[0],"ParseTree2.txt");
// 	node[0] = del_child(node[0],1);
// 	printParseTree(node[0],"ParseTree3.txt");

// 	return 0;
// }