// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

unsigned int word_count; //-- these are variables I added, "unsigned" goes to 4 billion instead of (-2 to 2 billion)
unsigned int hash_value;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    //-- check a value, then move the pointer to the next value in the linked list
    hash_value = hash(word);

    node *cursor = table[hash_value];

    while (cursor != 0)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    //-- the provided function uses the ascii values 65(A) to 90(Z)
    //-- so it is taking the first letter and subtracting 65 from it to get 0-25
    return toupper(word[0]) - 'A';

    // unsigned long total = 0;
    // for (int i = 0; i < strlen(word); i++);
    // {
    //     total += tolower(word[i]);
    // }
    // return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = (fopen(dictionary, "r"));

    if (file == NULL)
    {
        printf("Can't open %s\n", dictionary);
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));

        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);
        hash_value = hash(word);
        n->next = table[hash_value]; //-- "table" is from the hash section written for me already, above
        table[hash_value] = n;
        word_count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (word_count > 0)
    {
        return word_count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    //-- have to use a temp variable to store the current pointer location so you don't lose the whole list
    //-- this was shown in the lecture video
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }

        if (cursor == NULL)
        {
            return true;
        }
    }
    return false;
}
