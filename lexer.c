#include"lexer.h"

currInfo currVariables;
currVariables.lineNo=1;
currVariables.state=0;
currVariables.offset=0;
currVariables.flag=0;
currVariables.bufferSize = -1;

char getNextChar(FILE *fp){
	if (currVariables.flag==1)
		return 26;

	if (currVariables.bufferSize==-1 || currVariables.offset==currVariables.bufferSize){
		currVariables.bufferSize = fread(buffer, (size_t)sizeof(char), (size_t)MAX_SIZE, fp);
		currVariables.offset = 1;

		if (currVariables.bufferSize==0){
			currVariables.flag=1;
			return 1;
		}
		else
			return buffer[0];
	}

	return buffer[(currVariables.offset)++];
	// if(currVariables.bufferSize==0) check later
}

tokenInfo* genearteNewToken(char* str,tokenId tid)
{
	tokenInfo* new =  (tokenInfo*)malloc(sizeof(tokenInfo));
	
	new->tid = tid;
	new->name = (char *)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(new->name,str);
	new->lineNo = currVariables.lineNo;

	return new;
}
tokenInfo* getNextToken(FILE *fp){
	int buffer2Pos = -1;
	currVariables.state = 0;
	char curr = 0;
	char buffer2(MAX_LENGTH);

	while(1){
		cur = getNextChar(fp);
		if(currVariables.flag == 1)
			return genearteNewToken("$",TK_DOLLAR); //$1 or $

		buffer2Pos++;
		buffer2[buffer2Pos] = cur;

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
						return genearteNewToken(&curr, TK_SQL);
					case ']':
						return genearteNewToken(&curr, TK_SQR);
					case ',':
						return genearteNewToken(&curr, TK_COMMA);
					case ';':
						return genearteNewToken(&curr, TK_SEM);
					case ':':
						return genearteNewToken(&curr, TK_COLON);
					case '.':
						return genearteNewToken(&curr, TK_DOT);
					case '(':
						return genearteNewToken(&curr, TK_OP);
					case ')':
						return genearteNewToken(&curr, TK_CP);
					case '+':
						return genearteNewToken(&curr, TK_PLUS);
					case '-':
						return genearteNewToken(&curr, TK_MINUS);
					case '*':
						return genearteNewToken(&curr, TK_MUL);
					case '/':
						return genearteNewToken(&curr, TK_DIV);
					case '~':
						return genearteNewToken(&curr, TK_NOT);
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
						return NULL;	//// TODO  error handling 


				}
				break;
			case 1:
				switch(curr){
					case '-':
						currVariables.state=2;
						break;
					case '=':
						return genearteNewToken("<=", TK_LE);
					default:
						currVariables.offset--;
						return genearteNewToken("<", TK_LT);
				}
				break;
			case 2:
				if(curr=='-'){
					currVariables.state=3;
				}
				else{
					return NULL;	// TODO error handling
				}
				break;
			case 3:
				if(curr=='-'){
					return genearteNewToken("<---",TK_ASSIGNOP);
				}
				else{
					return NULL;	// TODO error handling
				}
				break;
			case 4:
				if(curr=='='){
					return genearteNewToken(">=",TK_GE);
				}
				else{
					currVariables.offset--;
					return genearteNewToken(">",TK_GT);
				}
				break;
			case 5:
				if(curr>='a' && curr<='z'){
					currVariables.state=6;
					break;
				}
				else{
					return NULL;	// TODO error handling					
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
					return genearteNewToken(buffer2, TK_RECORDID);
				}
				break;
			case 7:
				if(curr=='='){
					currVariables.offset--;
					return genearteNewToken('!=', TK_NE);
				}
				else{
					return NULL;	// TODO error handling					
				}
				break;
			case 8:
				if(curr=='='){
					currVariables.offset--;
					return genearteNewToken('==', TK_EQ);
				}
				else{
					return NULL;	// TODO error handling					
				}
				break;

			case 9:
				if(curr=='&')
					currVariables.state=10;
				else{
					return NULL;	// TODO error handling					
				}
				break;
			case 10:
				if(curr=='&')
					return genearteNewToken("&&&", TK_AND);
				else{
					return NULL;	// TODO error handling					
				}
				break;

			case 11:
				if(curr=='@')
					currVariables.state=12;
				else{
					return NULL;	// TODO error handling					
				}
				break;
			case 12:
				if(curr=='@')
					return genearteNewToken("@@@", TK_OR);
				else{
					return NULL;	// TODO error handling					
				}
				break;
			case 13:
				if((curr>='a' && curr<='z') || (curr>='A' && curr<='Z'))
					currVariables.state=14;
				else{
					return NULL;	// TODO error handling					
				}
				break;
			case 14:
				if((curr>='a' && curr<='z') || (curr>='A' && curr<='Z'));
				else if(curr<='9' && curr>='0')
					currVariables.state=15;
				else{
					//TODO
					//discard state 16
					//implement lookup here only
					// currVariables.state=16;
				}
				break;
			case 15:
				if(curr<='9' && curr>='0');
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return genearteNewToken(buffer2, TK_FUNID);
				}
				break;
			case 16:
				// TODO LOOKUP
				break;
			case 17:
				if(curr<='9' && curr>='0');
				else if(curr=='.')
					currVariables.state=18;
				else{
					currVariables.offset--;
					buffer2[buffer2Pos]=0;
					return genearteNewToken(buffer2, TK_NUM);
				}
			case 18:

		}
	}

}