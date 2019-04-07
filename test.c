#include <stdio.h>

void case1(int i){
	printf("abs %d\n", i);
}

void case2(int i){
	printf("lujkasgerfjw %d\n", i);
}

static void (*cases[2])() = {case1, case2};

int main(){

	cases[0](2);
	return 0;
}