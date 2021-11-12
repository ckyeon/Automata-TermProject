#include <iostream>
#include <vector>

using namespace std;

#define MAXNODE 100
#define MAXSTACK 100


struct oneState {
	int a, b, e[2], output;
};

struct nfaTable {
	int numOfNode;
	struct oneState state[MAXNODE];
} nfaStack[MAXSTACK];

struct oneState a_State_0 = { 1, -1, -1, -1, 0 };
struct oneState a_State_1 = { -1, -1, -1, -1, 1 };

struct oneState b_State_0 = { -1, 1, -1, -1, 0 };
struct oneState b_State_1 = { -1, -1, -1, -1, 1 };


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

	for (int i = 0; i < input_String.length(); i++) 
	{
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

		if (input_String[i] == '^')
			continue;

		result += input_String[i];

		if (input_String[i] == 'a' || input_String[i] == 'b')
		{
			result += '.';
		}
	}

	return result;
}

void Re_to_NFA(string converted_Input_String) 
{
	for (int i = 0; i < converted_Input_String.length(); i++) 
	{

		if (converted_Input_String[i] == '|')
		{
			struct nfaTable M2 = pop();
			struct nfaTable M1 = pop();

			struct oneState* copy_M1_State = new struct oneState[M1.numOfNode];
			struct oneState* copy_M2_State = new struct oneState[M2.numOfNode];


			// M3 nfaTable을 구성할 State 생성
			
			// M3의 initial_State
			// 0 Node(initial_State)에서 1, n(M1) + 1 Node에 lambda edge 삽입
			struct oneState initial_State = { -1, -1, {1, M1.numOfNode + 1}, 0 };

			// M1의 모든 Node 번호에 1씩 더하면서 복사
			for (int i = 0; i < M1.numOfNode; i++)
			{
				int a = M1.state[i].a;
				int b = M1.state[i].b;

				copy_M1_State[i] = M1.state[i];
				copy_M1_State[i].a = (a != -1) ? (a + 1) : -1;
				copy_M1_State[i].b = (b != -1) ? (b + 1) : -1;
			}

			// M2의 모든 Node 번호에 n(M1) + 1씩 더하면서 복사
			for (int i = 0; i < M2.numOfNode; i++)
			{
				int a = M2.state[i].a;
				int b = M2.state[i].b;

				copy_M2_State[i] = M2.state[i];
				copy_M2_State[i].a = (a != -1) ? (a + M1.numOfNode + 1) : -1;
				copy_M2_State[i].b = (b != -1) ? (b + M1.numOfNode + 1) : -1;
			}

			// M3의 final_State
			// n(M1) + n(M2) + 1 Node(final_State)를 final state로 설정
			struct oneState final_State = { -1, -1, {-1, -1}, 1 };


			// M3 nfaTable 정의 및 위에서 정의한 State로 초기화
			// numOfNode = n(M1) + n(M2) + 2 (initial state, final state 추가로 인한 + 2)
			struct nfaTable M3 = { M1.numOfNode + M2.numOfNode + 2, initial_State, copy_M1_State[0], copy_M1_State[1], copy_M2_State[0], copy_M2_State[1], final_State };

			// n(M1) Node에서 n(M1) + n(M2) + 1 Node로 lambda edge 삽입
			M3.state[M1.numOfNode].e[0] = M1.numOfNode + M2.numOfNode + 1;
			// n(M1) Node를 nonfinal로 변경
			M3.state[M1.numOfNode].output = 0;

			// n(M1) + n(M2) Node에서 n(M1) + n(M2) + 1 Node로 lambda edge 삽입
			M3.state[M1.numOfNode + M2.numOfNode].e[0] = M1.numOfNode + M2.numOfNode + 1;
			// n(M1) + n(M2) Node를 nonfinal로 변경
			M3.state[M1.numOfNode + M2.numOfNode].output = 0;


			// nfaStack에 push
			push(M3);


			// 동적할당 해제
			delete[] copy_M1_State;
			delete[] copy_M2_State;
		}

		if (converted_Input_String[i] == '*')
		{

		}


		if (i == converted_Input_String.length() - 1)
			continue;

		if (converted_Input_String[i] == 'a' && converted_Input_String[i + 1] == '.')
		{

		}
		else if (converted_Input_String[i] == 'a')
		{
			struct nfaTable a_NFA_Table = { 2, a_State_0, a_State_1 };
			push(a_NFA_Table);
		}


		if (converted_Input_String[i] == 'b' && converted_Input_String[i + 1] == '.')
		{

		}
		else if (converted_Input_String[i] == 'b')
		{
			struct nfaTable b_NFA_Table = { 2, b_State_0, b_State_1 };

			push(b_NFA_Table);
		}
	}
}

int main(void) 
{
	string converted_Input_String = read_Regular_Expression();
	Re_to_NFA(converted_Input_String);

	return 0;
}