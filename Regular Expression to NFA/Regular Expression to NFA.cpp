#include <iostream>

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


void push(struct nfaTable m);
struct nfaTable pop();
string read_Regular_Expression();
string insert_Concatenation_Operator_and_Convert_Into_Postfix_Notation(string input_String);
void Re_to_NFA(string converted_Input_String);
void acceptance_Test();
void print_Closure(int* dat);

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
		// (a)^*에서 ^가 들어왔을 경우 continue
		if (input_String[i] == '^') {
			i++;
			continue;
		}

		// (가 열리면 안의 내용을 한번에 처리
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

		// *는 바로 추가
		if (input_String[i] == '*')
		{
			result += input_String[i];
			i++;
		}

		// a와 b는 뒤에 . 추가
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

void acceptance_Test() {
	// NFA table
	struct nfaTable table = nfaStack[top];

	// 갈 수 있는 Node를 기록하는 dat 배열
	int* dat{ new int[table.numOfNode] {} };

	// 입력할 String
	string test_String;
	cin >> test_String;

	// 0 staate의 lambda-closure 기록
	dat[0] = 1;
	for (int i = 0; i < table.numOfNode; i++)
	{
		if (dat[i] == 0)
			continue;

		int e0 = table.state[i].e[0];
		int e1 = table.state[i].e[1];

		if (e0 != -1)
			dat[e0] = 1;

		if (e1 != -1)
			dat[e1] = 1;
	}

	printf("0 state의 lambda-closure");
	printf("\n-----------------------------------------");
	print_Closure(dat);

	// initial state로의 edge 제거
	dat[0] = 0;
	for (int j = 0; j < test_String.length(); j++)
	{
		// dat의 값이 바뀌며 방문해야 할 곳을 못하는 경우를 방지
		int* buffer{ new int[table.numOfNode] {} };

		for (int i = 0; i < table.numOfNode; i++)
			buffer[i] = dat[i];

		// nfa table을 돌며 방문할 수 있는 곳 buffer에 기록
		for (int i = 0; i < table.numOfNode; i++)
		{
			// 방문하지 못하는 state는 continue
			if (dat[i] == 0 && buffer[i] == 0)
				continue;

			int a = table.state[i].a;
			int b = table.state[i].b;
			int e0 = table.state[i].e[0];
			int e1 = table.state[i].e[1];

			if (test_String[j] == 'a')
				if (a != -1)
					buffer[a] = 1;

			if (test_String[j] == 'b')
				if (b != -1)
					buffer[b] = 1;

			if (e0 != -1)
				buffer[e0] = 1;

			if (e1 != -1)
				buffer[e1] = 1;

			if (j == 0)
				continue;

			// 상태가 바뀌며 갈 수 없게된 곳 제거
			if (test_String[j - 1] == 'a' && test_String[j] == 'b')
				if (a != -1)
					buffer[a] = 0;

			// 상태가 바뀌며 갈 수 없게된 곳 제거
			if (test_String[j - 1] == 'b' && test_String[j] == 'a')
				if (b != -1)
					buffer[b] = 0;
		}

		// buffer에 기록뒨 Closure dat에 기록
		for (int i = 0; i < table.numOfNode; i++)
		{
			dat[i] = buffer[i];
		}

		printf("\n%c state의 lambda-closure", test_String[j]);
		printf("\n-----------------------------------------");
		print_Closure(dat);

		delete[] buffer;
	}

	// 최종 Closure를 확인해 accept, reject 판별
	if (dat[table.numOfNode - 1] == 1)
	{
		printf("\nAccept\n");
	}
	else
		printf("\nreject\n");


	delete[] dat;
}

void print_Closure(int* dat) {
	printf("\n|");
	for (int i = 0; i < nfaStack[top].numOfNode; i++)
	{
		printf(" %d |", i);
	}

	printf("\n-----------------------------------------\n|");
	for (int i = 0; i < nfaStack[top].numOfNode; i++)
		printf(" %d |", dat[i]);
	printf("\n-----------------------------------------\n");
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
	printf("\n\n");

	acceptance_Test();

	return 0;
}