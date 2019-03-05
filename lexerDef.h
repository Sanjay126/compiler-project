/*
Group Number - 12
Sanjay Malhotra 2016A7PS0126P
Nilay Arora 2016A7PS0117P
Tushar Goel 2016A7PS0023P
Adit Shastri 2016A7PS0121P
*/
#ifndef LEXERDEF
#define LEXERDEF

# define MAX_SIZE 1024
# define MAX_LENGTH 250
# define ull unsigned long long

//enum for tokens
typedef enum {
    TK_WITH,
    TK_THEN,
    TK_TYPE,
    TK_EQ,
    TK_DOLLAR,
    TK_COMMA,
    TK_RNUM,
    TK_CL,
    TK_AND,
    TK_PARAMETER,
    TK_DIV,
    TK_ID,
    TK_GLOBAL,
    TK_GE,
    TK_LIST,
    TK_IF,
    TK_RECORD,
    TK_NUM,
    TK_NE,
    TK_LE,
    TK_DOT,
    TK_INPUT,
    TK_MAIN,
    TK_CALL,
    TK_PLUS,
    TK_ELSE,
    TK_COMMENT,
    TK_ASSIGNOP,
    TK_WRITE,
    TK_COLON,
    TK_WHILE,
    TK_REAL,
    TK_READ,
    TK_NOT,
    TK_SQL,
    TK_OUTPUT,
    TK_SQR,
    TK_ENDIF,
    TK_MINUS,
    TK_SEM,
    TK_FIELDID,
    TK_PARAMETERS,
    TK_OP,
    TK_ENDWHILE,
    TK_MUL,
    TK_FUNID,
    TK_INT,
    TK_RETURN,
    TK_GT,
    TK_END,
    TK_LT,
    TK_OR,
    TK_ENDRECORD,
    eps,
    TK_RECORDID,
} tokenId;

//Token struct
struct tokenInfo{
    tokenId tid; //enum value associated with token
    char * name; //lexeme value
    ull lineNo; //lineNo token is present at
};

typedef struct tokenInfo* TokenInfo;

//buffer for reading a chunk at a time from the file
char buffer[MAX_SIZE];

//struct containing variables of current state of program
typedef struct{
    ull lineNo; //current lineno 
    int state; //current state of DFA
    int offset; //offset in buffer where character is being read from
    int flag; //flag whether end of file reached or not
    int bufferSize; //size of the current buffer read from file
    int lexicalError; //flag whether any lexical error present or not
}currentInfo;

#endif
