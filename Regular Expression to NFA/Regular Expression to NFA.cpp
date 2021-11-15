#include <iostream>
#include <vector>

using namespace std;

#define MAXNODE 30
#define MAXSTACK 30


struct oneState {
	int a, b, e[2], output;
};

struct nfaTable {
	int numOfNode;
	struct oneState state[MAXNODE];
} nfaStack[MAXSTACK];


string read_Regular_Expression();
string insert_Concatenation_Operator_and_Convert_Into_Postfix_Notation(string input_String);
void Re_to_NFA(string converted_Input_String);


int top = -1;

void push(struct nfaTable m) 
{
	top++;
	nfaStack[top] = m;
}

struct nfaTable pop() {
	return nfaStack[top--];
}

string read_Regular_Expression()
{
	string input_String;
	string converted_Input_String;
	
	cin >> input_String;
	converted_Input_String = insert_Concatenation_Operator_and_Convert_Into_Postfix_Notation(input_String);

	return converted_Input_String;
}  

string insert_Concatenation_Operator_and_Convert_Into_Postfix_Notation(string input_String)
{
	string result;

	int i = 0;
	while (i < input_String.length())
	{
		if (input_String[i] == '^') {
			i++;
			continue;
		}

		if (input_String[i] == '(')
		{
			i++;

			while (input_String[i] != ')')
			{
				if (input_String[i] == '|' || input_String[i] == '^')
				{
					i++;
					continue;
				}

				result += input_String[i++];
			}
			i++;

			result += '|';
		}

		if (input_String[i] == 'a' || input_String[i] == 'b')
		{
			result += input_String[i];
			result += '.';
			i++;
		}
	}

	return result;
}

