#include<stdio.h>

#ifndef symbolTableDef
#define symbolTableDef

struct recordDecl{
	char* type;
	char* name;
	struct recordDecl *next;
};

typedef struct recordDecl* rec_dec;

typedef struct entry{
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
	struct scopetable *prevScope;
	entry** arr;
};
typedef struct scopetable *scopeTable;

typedef struct{
	int size;
	scopeTable curr;
}symbolTable;


#endif