#include<stdio.h>

struct recordDecl{
	char* type;
	char* name;
	struct recordDecl *next;
};

typedef struct *recordDecl rec_dec;



typedef struct {
    char *name;
    char *type;
    char *scope;
    int width;
    int offset;
    int lineNo;
    rec_dec *record;
    struct entry *next;
}entry;

struct scopetable{
	int size;
	int scope;
	struct 	scopetable	prevScope;
	struct entry *table;
};
typedef struct scopetable scopeTable;

struct {
	int size;
	scopeTable table;
}symbolTable;

