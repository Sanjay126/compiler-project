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

struct scopetable;
typedef struct entry{
    char *name;
    char *type;
    char *scope;
    int record_or_not;
    int width;
    int global;
    // int offset;
    int lineNo;
    RecordValue recVal;
    // rec_dec record;
    int inout;
    struct entry *next;
	int countVariables;
	struct scopetable *funcScopePtr;
}entry;

struct paramNode{
	char* paramName;
	struct paramNode* next;
};

typedef struct paramNode* ParamNode;

struct scopetable{
	int size;
	char* scope;
	ParamNode inputParams;
	ParamNode outputParams;
	struct scopetable *prevScope;
	entry** arr;
};
typedef struct scopetable *scopeTable;

typedef struct{
	int size;
	Records recs;
	scopeTable curr;
}symbolTable;


#endif