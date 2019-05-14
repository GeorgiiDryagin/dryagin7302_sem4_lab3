#pragma once

#include "Useful.h"

struct List;
class Huffman;

class TreeNode
{
private:
	char *val;
	int frequency;
	TreeNode *left;
	TreeNode *right;
public:
	friend List;
	friend Huffman;
	TreeNode(char* v) {
		val = string_copy(v);
		frequency = 1;
		left = nullptr;
		right = nullptr;
	}
	~TreeNode()
	{
		if (val) free(val);
		if (left) free(left);
		if (right) free(right);
	}
};

struct List
{
public:
	friend Huffman;
	List() { item = nullptr; next = nullptr; prev = nullptr; };
	~List() { if (item) free(item); if (next) free(next); };
private:
	TreeNode* item;
	List* next;
	List* prev;
};

class Huffman
{
private:
	TreeNode* root;
	List* head;
	List* tail;
	int code_len;

	void sort_list()
	{
		List* cur = head->next;
		while (cur)
		{
			List* next_to_sort = cur->next;
			List* place = cur->prev; //to place after it
			while (place && place->item->frequency < cur->item->frequency)
			{
				if (place->item->frequency < cur->item->frequency) place = place->prev;
			}
			if (place != cur->prev)
				if (!place)
				{
					if (cur->next) cur->next->prev = cur->prev;
					if (cur->prev) cur->prev->next = cur->next;
					head->prev = cur;
					cur->prev = nullptr;
					cur->next = head;
					head = cur;
				}
				else
				{
					if (cur->next) cur->next->prev = cur->prev;
					if (cur->prev) cur->prev->next = cur->next;
					place->next->prev = cur;
					cur->next = place->next;
					place->next = cur;
					cur->prev = place;
				}

			if (cur->next == nullptr) tail = cur;
			cur = next_to_sort;
		}
	}

	void push_sorted_descending(TreeNode* val)
	{
		if (!head)	//add as first
		{
			head = new List;
			head->item = val;
			tail = head;
		}
		else
		{
			if (val->frequency > head->item->frequency)	//add as first
			{
				List* node = new List;
				node->item = val;
				node->next = head;
				head->prev = node;
				head = node;
			}
			else
			{
				List* cur = head;
				while (cur->next)
				{
					if (val->frequency > cur->next->item->frequency)	//if we need to insert between cur & cur->next
					{
						List* node = new List;
						node->item = val;

						node->next = cur->next;
						node->prev = cur;
						cur->next->prev = node;
						cur->next = node;
						return;
					}
					if (string_compare(cur->item->val, val->val) == 0)	//no need to add
					{
						cur->item->frequency++;
						free(val);
						return;
					}
					cur = cur->next;
				}

				if (string_compare(cur->item->val, val->val) == 0) 	//no need to add
				{
					cur->item->frequency++;
					free(val);
				}
				else
				{
					cur->next = new List;		//add as last	//check if null
					cur->next->next = nullptr;
					cur->next->prev = cur;
					cur->next->item = val;
					tail = cur->next;
				}
			}

		}
	};

	void pop_back_twice()
	{
		if (head && (head != tail)) //if there at least one list-node
		{
			if (tail->prev->prev) //if there more than one list-node
			{
				tail = tail->prev->prev;
				free(tail->next);
				tail->next = nullptr;
			}
		}
	};

	void get_list(char *message)
	{
		if (message)
		{
			if (strlen(message) > 0)
			{
				for (int i = 0; i < strlen(message); i++)
				{
					push_sorted_descending(new TreeNode(give_some_chars(1, message[i])));
				}
			}
			else throw invalid_argument("String is empty");
		}
		else throw invalid_argument("No string to code");
	};

	void buid_tree()
	{
		while (head != tail)
		{
			TreeNode* node = new TreeNode(string_concat(tail->item->val, tail->prev->item->val));
			node->frequency = tail->item->frequency + tail->prev->item->frequency;
			node->left = tail->item;
			node->right = tail->prev->item;
			push_sorted_descending(node);
			pop_back_twice();
		}
		root = head->item;
	}

public:
	Huffman()
	{
		root = nullptr;
		head = nullptr;
		tail = nullptr;
	};
	
	~Huffman()
	{
		if (root) free(root);
		if (head) free(head);
	};

	int get_code_len() { return code_len; };

	void list_output()
	{
		if (head)
		{
			List* cur = head;
			while (cur)
			{
				cout << cur->item->val << cur->item->frequency << " ";
				cur = cur->next;
			}
			cout << endl;
		}
	}

	bool *code_message(char* message)
	{
		code_len = 0;
		{
			if (message)
			{
				if (strlen(message))
				{
					get_list(message);
					sort_list();
					buid_tree();

					bool* res;
					if (root->left && root->right)
					{
						res = nullptr;
						bool tr = true;
						bool fa = false;
						for (int i = 0; i < strlen(message); i++)
						{
							TreeNode* cur = root;

							while (cur->left)
							{
								if (is_in_string(cur->left->val, message[i]))
								{
									cur = cur->left;
									res = array_concat(res, &tr, code_len, 1);
								}
								else
									if (is_in_string(cur->right->val, message[i]))
									{
										cur = cur->right;
										res = array_concat(res, &fa, code_len, 1);
									}
								code_len++;
							}
						}
					}
					else
					{
						res = (bool*)malloc(strlen(message) * sizeof(bool));
						for (int i = 0; i < strlen(message); i++)
							res[i] = true;
						code_len = strlen(message);
					}
					return res;
				}
				else throw invalid_argument("Message is empty");
			}
			else throw invalid_argument("Message is empty");
		}
	}

	char* decode_message(bool* code)
	{
		if (code)
		{
			if (root)
			{
				char* message = nullptr;
				if (root->left && root->right)
				{
					TreeNode* cur = root;
					for (int i = 0; i < code_len; i++)
					{
						if (code[i] == true)
							cur = cur->left;
						else
							cur = cur->right;

						if (!cur->left)	//cur is leaf
						{
							message = string_concat(message, cur->val);
							cur = root;
						}
					}
				}
				else
				{
					message = (char*)malloc((code_len + 1) * sizeof(char));
					for (int i = 0; i < code_len; i++)
						message[i] = root->val[0];
					message[code_len] = 0;
				}
				return message;
			}
			else throw runtime_error("No given message to code");
		}
		else throw invalid_argument("No code");
	}

	void out_code(char* message)
	{
		bool* res = code_message(message);
		for (int i = 0; i < code_len; i++)
			cout << res[i];
		cout << endl;
	}

	float coef(char *mess)
	{
		float t = ((float)strlen(mess) * 8.0);
		float f = code_len / t;
		return f;
	}
};
