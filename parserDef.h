#ifndef PARSERDEF
#define PARSERDEF

typedef enum{
	assignmentStmt,
	global_or_not,
	booleanExpression,
	highPrecedenceOperators,
	singleOrRecId,
	constructedDatatype,
	dataType,
	primitiveDatatype,
	typeDefinitions,
	more_ids,
	idList,
	relationalOp,
	temp,
	output_par,
	stmt,
	declarations,
	input_par,
	logicalOp,
	newallvar,
	program,
	remaining_list,
	all,
	term,
	stmts,
	returnStmt,
	declaration,
	iterativeStmt,
	otherFunctions,
	factor,
	allnew,
	new_24,
	function,
	arithmeticExpression,
	var,
	fieldDefinitions,
	allVar,
	mainFunction,
	funCallStmt,
	parameter_list,
	fieldDefinition,
	typeDefinition,
	expPrime,
	outputParameters,
	termPrime,
	lowPrecedenceOperators,
	optionalReturn,
	elsePart,
	moreFields,
	conditionalStmt,
	otherStmts,
	ioStmt,
	inputParameters,
} nonTerminals;

#define no_of_t  55
#define no_of_nt 52
#define no_of_sym 107


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
	TokenInfo tk;
	struct parseTree* next;//for looping through children
	struct parseTree* children;
	struct parseTree* parent;
};
typedef struct parseTree* ParseTree;

struct snode{
	int id;
	struct snode * next;
	ParseTree pt_node;
};
typedef struct snode* SNode;

struct stack{
	int size;
	SNode head;		
};
typedef struct stack* Stack;

#endif