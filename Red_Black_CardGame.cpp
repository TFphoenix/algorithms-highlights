//Card game (black and red cards only)
//1. Cards are equally shuffled among the 2 players
//2. Player who plays red card makes the other one draw the whole board (only if red card it's not the only one on the board)
//3. The first player who remains with no cards in his deck wins the game
#include <iostream>
#include <stack>
#include <random>
#include <ctime>

#define LOG(x) std::cout<<x
#define CIN(x) std::cin>>x

void deck_validation(unsigned &nr_cards)
{
	int nr;
	do {
		LOG("Number of cards the deck will contain: ");
		CIN(nr);
		if (nr <= 2)
			LOG("ERROR: Number smaller than 2 entered\n");
		else if (nr % 2 != 0)
			LOG("ERROR: Odd number entered\n");
	} while (nr % 2 != 0 || nr <= 2);
	nr_cards = nr;
}

void deck_rand(unsigned nr_cards, std::stack<bool> &deck)
{
	unsigned nr_red = 0, nr_black = 0;

	//random generator+seed
	std::mt19937 generator;
	generator.seed((unsigned)std::time(0));
	std::uniform_int_distribution<uint32_t> rand_nr(0, 1);

	//populating deck
	for (unsigned i = 0;i < nr_cards / 2;i++)
	{
		int randomval = rand_nr(generator);
		deck.push(randomval);
		switch (randomval)
		{
		case 0:
			nr_black++;
			break;
		case 1:
			nr_red++;
			break;
		}
	}
	for (unsigned i = nr_cards / 2;i < nr_cards;i++)
	{
		if (nr_red > nr_black)
		{
			deck.push(0);
			nr_black++;
		}
		else
		{
			deck.push(1);
			nr_red++;
		}
	}
}

void print_deck(std::stack<bool> deck)
{
	if (deck.empty())
		LOG("EMPTY");
	else
		while (!deck.empty())
		{
			switch (deck.top())
			{
			case 1:
				LOG("R ");
				break;
			case 0:
				LOG("B ");
				break;
			}
			deck.pop();
		}
	LOG("\n");
}

void deck_shuffle(std::stack<bool> &to_deck, std::stack<bool> &from_deck, unsigned nr)
{
	for (unsigned i = 0;i < nr;i++)
	{
		to_deck.push(from_deck.top());
		from_deck.pop();
	}
}

void shuffle_decks(std::stack<bool> &deck_start, std::stack<bool> &deck_1, std::stack<bool> &deck_2, unsigned nr)
{
	bool to_deck = true;
	for (unsigned i = 0;i < nr;i++)
	{
		if (to_deck)
			deck_1.push(deck_start.top());
		else
			deck_2.push(deck_start.top());
		deck_start.pop();

		to_deck = !to_deck;
	}
}

bool check_board(std::stack<bool> board)
{
	if (!board.empty() && board.top() == 1)
	{
		board.pop();
		if (!board.empty())
			return 1;
	}
	return 0;
}

void simulate_game(std::stack<bool> &deck_1, std::stack<bool> &deck_2)
{
	unsigned round = 1;
	std::stack<bool> board;

	while (deck_1.empty() == deck_2.empty())
	{
		LOG("\nROUND " << round << ":\n");round++;
		LOG("Initial Board: ");print_deck(board);
		LOG("Player 1 deck: ");print_deck(deck_1);
		LOG("Player 2 deck: ");print_deck(deck_2);
		LOG("-----\n");

		//player 1's turn
		board.push(deck_1.top());
		deck_1.pop();
		LOG("Player 1 played a card => Board: ");print_deck(board);

		if (check_board(board))
		{
			//player 2 takes all cards from board
			while (!board.empty())
			{
				deck_2.push(board.top());
				board.pop();
			}
		}

		//player 2's turn
		board.push(deck_2.top());
		deck_2.pop();
		LOG("Player 2 played a card => Board: ");print_deck(board);

		if (check_board(board))
		{
			//player 1 takes all cards from board
			while (!board.empty())
			{
				deck_1.push(board.top());
				board.pop();
			}
		}
		LOG("Final Board: ");print_deck(board);
	}

	if (deck_1.empty())
		LOG("\nPlayer 1 wins!\n\n");
	else
		LOG("\nPlayer 2 wins!\n\n");
}

int main()
{
	unsigned nr_cards;
	std::stack<bool> deck_start, deck_p1, deck_p2;

	//starting deck
	deck_validation(nr_cards);
	deck_rand(nr_cards, deck_start);
	LOG("Starting deck: ");
	print_deck(deck_start);

	//player decks
	shuffle_decks(deck_start, deck_p1, deck_p2, nr_cards);
	LOG("Player 1 deck: ");
	print_deck(deck_p1);
	LOG("Player 2 deck: ");
	print_deck(deck_p2);

	//game
	simulate_game(deck_p1, deck_p2);

	system("pause");
	return 0;
}