void Re_to_NFA(string converted_Input_String) 
{
	for (int i = 0; i < converted_Input_String.length(); i++) 
	{
		if (converted_Input_String[i] == 'a')
		{
			struct nfaTable a;
			
			a.numOfNode = 2;
			a.state[0] = { 1, -1, -1, -1, 0 };
			a.state[1] = { -1, -1, -1, -1, 1 };

			push(a);
		}


		if (converted_Input_String[i] == 'b')
		{
			struct nfaTable b;

			b.numOfNode = 2;
			b.state[0] = { -1, 1, -1, -1, 0 };
			b.state[1] = { -1, -1, -1, -1, 1 };

			push(b);
		}

		if (converted_Input_String[i] == '|')
		{
			// 스택에서 M2 pop
			struct nfaTable M2 = pop();

			// 스택에서 M1 pop
			struct nfaTable M1 = pop();


			// M3 nfaTable 정의
			struct nfaTable M3;

			// numOfNode = n(M1) + n(M2) + 2 (initial state, final state 추가로 인한 + 2)
			M3.numOfNode = M1.numOfNode + M2.numOfNode + 2;

			// 0 Node에서 1 과 n(M1) + 1 Node로 lambda edge 삽입
			M3.state[0] = { -1, -1, { 1, M1.numOfNode + 1 }, 0 };

			// M1의 모든 Node 번호에 1씩 더하면서 M3에 복사
			for (int i = 0; i < M1.numOfNode; i++)
			{
				int a = M1.state[i].a;
				int b = M1.state[i].b;
				int e0 = M1.state[i].e[0];
				int e1 = M1.state[i].e[1];

				M1.state[i].a += (a != -1) ? 1 : 0;
				M1.state[i].b += (b != -1) ? 1 : 0;
				M1.state[i].e[0] += (e0 != -1) ? 1 : 0;
				M1.state[i].e[1] += (e1 != -1) ? 1 : 0;

				// initial state 때문에 i + 1 부터 복사
				M3.state[i + 1] = M1.state[i];	
			}

			// n(M1) Node에서 n(M1) + n(M2) + 1 Node로 lambda edge 삽입
			if (M3.state[M1.numOfNode].e[0] == -1)
				M3.state[M1.numOfNode].e[0] = M1.numOfNode + M2.numOfNode + 1;
			else
				M3.state[M1.numOfNode].e[1] = M1.numOfNode + M2.numOfNode + 1;	
			// n(M1) Node를 nonfinal로 변경
			M3.state[M1.numOfNode].output = 0;

			// M2의 모든 Node 번호에 1씩 더하면서 M3에 복사
			for (int i = 0; i < M2.numOfNode; i++)
			{
				int a = M2.state[i].a;
				int b = M2.state[i].b;
				int e0 = M2.state[i].e[0];
				int e1 = M2.state[i].e[1];

				M2.state[i].a += (a != -1) ? (M1.numOfNode + 1) : 0;
				M2.state[i].b += (b != -1) ? (M1.numOfNode + 1) : 0;
				M2.state[i].e[0] += (e0 != -1) ? (M1.numOfNode + 1) : 0;
				M2.state[i].e[1] += (e1 != -1) ? (M1.numOfNode + 1) : 0;

				// M1과 initial state 때문에 i + n(M1) + 1 부터 복사
				M3.state[i + M1.numOfNode + 1] = M2.state[i]; 
			}

			// n(M1) + n(M2) Node에서 n(M1) + n(M2) + 1 Node로 lambda edge 삽입
			if (M3.state[M1.numOfNode + M2.numOfNode].e[0] == -1)
				M3.state[M1.numOfNode + M2.numOfNode].e[0] = M1.numOfNode + M2.numOfNode + 1;
			else
				M3.state[M1.numOfNode + M2.numOfNode].e[1] = M1.numOfNode + M2.numOfNode + 1;
			// n(M1) + n(M2) Node를 nonfinal로 변경
			M3.state[M1.numOfNode + M2.numOfNode].output = 0;

			// n(M1) + n(M2) + 1 Node를 final state로 설정
			M3.state[M1.numOfNode + M2.numOfNode + 1] = { -1, -1, { -1, -1 }, 1 };


			// nfaStack에 push
			push(M3);
		}

		if (converted_Input_String[i] == '.')
		{
			// 스택에서 M2 pop
			struct nfaTable M2 = pop();

			// 스택에서 M1 pop
			struct nfaTable M1 = pop();


			// M3 nfaTable 정의
			struct nfaTable M3;

			// numOfNode = n(M1) + n(M2) - 1
			M3.numOfNode = M1.numOfNode + M2.numOfNode - 1;

			// M1을 M3에 복사
			for (int i = 0; i < M1.numOfNode; i++)
			{
				M3.state[i] = M1.state[i];
			}

			// n(M1) - 1 Node를 nonfinal로 변경
			M3.state[M1.numOfNode - 1].output = 0;

			// M2의 모든 Node 번호에 n(M1) - 1씩 더하면서 M3에 복사
			for (int i = 0; i < M2.numOfNode; i++)
			{
				int a = M2.state[i].a;
				int b = M2.state[i].b;
				int e0 = M2.state[i].e[0];
				int e1 = M2.state[i].e[1];

				M2.state[i].a += (a != -1) ? (M1.numOfNode - 1) : 0;
				M2.state[i].b += (b != -1) ? (M1.numOfNode - 1) : 0;
				M2.state[i].e[0] += (e0 != -1) ? (M1.numOfNode - 1) : 0;
				M2.state[i].e[1] += (e1 != -1) ? (M1.numOfNode - 1) : 0;

				// n(M1) - 1 Node가  때문에 i + n(M1) - 1 부터 복사
				M3.state[i + M1.numOfNode - 1] = M2.state[i];
			}

			// n(M1) + n(M2) - 2 Node를 final state로 설정
			M3.state[M1.numOfNode + M2.numOfNode - 2] = { -1, -1, { -1, -1 }, 1 };
		
		
			// nfaStack에 push
			push(M3);
		}

		if (converted_Input_String[i] == '*')
		{
			// 스택에서 M1 pop
			struct nfaTable M1 = pop();


			// M3 nfaTable 정의
			struct nfaTable M3;

			// numOfNode = n(M1) + 2 (initial state, final state 추가로 인한 + 2)
			M3.numOfNode = M1.numOfNode + 2;

			// 0 Node에서 1 과 n(M1) + 1 Node로 lambda edge 삽입
			M3.state[0] = { -1, -1, { 1, M1.numOfNode + 1 }, 0 };

			// M1의 모든 Node 번호에 1씩 더하면서 M3에 복사
			for (int i = 0; i < M1.numOfNode; i++)
			{
				int a = M1.state[i].a;
				int b = M1.state[i].b;
				int e0 = M1.state[i].e[0];
				int e1 = M1.state[i].e[1];

				M1.state[i].a += (a != -1) ? 1 : 0;
				M1.state[i].b += (b != -1) ? 1 : 0;
				M1.state[i].e[0] += (e0 != -1) ? 1 : 0;
				M1.state[i].e[1] += (e1 != -1) ? 1 : 0;

				// initial state 때문에 i + 1 부터 복사
				M3.state[i + 1] = M1.state[i];
			}

			// n(M1) Node에서 1 Node로 lambda edge 삽입
			if (M3.state[M1.numOfNode].e[0] == -1)
				M3.state[M1.numOfNode].e[0] = 1;
			else
				M3.state[M1.numOfNode].e[1] = 1;

			// n(M1) Node에서 n(M1) + 1 Node로 lambda edge 삽입
			if (M3.state[M1.numOfNode].e[0] == -1)
				M3.state[M1.numOfNode].e[0] = M1.numOfNode + 1;
			else
				M3.state[M1.numOfNode].e[1] = M1.numOfNode + 1;
			// n(M1) Node를 nonfinal로 변경
			M3.state[M1.numOfNode].output = 0;
			
			// n(M1) + 1 Node를 final state로 설정
			M3.state[M1.numOfNode + 1] = { -1, -1, { -1, -1 }, 1 };


			// nfaStack에 push
			push(M3);
		}
	}
}

int main(void) 
{
	string converted_Input_String = read_Regular_Expression();
	Re_to_NFA(converted_Input_String);

	cout << "\n변환한 Input_String: " << converted_Input_String << endl;
	cout << "\nNode의 개수: " << nfaStack[top].numOfNode << endl;
	cout << "NFA Table" << endl;
	printf("| state |  a  |  b  | e[0] | e[1] | output |\n");
	for (int i = 0; i < nfaStack[top].numOfNode; i++)
	{
		printf("|   %d   |  %2d |  %2d |  %2d  |  %2d  |   %2d   |\n", i, 
			nfaStack[top].state[i].a, nfaStack[top].state[i].b,
			nfaStack[top].state[i].e[0], nfaStack[top].state[i].e[1],
			nfaStack[top].state[i].output);
	}

	return 0;
}