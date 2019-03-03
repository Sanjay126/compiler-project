#ifndef PARSERDEF
#define PARSERDEF

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

int no_of_t = 55, no_of_nt=47;

char* ntArray[] = {"factor","output_par","relationalOp","fieldDefinitions","arithmeticExpression","remaining_list","otherFunctions","typeDefinition","moreFields","declaration","inputParameters","dataType","funCallStmt","singleOrRecId","parameter_list","conditionalStmt","optionalReturn","global_or_not","outputParameters","op1","ioStmt","stmt","primitiveDatatype","function","otherStmts","ex2","declarations","iterativeStmt","constructedDatatype","assignmentStmt","returnStmt","B1","term","more_ids","typeDefinitions","allVar","C1","term2","program","mainFunction","op2","idList","stmts","fieldDefinition","input_par","booleanExpression","logicalOp"};

typedef int** table;

// typedef struct{
// 	int arr[no_of_nt][no_of_t];

// } Table;

struct node{
	int id;
	struct node * next;
};
typedef struct node Node;

struct ruleRHS{
	Node* head;
	int size;
	struct ruleRHS* next;
};
typedef struct *ruleRHS RuleRHS;

struct grammar{
	RuleRHS rules[no_of_nt];
	// int size;
};

typedef struct grammar Grammar;

#endif