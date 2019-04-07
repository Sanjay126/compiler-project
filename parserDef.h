/*
Group Number - 12
Sanjay Malhotra 2016A7PS0126P
Nilay Arora 2016A7PS0117P
Tushar Goel 2016A7PS0023P
Adit Shastri 2016A7PS0121P
*/
#ifndef PARSERDEF
#define PARSERDEF

//Enum for non-terminals
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

#define no_of_t  55 //Number of terminals
#define no_of_nt 52 //Number of non-terminals
#define no_of_sym 107 //Number of symbols -> no of tokens plus no of nonTerminals

//2D matrix for parse tree
typedef int** parseTable;

//For the hash table
typedef struct hashNode{
	int index;
	char s[MAX_LENGTH];
	struct hashNode* next;
}hashNode;

typedef hashNode** hashTable;
//end

//linked list for Right hand side of grammer rules tokens
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

//Array of ruleRhs pointer for representing grammer. Each index in array represent rules starting from the non-terminal denoted by that ID.
struct grammar{
	RuleRHS *rules; 
};
typedef struct grammar Grammar;

//Parsetree Node
struct parseTree{
	int non_term_id; //Id of the Non-terminal (non leaf node)
	TokenInfo tk; //Pointer to token in leaf nodes
	struct parseTree* next;//for looping through children
	struct parseTree* children; //pointing to head of linked list of children
	struct parseTree* parent; //pointer to parent of node
	int ruleNo;
};
typedef struct parseTree* ParseTree; 

//Stack Node 
struct snode{
	int id;
	struct snode * next; 
	ParseTree pt_node; //Pointer to Parse Tree Node
};
typedef struct snode* SNode;

//Stack struct
struct stack{
	int size;
	SNode head;		
};
typedef struct stack* Stack;

#endif