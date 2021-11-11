#include <stdio.h>

#define MAXNODE 100
#define MAXSTACK 100

struct oneState {
	int a, b, e[2], output;
};

struct nfaTable {
	int numOfNode;
	struct oneState state[MAXNODE];
} nfaStack[MAXSTACK];

int top = -1;

void push(struct nfaTable m) 
{
	top++;
	nfaStack[top] = m;
}

struct nfaTable pop() {
	return nfaStack[top--];
}

void read_Regular_Expression() {
	char* input_string;
	scanf("%s", input_string);
}

int main(void) {
	

	return 0;
}