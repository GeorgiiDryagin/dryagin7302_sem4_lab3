#pragma once
#include "Useful.h"

using namespace std;
class Node
{
private:

public:
	char* item;
	size_t frequency;
	bool* code;
	Node *next;
	Node *previous;

	Node *left;
	Node *right;
	Node *parent;

	//output format "(item:frequency:code)"
	char *out_fields(size_t code_len)
	{
		char* freq = inttostr(frequency);
		int out_len = 5 + strlen(item) + strlen(freq) + code_len;
		char* out = (char*)malloc(sizeof(char)*(out_len));
		int i = 0;
		while (i < out_len)
		{
			out[i++] = '(';
			for (int j = 0; j < strlen(item); j++)
				out[i++] = item[j];
			out[i++] = ':';
			for (int j = 0; j < strlen(freq); j++)
				out[i++] = freq[j];
			out[i++] = ':';
			if (code)
				for (int j = 0; j < code_len; j++)
					out[i++] = (code[j]) ? '1' : '0';
			out[i++] = ')';
			out[i++] = 0;
		}
		return out;
	};

	Node(char* new_item, size_t new_frequency)
	{
		item = string_copy(new_item);
		frequency = new_frequency;
		bool* code = nullptr;
		next = nullptr;
		previous = nullptr;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	};

	~Node()
	{
		if (left) free(left);
		if (right) free(right);
		if (item) free(item);
	};

	size_t find_max_level(size_t level, size_t max_level)
	{
		if (this->left) max_level = this->left->find_max_level(level + 1, max_level);
		if (this->right) max_level = this->right->find_max_level(level + 1, max_level);

		if (!left && !right && level > max_level) max_level = level;
		return max_level;
	};

	//code = parent->code + 0 if this is left_son else 1
	void assign_codes_rec(bool* prev_code, bool* this_code, size_t level, bool* point_true, bool* point_false)
	{
		if (this_code) 
			this->code = array_concat(prev_code, this_code, level - 1, 1);

		if (this->left)	this->left->assign_codes_rec(this->code, point_false, level + 1, point_true, point_false);
		if (this->right)	this->right->assign_codes_rec(this->code, point_true, level + 1, point_true, point_false);
	};

	//Output vertically. One level nodes is one vertical line
	char* output_vertically(char *output, int level)
	{
		if (this->right) output = this->right->output_vertically(output, level + 1);

		for (int i = 0; i < level; i++)
			output = string_concat(output, "    ");
		output = string_concat(output, string_concat(this->out_fields(level), "\n"));

		if (this->left) output = this->left->output_vertically(output, level + 1);

		return output;
	};

	//Output horizontally. One level nodes is one horizontal line
	char** output_horizontally(char** output, int level, bool** scheme, bool* path, size_t width, bool* t, bool* f)
	{
		//scheme[i][j] ( in i - line, j - number of the node from the left edge) = 1 - if node exists
		//path - 0 to the left, 1 to the right, on the way from root to this node
		//width - width of console
		int place = binary_to_decade(path, level);	// [j] in scheme
		scheme[level][place] = 1;

		for (int i = place - 1; i >= 0 && scheme[level][i] == 0; i--)
			output[level] = string_concat(output[level], give_some_chars(width / pow(2, level), ' '));

		output[level] = string_concat(output[level], give_some_chars(width / pow(2, level + 1) - strlen(this->out_fields(level)) / 2, ' '));
		output[level] = string_concat(output[level], this->out_fields(level));
		output[level] = string_concat(output[level], give_some_chars(width / pow(2, level + 1) - strlen(this->out_fields(level)) / 2, ' '));

		if (this->left) output = this->left->output_horizontally(output, level + 1, scheme, array_concat(path, f, level, 1), width, t, f);
		if (this->right) output = this->right->output_horizontally(output, level + 1, scheme, array_concat(path, t, level, 1), width, t, f);
		return output;

	};

	//return code of character after Huffman coding
	bool* code_message(char c, int &level) 
	{
		if (!this->left && !this->right)
			return code;

		level++;
		if (this->left)
			if (is_in_string(this->left->item, c)) return this->left->code_message(c, level);
		if (this->right)
			if (is_in_string(this->right->item, c)) return this->right->code_message(c, level);
		throw 
			runtime_error("Trying to find char, that is not in huffman tree");
		return nullptr;
	};
};

class List
{
private:

public:
	Node *head;
	Node *tail;
	bool coded;
	size_t coded_len;

	List() 
	{ 
		head = nullptr;
		tail = nullptr;
		bool coded = 0;
		coded_len = 0;
	};
	~List() { this->clear(); };

	//
	void get_list(char* string)
	{
		if (!coded)
			if (string)
				for (int i = 0; i < strlen(string); i++)
				{
					bool add = 1;	//going to add a new node to the list
					Node *cur = head;
					while (cur && add)
					{
						if (*(cur->item) == *(string + i))
						{
							add = 0;
							cur->frequency++;
						}
						cur = cur->next;
					}
					if (add)
					{
						Node *node = new Node(give_some_chars(1, *(string + i)), 1);
						this->insert_node_sorted_by_frequency(node);
					}
				}
		else throw runtime_error("It is prohibited to add characters to coded message");
	}

