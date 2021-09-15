/*
IMPORTANT:
	EXCLUDED LETTER:	Q	(line 49)
	FILLER LETTER:		X	(lines 162-171)
*/
#include <iostream>
#include <string.h>

void printM(int dim, int matrix[5][5])
{
	for (int i = 0;i < dim;i++)
	{
		for (int j = 0;j < dim;j++)
			std::cout << (char)matrix[i][j] << " ";
		std::cout << "\n";
	}
}

void interchange(int &x, int &y)
{
	int aux = x;
	x = y;
	y = aux;
}

//generates keyM
void format_key(char key[101], int keyM[5][5])
{
	bool frecv[26] = { false };
	int line = 0, col = 0;

	///se parcurge key si se adauga simultan in keyM cat si in frecv literele ce respecta conditiile
	for (unsigned i = 0;i < strlen(key);i++)
	{
		if (!frecv[key[i] - 65] && key[i] != ' ' && key[i] != 'Q')///conditii
		{
			frecv[key[i] - 65] = true;
			keyM[line][col++] = key[i];

			if (col == 5)
			{
				col = 0;
				line++;
			}
		}
	}

	///eliminam Q / consideram ca deja l-am intordus
	frecv[16] = true;

	///se completeaza keyM cu literele din alfabet ramase
	for (int i = 0;i < 26;i++)
	{
		if (!frecv[i])
			keyM[line][col++] = i + 65;
		if (col == 5)
		{
			col = 0;
			line++;
		}
	}

}

void remove_spaces(char txt[101])
{
	if (txt == NULL)
		return;

	int n = 0;

	for (unsigned i = 0;i < strlen(txt);i++)
		if (txt[i] != ' ')
			txt[n++] = txt[i];
	txt[n] = '\0';
}

void push_string_right(char txt[101], unsigned pos)
{
	unsigned i = strlen(txt);
	txt[strlen(txt) + 1] = '\0';
	for (i;i > pos;i--)
	{
		txt[i] = txt[i - 1];
	}
}

//searches for c1 and c2 in keyM and returns their position
void search_pos(int keyM[5][5], char c1, char c2, int &c1_i, int &c1_j, int &c2_i, int &c2_j)
{
	for (int i = 0;i < 5;i++)
		for (int j = 0;j < 5;j++)
			if ((char)keyM[i][j] == c1)
			{
				c1_i = i;
				c1_j = j;
			}
			else if ((char)keyM[i][j] == c2)
			{
				c2_i = i;
				c2_j = j;
			}
}

//encrypts a pair using P.F.C. rules
void encrypt_pair(char &c1, char &c2, int keyM[5][5])
{
	int c1_i, c1_j, c2_i, c2_j;
	search_pos(keyM, c1, c2, c1_i, c1_j, c2_i, c2_j);

	///criptez pozitiile
	if (c1_i == c2_i)///LINE MATCH
	{
		if (c1_j == 4)
		{
			c1_j = 0;
			c2_j++;
		}
		else if (c2_j == 4)
		{
			c2_j = 0;
			c1_j++;
		}
		else
		{
			c1_j++;
			c2_j++;
		}
	}
	else if (c1_j == c2_j)///COLUMN MATCH
	{
		if (c1_i == 4)
		{
			c1_i = 0;
			c2_i++;
		}
		else if (c2_i == 4)
		{
			c2_i = 0;
			c1_i++;
		}
		else
		{
			c1_i++;
			c2_i++;
		}
	}
	else///NO MATCH
		interchange(c1_j,c2_j);

	///criptez valorile
	c1 = (char)keyM[c1_i][c1_j];
	c2 = (char)keyM[c2_i][c2_j];
}

//encrypts msg using P.F.C. rules
void encrypt_msg(char msg[101], int keyM[5][5])
{
	remove_spaces(msg);
	for (unsigned i = 0;i < strlen(msg);i += 2)
	{
		if (msg[i] == msg[i + 1])
		{
			push_string_right(msg, i + 1);
			msg[i + 1] = 'X';
		}
		else if (msg[i + 1] == '\0')
		{
			msg[i + 2] = '\0';
			msg[i + 1] = 'X';
		}
		encrypt_pair(msg[i], msg[i + 1], keyM);
	}
}

int main()
{
	char key[101], msg[101];
	int keyM[5][5];

	std::cout << "PLAYFAIR CIPHER \t by Teodor Mihaescu \n -------- \nEnter key: ";
	std::cin.getline(key, 101);

	format_key(key, keyM);

	std::cout << "Enter the message that you want to encrypt: ";
	std::cin.getline(msg, 101);

	encrypt_msg(msg, keyM);
	std::cout << "Your encrypted message is: " << msg;

	std::cout << "\n";
	system("pause");
	return 0;
}
