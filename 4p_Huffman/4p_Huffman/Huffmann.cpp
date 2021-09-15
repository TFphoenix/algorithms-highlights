/*
Huffmann's Tree Data Compression - "Greedy" Alg.
Teodor-Adrian Mihaescu

Frecv. from: https://en.wikipedia.org/wiki/Letter_frequency (applied for 1.000 words)
(e.g. a=8.167% --> 81)			!!! EQUAL FRECV. NOT ALLOWED !!!

Input ex.:
26
a	b	c	d	e	f	g	h	i	j	k	l	m	n	o	p	q	r	s	t	u	v	w	x	y	z
81	14	27	42	127	22	20	60	69	3	7	40	24	67	75	19	1	59	63	92	27	9	53	2	39	0

Suggested ex.:
6
a	b	c	d	e	f
45	13	12	16	9	5
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define I_O
#define STRUCTS
#define FRECV_STEP1
#define TREE_STEP2
#define COMPRESSION_STEP3

#ifdef I_O
#define CI(x) std::cin>>x
#define CO(x) std::cout<<x
#define BR std::cout<<"\n"
#endif

#ifdef STRUCTS
struct frecv_chr {
	char chr;
	int frecv;
	std::string hm_code;
};

struct tree {
	int data;
	tree* left = NULL;//?
	tree* right = NULL;//?
};
#endif

#ifdef FRECV_STEP1
int symbols_read(frecv_chr* symbols, const char* filename)
{
	int size;
	std::ifstream fin(filename);

	fin >> size;
	for (int i = 0; i < size; i++)
		fin >> symbols[i].chr;
	for (int i = 0; i < size; i++)
		fin >> symbols[i].frecv;

	return size;
}

void symbols_print(frecv_chr* symbols, const int size)
{
	for (int i = 0; i < size; i++)
		CO(symbols[i].chr << ":" << symbols[i].frecv << " | ");
}

void symbols_interchange(frecv_chr &x, frecv_chr &y)
{
	frecv_chr aux = x;
	x = y;
	y = aux;
}

void symbols_sort(frecv_chr* arr, int left, int right)
{
	int i = left, j = right, pivot = arr[(left + right) / 2].frecv;

	do {
		while (arr[i].frecv < pivot)i++;
		while (arr[j].frecv > pivot)j--;

		if (i <= j)
		{
			symbols_interchange(arr[i], arr[j]);
			i++; j--;
		}
	} while (i <= j);

	if (left < j)
		symbols_sort(arr, left, j);
	if (i < right)
		symbols_sort(arr, i, right);
}

void array_cpy(frecv_chr* arr_1, int* arr_2, const int size)
{
	for (int i = 0; i < size; i++)
		arr_2[i] = arr_1[i].frecv;
}

void symbols_cpy(frecv_chr* symbols, std::vector<tree*> &hm_tree, int size)
{
	for (int i = 0; i < size; i++)
	{
		tree* node = new tree;
		node->data = symbols[i].frecv;
		hm_tree.push_back(node);

	}
}

void symbols_test(std::vector<tree*> hm_tree)
{
	for (unsigned int i = 0; i < hm_tree.size(); i++)
		std::cout << hm_tree[i]->data << " ";
}
#endif

//DE REZOLVAT: delete la pointeri a.i. sa nu mai deterioreze arborele (in tree_shift_and_sort si tree_creation)
#ifdef TREE_STEP2
void tree_interchange(tree* &x, tree* &y)
{
	tree* aux = x;
	x = y;
	y = aux;
}

void tree_sort(std::vector<tree*> &hm_tree, int left, int right)
{
	int i = left, j = right, pivot = hm_tree[(left + right) / 2]->data;

	do {
		while (hm_tree[i]->data < pivot)i++;
		while (hm_tree[j]->data > pivot)j--;

		if (i <= j)
		{
			tree_interchange(hm_tree[i], hm_tree[j]);
			i++; j--;
		}
	} while (i <= j);

	if (left < j)
		tree_sort(hm_tree, left, j);
	if (i < right)
		tree_sort(hm_tree, i, right);
}

void tree_shift_and_sort(std::vector<tree*> &hm_tree)
{
	///shifting from element [1] + deleting last element(old element [1])
	for (unsigned int i = 1; i < hm_tree.size() - 1; i++)
		tree_interchange(hm_tree[i], hm_tree[i + 1]);
	//delete hm_tree[hm_tree.size() - 1];
	hm_tree.pop_back();

	///sorting resulted array
	tree_sort(hm_tree, 0, hm_tree.size() - 1);
}

void tree_creation(std::vector<tree*> &hm_tree)
{
	///TEST
	symbols_test(hm_tree);
	BR;

	if (hm_tree.size() == 1)
		return;

	///binding first two nodes + sum
	tree* node = new tree;
	node->data = hm_tree[0]->data + hm_tree[1]->data;
	node->left = hm_tree[0];
	node->right = hm_tree[1];

	///inserting resulted node into tree on position [0] + deleting unnecessary pointer
	tree_interchange(hm_tree[0], node);
	//delete node;

	tree_shift_and_sort(hm_tree);
	tree_creation(hm_tree);
}
#endif

#ifdef COMPRESSION_STEP3
void code_print_test(const std::string code)
{
	BR;
	for (unsigned int i = 0; i < code.size(); i++)
		CO(code[i]);
	BR;
}

void tree_search_by_frecv(const tree* node, const int frecv_value, std::string &code, std::string &final_code)
{
	if (node->left == NULL && node->right == NULL)
	{
		if (node->data == frecv_value)
			final_code = code;
		else
			code.pop_back();
		return;
	}

	code.push_back('0');
	tree_search_by_frecv(node->left, frecv_value, code, final_code);
	code.push_back('1');
	tree_search_by_frecv(node->right, frecv_value, code, final_code);
	code.pop_back();
}

void table_populate(const tree* hm_root, frecv_chr* symbols, const int size)
{
	std::string aux;
	for (int i = 0; i < size; i++)
	{
		aux = symbols[i].hm_code;
		tree_search_by_frecv(hm_root, symbols[i].frecv, aux, symbols[i].hm_code);
	}
}

void table_print(const frecv_chr* symbols, const int size)
{
	CO(" Chr.\t| Frecv.\t| HM-Code");
	BR;
	CO("---------------------------------------");
	BR;
	for (int i = 0; i < size; i++)
		CO(symbols[i].chr << "\t|\t" << symbols[i].frecv << "\t|\t" << symbols[i].hm_code << "\n");
}

std::string alpha_compress(const char symbol, const frecv_chr* symbols, const int size)
{
	for (int i = 0; i < size; i++)
		if (symbols[i].chr == symbol)
			return symbols[i].hm_code;
	return NULL;
}

void message_concatenate(std::string &message, std::string code)
{
	//concatenate obtained code to compressed_message
	for (int i = 0; i < code.size(); i++)
		message.push_back(code[i]);
}

void message_compress(const frecv_chr* symbols, const int size)
{
	std::string message, compressed_message;
	CO("Insert the message that you want to compress: "); CI(message);

	for (int i = 0; i < message.size(); i++)
	{
		std::string code;
		code = alpha_compress(message[i], symbols, size);
		message_concatenate(compressed_message, code);
	}

	char option;
	CO("Compressed message: " << compressed_message); BR;
	CO("Compress another message? (y/n): "); CI(option);
	if (option == 'y')
		message_compress(symbols, size);
}
#endif


int main()
{
	frecv_chr initial_symbols[100]; int size;//array with frecv and tags
	std::vector<tree*> hm_tree;//Huffmann's tree

	///frecv array
	size = symbols_read(initial_symbols, "frecvente.txt");
	symbols_cpy(initial_symbols, hm_tree, size);

	///Huffmann Tree
	CO("Huffmann's Tree Node Hierarchy:\n---------------------------------------\n");
	tree_sort(hm_tree, 0, hm_tree.size() - 1);
	tree_creation(hm_tree);

	///Compression Table
	CO("\n\nHuffmann's Tree Compression Table:\n---------------------------------------\n");
	table_populate(hm_tree[0], initial_symbols, size);
	table_print(initial_symbols, size);

	///Message compression
	CO("\n\nHuffmann's Tree Message Compression:\n---------------------------------------\n");
	message_compress(initial_symbols, size);

	system("pause");
	return 0;
}