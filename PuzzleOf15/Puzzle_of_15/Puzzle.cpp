#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <unordered_set>
#include <stack>

#pragma region DEFINITIONS
#define I_O
#define DIMENSIONS
#define B_BLOCKS
#define OVERLOADS
#define READ_WRITE
#define BASIC_FNC
#define SOLVABILITY_STEP1
#define SOLVE_PUZZLE_STEP2
#pragma endregion

#ifdef I_O
#define CI(x) std::cin>>x
#define CO(x) std::cout<<x
#define BR std::cout<<"\n"
#define END system("pause");return 0;
#endif

#ifdef DIMENSIONS
#define STATE_DIM 3
#endif

#ifdef B_BLOCKS
enum directions {
	null,
	up,
	right,
	down,
	left
};
struct tree {
	int state[STATE_DIM][STATE_DIM];
	int stateID = 0;
	int depth = 0;
	directions action = null;
	tree* parent = NULL;
};
struct position {
	int row;
	int col;
};
#endif

#ifdef OVERLOADS
bool operator==(const tree &x, const tree &y)
{
	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
			if (x.state[i][j] != y.state[i][j])
				return false;
	return true;
}
namespace std
{
	template<>
	struct hash<tree>
	{
		size_t
			operator()(const tree & obj) const
		{
			return hash<int>()(obj.stateID);
		}
	};
}
#endif

#ifdef READ_WRITE
void initial_state_read(tree* initial_state, const char* filename)
{
	std::ifstream fin(filename);
	int location = 0;

	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
		{
			fin >> initial_state->state[i][j];
			initial_state->stateID += initial_state->state[i][j] * location;
			location++;
		}

	fin.close();
}
void state_print(tree node)
{
	for (int i = 0; i < STATE_DIM; i++)
	{
		for (int j = 0; j < STATE_DIM; j++)
			CO(node.state[i][j] << "\t");
		BR;
	}
}
#endif

#ifdef BASIC_FNC
void interchange(int &a, int &b)
{
	int aux = a;
	a = b;
	b = aux;
}
#endif

#ifdef SOLVABILITY_STEP1
int number_of_inversions(int matrix[STATE_DIM][STATE_DIM], position &blank)
{
	int arr[STATE_DIM*STATE_DIM], k = 0;
	int inversions = 0;

	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
			if (matrix[i][j] != 0)
				arr[k++] = matrix[i][j];
			else
				blank.row = STATE_DIM - i + 2;

	for (int i = 0; i < k - 1; i++)
		for (int j = i + 1; j < k; j++)
			if (arr[i] > arr[j])
				inversions++;

	return inversions;
}
bool state_solvability(tree* this_state)
{
	position blank;
	int inversions = number_of_inversions(this_state->state, blank);

	/*if (STATE_DIM % 2 != 0 && inversions % 2 == 0)
	return true;
	else if (STATE_DIM % 2 == 0 && blank.row % 2 == 0 && inversions % 2 != 0)
	return true;
	else if (STATE_DIM % 2 == 0 && blank.row % 2 != 0 && inversions % 2 == 0)
	return true;*/
	if (((STATE_DIM % 2 != 0) && (inversions % 2 == 0)) || ((STATE_DIM % 2 == 0) && ((blank.row % 2 != 0) == (inversions % 2 == 0))))
		return true;
	return false;
}
#endif

#ifdef SOLVE_PUZZLE_STEP2
#pragma region CLOSED_USET
bool closed_insert(std::unordered_set<tree> &closed, const tree node)
{
	///duplicated state
	auto search = closed.find(node);
	if (search != closed.end())
		return false;

	///new state
	closed.insert(node);
	return true;
}
void closed_print(const std::vector<tree> closed)
{
	CO("Printing closed-vector: "); BR;
	for (unsigned i = 0; i < closed.size(); i++)
	{
		state_print(closed[i]);
		BR; BR;
	}
	BR;
}
#pragma endregion

