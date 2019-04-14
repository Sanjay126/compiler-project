#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexerDef.h"
#include "lexer.h"
#include "parserDef.h"	
#include "parser.h"	
#include "ast.h"

ParseTree ast;

ParseTree del_child(ParseTree node,int i)
{
	noOfASTnodes--;
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

		while(count < i && ptr && ptr->next)
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
			if(ptr->children == NULL)
			{
				prev->next = ptr->next;
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
			}
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
	if(p->children->next->children == NULL)
		p = del_child(p,1);
	if(p->children->children == NULL)
		p = del_child(p,0);
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

	p = del_child(p, 0);
	p = del_child(p, 2);
	p = del_child(p, 2);
	p = del_child(p, 1);
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
	p = del_child(p,0);
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

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case27(ParseTree p){
	// <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID  <global_or_not> TK_SEM

	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 3);
	p = del_child(p ,2);
	p = del_child(p, 0);
	return p;
}

ParseTree case28(ParseTree p){
	 //<global_or_not> ===> TK_COLON  TK_GLOBAL

	p = del_child(p, 0);
	return p;
}

ParseTree case29(ParseTree p){
	 //<global_or_not> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case30(ParseTree p){
	 // <otherStmts> ===> <stmt> <otherStmts1>

	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case31(ParseTree p){
	// <otherStmts> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case32(ParseTree p){
	// <stmt> ===> <assignmentStmt>

	return p;
}

ParseTree case33(ParseTree p){
	// <stmt> ===> <iterativeStmt>

	// p = del_child(p, 0);
	return p;
}

ParseTree case34(ParseTree p){
	// <stmt> ===> <conditionalStmt>
	
	return p;
}

ParseTree case35(ParseTree p){
	 //<stmt> ===> <ioStmt>
	
	return p;
}

ParseTree case36(ParseTree p){
	// <stmt> ===> <funCallStmt>
	
	return p;
}

ParseTree case37(ParseTree p){
	// <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP  <arithmeticExpression> TK_SEM

	p = del_child(p, 1);
	p = del_child(p, 2);
	// p = del_child(p, 1);
	return p;
}

ParseTree case38(ParseTree p){
	//<singleOrRecId> ===>TK_ID  <new_24>

	p = del_child(p,1);
	return p;
}

ParseTree case39(ParseTree p){
	//<new_24> ===> TK_DOT  TK_FIELDID

	p = del_child(p, 0);
	return p;
}

ParseTree case40(ParseTree p){
	//<new_24> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case41(ParseTree p){
	//<funCallStmt> ===> <outputParameters> TK_CALL  TK_FUNID  TK_WITH TK_PARAMETERS <inputParameters>  TK_SEM

	p = del_child(p, 1);
	p = del_child(p, 2);
	p = del_child(p, 2);
	p = del_child(p, 3);
	return p;
}

ParseTree case42(ParseTree p){
	//<outputParameters> ==> TK_SQL <idList> TK_SQR TK_ASSIGNOP

	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case43(ParseTree p){
	//<outputParameters> ==> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case44(ParseTree p){
	//<inputParameters> ===> TK_SQL <idList> TK_SQR

	p = del_child(p, 2);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case45(ParseTree p){
	//<iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE

	p = del_child(p, 0);
	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 3);
	p = del_child(p, 2);
	p = del_child(p, 1);
	return p;
}

ParseTree case46(ParseTree p){
	//<conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>

	p = del_child(p, 0);
	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 1);
	p = del_child(p, 2);
	p = del_child(p, 1);
	return p;
}

ParseTree case47(ParseTree p){
	//<elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF

	p = del_child(p, 0);
	p = del_child(p, 2);
	p = del_child(p, 1);
	p = del_child(p , 0);
	return p;
}

ParseTree case48(ParseTree p){
	//<elsePart> ===> TK_ENDIF

	free(p->children);
	p->children = NULL;
	free(p);
	return NULL;
}

ParseTree case49(ParseTree p){
	//<ioStmt> ===> TK_READ TK_OP <singleOrRecId> TK_CL TK_SEM

	p = del_child(p, 1);
	p = del_child(p, 2);
	p = del_child(p, 2);
	return p;
}

ParseTree case50(ParseTree p){
	//<ioStmt> ===>  TK_WRITE TK_OP <allVar> TK_CL TK_SEM

	p = del_child(p, 1);
	p = del_child(p, 2);
	p = del_child(p, 2);
	// p = del_child(p, 1);
	return p;
}

ParseTree case51(ParseTree p){
	//<allVar> ===>  TK_ID <newallvar>

	p = del_child(p, 1);
	return p;
}

ParseTree case52(ParseTree p){
	//<allVar> ===>  TK_RNUM

	return p;
}

ParseTree case53(ParseTree p){
	//<allVar> ===>  TK_NUM

	return p;
}

ParseTree case54(ParseTree p){
	//<newallvar> ===>  TK_DOT TK_FIELDID
	
	p = del_child(p,0);
	return p;
}

ParseTree case55(ParseTree p){
	//<newallvar> ===>  eps

	free(p->children);
	p->children = NULL;
	return p;
}

//	TODO
ParseTree case56(ParseTree p){
	//<arithmeticExpression> ===>  <term> <expPrime>

	p = del_child(p, 1);
	if(p->children->children->next == NULL)
		p = del_child(p, 0);
	return p;
}

ParseTree case57(ParseTree p){
	//<expPrime> ===>  <lowPrecedenceOperators> <term> <expPrime>

	p = del_child(p, 2);
	if(p->children->next->children->next ==  NULL)
		p = del_child(p,1);
	p = del_child(p, 0);
	// p = del_child(p, 1);
	return p;
}

ParseTree case58(ParseTree p){
	//<expPrime> ===>  eps
	
	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case59(ParseTree p){
	//<term> ===>  <factor> <termPrime>

	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case60(ParseTree p){
	//<termPrime> ===>  <highPrecedenceOperators> <factor> <termPrime>

	p = del_child(p, 2);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case61(ParseTree p){
	//<termPrime> ===>  eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case62(ParseTree p){
	//<factor> ===> <all>

	p = del_child(p, 0);
	return p;
}
// ENDTODO


ParseTree case63(ParseTree p){
	//<factor> ===> TK_OP <arithmeticExpression> TK_CL

	p = del_child(p, 0);
	p = del_child(p, 1);
	return p;
}

ParseTree case64(ParseTree p){
	//<lowPrecedenceOperators> ===>  TK_MINUS

	return p;
}

ParseTree case65(ParseTree p){
	//<lowPrecedenceOperators> ===> TK_PLUS

	return p;
}

ParseTree case66(ParseTree p){
	//<highPrecedenceOperators> ===> TK_MUL

	return p;
}

ParseTree case67(ParseTree p){
	//<highPrecedenceOperators> ===> TK_DIV

	return p;
}

ParseTree case68(ParseTree p){
	//<all> ===>  TK_ID <allnew>

	p = del_child(p, 1);
	return p;
}

ParseTree case69(ParseTree p){
	//<all> ===>  TK_NUM

	return p;
}

ParseTree case70(ParseTree p){
	//<all> ===>  TK_RNUM

	return p;
}

ParseTree case71(ParseTree p){
	//<allnew> ===>  <temp>

	p = del_child(p, 0);
	return p;
}

ParseTree case72(ParseTree p){
	//<allnew> ===>  eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case73(ParseTree p){
	//<temp> ===>  eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case74(ParseTree p){
	//<temp> ===> TK_DOT TK_FIELDID

	p = del_child(p, 0);
	return p;
}

ParseTree case75(ParseTree p){
	//<booleanExpression> ===> TK_OP  <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL

	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 2);
	p = del_child(p, 3);
	p = del_child(p, 2);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case76(ParseTree p){
	//<booleanExpression> ===> <Var> <relationalOp> <Var>

	p = del_child(p, 2);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case77(ParseTree p){
	//<booleanExpression> ===> TK_NOT TK_OP <booleanExpression1>TK_CL

	p = del_child(p, 3);
	p = del_child(p, 2);
	p = del_child(p, 1);

	return p;
}

ParseTree case78(ParseTree p){
	//<var> ===> <TK_ID>

	return p;
}

ParseTree case79(ParseTree p){
	//<var> ===> TK_NUM

	return p;
}

ParseTree case80(ParseTree p){
	//<var> ===> TK_RNUM

	return p;
}

ParseTree case81(ParseTree p){
	//<logicalOp> ===> TK_AND

	return p;
}

ParseTree case82(ParseTree p){
	//<logicalOp> ===> TK_OR

	return p;
}

ParseTree case83(ParseTree p){
	//<relationalOp> ===> TK_LT

	return p;
}

ParseTree case84(ParseTree p){
	//<relationalOp> ===> TK_LE

	return p;
}

ParseTree case85(ParseTree p){
	//<relationalOp> ===> TK_EQ

	return p;
}

ParseTree case86(ParseTree p){
	//<relationalOp> ===> TK_GT

	return p;
}

ParseTree case87(ParseTree p){
	//<relationalOp> ===> TK_GE

	return p;
}

ParseTree case88(ParseTree p){
	//<relationalOp> ===> TK_NE

	return p;
}

ParseTree case89(ParseTree p){
	//<returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM

	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case90(ParseTree p){
	//<optionalReturn> ===> TK_SQL <idList> TK_SQR

	p = del_child(p, 0);
	p = del_child(p, 1);
	p = del_child(p, 0);
	return p;
}

ParseTree case91(ParseTree p){
	//<optionalReturn> ===> eps

	free(p->children);
	p->children = NULL;
	return p;
}

ParseTree case92(ParseTree p){
	//<idList> ===> TK_ID <more_ids>

	p = del_child(p, 1);
	return p;
}

ParseTree case93(ParseTree p){
	//<more_ids> ===> TK_COMMA <idList>

	p = del_child(p, 0);
	p = del_child(p, 0);
	return p;
}

ParseTree case94(ParseTree p){
	//<more_ids> ===> eps
	
	free(p->children);
	p->children = NULL;
	return p;
}


static ParseTree (*functions[94])() = {case1, case2, case3, case4, case5, case6, case7, case8, case9, case10, case11, case12, case13, case14, case15, case16, case17, case18, case19, case20, case21, case22, case23, case24, case25, case26, case27, case28, case29, case30, case31, case32, case33, case34, case35, case36, case37, case38, case39, case40, case41, case42, case43, case44, case45, case46, case47, case48, case49, case50, case51, case52, case53, case54, case55, case56, case57, case58, case59, case60, case61, case62, case63, case64, case65, case66, case67, case68, case69, case70, case71, case72, case73, case74, case75, case76, case77, case78, case79, case80, case81, case82, case83, case84, case85, case86, case87, case88, case89, case90, case91, case92, case93, case94};


ParseTree createAST(ParseTree PT)
{
	// noOfASTnodes=noOfPTNode;
	if(PT->children == NULL)
		return PT;
	ParseTree ptr  = PT->children;
	while(ptr)
	{
		ptr = createAST(ptr);
		ptr = ptr->next;
	}

	if(PT->ruleNo)
		PT = functions[PT->ruleNo-1](PT);

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