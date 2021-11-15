#include "String acceptance module.h"
#include <iostream>

using namespace std;


void acceptance_Test(struct nfaTable NFAtable) {
	string test_String;
	cin >> test_String;

	printf(" |  ");
	for (int i = 0; i < NFAtable.numOfNode; i++)
	{
		printf("%d  |", i);
	}
	putchar('\n');

	


}