#pragma region FRINGE_QUEUE
tree* state_do_action(tree* node, const directions action)
{
	tree* new_node = new tree;
	position blank;

	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
		{
			if (node->state[i][j] == 0)
			{
				blank.row = i;
				blank.col = j;
			}
			new_node->state[i][j] = node->state[i][j];
		}

	///action: possible -> do action
	///action: impossibile -> return NULL
	switch (action)
	{
	case up:
		if (blank.row == 0)
			return NULL;
		interchange(new_node->state[blank.row - 1][blank.col],
			new_node->state[blank.row][blank.col]);
		break;
	case right:
		if (blank.col == STATE_DIM - 1)
			return NULL;
		interchange(new_node->state[blank.row][blank.col + 1],
			new_node->state[blank.row][blank.col]);
		break;
	case down:
		if (blank.row == STATE_DIM - 1)
			return NULL;
		interchange(new_node->state[blank.row + 1][blank.col],
			new_node->state[blank.row][blank.col]);
		break;
	case left:
		if (blank.col == 0)
			return NULL;
		interchange(new_node->state[blank.row][blank.col - 1],
			new_node->state[blank.row][blank.col]);
		break;
	}

	///stateID for new_node
	int location = 0;
	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
		{
			new_node->stateID += new_node->state[i][j] * location;
			location++;
		}

	///populate + return: new_node
	new_node->action = action;
	new_node->depth = node->depth + 1;
	new_node->parent = node;
	return new_node;
}
void fringe_expand(std::queue<tree*> &fringe, tree* node)
{
	tree* new_node1 = state_do_action(node, up);
	tree* new_node2 = state_do_action(node, right);
	tree* new_node3 = state_do_action(node, down);
	tree* new_node4 = state_do_action(node, left);

	if (new_node1 != NULL)
		fringe.push(new_node1);
	if (new_node2 != NULL)
		fringe.push(new_node2);
	if (new_node3 != NULL)
		fringe.push(new_node3);
	if (new_node4 != NULL)
		fringe.push(new_node4);
}
void fringe_print(std::queue<tree*> fringe, const int test)
{
	CO(test << " Printing fringe-queue: "); BR;
	while (!fringe.empty())
	{
		state_print(*fringe.front());
		fringe.pop();
		BR; BR;
	}
	BR;
}
#pragma endregion

#pragma region IF_SOLVED
bool state_solved(const tree* node)
{
	if (node->state[STATE_DIM - 1][STATE_DIM - 1] != 0)
		return false;

	for (int i = 0; i < STATE_DIM; i++)
		for (int j = 0; j < STATE_DIM; j++)
			if (i == STATE_DIM - 1 && j == STATE_DIM - 2)
				return true;
			else if (j == STATE_DIM - 1)
			{
				if (node->state[i][j] > node->state[i + 1][0])
					return false;
			}
			else if (node->state[i][j] > node->state[i][j + 1])
				return false;
	return true;
}
void print_result_to_file(const tree* node, const int generation_no, const char* filename)
{
	std::ofstream fout(filename);
	std::stack<tree> solving_steps;
	int steps_no = node->depth;

	///populate solving_steps
	while (node->parent != NULL)
	{
		solving_steps.push(*node);
		node = node->parent;
	}

	///OUTPUT
	fout << "INITIAL STATE:\n";
	for (int i = 0;i < STATE_DIM;i++)
	{
		for (int j = 0;j < STATE_DIM;j++)
			fout << node->state[i][j] << "\t";
		fout << "\n";
	}
	fout << "\nNR. OF BROWSED STATES:\n";
	fout << generation_no << "\n";
	fout << "\nSTEPS TAKEN:\n";
	fout << steps_no << "\n";
	fout << "\nSTEPS HISTORY:\n";
	int nr = 1;
	while (!solving_steps.empty())
	{
		fout << "step " << nr++ << ":  \t";
		fout << "Move blank ";
		switch (solving_steps.top().action)
		{
		case up:
			fout << "UP";
			break;
		case right:
			fout << "RIGHT";
			break;
		case down:
			fout << "DOWN";
			break;
		case left:
			fout << "LEFT";
			break;
		}
		fout << "\n";
		solving_steps.pop();
	}
}
#pragma endregion

void solve_puzzle(tree* initial_state, const char* filename)
{
	std::unordered_set<tree> closed;
	std::queue<tree*> fringe;
	fringe.push(initial_state);

	int generation_no = 0;
	while (!fringe.empty())
	{
		///is puzzle solved?
		if (state_solved(fringe.front()))
		{
			print_result_to_file(fringe.front(), generation_no, filename);
			CO("!!! RESULT FOUND !!!\nResult printed to file\n");
			CO("State nr. " << generation_no);BR;
			state_print(*fringe.front());BR;
			return;
		}

		///memorize + expand current node(if it's not duplicate)
		if (closed_insert(closed, *fringe.front()))
			fringe_expand(fringe, fringe.front());

		fringe.pop();

		///print generations(states) for 100.000 interval
		generation_no++;
		if (generation_no % 100000 == 0)
		{
			CO("State nr. " << generation_no << " - Generating...");BR;
			state_print(*fringe.front());BR;
		}
	}

	CO("ERROR: Fringe is empty, no results found!\n");
}
#endif

int main()
{
	///INITIAL_STATE
	tree* initial_state = new tree;
	initial_state_read(initial_state, "puzzle.in");

	///STEP 1: VERIFY SOLVABILITY
	if (!state_solvability(initial_state))
	{
		CO("ERROR: Unsolvable puzzle!\n");
		END;
	}

	///STEP 2: SOLVE PUZZLE
	solve_puzzle(initial_state, "puzzle.out");

	END;
}