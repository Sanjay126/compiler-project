#ifndef LEXERDEF
#define LEXERDEF

# define MAX_SIZE 1024
# define MAX_LENGTH 250
# define ull unsigned long long

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

struct tokenInfo{
    tokenId tid;
    char * name;
    ull lineNo;
};

typedef struct tokenInfo* TokenInfo;

typedef struct{
    ull lineNo;
    int state;
    int offset;
    int flag;
    int bufferSize;
    int lexicalError;
}currentInfo;


char buffer[MAX_SIZE];
#endif
