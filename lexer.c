# include <stdio.h>
# include <stdlib.h>
# include <string.h>
#include <stdbool.h>
#include"lexerDef.h"
currentInfo currVariables = {1,0,0,0,-1,0};

void intialiseGlobalVariablesLexer(){
	currVariables.lineNo = 1;
	currVariables.state = 0;
	currVariables.offset = 0;
	currVariables.flag = 0;
	currVariables.bufferSize = -1;
	currVariables.lexicalError = 0;
}

void removeComments(char *testcaseFile, char *cleanFile){
	FILE *fp12 = fopen(testcaseFile,"r");
	FILE *nfp12 = fopen(cleanFile,"w");

	while(!feof(fp12)){
		char c = fgetc(fp12);
		if(c == '%')
			while(!feof(fp12) && c != '\n')
				c = fgetc(fp12);
		else if(!feof(fp12))
			fputc(c,nfp12);
	}
	fclose(fp12);
	fclose(nfp12);
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

TokenInfo generateNewToken(char* str,tokenId tid){
	TokenInfo new =  (TokenInfo)malloc(sizeof(struct tokenInfo));
	new->tid = tid;
	new->name = (char *)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(new->name,str);
	new->lineNo = currVariables.lineNo;
	return new;
}


TokenInfo cmpFunc(char* str, int iddd){
	int l = 24;
	char* strings[] = {"with","parameters","end","while","int","real","type","_main","global","parameter","list","input","output","endwhile","if","then","endif","read","write","return","call","record","endrecord","else"};

	int tokens[] = {TK_WITH,TK_PARAMETERS,TK_END,TK_WHILE,TK_INT,TK_REAL,TK_TYPE,TK_MAIN,TK_GLOBAL,TK_PARAMETER,TK_LIST,TK_INPUT,TK_OUTPUT,TK_ENDWHILE,TK_IF,TK_THEN,TK_ENDIF,TK_READ,TK_WRITE,TK_RETURN,TK_CALL,TK_RECORD,TK_ENDRECORD,TK_ELSE};
	for(int i=0; i<l; i++)
		if(strcmp(str, strings[i])==0)
			return generateNewToken(str, tokens[i]);
	return generateNewToken(str,iddd);
}


TokenInfo getNextToken(FILE *fp){
	int buffer2Pos = -1;
	currVariables.state = 0;
	char curr = 0;
	char buffer2[MAX_LENGTH];

	while(1){
		curr = getNextChar(fp);
		buffer2Pos++;
		buffer2[buffer2Pos] = curr;
		switch(currVariables.state){
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
						if(currVariables.flag == 1)
  							return generateNewToken("$",TK_DOLLAR); //$1 or $
						else{
							printf("Line %llu: Unknown Symbol %c\n", currVariables.lineNo,curr);
							return NULL; 
						}
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
					printf("Line %llu: Unknown Pattern <-\n", currVariables.lineNo);	// TODO error handling
					return generateNewToken("",TK_ASSIGNOP);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 3:
				if(curr=='-'){
					return generateNewToken("<---",TK_ASSIGNOP);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern <--\n", currVariables.lineNo);	// TODO error handling
					return generateNewToken("",TK_ASSIGNOP);	//error token with empty lexeme(expected token returned to help syntax analyzer)
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
					printf("Line %llu: Unknown Pattern #", currVariables.lineNo);	// TODO error handling
					return generateNewToken("",TK_RECORDID);	//error token with empty lexeme(expected token returned to help syntax analyzer)
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
					printf("Line %llu: Unknown Pattern !\n", currVariables.lineNo);	// TODO error handling
					return generateNewToken("",TK_NE);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 8:
				if(curr=='='){
					return generateNewToken("==", TK_EQ);
				}
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern =\n", currVariables.lineNo);	// TODO error handling
					return generateNewToken("",TK_EQ);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;

			case 9:
				if(curr=='&')
					currVariables.state=10;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern &", currVariables.lineNo);
					return generateNewToken("",TK_AND);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 10:
				if(curr=='&')
					return generateNewToken("&&&", TK_AND);
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern &&\n", currVariables.lineNo);
					return generateNewToken("",TK_AND);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;

			case 11:
				if(curr=='@')
					currVariables.state=12;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern @\n", currVariables.lineNo);
					return generateNewToken("",TK_OR);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 12:
				if(curr=='@')
					return generateNewToken("@@@", TK_OR);
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern @@\n", currVariables.lineNo);
					return generateNewToken("",TK_OR);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 13:
				if((curr>='a' && curr<='z') || (curr>='A' && curr<='Z'))
					currVariables.state=14;
				else{
					currVariables.lexicalError = 1;
					currVariables.offset--;
					printf("Line %llu: Unknown Pattern _\n", currVariables.lineNo);
					return generateNewToken("",TK_FUNID);	//error token with empty lexeme(expected token returned to help syntax analyzer)
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
					// printf("\n---%d---\n", buffer2Pos);
					if(buffer2Pos<=31)
						return cmpFunc(buffer2, TK_FUNID);
					else{
						printf("Line %llu: Function ID is longer than the prescribed length of 30 characters\n", currVariables.lineNo);
						return generateNewToken("",TK_FUNID);	//error token with empty lexeme(expected token returned to help syntax analyzer)
					}
					
				}
				break;
			case 15:
				if(curr<='9' && curr>='0');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					printf("\n---%d---\n", buffer2Pos);
					if(buffer2Pos<=31)
						return generateNewToken(buffer2, TK_FUNID);
					else{
						printf("Line %llu: Function ID is longer than the prescribed length of 30 characters\n", currVariables.lineNo);
						return generateNewToken("",TK_FUNID);	//error token with empty lexeme(expected token returned to help syntax analyzer)
					}
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
					buffer2[buffer2Pos]=0;
					printf("Line %llu: Unknown Pattern %c\n", currVariables.lineNo,curr);
					return generateNewToken("",TK_RNUM);	//error token with empty lexeme(expected token returned to help syntax analyzer)
				}
				break;
			case 20:
				if(curr=='\n'){
					memset(buffer2, 0, MAX_LENGTH*sizeof(char));
					currVariables.state=0;
					currVariables.lineNo++;
					buffer2Pos=-1;
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
					return cmpFunc(buffer2, TK_FIELDID);
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
				if(curr>='2' && curr<='7');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					if(buffer2Pos<=21)
						return generateNewToken(buffer2, TK_ID);					
					else{
						printf("Line %llu: Identifier is longer than the prescribed length of 20 characters\n", currVariables.lineNo);
						return generateNewToken("",TK_ID);	//error token with empty lexeme(expected token returned to help syntax analyzer)
					}
				}
				break;

		}
	}
}