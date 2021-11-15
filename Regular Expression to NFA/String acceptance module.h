#ifndef STRING_ACCEPTANCE_MODULE_H
#define STRING_ACCEPTANCE_MODULE_H

#define MAXNODE 30
#define MAXSTACK 30


struct oneState {
	int a, b, e[2], output;
};

struct nfaTable {
	int numOfNode;
	struct oneState state[MAXNODE];
} nfaStack[MAXSTACK];


void acceptance_Test(struct nfaTable table);

#endif // !STRING_ACCEPTANCE_MODULE_H
