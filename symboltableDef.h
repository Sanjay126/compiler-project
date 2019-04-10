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
    // struct entry *prev;
}entry;

struct scopetable{
	int size;
	int scope;
	struct scopeTable *prevScope;
	// struct entry* scopeEntry;
	struct entry *head;
};
typedef struct scopetable *scopeTable;

struct {
	int size;
	scopeTable curr;
}symbolTable;

