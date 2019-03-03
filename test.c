#include <stdio.h>

typedef enum{
	B,
	C,
	D
}A;

int main(){
	char* b = "B";
	A myenum = b;
	printf("%d",myenum);
}