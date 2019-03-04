# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <stdbool.h>
#include"lexerDef.h"
currentInfo currVariables = {1,0,0,0,-1,0};

void removeComments(char *testcaseFile, char *cleanFile){
	FILE *fp = fopen(testcaseFile,"r");
	FILE *nfp = fopen(cleanFile,"w");

	while(!feof(fp))
	{
		char c = fgetc(fp);
		if(c == '%')
		{
			while(!feof(fp) && c != '\n')
				c = fgetc(fp);
		}
		else
		{
			if(!feof(fp))
				fputc(c,nfp);
		}
	}
	fclose(fp);
	fclose(nfp);
}


char getNextChar(FILE *fp){
	if (currVariables.flag==1)
		return 26;

	if (currVariables.bufferSize==-1 || currVariables.offset==currVariables.bufferSize){
		currVariables.bufferSize = fread(buffer, (size_t)sizeof(char), (size_t)MAX_SIZE, fp);
		currVariables.offset = 1;

		if (currVariables.bufferSize==0){
			currVariables.flag=1;
			return 26;
		}
		else
			return buffer[0];
	}

	return buffer[(currVariables.offset)++];
	// if(currVariables.bufferSize==0) check later
}

TokenInfo generateNewToken(char* str,tokenId tid)
{
	TokenInfo new =  (TokenInfo)malloc(sizeof(struct tokenInfo));
	
	new->tid = tid;
	new->name = (char *)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(new->name,str);
	new->lineNo = currVariables.lineNo;

	return new;
}


TokenInfo cmpFunc(char* str){
	if(strcmp(str, "with")==0)
		return generateNewToken(str, TK_WITH);
	else if(strcmp(str, "parameters")==0)
		return generateNewToken(str, TK_PARAMETERS);
	else if(strcmp(str, "end")==0)
		return generateNewToken(str, TK_END);
	else if(strcmp(str, "while")==0)
		return generateNewToken(str, TK_WHILE);
	else if(strcmp(str, "int")==0)
		return generateNewToken(str, TK_INT);
	else if(strcmp(str, "real")==0)
		return generateNewToken(str, TK_REAL);
	else if(strcmp(str, "type")==0)
		return generateNewToken(str, TK_TYPE);
	else if(strcmp(str, "_main")==0)
		return generateNewToken(str, TK_MAIN);
	else if(strcmp(str, "global")==0)
		return generateNewToken(str, TK_GLOBAL);
	else if(strcmp(str, "parameter")==0)
		return generateNewToken(str, TK_PARAMETER);
	else if(strcmp(str, "list")==0)
		return generateNewToken(str, TK_LIST);
	else if(strcmp(str, "input")==0)
		return generateNewToken(str, TK_INPUT);
	else if(strcmp(str, "output")==0)
		return generateNewToken(str, TK_OUTPUT);
	else if(strcmp(str, "endwhile")==0)
		return generateNewToken(str, TK_ENDWHILE);
	else if(strcmp(str, "if")==0)
		return generateNewToken(str, TK_IF);
	else if(strcmp(str, "then")==0)
		return generateNewToken(str, TK_THEN);
	else if(strcmp(str, "endif")==0)
		return generateNewToken(str, TK_ENDIF);
	else if(strcmp(str, "read")==0)
		return generateNewToken(str, TK_READ);
	else if(strcmp(str, "write")==0)
		return generateNewToken(str, TK_WRITE);
	else if(strcmp(str, "return")==0)
		return generateNewToken(str, TK_RETURN);
	else if(strcmp(str, "call")==0)
		return generateNewToken(str, TK_CALL);
	else if(strcmp(str, "record")==0)
		return generateNewToken(str, TK_RECORD);
	else if(strcmp(str, "endrecord")==0)
		return generateNewToken(str, TK_ENDRECORD);
	else if(strcmp(str, "else")==0)
		return generateNewToken(str, TK_ELSE);
	return generateNewToken(str,TK_FIELDID);
}


