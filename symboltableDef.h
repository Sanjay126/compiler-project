#include<stdio.h>
#include<stdbool.h>
#ifndef symbolTableDef
#define symbolTableDef

struct recordDecl{
	char* type;
	char* name;
	struct recordDecl *next;
};

typedef struct recordDecl* rec_dec;

struct record{
	char* name;
	int size;
	rec_dec head;
	struct record* next;
};
typedef struct record* Record;

struct records{
	int count;
	Record head;
};

typedef struct records* Records;

struct recordValue{
	char* name;
	int valueInt;
	float valueFloat;
	bool isInt;
	struct recordValue* next;
};
typedef struct recordValue* RecordValue;


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
	char* scope;
	struct scopetable *prevScope;
	entry** arr;
};
typedef struct scopetable *scopeTable;

typedef struct{
	int size;
	scopeTable curr;
}symbolTable;


#endif