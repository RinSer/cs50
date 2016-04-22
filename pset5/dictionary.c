/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "dictionary.h"

#define SKIP 97
#define CHARS 27

typedef struct node
{
	bool is_word;
	struct node* child[CHARS];
} node;

// The root node
node* root;
// Number of words in the dictionary
int word_count;

bool free_node(node* nd);

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
	if (strlen(word) < 1)
		return false;
    
	node* current_node = root;	
	for (int i = 0; i < strlen(word); i++)
	{
		
		char letter = tolower(word[i]);
		
		int node_index = letter > 96 ? letter-SKIP : CHARS-1;
	
		if (current_node->child[node_index] == NULL)
		{			
			return false;
		}
		else
		{
			current_node = current_node->child[node_index];
			
			if (i == strlen(word)-1)
			{
				return current_node->is_word;
			}
		}
	}

	return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
	// Initialize the root node and word count
	root = malloc(sizeof(node));
	// Initialize the word count
	word_count = 0;

	// Open the dictionary file    
	FILE* dict_file = fopen(dictionary, "r");
	if (dict_file == NULL)
	{
		return false;
	}
	
	// The word buffer
	char word[LENGTH+1];

	// Initialize all root nodes as NULL
	for (int i = 0; i < CHARS; i++)
	{
		root->child[i] = NULL;
	}
	
	root->is_word = false;

	// Read a word from the file and save it as TRIE nodes
	while (fscanf(dict_file, "%s", word) != EOF)
	{			
		node* current_node = root;

		for (int i = 0; i < strlen(word); i++)
		{
			int node_index = word[i] > 96 ? word[i]-SKIP : CHARS-1;

			if (current_node->child[node_index] == NULL)
			{
				current_node->child[node_index] = malloc(sizeof(node));
				// Initialize all new node's children as NULL
				for (int i = 0; i < CHARS; i++)
				{
					current_node->child[node_index]->child[i] = NULL;
				}
				
				current_node->child[node_index]->is_word = false;
			}
			
			current_node = current_node->child[node_index];
			
			if (i == strlen(word)-1)
			{
				word_count++;				
				current_node->is_word = true;
			}
		}
	}

	fclose(dict_file);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return word_count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    return free_node(root);
}

/**
 * Recursively frees the node memory from given node to all its ancestors
 */
bool free_node(node* next)
{
	for (int i = 0; i < CHARS; i++)
	{
		if (next->child[i] != NULL && !free_node(next->child[i]))
		{
			return false;
		}
	}
	
	free(next);
	return true;
}