TokenInfo getNextToken(FILE *fp){
	int buffer2Pos = -1;
	currVariables.state = 0;
	char curr = 0;
	char buffer2[MAX_LENGTH];

	while(1){
		curr = getNextChar(fp);
		if(currVariables.flag == 1)
  
  			return generateNewToken("$",TK_DOLLAR); //$1 or $

		buffer2Pos++;
		buffer2[buffer2Pos] = curr;

		switch(currVariables.state)
		{
			case 0 :
				switch(curr){
					case '\n': 
						currVariables.lineNo++;
					case ' ':
					case '\t':
					case '\r':
						buffer2Pos=-1;
						break;
					case '<':
						currVariables.state=1;
						break;
					case '>':
						currVariables.state=4;
						break;
					case '#':
						currVariables.state=5;
						break;
					case '!':
						currVariables.state=7;
						break;
					case '=':
						currVariables.state=8;
						break;
					case '&':
						currVariables.state=9;
						break;
					case '@':
						currVariables.state=11;
						break;
					case '_':
						currVariables.state=13;
						break;
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						currVariables.state=17;
						break;
					case '%':
						currVariables.state=20;
						break;
					case '[':
						return generateNewToken(&curr, TK_SQL);
					case ']':
						return generateNewToken(&curr, TK_SQR);
					case ',':
						return generateNewToken(&curr, TK_COMMA);
					case ';':
						return generateNewToken(&curr, TK_SEM);
					case ':':
						return generateNewToken(&curr, TK_COLON);
					case '.':
						return generateNewToken(&curr, TK_DOT);
					case '(':
						return generateNewToken(&curr, TK_OP);
					case ')':
						return generateNewToken(&curr, TK_CL);
					case '+':
						return generateNewToken(&curr, TK_PLUS);
					case '-':
						return generateNewToken(&curr, TK_MINUS);
					case '*':
						return generateNewToken(&curr, TK_MUL);
					case '/':
						return generateNewToken(&curr, TK_DIV);
					case '~':
						return generateNewToken(&curr, TK_NOT);
					case 'b':
					case 'c':
					case 'd':
						currVariables.state=21;
						break;
					case 'a':
					case 'e':
					case 'f':
					case 'g':
					case 'h':
					case 'i':
					case 'j':
					case 'k':
					case 'l':
					case 'm':
					case 'n':
					case 'o':
					case 'p':
					case 'q':
					case 'r':
					case 's':
					case 't':
					case 'u':
					case 'v':
					case 'w':
					case 'x':
					case 'y':
					case 'z':
						currVariables.state=22;
						break;
					default :
						printf("ERROR : Undefined Symbol '%d' at line number %llu", curr,currVariables.lineNo);
						return NULL;	//// TODO  error handling 
				}
				break;
			case 1:
				switch(curr){
					case '-':
						currVariables.state=2;
						break;
					case '=':
						return generateNewToken("<=", TK_LE);
					default:
						currVariables.offset--;
						return generateNewToken("<", TK_LT);
				}
				break;
			case 2:
				if(curr=='-'){
					currVariables.state=3;
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '-'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 3:
				if(curr=='-'){
					return generateNewToken("<---",TK_ASSIGNOP);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '-'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 4:
				if(curr=='='){
					return generateNewToken(">=",TK_GE);
				}
				else{
					currVariables.offset--;
					return generateNewToken(">",TK_GT);
				}
				break;
			case 5:
				if(curr>='a' && curr<='z'){
					currVariables.state=6;
					break;
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : [a-z]\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 6:
				if(curr>='a' && curr<='z'){
					currVariables.state=6;
					break;
				}
				else{
					buffer2[buffer2Pos] = '\0';
					currVariables.offset--;
					return generateNewToken(buffer2, TK_RECORDID);
				}
				break;
			case 7:
				if(curr=='='){
					return generateNewToken("!=", TK_NE);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '='\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 8:
				if(curr=='='){
					return generateNewToken("==", TK_EQ);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '='\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;

			case 9:
				if(curr=='&')
					currVariables.state=10;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '&'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 10:
				if(curr=='&')
					return generateNewToken("&&&", TK_AND);
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '&'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;

			case 11:
				if(curr=='@')
					currVariables.state=12;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '@'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 12:
				if(curr=='@')
					return generateNewToken("@@@", TK_OR);
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '@'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 13:
				if((curr>='a' && curr<='z') || (curr>='A' && curr<='Z'))
					currVariables.state=14;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '[a-z|A-Z]'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 14:
				if((curr>='a' && curr<='z') || (curr>='A' && curr<='Z'));
				else if(curr<='9' && curr>='0')
					currVariables.state=15;
				else{
					//discard state 16
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return cmpFunc(buffer2);
				}
				break;
			case 15:
				if(curr<='9' && curr>='0');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return generateNewToken(buffer2, TK_FUNID);
				}
				break;
			// case 16:
			// 	// TODO LOOKUP
			// 	break;
			case 17:
				if(curr<='9' && curr>='0');
				else if(curr=='.')
					currVariables.state=18;
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return generateNewToken(buffer2, TK_NUM);
				}
				break;
			case 18:
				if(curr<='9' && curr>='0')
					currVariables.state=19;
				else{
					currVariables.offset-=2;
					buffer2[buffer2Pos-1]=0;
					return generateNewToken(buffer2, TK_NUM);
				}
				break;
			case 19:
				if(curr<='9' && curr>='0'){
					// buffer2Pos++;
					// buffer2Pos = MAX_LENGTH > buffer2Pos+1
					buffer2[buffer2Pos+1]=0;
					return generateNewToken(buffer2, TK_RNUM);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("ERROR : Unexpected Symbol '%c' at line number %llu.EXPECTED : '[0-9]'\n", curr,currVariables.lineNo);	// TODO error handling
					return NULL;
				}
				break;
			case 20:
				if(curr=='\n'){
					memset(buffer2, 0, MAX_LENGTH*sizeof(char));
					currVariables.state=0;
					currVariables.lineNo++;
				}
				break;
			case 21:
				if(curr>='2' && curr<='7')
					currVariables.state=23;
				else if(curr>='a' && curr<='z')
					currVariables.state=22;
				else{
					buffer2[buffer2Pos]=0;
					return generateNewToken(buffer2, TK_FIELDID);
				}
				break;
			case 22:
				if(curr>='a' && curr<='z');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return cmpFunc(buffer2);
				}
				break;
			case 23:
				if(curr=='b' || curr=='c' || curr=='d');
				else if(curr>='2' && curr<='7')
					currVariables.state=24;
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return generateNewToken(buffer2, TK_ID);
				}
				break;
			case 24:
				if(curr=='b' || curr=='c' || curr=='d');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return generateNewToken(buffer2, TK_ID);					
				}
				break;
		}
	}

}