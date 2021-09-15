#include <iostream>
#include <fstream>

#define BOARD
#define VALIDATION_NEW
#define POSITION
#define GAME

#ifdef BOARD
void board_read(int board[9][9])
{
	std::ifstream fin("board.txt");

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			fin >> board[i][j];
}

void board_print(int board[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			std::cout << board[i][j] << " ";
		std::cout << "\n";
	}
}
#endif

#ifdef VALIDATION_NEW
bool validation_column(int board[9][9])
{
	for (int j = 0; j < 9; j++)
	{
		int frecv[10] = {};

		//count frecv
		for (int i = 0; i < 9; i++)
			frecv[board[i][j]]++;

		//valid frecv
		for (int f = 1; f < 10; f++)
			if (frecv[f] != 1)
				return false;
	}
	return true;
}

bool validation_line(int board[9][9])
{
	for (int i = 0; i < 9; i++)
	{
		int frecv[10] = {};

		//count frecv
		for (int j = 0; j < 9; j++)
		{
			frecv[board[i][j]]++;
			std::cout << board[i][j] << " ";
		}
		std::cout << "\n";

		//valid frecv
		for (int f = 1; f < 10; f++)
			if (frecv[f] == 0)
				return false;
	}
	return true;
}

bool validation_3x3(int board[9][9], int from_i, int to_i, int from_j, int to_j)
{
	int frecv[10] = {};

	for (int i = from_i; i < to_i; i++)
		for (int j = from_j; j < to_j; j++)
			frecv[board[i][j]]++;

	for (int f = 1; f < 10; f++)
		if (frecv[f] == 0)
			return false;

	return true;
}

bool validation_9x9(int board[9][9])
{
	int from_i = 0, to_i = 3;
	int from_j = 0, to_j = 3;
	for (int k = 0; k < 9; k++)
	{
		if (!validation_3x3(board, from_i, to_i, from_j, to_j))
			return false;
		from_j += 3;
		to_j += 3;
		if (k == 2 || k == 5 || k == 8)
		{
			from_j = 0; to_j = 3;
			from_i += 3; to_i += 3;
		}
	}
	return true;
}

//main validation
bool board_validation(int board[9][9])
{
	if (validation_line(board) && validation_column(board) && validation_9x9(board))
		return true;
	return false;
}
#endif

#ifdef POSITION
void position_3x3(int board[9][9],int from_i, int to_i, int from_j, int to_j,int segment)
{
	for (int i = from_i; i < to_i; i++)
		for (int j = from_j; j < to_j; j++)
			if (board[i][j] == 9)
				std::cout << "\nMaxim pentru segmentul " << segment << " pe pozitia: " << i << ":" << j;
}

void position_9x9(int board[9][9])
{
	int from_i = 0, to_i = 3;
	int from_j = 0, to_j = 3;
	for (int k = 0; k < 9; k++)
	{
		position_3x3(board, from_i, to_i, from_j, to_j,k);
		from_j += 3;
		to_j += 3;
		if (k == 2 || k == 5 || k == 8)
		{
			from_j = 0; to_j = 3;
			from_i += 3; to_i += 3;
		}
	}
	std::cout << "\n";
}
#endif

#ifdef GAME
struct position {
	int i;
	int j;
};

void sudoku_read(int board[9][9])
{
	std::ifstream fin("sudoku.txt");
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			fin >> board[i][j];
}

int count_zero(int board[9][9])
{
	int counter = 0;
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (!board[i][j])
				counter++;
	return counter;
}

position first_zero(int board[9][9])
{
	position first0; first0.i = 0; first0.j = 0;

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			if (!board[i][j])
			{
				first0.i = i; first0.j = j;
				return first0;
			}

	return first0;
}

void sudoku_game()
{
	int sudoku[9][9];
	sudoku_read(sudoku);
	
	std::cout << "\n----------\nSUDOKU GAME\n----------\n";

	bool completed = false;
	while (!completed)
	{
		int input=-1;
		position current;

		board_print(sudoku);
		std::cout << "Introduceti o valoare pt. primul 0: "; std::cin >> input;

		current = first_zero(sudoku);
		sudoku[current.i][current.j] = input;

		if (!count_zero(sudoku))
			completed = true;
	}

	if (board_validation(sudoku))
		std::cout << "\nFelicitari! Ati castigat jocul!";
	else
	{
		std::cout << "\nAti pierdut...\nMai incercati? (d/n)";
		char retry; std::cin >> retry;
		if (retry == 'd')
			sudoku_game();
		else
			"\nIesire joc...";
	}
}
#endif

int main()
{
	int board[9][9];
	board_read(board);

	//A
	if (board_validation(board))
		std::cout << "Tabla este valida\n";
	else
		std::cout << "Tabla nu este valida\n";

	//B
	position_9x9(board);

	//C
	sudoku_game();

	system("pause");
	return 0;
}