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

#define no_of_t  55
#define no_of_nt 47
#define no_of_sym 102


typedef int** parseTable;

// for the hash table
typedef struct hashNode{
	int index;
	char s[MAX_LENGTH];
	struct hashNode* next;
}hashNode;

typedef hashNode** hashTable;
//end

//linked list for rhs token
struct node{
	int id;
	struct node * next;
};
typedef struct node* Node;

//linked list for rules starting with a given non terminal denoted by index
struct ruleRHS{
	Node head;
	int size;
	struct ruleRHS* next;
	int ruleNo;
};
typedef struct ruleRHS* RuleRHS;

struct grammar{
	RuleRHS *rules;
	// int size;
};
// char* ntArray[no_of_nt];
// char* symbolArray[no_of_sym];
// char* tokenArray[no_of_t];
typedef struct grammar Grammar;

//parsetree Node
struct parseTree{
	int non_term_id;
	tokenInfo* tk;
	struct parseTree* next;//for looping through children
	struct parseTree* children;
	struct parseTree* parent;
};
struct stack{
	int size;
	Node head;		
};
typedef struct stack* Stack;

typedef struct parsetree* ParseTree;
#endif