	//delete all nodes
	void clear()
	{
		if (head)
		{
			Node* next = head->next;
			do
			{
				delete head;
				head = next;
				if (head) next = next->next;
			} while (head);
		}
		head = nullptr;
		tail = nullptr;
	};

	//move tail to the head for two steps
	void false_double_pop_back()
	{
		if (!isEmpty())
		{
				tail = tail->previous;
				if (tail)
				{
					tail = tail->previous;
					tail->next = nullptr;
				}
		}
	};

	//add a node betwin nodes with larger and less frequances
	void insert_node_sorted_by_frequency(Node *node)
	{
		if (!coded)
		{
			if (!isEmpty()) 
			{
				if (node->frequency >= head->frequency)
				{
					node->next = head;
					head->previous = node;
					head = node;
				}
				else
					if (node->frequency < tail->frequency)
					{
						node->previous = tail;
						tail->next = node;
						tail = node;
					}
					else
					{
						Node* cur = head;
						while (cur->frequency > node->frequency)
							cur = cur->next;
						node->previous = cur->previous;
						if (cur->previous) cur->previous->next = node;
						cur->previous = node;
						node->next = cur;
					}
			}
			else
			{
				head = node;
				tail = node;
			}
		}
		//else throw exception
	};

	//add a parent node by its sons
	void create_tree_node(Node* left_son, Node* right_son)
	{
		Node *node = new Node(string_concat(left_son->item, right_son->item), left_son->frequency + right_son->frequency);
		node->left = left_son;
		node->right = right_son;
		left_son->parent = node;
		right_son->parent = node;
		this->insert_node_sorted_by_frequency(node);
	};

	//rerun string symbol:frequency for every node 
	char* output_frequences()
	{
		if (!isEmpty())
		{
			char* out = nullptr;
			Node* cur = head;
			while (cur)
			{
				out = string_concat(out, string_concat(string_concat(cur->item, ":"),string_concat(inttostr(cur->frequency), " ")));
				cur = cur->next;
			}
			return out;
		}
		else 
			return "";
	};

	//return tree as a strimg every level of tree is on one hirizontal line and contains '\n' at the end
	char* output_horizontally()
	{
		if (!isEmpty())
		{
			if (coded)
			{
				size_t width = 160;	//width of console
				int max_level = this->head->find_max_level(0, 0);
				char** output = (char**)malloc(sizeof(char*)*(max_level + 1));
				for (int i = 0; i < max_level + 1; i++)
					output[i] = nullptr;

				bool** scheme = (bool**)malloc(sizeof(bool*)*max_level + 1);
				for (int i = 0; i < max_level + 1; i++)
				{
					scheme[i] = (bool*)malloc(sizeof(bool) * pow(2, i));
					for (int j = 0; j < pow(2, i); j++)
						scheme[i][j] = 0;
				}

				bool t = 1, f = 0;
				bool *point_true = &t, *point_false = &f;
				head->output_horizontally(output, 0, scheme, nullptr, width, point_true, point_false);
				return straighten_array(output, width, max_level + 1);
			}
		}
		else return "";
	};

	//Output vertically. One level nodes is one vertical line
	char* output_vertically()
	{
		if (coded)
		{
			int max_lev = head->find_max_level(0, 0);
			char* output = (char*)malloc(sizeof(char));
			*output = 0;
			output = head->output_vertically(output, 0);
			return output;
		}
	};

	//assign bool code according to Huffman algorithm
	void assign_codes()
	{
		if(!isEmpty())
			if (head == tail)
			{
				bool t = true, f = false, *point_true = &t, *point_false = &f;
				head->code = nullptr;
				head->assign_codes_rec(nullptr, nullptr, 0, point_true, point_false);
			}
	};

	//build a tree according to Huffman algorithmm
	void Huffman()
	{
		if (!isEmpty())
		{
			while (head != tail)
			{
				this->create_tree_node(tail->previous, tail);
				this->false_double_pop_back();
			}
			this->assign_codes();
			coded = 1;
		}
		else throw runtime_error("Huffman algorithm can not be applied to an empty list");
	};

	//return 0 if there is no nodes in tree, else return 1
	bool isEmpty() { if (tail && head) return false; return true; };

	//translate char array into bool array, available only after List.Huffman()
	bool* code_message(char* message)
	{
		if (coded)
		{
			if (message)
			{

				bool* result = nullptr;
				coded_len = 0;
				for (int i = 0; i < strlen(message); i++)
				{
					int level = 0;
					bool *code = head->code_message(*(message + i), level);
					result = array_concat(result, code, coded_len, level);
					coded_len += level;
				}
				return result;
			}
			else return nullptr;
		}
		else return nullptr;
		//	throw runtime_error("List.code_message available only after List.Huffman()");
	}

	//translate bool array into char array, available only after List.Huffman()
	char* decode_message(bool* coded_mess)
	{
		if (coded_mess)
		{
			if (coded_mess)
			{
				char* result = nullptr;
				Node *cur = head;
				for (int i = 0; i < coded_len; i++)
				{
					if (cur->left || cur->right)
					{
						if (!*(coded_mess + i))
							cur = cur->left;
						else
							cur = cur->right;
						if (!cur->left && !cur->right)
						{
							result = string_concat(result, cur->item);
							cur = head;
						}
					}
				}
				return result;
			}
			else return nullptr;
		}
		else
			throw runtime_error("List.decode_message available only after List.Huffman()");
	}
};
