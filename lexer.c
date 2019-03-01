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

tokenInfo* newToken(char* str,tokenId tid)
{
	tokenInfo* new =  (tokenInfo*)malloc(sizeof(tokenInfo));
	
	new->tid = tid;
	new->name = (char *)malloc(sizeof(char)*(strlen(str)+1));
	strcpy(new->name,str);
	new->lineNo = currVariables.lineNo;

	return new;
}
tokenInfo* getNextToken(FILE *fp){
	int currPos = -1;
	currVariables.state = 0;
	char curr = 0;
	char buffer2(MAX_LENGTH);

	while(1){
		cur = getNextChar(fp);
		if(currVariables.flag == 1)
			return newToken("$",TK_DOLLAR); //$1 or $

		currPos++;
		buffer2[currPos] = cur;

		switch(currVariables.state)
		{
			case 0 :
				
		}
	}

}