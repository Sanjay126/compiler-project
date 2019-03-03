#include "parserDef.h"

#ifndef PARSER
#define PARSER

typedef enum{
	factor,
	output_par,
	relationalOp,
	fieldDefinitions,
	arithmeticExpression,
	remaining_list,
	otherFunctions,
	typeDefinition,
	moreFields,
	declaration,
	inputParameters,
	dataType,
	funCallStmt,
	singleOrRecId,
	parameter_list,
	conditionalStmt,
	optionalReturn,
	global_or_not,
	outputParameters,
	op1,
	ioStmt,
	stmt,
	primitiveDatatype,
	function,
	otherStmts,
	ex2,
	declarations,
	iterativeStmt,
	constructedDatatype,
	assignmentStmt,
	returnStmt,
	B1,
	term,
	more_ids,
	typeDefinitions,
	allVar,
	C1,
	term2,
	program,
	mainFunction,
	op2,
	idList,
	stmts,
	fieldDefinition,
	input_par,
	booleanExpression,
	logicalOp,
} nonTerminals;

int num_of_t = 55, num_of_nt=47;

char* ntArray[] = {"factor","output_par","relationalOp","fieldDefinitions","arithmeticExpression","remaining_list","otherFunctions","typeDefinition","moreFields","declaration","inputParameters","dataType","funCallStmt","singleOrRecId","parameter_list","conditionalStmt","optionalReturn","global_or_not","outputParameters","op1","ioStmt","stmt","primitiveDatatype","function","otherStmts","ex2","declarations","iterativeStmt","constructedDatatype","assignmentStmt","returnStmt","B1","term","more_ids","typeDefinitions","allVar","C1","term2","program","mainFunction","op2","idList","stmts","fieldDefinition","input_par","booleanExpression","logicalOp"};

char* symbolArray[] = {"factor","output_par","relationalOp","fieldDefinitions","arithmeticExpression","remaining_list","otherFunctions","typeDefinition","moreFields","declaration","inputParameters","dataType","funCallStmt","singleOrRecId","parameter_list","conditionalStmt","optionalReturn","global_or_not","outputParameters","op1","ioStmt","stmt","primitiveDatatype","function","otherStmts","ex2","declarations","iterativeStmt","constructedDatatype","assignmentStmt","returnStmt","B1","term","more_ids","typeDefinitions","allVar","C1","term2","program","mainFunction","op2","idList","stmts","fieldDefinition","input_par","booleanExpression","logicalOp","TK_ASSIGNOP",    "TK_COMMENT",    "TK_FIELDID",    "TK_ID",    "TK_NUM",    "TK_RNUM",    "TK_FUNID",    "TK_RECORDID",    "TK_WITH",    "TK_PARAMETERS",    "TK_END",    "TK_WHILE",    "TK_INT",    "TK_REAL",    "TK_TYPE",    "TK_MAIN",    "TK_GLOBAL",    "TK_PARAMETER",    "TK_LIST",    "TK_SQL",    "TK_SQR",    "TK_INPUT",    "TK_OUTPUT",    "TK_SEM",    "TK_COLON",    "TK_DOT",    "TK_COMMA",    "TK_ENDWHILE",    "TK_OP",    "TK_CL",    "TK_IF",    "TK_THEN",    "TK_ENDIF",    "TK_READ",    "TK_WRITE",    "TK_RETURN",    "TK_PLUS",    "TK_MINUS",    "TK_MUL",    "TK_DIV",    "TK_CALL",    "TK_RECORD",    "TK_ENDRECORD",    "TK_ELSE",    "TK_AND",    "TK_OR",    "TK_NOT",    "TK_LT",    "TK_LE",    "TK_EQ",    "TK_GT",    "TK_GE",    "TK_NE",    "TK_DOLLAR",    "eps"};


typedef int** table;

struct node{
	int id;
	struct node * next;
};
typedef struct *node Node;

typedef struct {
	int index;
	char s[MAX_LENGTH];
	struct hashNode* next;
}hashNode;

typedef hashNode** hashTable;

struct ruleRHS{
	Node* head;
	int size;
	struct ruleRHS* next;
};
typedef struct ruleRHS RuleRHS;

struct grammar{
	RuleRHS rules[num_of_nt];
	// int size;
};

typedef struct grammar Grammar;

void createParseTable(char* F, table T);
int hash(char *v, int M);

#endif